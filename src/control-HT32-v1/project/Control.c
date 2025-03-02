#include "ht32.h"
#include "Control.h"
#include "ADC.h"
#include "Delay.h"
#include "Motor.h"
#include "USART1.h"

/** 
  * @brief 育种箱控制中枢
  * @param HT_ADCn: where HT_ADCn is the selected ADC from the ADC peripherals.
  * @retval None
  */

u8  Box_Configuration[32];					//存放育种箱的所有数据，用于与上位机通信

/** 
  * @brief 液位传感器
  * @param 
  * @retval 0 or 1, 0:No water, 1:There is water
  */
	void Liqiud_State(void)
	{
		
		Box_Configuration[2] =  GPIO_ReadInBit( HT_GPIOC,  GPIO_PIN_9);
	}
	
	
	
	/** 
  * @brief 土壤传感器
  * @param 无
  * @retval 0~100；湿度增加
  */
	void Soil_Control(void)
	{
		u8 Soil1= 0;
		Soil1 = 100 - (uint16_t)((float)adc_result[0] / 4095 * 100); // 使用浮点数进行计算
		delay_ms(100);							//延时100ms，手动增加一些转换的间隔时间
	  Box_Configuration[3] =Soil1;
	}
	
	/** 
  * @brief 水泵状态
  * @param 无
  * @retval 将水泵的抽水速度反馈给上位机
  */
	 void Water_Switch(void)
  {
	 
	  Box_Configuration[4] = Motor_Water();
	  
  }
	
	/** 
  * @brief 风扇状态
  * @param 无
  * @retval 0~100；湿度增加
  */
	 void Fan_Switch(void)
  {
	 
	  Box_Configuration[5] = Motor_Fan();
	  
  }
	
	/** 
  * @brief 生长灯状态
  * @param 无
  * @retval 0~100；湿度增加
  */
	 void Light_Switch(void)
  {
	 
	  Box_Configuration[6] = Motor_Light();
	  
  }
	
	
	
	   /**
  * 函    数：将育种箱信息发送出去
  * 参    数：无
  * 返 回 值：无
	* 注意事项：memcpy(Gas_information,Box_Configuration,54);将上一次的数据存在Gas_information里
	*	如果气体传感器出现故障，则可以将上一次的数据发送给上位机，保证上位机有数据显示，
	* 直到传感器好了就刷新上位机数据
  */  
  
//发送信息给上位机
	void Send_Box_Configuration(void)
	{
		USART1_tx_data(Box_Configuration, 27);
	}
