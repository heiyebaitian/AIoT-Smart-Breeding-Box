#include <WiFi.h>
#include <PubSubClient.h>
#include <TaskScheduler.h>  // 多线程调度器
#include "MQTT_driver.h"  //  MQTT驱动
#include "task_app.h" //  协程回调函数

#define digitalToggle(x) digitalWrite(x, !digitalRead(x))   // 翻转IO函数

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
const char *mqtt_topic_pub = "esp32/test"; // 需要发布到的主题
const char *mqtt_topic_sub = "esp32/test"; // 需要订阅的主题

unsigned long previousConnectMillis = 0; // 毫秒时间记录
const long intervalConnectMillis = 5000; // 时间间隔
unsigned long previousPublishMillis = 0; // 毫秒时间记录
const long intervalPublishMillis = 5000; // 时间间隔
unsigned long currentMillis = 0;  // 当前时间记录

WiFiClient tcpClient; // 声明TCP连接
PubSubClient mqttClient;// 声明MQTT连接
Scheduler ts; // 声明协程管理器

/* 函数声明 */
void Serial_init();
void IO_init();



/* 创建多协程任务信息 */
Task State_check_app_task(10000,TASK_FOREVER,&State_check_app);  // 创建任务 状态检查任务 任务间隔10s 任务次数：始终
Task MQTT_event_app_task(TASK_IMMEDIATE,TASK_FOREVER,&MQTT_event_app);  // 创建任务 MQTT事物任务  任务间隔0ms 任务次数：始终


void setup() {
  // put your setup code here, to run once:
    Serial_init();
    IO_init();
    setup_iot_server();

    //协程初始化
    ts.init();//初始化 scheduler
    ts.addTask(State_check_app_task);//将 State_check_app_task 装载到任务管理器
    ts.addTask(MQTT_event_app_task);//将 MQTT_event_app_task 装载到任务管理器

    //启动任务
    State_check_app_task.enable(); //启动 State_check_app_task 任务
    MQTT_event_app_task.enable(); //启动 MQTT_event_app_task 任务

}

void loop() {
  // put your main code here, to run repeatedly:
    digitalToggle(stateLedPin); // 20240720:此操作无实际作用，用于防止看门狗饿死，未来会移除
    currentMillis = millis(); // 读取当前时间
    ts.execute(); //多任务管理器保活
}

/* 串口初始化代码 */
void Serial_init(){
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial2.begin(115200);
}

/* IO初始化代码 */
void IO_init(){
    pinMode(stateLedPin,OUTPUT);
    digitalWrite(stateLedPin,HIGH);
}
