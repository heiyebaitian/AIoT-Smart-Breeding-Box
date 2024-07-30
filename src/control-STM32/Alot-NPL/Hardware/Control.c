#include "stm32f10x.h"                  // Device header
#include "Serial.h"
#include "AD.h"
#include "PWM.h"
#include "Liquid.h"
#include "Delay.h"
#include "U.h"
#include "Motor.h"
#include "Gas.h"

extern uint16_t AD_Value[4];
extern uint8_t Serial_RxPacket_Water[1];				//水泵开关
uint16_t Box_Configuration[30];							//串口发送信息，FF，FE，液位信息， 土壤湿度，
uint16_t Gas_information[16];            //用于接收上一次的水泵信息

/**
  * 函    数：液体传感器发送数据
  * 参    数：无
  * 返 回 值：无
  */
  
	void Level_Information(void)
	{
		uint8_t sensor_value = 0;
		sensor_value = GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_12);
		Box_Configuration[2] =sensor_value;
		
	}
	
	
/**
  * 函    数：土壤湿度通过串口发送
  * 参    数：无
  * 返 回 值：无
  */
  
  void Soil_Control(void)
  {
	  uint16_t Soil1 = 0;
	  /*
	  *由于传感器随湿度增大，电压减小，通过100-电压实现数据与湿度增长呈正比
	  */
		Soil1 = 100 - (uint16_t)((float)AD_Value[0] / 4095 * 100); // 使用浮点数进行计算
		Delay_ms(100);							//延时100ms，手动增加一些转换的间隔时间
	  Box_Configuration[3] =Soil1;
  }
  

/**
  * 函    数：接收数据控制水泵，并返回水泵状态给上位机
  * 参    数：无
  * 返 回 值：无
  */  
  void Water_Switch(void)
  {
	  uint16_t Water_Switch1 = Serial_RxPacket[1];
	  int Water_Status = 0;
	  Box_Configuration[4] = Motor_Water(Water_Switch1);
	  
  }
 /**
  * 函    数：接收数据控制风扇，并返回风扇状态给上位机
  * 参    数：无
  * 返 回 值：无
  */  
  
    void Fan_Switch(void)
  {
	  int Fan_Status = 0;
	  uint16_t Fan_Switch = Serial_RxPacket[2];
	 
	  Box_Configuration[5] = Motor_Fan(Fan_Switch);
  }
  
    
    /**
  * 函    数：接收数据控制灯，并返回灯状态给上位机
  * 参    数：无
  * 返 回 值：无
  */  
  
    void Light_Switch(void)
  {
	  int Light_Status = 0;
	  uint16_t Light_Switch = Serial_RxPacket[3];	  
	  Box_Configuration[6] = Motor_Light(Light_Switch);
  }
  
  
     /**
  * 函    数：接收七合一串口信息接收并转存到数组固定位置
  * 参    数：无
  * 返 回 值：无
  */  
  void Gas_sensor(void)
  {
		USART2_Init();
	  
		uint16_t i = 1;
		if (Serial_GetRxFlag1() == 1)	//如果接收到气体数据
	    {
			for(i = 1; i < 15; i++)
			{
				Box_Configuration[6 + i] = Serial_RxPacket1[i-1];
			}
		}
	  
  }

   /**
  * 函    数：将育种箱信息发送出去
  * 参    数：无
  * 返 回 值：无
  */  
  void Send_Box_Configuration(void)
  {
   
    memcpy(Gas_information,Box_Configuration,54);
	  if(Gas_GetRxFlag() == 1)
	  {
		Serial_SendArray1(Box_Configuration, 27);
	  }
    else
    {
    Serial_SendArray1(Gas_information,27);
    }
  }




