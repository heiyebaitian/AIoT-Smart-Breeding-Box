#include <WiFi.h>
#include <PubSubClient.h>
#include <TaskScheduler.h>  // 多线程调度器
#include "MQTT_driver.h"  //  MQTT驱动
#include "task_app.h" //  协程回调函数


#define digitalToggle(x) digitalWrite(x, !digitalRead(x))   // 翻转IO函数
#define NOP do { __asm__ __volatile__ ("nop"); } while (0)  // NOP函数

#define IOT_DATA_UPLOAD_DELAY 3000  // IOT数据上传周期(ms)
#define LINK_STATE_CHECK_DELAY 10000  // 设备在线状态检查周期(ms)

const int DEBUG_MODE=1;

/* I/O引脚配置 */
const int stateLedPin = 48;


/* WiFi相关配置信息 */
const char *wifi_ssid = "iotgateway";
const char *wifi_password = "guoxilin";

/* MQTT相关配置信息 */
const char *mqtt_broker_addr = "192.168.1.101"; // MQTT服务器地址
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
int temperature_normalization = 20; // 常态化培养区温度
uint16_t humidity_normalization = 50; // 常态化培养区湿度 
uint16_t sh_normalization = 50; // 常态化培养区土壤湿度 
uint16_t co2_normalization = 100; // 常态化培养区二氧化碳 
uint16_t ch2o_normalization = 100; //  常态化培养区甲醛 
uint16_t tvoc_normalization = 100; // 常态化培养区TVOC 
bool fan_state_normalization = false;  //  常态化培养区通风系统状态
bool light_state_normalization = false;  //  常态化培养区光照系统状态
bool refrigeration_state_normalization = false;  //  常态化培养区制冷系统状态
bool heating_state_normalization = false; //  常态化培养区制热系统状态



int temperature_differentiation = 20; // 差异化培养区温度
uint16_t humidity_differentiation = 50; // 差异化培养区湿度 
uint16_t sh_differentiation = 50; // 差异化培养区土壤湿度 
uint16_t co2_differentiation = 100; // 差异化培养区二氧化碳 
uint16_t ch2o_differentiation = 100; //  差异化培养区甲醛 
uint16_t tvoc_differentiation = 100; // 差异化培养区TVOC 
bool fan_state_differentiation = false;  // 差异化培养区通风系统状态
bool light_state_differentiation = false;  //  差异化培养区光照系统状态
bool refrigeration_state_differentiation = false;  //  差异化培养区制冷系统状态
bool heating_state_differentiation = false; //  差异化培养区制热系统状态


/* 机械臂数据 */
bool waterpump_state = false; //  滴灌系统状态
uint16_t water_liquid_level = 20;  //  水箱液位




WiFiClient tcpClient; // 声明TCP连接
PubSubClient mqttClient;// 声明MQTT连接
Scheduler ts; // 声明协程管理器


/* 函数声明 */
void Serial_init();
void IO_init();



/* 创建多协程任务信息 */
Task Link_state_check_app_task(LINK_STATE_CHECK_DELAY,TASK_FOREVER,&State_check_app);  // 创建任务 连接状态检查任务 任务间隔10s 任务次数：始终
Task MQTT_event_app_task(TASK_IMMEDIATE,TASK_FOREVER,&MQTT_event_app);  // 创建任务 MQTT事物任务  任务间隔0ms 任务次数：始终
Task Iot_data_upload_app_task(IOT_DATA_UPLOAD_DELAY,TASK_FOREVER,&Iot_data_upload_app);  // 创建任务 连接状态检查任务 任务间隔10s 任务次数：始终



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

    //启动任务
    Link_state_check_app_task.enable(); //启动 Link_state_check_app_task 任务
    MQTT_event_app_task.enable(); //启动 MQTT_event_app_task 任务
    Iot_data_upload_app_task.enable(); //启动 Iot_data_upload_app_task 任务



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
  Serial.begin(115200); //TX:43 RX:44
  Serial1.begin(115200); //TX:16 RX:15
  Serial2.begin(115200); //TX:20 RX:19
}

/* IO初始化代码 */
void IO_init(){
  pinMode(stateLedPin,OUTPUT);
  digitalWrite(stateLedPin,HIGH);
}
