#include "stm32f10x.h"
#include "stm32f10x_usart.h"
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "Delay.h"
#include "Control.h"


uint16_t Serial_TxPacket1[16];				//定义发送数据包数组，数据包格式：FF 01 02 03 04 FE
uint16_t Serial_RxPacket1[16];				//定义接收数据包数组
uint16_t Serial_RxFlag1;					//定义接收数据包标志位

void USART2_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    USART_InitTypeDef USART_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    USART_InitStructure.USART_BaudRate = 9600;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);

    USART_ITConfig(USART2, USART_IT_RXNE, ENABLE);
    USART_Cmd(USART2, ENABLE);
	
}


uint8_t Serial_GetRxFlag1(void)
{
	if (Serial_RxFlag1 == 1)			//如果标志位为1
	{
		Serial_RxFlag1 = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}



void USART2_IRQHandler(void)
{
	static uint8_t RxState1 = 0;		//定义表示当前状态机状态的静态变量
	static uint8_t pRxPacket1 = 6;	//定义表示当前接收数据位置的静态变量
	if (USART_GetITStatus(USART2, USART_IT_RXNE) == SET)		//判断是否是USART1的接收事件触发的中断
	{
		uint8_t RxData = USART_ReceiveData(USART2);				//读取数据寄存器，存放在接收的数据变量
		
		/*使用状态机的思路，依次处理数据包的不同部分*/
		
		/*当前状态为0，接收数据包包头*/
		if (RxState1 == 0)
		{
			if (RxData == 0x3C)			//如果数据确实是包头
			{
				RxState1 = 1;			//置下一个状态
				pRxPacket1 = 0;			//数据包的位置归零
			}
		}
		/*当前状态为1，接收数据包数据*/
		else if (RxState1 == 1)
		{
			Serial_RxPacket1[pRxPacket1] = RxData;	//将数据存入数据包数组的指定位置
			pRxPacket1 ++;				//数据包的位置自增
			if (pRxPacket1 > 16)			//如果收够4个数据
			{
				RxState1 = 2;			//置下一个状态
			}
		}
		/*当前状态为2，接收数据包包尾*/
		else if (RxState1 == 2)
		{
						//如果数据确实是包尾部
			
				RxState1 = 0;			//状态归0
				Serial_RxFlag1 = 1;		//接收数据包标志位置1，成功接收一个数据包
		
		
		}
		
		USART_ClearITPendingBit(USART2, USART_IT_RXNE);		//清除标志位
	}
}
