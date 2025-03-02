// gimbal_control.cpp
#include "gimbal_control.h"


/* 初始化函数 */
void driver_init(){
    pinMode(xstepPin,OUTPUT);     
    pinMode(xdirPin,OUTPUT);     
    pinMode(ystepPin,OUTPUT);     
    pinMode(ydirPin,OUTPUT);      
    pinMode(zstepPin,OUTPUT);    
    pinMode(zdirPin,OUTPUT);     
    pinMode(x_stopPin,INPUT_PULLUP);     
    pinMode(y_stopPin,INPUT_PULLUP);     
    pinMode(z_stopPin,INPUT_PULLUP); 
    pinMode(crawlPin,OUTPUT);
    pinMode(dripPin, OUTPUT);    

    pinMode(enablePin,OUTPUT);   // 使能引脚为输出模式
    digitalWrite(enablePin,HIGH); // 将使能控制引脚设置为高电平从而让电机驱动板进入工作状态             

  // 写入运动速度加速度参数
    stepperX.setMaxSpeed(x_vel);     
    stepperX.setAcceleration(x_acc);   
    stepperY.setMaxSpeed(y_vel);     
    stepperY.setAcceleration(y_acc);  
    stepperZ.setMaxSpeed(z_vel);     
    stepperZ.setAcceleration(z_acc); 

    Serial1.setRX(1);
    Serial1.setTX(0);
    Serial1.begin(115200);   // 初始化串口通讯
    Serial.begin(115200);   // 初始化串口通讯
    stepper_calibration();
}


void stepper_stop()
{
    x_stop = digitalRead(x_stopPin);
    y_stop = digitalRead(y_stopPin);
    z_stop = digitalRead(z_stopPin);
}



void stepper_calibration(){
    int flag = 1;
    while (flag)
    {
        x_stop = digitalRead(x_stopPin);
        y_stop = digitalRead(y_stopPin);
        z_stop = digitalRead(z_stopPin);
        myservo.write(70); 
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
            //Serial.println("X move");
        }
        if(!y_stop){  
            stepperY.move(-2000);
            stepperY.run();
            //Serial.println("Y move");
        }
        else if (flag == 1)
        {

            flag = 2;
        }
        if(!z_stop && flag == 2 ){  
            stepperZ.move(-2000);
            stepperZ.run();
            //Serial.println("Z move");
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

/********************水泵*******************/
int Motor_Water(int staus)
{
	if (staus == 0x01)						
	{
		digitalWrite(dripPin, HIGH);	
		return 0x01;
	}
	else									
	{
		digitalWrite(dripPin, LOW);	
		return 0x00;
	}
}
