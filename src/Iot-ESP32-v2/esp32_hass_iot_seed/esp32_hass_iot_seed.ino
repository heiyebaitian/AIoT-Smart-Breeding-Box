#include <WiFi.h>
#include <PubSubClient.h>
#include <TaskScheduler.h>  // 多线程调度器
#include "MQTT_driver.h"  //  MQTT驱动
#include "task_app.h" //  协程回调函数
#include "sensor_driver.h" // 传感器驱动


#define digitalToggle(x) digitalWrite(x, !digitalRead(x))   // 翻转IO函数
#define NOP do { __asm__ __volatile__ ("nop"); } while (0)  // NOP函数

#define IOT_DATA_UPLOAD_DELAY 3000  // IOT数据上传周期(ms)
#define LINK_STATE_CHECK_DELAY 10000  // 设备在线状态检查周期(ms)
#define SENSOR_READ_DELAY 3000  //  传感器数据采集周期(ms)

#define SERIAL_BPS 115200 // 串口0波特率
#define SERIAL_DEBUG_BPS 230400 //串口0 DEBUG波特率
#define SERIAL1_BPS 9600 // 串口1波特率
#define SERIAL2_BPS 115200 // 串口2波特率



const bool DEBUG_MODE = true;

/* I/O引脚配置 */
//const int stateLedPin = 48; // LED状态灯(已弃用)
const int sh_normalization_Pin = 1; // 常态化培养区土壤湿度传感器
const int sh_differentiation_Pin = 2; // 差异化培养区土壤湿度传感器
const int fan_normalization_Pin = 6; // 常态化培养区通风系统
const int light_normalization_Pin = 7; // 常态化培养区光照系统
const int refrigeration_normalization_Pin = 17; // 常态化培养区制冷系统
const int heating_normalization_Pin = 18; // 常态化培养区制热系统
const int fan_differentiation_Pin = 8; // 差异化培养区通风系统
const int light_differentiation_Pin = 3; // 差异化培养区光照系统
const int refrigeration_differentiation_Pin = 46; // 差异化培养区制冷系统
const int heating_differentiation_Pin = 9; // 差异化培养区制热系统
const int water_liquid_level_Pin = 21; // 液位传感器



/* WiFi相关配置信息 */
const char *wifi_ssid = "iotgateway";
const char *wifi_password = "12345678";

/* MQTT相关配置信息 */
const char *mqtt_broker_addr = "192.168.166.101"; // MQTT服务器地址
const uint16_t mqtt_broker_port = 1883; // MQTT服务器端口            
const char *mqtt_username = "esp32"; // MQTT账号
const char *mqtt_password = "12345678"; // MQTT密码
const uint16_t mqtt_client_buff_size = 4096; // 客户端缓存大小（非必须）
String mqtt_client_id = "esp32_client"; // 客户端ID
const char *mqtt_willTopic = "esp32/state"; // MQTT连接遗嘱主题

const char *mqtt_topic_pub = "esp32/test"; // 需要发布到的主题
const char *mqtt_topic_sub = "esp32/test"; // 需要订阅的主题

/* 系统事件时间记录 */
unsigned long previousConnectMillis = 0; // 毫秒时间记录
const long intervalConnectMillis = 5000; // 时间间隔
unsigned long previousPublishMillis = 0; // 毫秒时间记录
const long intervalPublishMillis = 5000; // 时间间隔
unsigned long currentMillis = 0;  // 当前时间记录

/* 系统标志位 */
bool enable_Iot_data_upload = true;

/* 系统运行数据 */
long rssi = 0;
/* 传感器及系统状态数据 */
double temperature_normalization = 25; // 常态化培养区温度
double humidity_normalization = 60; // 常态化培养区湿度 
uint16_t sh_normalization = 60; // 常态化培养区土壤湿度 
uint16_t co2_normalization = 200; // 常态化培养区二氧化碳 
uint16_t ch2o_normalization = 100; //  常态化培养区甲醛 
uint16_t tvoc_normalization = 100; // 常态化培养区TVOC 
bool fan_state_normalization = false;  //  常态化培养区通风系统状态
bool light_state_normalization = false;  //  常态化培养区光照系统状态
bool refrigeration_state_normalization = false;  //  常态化培养区制冷系统状态
bool heating_state_normalization = false; //  常态化培养区制热系统状态



double temperature_differentiation = 25; // 差异化培养区温度
double humidity_differentiation = 60; // 差异化培养区湿度 
uint16_t sh_differentiation = 60; // 差异化培养区土壤湿度 
uint16_t co2_differentiation = 200; // 差异化培养区二氧化碳 
uint16_t ch2o_differentiation = 100; //  差异化培养区甲醛 
uint16_t tvoc_differentiation = 100; // 差异化培养区TVOC 
bool fan_state_differentiation = false;  // 差异化培养区通风系统状态
bool light_state_differentiation = false;  //  差异化培养区光照系统状态
bool refrigeration_state_differentiation = false;  //  差异化培养区制冷系统状态
bool heating_state_differentiation = false; //  差异化培养区制热系统状态


/* 机械臂数据 */
bool waterpump_state = false; //  滴灌系统状态
uint16_t water_liquid_level = 100;  //  水箱液位




WiFiClient tcpClient; // 声明TCP连接
PubSubClient mqttClient;// 声明MQTT连接
Scheduler ts; // 声明协程管理器


/* 函数声明 */
void Serial_init();
void IO_init();




/* 创建多协程任务信息 */
Task Link_state_check_app_task(LINK_STATE_CHECK_DELAY,TASK_FOREVER,&State_check_app);  // 创建任务 连接状态检查任务 任务次数：始终
Task MQTT_event_app_task(TASK_IMMEDIATE,TASK_FOREVER,&MQTT_event_app);  // 创建任务 MQTT事务任务 任务次数：始终
Task Iot_data_upload_app_task(IOT_DATA_UPLOAD_DELAY,TASK_FOREVER,&Iot_data_upload_app);  // 创建任务 连接状态检查任务 任务次数：始终
Task Serial1_analysis_app_task(TASK_IMMEDIATE,TASK_FOREVER,&Serial1_analysis_app);  // 创建任务 串口处理任务 任务次数：始终
Task Sensor_rw_app_task(TASK_IMMEDIATE,TASK_FOREVER,&Sensor_rw_app);  // 创建任务 传感器数据采集任务 任务次数：始终



void setup() {
  // put your setup code here, to run once:
    Serial_init();
    IO_init();
    setup_iot_server();


    //disableCore0WDT(); // 关闭定时器看门狗
    //协程初始化
    ts.init();//初始化 scheduler
    ts.addTask(Link_state_check_app_task);//将 Link_state_check_app_task 装载到任务管理器
    ts.addTask(MQTT_event_app_task);//将 MQTT_event_app_task 装载到任务管理器
    ts.addTask(Iot_data_upload_app_task);//将 Iot_data_upload_app_task 装载到任务管理器
    ts.addTask(Serial1_analysis_app_task);//将 Serial1_app_task 装载到任务管理器
    ts.addTask(Sensor_rw_app_task);//将 Sensor_read_app_task 装载到任务管理器

    //启动任务
    Link_state_check_app_task.enable(); //启动 Link_state_check_app_task 任务
    MQTT_event_app_task.enable(); //启动 MQTT_event_app_task 任务
    Iot_data_upload_app_task.enable(); //启动 Iot_data_upload_app_task 任务
    Serial1_analysis_app_task.enable(); //启动 Serial1_app_task 任务
    Sensor_rw_app_task.enable();  //启动 Sensor_read_app_task 任务
}

void loop() {
  // put your main code here, to run repeatedly:
    //digitalToggle(stateLedPin); // 20240720:此操作无实际作用，用于防止看门狗饿死，未来会移除
    currentMillis = millis(); // 读取当前时间
    ts.execute(); //多任务管理器保活
    delay(1); // 防止任务看门狗饿死
}



/* 串口初始化代码 */
void Serial_init(){
  if(DEBUG_MODE) Serial.begin(SERIAL_DEBUG_BPS); // DEBUG模式
  else Serial.begin(SERIAL_BPS); //TX:43 RX:44
  Serial1.begin(SERIAL1_BPS); //TX:16 RX:15 气体传感器占用
  Serial2.begin(SERIAL2_BPS); //TX:20 RX:19 RP2040占用
}

/* IO初始化代码 */
void IO_init(){
  //pinMode(stateLedPin,OUTPUT);
  //digitalWrite(stateLedPin,HIGH);
  pinMode(sh_normalization_Pin, INPUT);
  pinMode(sh_differentiation_Pin, INPUT);
  pinMode(water_liquid_level_Pin, INPUT_PULLUP);

  
  
  pinMode(fan_normalization_Pin, OUTPUT);
  digitalWrite(fan_normalization_Pin,HIGH);
  
  pinMode(light_normalization_Pin, OUTPUT);
  digitalWrite(light_normalization_Pin,HIGH);
  
  pinMode(refrigeration_normalization_Pin, OUTPUT);
  digitalWrite(refrigeration_normalization_Pin,HIGH);
  
  pinMode(heating_normalization_Pin, OUTPUT);
  digitalWrite(heating_normalization_Pin,HIGH);
  
  pinMode(fan_differentiation_Pin, OUTPUT);
  digitalWrite(fan_differentiation_Pin,HIGH);
  
  pinMode(light_differentiation_Pin, OUTPUT);
  digitalWrite(light_differentiation_Pin,HIGH);
  
  pinMode(refrigeration_differentiation_Pin, OUTPUT);
  digitalWrite(refrigeration_differentiation_Pin,HIGH);
  
  pinMode(heating_differentiation_Pin, OUTPUT);
  digitalWrite(heating_differentiation_Pin,HIGH);
  

}
