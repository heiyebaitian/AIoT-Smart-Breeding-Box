//-----------------------------------------------------------------------------
#include "ht32.h"
#include "GPIO.h"
#include "ADC.h"
#include "USART0.h"
#include "USART1.h"
#include "GPTM0.h"

#include "Delay.h"


#include "Control.h"



//-----------------------------------------------------------------------------
int main(void)
{

  GPIO_Configuration();
  ADC_Configuration();
  USART0_Configuration();
  USART1_Configuration();
  GPTM0_Configuration();
	
	//定义通讯协议
	Box_Configuration[0] = 0xFF;
	Box_Configuration[1] = 0xFE;
	
  while (1)
  {
		
		//土壤信息处理
			 Soil_Control();
		
		Water_Switch();
		Fan_Switch();
		Light_Switch();
		
		//向上位机发送信息
		Send_Box_Configuration();
	
		
  
		//串口1数据解析
		USART1_analyzer_data();
		//串口0数据解析,将数据放进总数组
		USART0_analyzer_data();
//		if(Serial_GetRxFlag() == 1)
//		{
//			USART1_tx_data(Serial_RxPacket1, 6);
//		}

  }
}


