#include "stm32f10x.h"                  // Device header
#include "PWM.h"

/**
  * 函    数：直流电机初始化
  * 参    数：无
  * 返 回 值：无
  */
void Motor_Init(void)
{
	/*开启时钟*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);		//开启GPIOA的时钟
	
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11| GPIO_Pin_12| GPIO_Pin_13| GPIO_Pin_14| GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);						//引脚初始化为推挽输出														
}


/**
  * 函    数：水泵直流电机开关
  * 参    数：staus上位机发送的水泵开关信息
  * 返 回 值：无
  */
uint8_t Motor_Water(uint16_t staus)
{
	if (staus == 0x01)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);	//PB10置高电平
		return 0x01;
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_10);	//PA10置低电平
		return 0x00;
	}
}

/**
  * 函    数:风扇直流电机开关
  * 参    数：staus上位机发送的风扇开关信息
  * 返 回 值：无
  */
uint8_t Motor_Fan(uint16_t staus)
{
	if (staus == 0x01)							//如果设置正转的速度值
	{		
		GPIO_ResetBits(GPIOB, GPIO_Pin_12);	//PA12置低电平
		return 0x01;
	}
	else									//否则，即设置反转的速度值
	{
		
		GPIO_SetBits(GPIOB, GPIO_Pin_12);	//PB12置高电平
		return 0x00;
		
	}
}

/**
  * 函    数:灯直流电机开关
  * 参    数：staus上位机发送的风扇开关信息
  * 返 回 值：无
  */
uint8_t Motor_Light(uint16_t staus)
{
	if (staus == 0x01)							//如果设置正转的速度值
	{
		GPIO_ResetBits(GPIOB, GPIO_Pin_14);	//PA4置低电平
		return 0x01;
	}
	else									//否则，即设置反转的速度值
	{
		
			GPIO_SetBits(GPIOB, GPIO_Pin_14);	//PB10置高电平
		return 0x00;
		
	}
}

/**
  * 函    数：直流电机设置速度模板
  * 参    数：Speed 要设置的速度，范围：-100~100
  * 返 回 值：无
  */
void Motor_SetSpeed(int8_t Speed)
{
	if (Speed >= 0)							//如果设置正转的速度值
	{
		GPIO_SetBits(GPIOB, GPIO_Pin_10);	//PB10置高电平
		GPIO_ResetBits(GPIOB, GPIO_Pin_11);	//PB11置低电平，设置方向为正转
	}
	else									//否则，即设置反转的速度值
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);	//PA4置低电平
		GPIO_SetBits(GPIOA, GPIO_Pin_5);	//PA5置高电平，设置方向为反转
		PWM_SetCompare3(-Speed);			//PWM设置为负的速度值，因为此时速度值为负数，而PWM只能给正数
	}
}
