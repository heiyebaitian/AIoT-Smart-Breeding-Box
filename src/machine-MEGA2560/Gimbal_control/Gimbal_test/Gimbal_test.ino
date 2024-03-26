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
const int x_stopPin = 9;    // x方向限位引脚
const int y_stopPin = 10;   // y方向限位引脚
const int z_stopPin = 11;   // z方向限位引脚
const int x_max = 13950;    // x方向最大步进数
const int y_max = 15250;    // y方向最大步进数
const int z_max = 11000;    // z方向最大步进数


AccelStepper stepperX(1,xstepPin,xdirPin);//建立步进电机对象1
AccelStepper stepperY(1,ystepPin,ydirPin);//建立步进电机对象2
AccelStepper stepperZ(1,zstepPin,zdirPin);//建立步进电机对象3
AccelStepper stepperA(1,astepPin,adirPin);//建立步进电机对象4

// 定义全局变量
int x_stop = 1;
int y_stop = 1;
int z_stop = 1;
int a_stop = 1;

int x_steps = 0;
int y_steps = 0;
int z_steps = 0;
int a_steps = 0; 

float x_vel = 2000;
float y_vel = 2000;
float z_vel = 2000;
float a_vel = 2000;

float x_acc = 1000;
float y_acc = 1000;
float z_acc = 1000;
float a_acc = 1000;

int task_enable = 0;
int task[][3] = {
  {100,100,11000},
  {100,4000,11000},
  {100,15000,11000},
  {4566,15000,11000},
  {4566,4000,11000},
  {9033,4000,11000},
  {9033,15000,11000},
  {13500,15000,11000},
  {13500,4000,11000},
};





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
                                
  stepperX.setMaxSpeed(x_vel);     
  stepperX.setAcceleration(x_acc);   
  stepperY.setMaxSpeed(y_vel);     
  stepperY.setAcceleration(y_acc);  
  stepperZ.setMaxSpeed(z_vel);     
  stepperZ.setAcceleration(z_acc);  
  stepperA.setMaxSpeed(a_vel);     
  stepperA.setAcceleration(a_acc);  

  Serial.begin(115200);   // 初始化串口通讯

  stepper_calibration();
}

void loop() {
  //stepper_stop();
  Serial_app();
  stepper_run();
  if(task_enable)  stepper_task(task,9);
}

/*
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
*/


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
      x_vel = command_text.toInt();
      y_vel = command_text.toInt();
      z_vel = command_text.toInt();
      a_vel = command_text.toInt();
      stepperX.setMaxSpeed(x_vel);
      stepperY.setMaxSpeed(y_vel);
      stepperZ.setMaxSpeed(z_vel);
      stepperA.setMaxSpeed(a_vel);
      Serial.print("全体速度设置为：");
      Serial.println(x_vel);
    }


    /* 对X电机写入速度 */
    if(command_head == "/000001"&&command_mid == " ") 
    {
      x_vel = command_text.toInt();
      stepperX.setMaxSpeed(x_vel);
      Serial.print("X电机速度设置为：");
      Serial.println(x_vel);
    }

    /* 对Y电机写入速度 */
    if(command_head == "/000002"&&command_mid == " ") 
    {
      y_vel = command_text.toInt();
      stepperY.setMaxSpeed(y_vel);
      Serial.print("Y电机速度设置为：");
      Serial.println(y_vel);
    }

    /* 对Z电机写入速度 */
    if(command_head == "/000003"&&command_mid == " ") 
    {
      z_vel = command_text.toInt();
      stepperZ.setMaxSpeed(z_vel);
      Serial.print("Z电机速度设置为：");
      Serial.println(z_vel);
    }

    /* 对A电机写入速度 */
    if(command_head == "/000004"&&command_mid == " ") 
    {
      a_vel = command_text.toInt();
      stepperA.setMaxSpeed(a_vel);
      Serial.print("A电机速度设置为：");
      Serial.println(a_vel);
    }

    /* 对全体电机写入加速度 */
    if(command_head == "/100000"&&command_mid == " ") 
    {
      x_acc = command_text.toInt();
      y_acc = command_text.toInt();
      z_acc = command_text.toInt();
      a_acc = command_text.toInt();
      stepperX.setAcceleration(x_acc);
      stepperY.setAcceleration(y_acc);
      stepperZ.setAcceleration(z_acc);
      stepperA.setAcceleration(a_acc);
      Serial.print("全体电机加速度设置为：");
      Serial.println(x_acc);
    }

    /* 对X电机写入加速度 */
    if(command_head == "/100001"&&command_mid == " ") 
    {
      x_acc = command_text.toInt();
      stepperX.setAcceleration(x_acc);
      Serial.print("X电机加速度设置为：");
      Serial.println(x_acc);
    }

    /* 对Y电机写入加速度 */
    if(command_head == "/100002"&&command_mid == " ") 
    {
      y_acc = command_text.toInt();
      stepperY.setAcceleration(y_acc);
      Serial.print("Y电机加速度设置为：");
      Serial.println(y_acc);
    }

    /* 对Z电机写入加速度 */
    if(command_head == "/100003"&&command_mid == " ") 
    {
      z_acc = command_text.toInt();
      stepperZ.setAcceleration(z_acc);
      Serial.print("Z电机加速度设置为：");
      Serial.println(z_acc);
    }

    /* 对A电机写入加速度 */
    if(command_head == "/100004"&&command_mid == " ") 
    {
      a_acc = command_text.toInt();
      stepperA.setAcceleration(a_acc);
      Serial.print("A电机加速度设置为：");
      Serial.println(a_acc);
    }

    /* 对全体电机写入绝对位置 */
    if(command_head == "/200000"&&command_mid == " ") 
    {
      x_steps = command_text.toInt();
      y_steps = command_text.toInt();
      z_steps = command_text.toInt();
      a_steps = command_text.toInt();
      stepperX.moveTo(x_steps);
      stepperY.moveTo(y_steps);
      stepperZ.moveTo(z_steps);
      stepperA.moveTo(a_steps);
      Serial.print("全体电机位置设置为：");
      Serial.println(x_steps);
    }

    /* 对X电机写入绝对位置 */
    if(command_head == "/200001"&&command_mid == " ") 
    {
      x_steps = command_text.toInt();
      stepperX.moveTo(x_steps);
      Serial.print("X电机位置设置为：");
      Serial.println(x_steps);
    }

    /* 对Y电机写入绝对位置 */
    if(command_head == "/200002"&&command_mid == " ") 
    {
      y_steps = command_text.toInt();
      stepperY.moveTo(y_steps);
      Serial.print("Y电机位置设置为：");
      Serial.println(y_steps);
    }

    /* 对Z电机写入绝对位置 */
    if(command_head == "/200003"&&command_mid == " ") 
    {
      z_steps = command_text.toInt();
      stepperZ.moveTo(z_steps);
      Serial.print("Z电机位置设置为：");
      Serial.println(z_steps);
    }

    /* 对A电机写入绝对位置 */
    if(command_head == "/200004"&&command_mid == " ") 
    {
      a_steps = command_text.toInt();
      stepperA.moveTo(a_steps);
      Serial.print("A电机位置设置为：");
      Serial.println(a_steps);
    }

    /* 获取全体电机绝对位置 */
    if(command_head == "/300000") 
    {
      Serial.print("X电机位置：");
      Serial.println(stepperX.currentPosition());
      Serial.print("Y电机位置：");
      Serial.println(stepperY.currentPosition());
      Serial.print("Z电机位置：");
      Serial.println(stepperZ.currentPosition());
    }

    /* 重设X电机绝对位置 */
    if(command_head == "/300001") 
    {
      stepperX.setCurrentPosition(0);
      Serial.print("X电机坐标系已经重建");
      Serial.println(x_steps);
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
      Serial.println(z_steps);
    }

    /* 重设A电机绝对位置 */
    if(command_head == "/300004") 
    {
      stepperA.setCurrentPosition(0);
      Serial.print("X电机坐标系已经重建");
      Serial.println(a_steps);
    }


    /* 启动自校准程序 */
    if(command_head == "/400000") 
    {
      stepper_calibration();
    }

    if(command_head == "/500000") 
    {
      task_enable = !task_enable;
    }
  }
}

void stepper_stop()
{
  x_stop = !digitalRead(x_stopPin);
  y_stop = !digitalRead(y_stopPin);
  z_stop = !digitalRead(z_stopPin);
}



void stepper_calibration(){
    int flag = 1;
    while (flag)
    {
        x_stop = digitalRead(x_stopPin);
        y_stop = digitalRead(y_stopPin);
        z_stop = digitalRead(z_stopPin);
        if (x_stop && y_stop && z_stop)
        {
            flag = 0;
            stepperX.setCurrentPosition(0);
            stepperY.setCurrentPosition(0);
            stepperZ.setCurrentPosition(0);
            stepperX.moveTo(0);
            stepperY.moveTo(0);
            stepperZ.moveTo(0);
            Serial.println("calibration complete");
            Serial.print("X电机位置：");
            Serial.println(stepperX.currentPosition());
            Serial.print("Y电机位置：");
            Serial.println(stepperY.currentPosition());
            Serial.print("Z电机位置：");
            Serial.println(stepperZ.currentPosition());
        }
        if(!x_stop){  //如果读取到限位开关未被按下，则继续向限位开关移动
            stepperX.move(-2000);
            stepperX.run();
            Serial.println("X move");
        }
        if(!y_stop){  
            stepperY.move(-2000);
            stepperY.run();
            Serial.println("Y move");
        }
        else if (flag == 1)
        {
          flag = 2;
        }
        if(!z_stop && flag == 2 ){  
            stepperZ.move(-2000);
            stepperZ.run();
            Serial.println("Z move");
        }
    }
}

// 电机运动函数
// 此函数需要保留在循环中电机才会运动，此函数不会阻塞执行
void stepper_run(){ 
    x_stop = digitalRead(x_stopPin);
    y_stop = digitalRead(y_stopPin);
    z_stop = digitalRead(z_stopPin);
    //  如果碰到限位开关则重新校准坐标系
    if(x_stop){
        stepperX.setCurrentPosition(0);
        stepperX.runToNewPosition(100);
        Serial.print("X电机位置：");
        Serial.println(stepperX.currentPosition());
    }
    if(y_stop){  
        stepperY.setCurrentPosition(0);
        stepperY.runToNewPosition(100);
        Serial.print("Y电机位置：");
        Serial.println(stepperY.currentPosition());
    }
    if(z_stop){  
        stepperZ.setCurrentPosition(0);
        stepperZ.runToNewPosition(100);
        Serial.print("Z电机位置：");
        Serial.println(stepperZ.currentPosition());
    }

    //  如果在范围内允许运动
    if(stepperX.targetPosition()>=0 && stepperX.targetPosition()<=x_max){
        stepperX.run();
    }
    if(stepperY.targetPosition()>=0 && stepperY.targetPosition()<=y_max){  
        stepperY.run();
    }
    if(stepperZ.targetPosition()>=0 && stepperZ.targetPosition()<=z_max){  
        stepperZ.run();
    }

    // 如果超出最大限制则不执行
    if(stepperX.targetPosition()>x_max){  
        Serial.println("X电机目标位置超限制！");
        stepperX.moveTo(stepperX.currentPosition());
    }
    if(stepperY.targetPosition()>y_max){  
        Serial.println("Y电机目标位置超限制！");
        stepperY.moveTo(stepperY.currentPosition());
    }
    if(stepperZ.targetPosition()>z_max){  
        Serial.println("Z电机目标位置超限制！");
        stepperZ.moveTo(stepperZ.currentPosition());
    }

    // 如果位负数则碰到限位开关停止
    if(!x_stop && stepperX.targetPosition()<0){
        stepperX.run();
        Serial.print("X电机位置：");
        Serial.println(stepperX.currentPosition());
    }
    if(!y_stop && stepperY.targetPosition()<0){  
        stepperY.run();
        Serial.print("Y电机位置：");
        Serial.println(stepperY.currentPosition());
    }
    if(!z_stop && stepperZ.targetPosition()<0){  
        stepperZ.run();
        Serial.print("Z电机位置：");
        Serial.println(stepperZ.currentPosition());
    }
}

void stepper_move(int x,int y,int z){
  stepperX.moveTo(x);
  stepperY.moveTo(y);
  stepperZ.moveTo(z);
}


void stepper_task(int task[][3],int row){
  int x_pos = stepperX.currentPosition();
  int y_pos = stepperY.currentPosition();
  int z_pos = stepperZ.currentPosition();
  static int task_flag = 0;
  if (x_pos == task[task_flag][0] && y_pos == task[task_flag][1] && z_pos == task[task_flag][2]) task_flag++;
  if(task_flag<row){
    stepper_move(task[task_flag][0],task[task_flag][1],task[task_flag][2]);
  }
  if(task_flag==row)
  {
    task_flag = 0;
  }
}
