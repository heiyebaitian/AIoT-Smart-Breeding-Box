#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Serial.h"
#include "AD.h"

#include "PWM.h"
#include "Control.h"

#include "Motor.h"
#include "Gas.h"

#include "U.h"

int main(void)
{
	/*模块初始化*/
	Serial_Init();		//串口初始化
	OLED_Init();
	AD_Init();
	PWM_Init();
	Motor_Init();
	
	
	//用于测试
	USART2_Init();
	Gas_Init();

	//初始化数据帧头
	Box_Configuration[0] =0xFF;
	Box_Configuration[1] =0xFE;
	
	/*定时中断，用于接收气体数据，NVIC中断分组*/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	while (1)
	{
		 
		//串口发送液位信息
		Level_Information();
		
		//串口发送土壤湿度
		Soil_Control();
		
		//串口发送水泵状态信息
		Water_Switch();
		
		//串口发送风扇状态信息
		Fan_Switch();
			
		//串口发送生长灯状态信息
		Light_Switch();
		
		//气体传感器发送
//		Gas_sensor();
		
		Delay_ms(1000);
		//育种箱信息发送
		Send_Box_Configuration();

	}
}
