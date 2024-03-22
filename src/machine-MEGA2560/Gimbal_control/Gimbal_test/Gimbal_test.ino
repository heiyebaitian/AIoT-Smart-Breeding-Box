#include <Arduino.h>
#include <AccelStepper.h>  //本示例程序使用AccelStepper库



// 定义电机控制用常量
const int enablePin = 8;  // 使能控制引脚

const int xdirPin = 5;     // x方向控制引脚
const int xstepPin = 2;    // x步进控制引脚
const int ydirPin = 6;     // y方向控制引脚
const int ystepPin = 3;    // y步进控制引脚
const int zdirPin = 7;     // z方向控制引脚
const int zstepPin = 4;    // z步进控制引脚
const int adirPin = 13;     // a方向控制引脚
const int astepPin = 12;    // a步进控制引脚
const int x_stopPin = 9;
const int y_stopPin = 10;
const int z_stopPin = 11;

const int moveSteps = 200;    //测试电机运行使用的运行步数

AccelStepper stepperX(1,xstepPin,xdirPin);//建立步进电机对象1
AccelStepper stepperY(1,ystepPin,ydirPin);//建立步进电机对象2
AccelStepper stepperZ(1,zstepPin,zdirPin);//建立步进电机对象3
AccelStepper stepperA(1,astepPin,adirPin);//建立步进电机对象4

// 定义全局变量
int x_stop = 1;
int y_stop = 1;
int z_stop = 1;
int a_stop = 1;

int xsteps = 0;
int ysteps = 0;
int zsteps = 0;
int asteps = 0; 

float xvel = 300;
float yvel = 300;
float zvel = 300;
float avel = 300;

float xacc = 20;
float yacc = 20;
float zacc = 20;
float aacc = 20;




void setup() {
  
  pinMode(xstepPin,OUTPUT);     // Arduino控制A4988x步进引脚为输出模式
  pinMode(xdirPin,OUTPUT);      // Arduino控制A4988x方向引脚为输出模式
  pinMode(ystepPin,OUTPUT);     // Arduino控制A4988y步进引脚为输出模式
  pinMode(ydirPin,OUTPUT);      // Arduino控制A4988y方向引脚为输出模式
  pinMode(zstepPin,OUTPUT);     // Arduino控制A4988z步进引脚为输出模式
  pinMode(zdirPin,OUTPUT);      // Arduino控制A4988z方向引脚为输出模式
  pinMode(astepPin,OUTPUT);     // Arduino控制A4988a步进引脚为输出模式
  pinMode(adirPin,OUTPUT);      // Arduino控制A4988a方向引脚为输出模式  
  pinMode(x_stopPin,INPUT_PULLUP);     // 定义X电机停止标志位
  pinMode(y_stopPin,INPUT_PULLUP);     // 定义Y电机停止标志位
  pinMode(z_stopPin,INPUT_PULLUP);     // 定义Z电机停止标志位
  
  pinMode(enablePin,OUTPUT);   // Arduino控制A4988使能引脚为输出模式
  digitalWrite(enablePin,LOW); // 将使能控制引脚设置为低电平从而让
                               // 电机驱动板进入工作状态
                                
  stepperX.setMaxSpeed(xvel);     // 设置电机最大速度300 
  stepperX.setAcceleration(xacc);  // 设置电机加速度20.0  
  stepperY.setMaxSpeed(yvel);     // 设置电机最大速度300 
  stepperY.setAcceleration(yacc);  // 设置电机加速度20.0 
  stepperZ.setMaxSpeed(zvel);     // 设置电机最大速度300 
  stepperZ.setAcceleration(zacc);  // 设置电机加速度20.0 
  stepperA.setMaxSpeed(avel);     // 设置电机最大速度300 
  stepperA.setAcceleration(aacc);  // 设置电机加速度20.0   

  Serial.begin(115200);   // 初始化串口通讯
}

void loop() {
  stepper_stop();
  Serial_app();
  stepper_task();
}

void stepper_task(){
  if (x_stop)
  {
    stepperX.run();
  }
  else
  {
    Serial.println("警告：电机X已达最大限制位置");
  }

  if (y_stop)
  {
    stepperY.run();
  }
  else
  {
    Serial.println("警告：电机Y已达最大限制位置");
  }
  
  if (z_stop)
  {
    stepperZ.run();
  }
  else
  {
    Serial.println("警告：电机Z已达最大限制位置");
  }
  
  if(a_stop)
  {
    stepperA.run();
  }
  else
  {
    Serial.println("警告：电机A已达最大限制位置");
  }
}

/* 上位机通讯程序 */
void Serial_app(){//上位机交互程序
  String inString ="" ; //串口接收命令存放区
  while(Serial.available()>0){ //检查缓冲区是否存在数据
    inString += char(Serial.read()); //读取缓冲区
    delay(10);      // 延时函数用于等待字符完全进入缓冲区
  }

  if(inString!=""){ //  命令解析区
    String command_head= "" ; //  命令头缓存
    String command_text= "" ; //  命令内容缓存
    String command_mid= "" ;
    command_head = inString.substring(0,7);// 命令头格式"/000000"
    command_mid = inString.substring(7,8);
    command_text = inString.substring(8,inString.length());

    
    /*解析命令头*/

    /* 对全体电机写入速度 */
    if(command_head == "/000000"&&command_mid == " ")
    {
      xvel = command_text.toInt();
      yvel = command_text.toInt();
      zvel = command_text.toInt();
      avel = command_text.toInt();
      stepperX.setMaxSpeed(xvel);
      stepperY.setMaxSpeed(yvel);
      stepperZ.setMaxSpeed(zvel);
      stepperA.setMaxSpeed(avel);
      Serial.print("全体速度设置为：");
      Serial.print(xvel);
    }


    /* 对X电机写入速度 */
    if(command_head == "/000001"&&command_mid == " ") 
    {
      xvel = command_text.toInt();
      stepperX.setMaxSpeed(xvel);
      Serial.print("X电机速度设置为：");
      Serial.print(xvel);
    }

    /* 对Y电机写入速度 */
    if(command_head == "/000002"&&command_mid == " ") 
    {
      yvel = command_text.toInt();
      stepperY.setMaxSpeed(yvel);
      Serial.print("Y电机速度设置为：");
      Serial.print(yvel);
    }

    /* 对Z电机写入速度 */
    if(command_head == "/000003"&&command_mid == " ") 
    {
      zvel = command_text.toInt();
      stepperZ.setMaxSpeed(zvel);
      Serial.print("Z电机速度设置为：");
      Serial.print(zvel);
    }

    /* 对A电机写入速度 */
    if(command_head == "/000004"&&command_mid == " ") 
    {
      avel = command_text.toInt();
      stepperA.setMaxSpeed(avel);
      Serial.print("A电机速度设置为：");
      Serial.print(avel);
    }

    /* 对全体电机写入加速度 */
    if(command_head == "/100000"&&command_mid == " ") 
    {
      xacc = command_text.toInt();
      yacc = command_text.toInt();
      zacc = command_text.toInt();
      aacc = command_text.toInt();
      stepperX.setAcceleration(xacc);
      stepperY.setAcceleration(yacc);
      stepperZ.setAcceleration(zacc);
      stepperA.setAcceleration(aacc);
      Serial.print("全体电机加速度设置为：");
      Serial.print(xacc);
    }

    /* 对X电机写入加速度 */
    if(command_head == "/100001"&&command_mid == " ") 
    {
      xacc = command_text.toInt();
      stepperX.setAcceleration(xacc);
      Serial.print("X电机加速度设置为：");
      Serial.print(xacc);
    }

    /* 对Y电机写入加速度 */
    if(command_head == "/100002"&&command_mid == " ") 
    {
      yacc = command_text.toInt();
      stepperY.setAcceleration(yacc);
      Serial.print("Y电机加速度设置为：");
      Serial.print(yacc);
    }

    /* 对Z电机写入加速度 */
    if(command_head == "/100003"&&command_mid == " ") 
    {
      zacc = command_text.toInt();
      stepperZ.setAcceleration(zacc);
      Serial.print("Z电机加速度设置为：");
      Serial.print(zacc);
    }

    /* 对A电机写入加速度 */
    if(command_head == "/100004"&&command_mid == " ") 
    {
      aacc = command_text.toInt();
      stepperA.setAcceleration(aacc);
      Serial.print("A电机加速度设置为：");
      Serial.print(aacc);
    }

    /* 对全体电机写入绝对位置 */
    if(command_head == "/200000"&&command_mid == " ") 
    {
      xsteps = command_text.toInt();
      ysteps = command_text.toInt();
      zsteps = command_text.toInt();
      asteps = command_text.toInt();
      stepperX.moveTo(xsteps);
      stepperY.moveTo(ysteps);
      stepperZ.moveTo(zsteps);
      stepperA.moveTo(asteps);
      Serial.print("全体电机位置设置为：");
      Serial.print(xsteps);
    }

    /* 对X电机写入绝对位置 */
    if(command_head == "/200001"&&command_mid == " ") 
    {
      xsteps = command_text.toInt();
      stepperX.moveTo(xsteps);
      Serial.print("X电机位置设置为：");
      Serial.print(xsteps);
    }

    /* 对Y电机写入绝对位置 */
    if(command_head == "/200002"&&command_mid == " ") 
    {
      ysteps = command_text.toInt();
      stepperY.moveTo(ysteps);
      Serial.print("Y电机位置设置为：");
      Serial.print(ysteps);
    }

    /* 对Z电机写入绝对位置 */
    if(command_head == "/200003"&&command_mid == " ") 
    {
      zsteps = command_text.toInt();
      stepperZ.moveTo(zsteps);
      Serial.print("Z电机位置设置为：");
      Serial.print(zsteps);
    }

    /* 对A电机写入绝对位置 */
    if(command_head == "/200004"&&command_mid == " ") 
    {
      asteps = command_text.toInt();
      stepperA.moveTo(asteps);
      Serial.print("A电机位置设置为：");
      Serial.print(asteps);
    }

    /* 获取全体电机绝对位置 */
    if(command_head == "/300000") 
    {
      Serial.print("X电机位置：");
      Serial.print(stepperX.currentPosition());
      Serial.println("Y电机位置：");
      Serial.println(stepperY.currentPosition());
      Serial.print("Z电机位置：");
      Serial.println(stepperZ.currentPosition());
      Serial.print("A电机位置：");
      Serial.println(stepperA.currentPosition());
    }

    /* 重设X电机绝对位置 */
    if(command_head == "/300001") 
    {
      stepperX.setCurrentPosition(0);
      Serial.print("X电机坐标系已经重建");
      Serial.println(xsteps);
    }

    /* 重设Y电机绝对位置 */
    if(command_head == "/300002") 
    {
      stepperY.setCurrentPosition(0);
      Serial.print("X电机坐标系已经重建");
      Serial.println(0);
    }

    /* 重设Z电机绝对位置 */
    if(command_head == "/300003") 
    {
      stepperZ.setCurrentPosition(0);
      Serial.print("X电机坐标系已经重建");
      Serial.println(zsteps);
    }

    /* 重设A电机绝对位置 */
    if(command_head == "/300004") 
    {
      stepperA.setCurrentPosition(0);
      Serial.print("X电机坐标系已经重建");
      Serial.println(asteps);
    }

  }
}

void stepper_stop()
{
  x_stop = digitalRead(x_stopPin);
  y_stop = digitalRead(y_stopPin);
  z_stop = digitalRead(z_stopPin);
}