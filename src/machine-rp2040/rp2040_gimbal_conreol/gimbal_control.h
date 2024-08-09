// gimbal_control.h  
#ifndef GIMBAL_CONREOL_H  
#define GIMBAL_CONREOL_H  


#include <Arduino.h> 
#include <stdint.h>
#include <AccelStepper.h> 
#include <Servo.h>

extern Servo myservo;

/* GPIO配置 */
extern const int enablePin;  // 使能控制引脚

extern const int xdirPin;     // x方向控制引脚
extern const int xstepPin;    // x步进控制引脚
extern const int ydirPin;     // y方向控制引脚
extern const int ystepPin;    // y步进控制引脚
extern const int zdirPin;     // z方向控制引脚
extern const int zstepPin;    // z步进控制引脚
extern const int x_stopPin;   // x方向光电门限位引脚
extern const int y_stopPin;   // y方向光电门限位引脚
extern const int z_stopPin;   // z方向光电门限位引脚
extern const int crawlPin;  // 机械臂引脚
extern const int dripPin;   //水泵引脚
extern const int x_max;       // x方向最大步进数
extern const int y_max;       // y方向最大步进数
extern const int z_max;       // z方向最大步进数

extern AccelStepper stepperX;//建立步进电机对象X
extern AccelStepper stepperY;//建立步进电机对象Y
extern AccelStepper stepperZ;//建立步进电机对象Z

/* 定义全局变量 */
// 信号停止状态标志
extern int x_stop;
extern int y_stop;
extern int z_stop;

// 步进电机步数
extern int x_steps;
extern int y_steps;
extern int z_steps;

// 运动速度
extern float x_vel;
extern float y_vel;
extern float z_vel;

// 运动加速度
extern float x_acc;
extern float y_acc;
extern float z_acc;





void driver_init();
void stepper_stop();
void stepper_calibration();
void stepper_run();
void stepper_move(int x,int y,int z);
int Motor_Water(int staus);



#endif //MQTT_DRIVER_H