#ifndef DEBUG_MODE 
#define DEBUG_MODE 0  //  定义是否开启Debug模式 1开启 0关闭
#define BLINKER_PRINT Serial //  定义Debug输出端口
#define BLINKER_WIFI  //  定义WIFI功能
#define DELAY_TS 5000 
#endif

#include <Blinker.h>  // 点灯科技Blinker库
#include <TaskScheduler.h>  // 多线程调度器

char auth[] = "e5663d2da408"; //  此处放置key
char ssid[] = "LenovoY9000P"; //  此处放置WIFI的SSID
char pswd[] = "guoxilin"; //  此处放置WIFI的PASSWORD

/* 系统常量 */
const int controlPin = 1;
const int statusPin = 2;

/* 系统全局变量 */
extern uint16_t Temperature[3] = {0,-5,30}; // 温度 0:当前温度 1:温度目标范围低位 2:温度目标范围高位
extern uint16_t Humidity[3] = {0,30,80}; // 湿度 0:当前湿度 1:湿度目标范围低位 2:湿度目标范围高位
extern uint16_t Sh[3] = {0,30,80}; // 土壤湿度 0:当前湿度 1:湿度目标范围低位 2:湿度目标范围高位
extern uint16_t CO2[3] = {0,0,2000}; // 二氧化碳 0:当前二氧化碳 1:二氧化碳目标范围低位 2:二氧化碳目标范围高位
extern uint16_t CH2O[3] = {0,0,500}; //  甲醛 0:当前甲醛 1:甲醛目标范围低位 2:甲醛目标范围高位
extern uint16_t TVOC[3] = {0,0,500}; // TVOC 0:当前TVOC 1:TVOC目标范围低位 2:TVOC目标范围高位
extern int fan_status = 0;  //  风扇状态
extern int fan_goal = 0;  //  风扇目标状态
extern int light_status = 0;  //  生长灯状态
extern int light_goal = 0;  //  生长灯目标状态
extern int waterpump_status = 0; //  水泵状态
extern int waterpump_goal = 0;  //  水泵目标状态
extern int water_status = 0;  //  水箱液位
extern int autotask_flag = 0;  //自动任务标记
extern int error_flag = 0;  //系统错误标记 0:为正常值


/* 注册组件对象 */
BlinkerButton Button_WaterPump("btn-wp"); //  水泵开关
BlinkerButton Button_Light("btn-lig"); //  灯开关
BlinkerButton Button_Fan("btn-fan"); //  风扇开关

BlinkerButton Button_water("btn-wat"); //  水箱液位指示器

BlinkerNumber Sensor_Temperature("num-tem");  // 温度读数控件
BlinkerNumber Sensor_Humidity("num-hum"); //  湿度读数控件
BlinkerNumber Sensor_CO2("num-co2"); //  CO2读数控件
BlinkerNumber Sensor_Sh("num-sh"); //  土壤湿度读数控件

BlinkerText Text_info("tex-info"); // 文本提示组件


/* 声明回调函数 */
void Blinker_app();
void Serial_app();
void Report_app();
void Autotask_app();

/* 声明一般函数 */
void Serial_init();

/* 声明功能函数 */
void Text_Text_info();
void dataStorage();
void Debug();
void Serial_analysis();
void Serial1_analysis();
void Serial2_analysis();



/* 创建多协程任务信息 */
Task Blink_app_task(TASK_IMMEDIATE,TASK_FOREVER,&Blinker_app);
Task Serial_app_task(TASK_IMMEDIATE,TASK_FOREVER,&Serial_app);  // 创建任务 串口任务  任务间隔0ms 任务次数：始终
Task Report_app_task(10000,TASK_FOREVER,&Report_app); // 创建任务 回报任务 任务间隔10s 任务次数：始终
Task Auto_app_task(60000,TASK_FOREVER,&Autotask_app);  // 创建任务 自动任务 任务间隔60s 任务次数：始终
Scheduler ts; // 声明协程管理器


void setup() {
    Serial_init();  // 初始化串口
    IO_init();  // 初始化IO

    // DEBUG模式检查及初始化
    #if DEBUG_MODE == 1
      #if defined(BLINKER_PRINT)
        BLINKER_DEBUG.stream(BLINKER_PRINT);
      #endif
    #endif
    
    // 初始化blinker
    Blinker.begin(auth, ssid, pswd);
    Blinker.attachData(dataRead);
    Blinker.attachDataStorage(dataStorage);
    Blinker_actuator_setup(); //  注册回调函数

    //协程初始化
    ts.init();//初始化 scheduler
    ts.addTask(Blink_app_task);//将 Blink_app_task 装载到任务管理器
    ts.addTask(Serial_app_task);//将 Serial_app_task 装载到任务管理器
    ts.addTask(Report_app_task);//将 Report_app_task 装载到任务管理器
    ts.addTask(Auto_app_task);//将 Auto_app_task 装载到任务管理器

    //启动任务
    Blink_app_task.enable(); //启动 Blink_app_task 任务
    Serial_app_task.enable(); //启动 Serial_app_task 任务
    Report_app_task.enable(); //启动 Report_app_task 任务
}

void loop() {
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
    pinMode(controlPin,OUTPUT);
    pinMode(statusPin,INPUT_PULLUP);
    digitalWrite(controlPin,HIGH);
}