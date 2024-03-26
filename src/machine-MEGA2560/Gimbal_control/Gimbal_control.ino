#include <Arduino.h>
#include <AccelStepper.h>  //AccelStepper库 步进电机控制

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

const int moveSteps = 200;    //测试电机运行使用的运行步数

AccelStepper stepperX(1,xstepPin,xdirPin);//建立步进电机对象1
AccelStepper stepperY(1,ystepPin,ydirPin);//建立步进电机对象2
AccelStepper stepperZ(1,zstepPin,zdirPin);//建立步进电机对象3
AccelStepper stepperA(1,astepPin,adirPin);//建立步进电机对象4

// 定义全局变量
int x_stop = 0;
int y_stop = 0;
int z_stop = 0;
int a_stop = 0;

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

// 声明自定义函数
void stepper_init();
void stepper_calibration();
void stepper_run();



void setup(){
    Serial.begin(115200);   // 初始化串口通讯
    stepper_init(); //初始化步进电机驱动
    stepper_calibration();  //步进电机自校准坐标系
}



void loop(){
    stepper_run();
}


/**
 * 初始化步进电机驱动
 * 该函数用于初始化步进电机驱动芯片A4988。
 * 它设置了步进电机的步进模式、方向、最大速度和加速度。
 * 同时，也为每个电机的停止状态设置了输入引脚，并初始化为高阻态。
 * 
 * 参数: 无
 * 返回值: 无
 */
void stepper_init(){
     // 设置X轴步进和方向引脚为输出模式
    pinMode(xstepPin,OUTPUT);
    pinMode(xdirPin,OUTPUT);
     // 设置Y轴步进和方向引脚为输出模式
    pinMode(ystepPin,OUTPUT);
    pinMode(ydirPin,OUTPUT);
    // 设置Z轴步进和方向引脚为输出模式
    pinMode(zstepPin,OUTPUT);
    pinMode(zdirPin,OUTPUT);
    // 设置A轴步进和方向引脚为输出模式
    pinMode(astepPin,OUTPUT);
    pinMode(adirPin,OUTPUT);  
     // 设置X、Y、Z轴停止信号引脚为输入pull-up模式
    pinMode(x_stopPin,INPUT_PULLUP);
    pinMode(y_stopPin,INPUT_PULLUP);
    pinMode(z_stopPin,INPUT_PULLUP);
    // 设置使能引脚为输出模式并将其置低电平以激活电机驱动
    pinMode(enablePin,OUTPUT);
    digitalWrite(enablePin,LOW);
    // 配置各个轴的最大速度和加速度
    stepperX.setMaxSpeed(x_vel);     // 设置X轴最大速度和加速度
    stepperX.setAcceleration(x_acc);
    stepperY.setMaxSpeed(y_vel);     // 设置Y轴最大速度和加速度
    stepperY.setAcceleration(y_acc);
    stepperZ.setMaxSpeed(z_vel);     // 设置Z轴最大速度和加速度
    stepperZ.setAcceleration(z_acc);
    stepperA.setMaxSpeed(a_vel);     // 设置A轴最大速度和加速度
    stepperA.setAcceleration(a_acc);  
}


/**
 * 步进电机校准函数
 * 无参数
 * 无返回值
 * 校准过程：不断读取X、Y、Z三个方向的限位开关状态，直到三个方向的限位开关都被按下，然后将三个步进电机的位置重置为0，并移动到原点。
 * 在校准过程中，如果发现任何一个方向的限位开关未被按下，则会向该方向继续移动步进电机，尝试触发限位开关。
 * 校准后范围：
 * X轴：0-13950(4000-13000)
 * Y轴：0-15250(4000-15000)
 * Z轴：0-11000
 */
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
        if(!z_stop){  
            stepperZ.move(-2000);
            stepperZ.run();
            Serial.println("Z move");
        }
    }
}


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

/**
 *  写入移动坐标函数
 * 
 * 该函数用于分别控制X、Y、Z三个方向上的步进电机移动到指定的位置。
 * 
 *  x X轴上的移动位置
 *  y Y轴上的移动位置
 *  z Z轴上的移动位置
 */
void stepper_move(int x,int y,int z){
  stepperX.moveTo(x);
  stepperY.moveTo(y);
  stepperZ.moveTo(z);
}




/**
 * 步进任务函数
 * 该函数用于根据给定的任务数组，控制步进电机执行一系列的位置移动任务。
 * 每个任务由一个包含 x, y, z 三个位置坐标的数组元素表示。函数会检查当前步进电机的位置，
 * 如果当前位置与当前任务的目标位置一致，则执行下一个任务点，直到所有任务完成。
 * 
 * @param task 一个二维整型数组，每个子数组包含三个整数，分别表示x, y, z方向上的目标位置。
 * @param row 表示任务数组的行数，即任务点的数量。
 * 
 * 注意：该函数需要持续循环以监测当前任务执行状态
 */
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