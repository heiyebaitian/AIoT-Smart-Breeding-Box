#include "ht32.h"
#include "USART1.h"

	/** 
  * @brief 返回水泵状态
  * @param 上位机发送水泵信号
  * @retval 0：关， 1：开
  */
u8 Motor_Water(void)
{
	u8 Water = Serial_RxPacket1[3];
	if(Water == 0x01)
	{
        GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_4);
//		 GPIO_SetOutBits(HT_GPIOC,  GPIO_PIN_4);
		return 0x01;
	}
	else
	{
        GPIO_SetOutBits(HT_GPIOC,  GPIO_PIN_4);
//		GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_4);
		return 0x00;
	}
}

/** 
  * @brief 返回风扇状态
  * @param 上位机发送水泵信号
  * @retval 0：关， 1：开
  */
u8 Motor_Fan(void)
{
	u8 Fan = Serial_RxPacket1[4];
	if(Fan == 0x01)
	{
//		 GPIO_SetOutBits(HT_GPIOC,  GPIO_PIN_5);
        GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_5);
		return 0x01;
	}
	else
	{
        GPIO_SetOutBits(HT_GPIOC,  GPIO_PIN_5);
//		GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_5);
		return 0x00;
	}
}

/** 
  * @brief 返回生长灯状态
  * @param 上位机发送水泵信号
  * @retval 0：关， 1：开
  */
u8 Motor_Light(void)
{
	u8 Staus = Serial_RxPacket1[5];
	if(Staus == 0x01)
	{
        GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_8);
//		 GPIO_SetOutBits(HT_GPIOC,  GPIO_PIN_6);
		return 0x01;
	}
	else
	{
//		GPIO_ClearOutBits(HT_GPIOC, GPIO_PIN_6);
        GPIO_SetOutBits(HT_GPIOC,  GPIO_PIN_8);
		return 0x00;
	}
}
