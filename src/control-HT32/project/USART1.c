//-----------------------------------------------------------------------------
#include "USART1.h"


u8  Serial_RxPacket1[6];

u8 Serial_RxFlag = 0;
//-----------------------------------------------------------------------------
__ALIGN4 _USART1_STRUCT rxd_scomm1;
__ALIGN4 _USART1_STRUCT txd_scomm1;

void USART1_tx_data(u8 *pt, u8 len);
//-----------------------------------------------------------------------------
void USART1_Configuration(void)
{
  USART_InitTypeDef USART_InitStruct;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.USART1   = 1;
  CKCUClock.Bit.AFIO     = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_4, AFIO_FUN_USART_UART);  // Config AFIO mode
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_5, AFIO_FUN_USART_UART);  // Config AFIO mode

  USART_InitStruct.USART_BaudRate = 115200;
  USART_InitStruct.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStruct.USART_StopBits = USART_STOPBITS_1;
  USART_InitStruct.USART_Parity = USART_PARITY_NO;
  USART_InitStruct.USART_Mode = USART_MODE_NORMAL;
  USART_Init(HT_USART1, &USART_InitStruct);
  USART_RxCmd(HT_USART1, ENABLE);
  USART_TxCmd(HT_USART1, ENABLE);

  USART_RXTLConfig(HT_USART1, USART_RXTL_04);
  USART_TXTLConfig(HT_USART1, USART_TXTL_04);
  USART_IntConfig(HT_USART1, USART_INT_RXDR | USART_INT_TOUT, ENABLE);
  HT_USART1->TPR = 0x80 | 40;

  NVIC_EnableIRQ(USART1_IRQn);
  USART1_init_buffer();
}

//-----------------------------------------------------------------------------
void USART1_init_buffer (void)
{
  rxd_scomm1.read_pt = 0;
  rxd_scomm1.write_pt = 0;
  rxd_scomm1.cnt = 0;

  txd_scomm1.read_pt = 0;
  txd_scomm1.write_pt = 0;
  txd_scomm1.cnt = 0;
}

//-----------------------------------------------------------------------------
void USART1_IRQHandler(void)
{
	u8 aaa[2] = {0x01, 0x02};
  // Tx, move data from buffer to UART FIFO
  if ((HT_USART1->SR) & USART_FLAG_TXC)
  {
    if (!txd_scomm1.cnt)
    {
      USART_IntConfig(HT_USART1, USART_INT_TXDE, DISABLE);
    }
    else
    {
      u16 i;
      for (i = 0; i < USART1_FIFO_LEN; i++)   //have FIFO?
      {
        USART_SendData(HT_USART1, txd_scomm1.buffer[txd_scomm1.read_pt]);
        txd_scomm1.read_pt = (txd_scomm1.read_pt + 1) % USART1_BUF_SIZE;
        txd_scomm1.cnt--;
        if (!txd_scomm1.cnt)
          break;
      }
    }
  }

	
  // Rx, move data from UART FIFO to buffer
  if ((HT_USART1->SR) & USART_FLAG_RXDR)
  {
    while(USART_GetFIFOStatus(HT_USART1, USART_FIFO_RX))
    {
      rxd_scomm1.buffer[rxd_scomm1.write_pt] = USART_ReceiveData(HT_USART1);

      rxd_scomm1.write_pt = (rxd_scomm1.write_pt + 1) % USART1_BUF_SIZE;
      rxd_scomm1.cnt++;
			
    }
		
//		USART1_tx_data(rxd_scomm1.buffer+rxd_scomm1.write_pt-rxd_scomm1.cnt, rxd_scomm1.cnt);
//		USART1_tx_data(rxd_scomm1.buffer, 6);
  }
  if ((HT_USART1->SR) & USART_FLAG_TOUT)
  {
    USART_ClearFlag(HT_USART1, USART_FLAG_TOUT);
    while(USART_GetFIFOStatus(HT_USART1, USART_FIFO_RX))
    {
      rxd_scomm1.buffer[rxd_scomm1.write_pt] = USART_ReceiveData(HT_USART1);

      rxd_scomm1.write_pt = (rxd_scomm1.write_pt + 1) % USART1_BUF_SIZE;
      rxd_scomm1.cnt++;
			
    }
  }
}


/**
  * 函    数：获取串口接收数据包标志位
  * 参    数：无
  * 返 回 值：串口接收数据包标志位，范围：0~1，接收到数据包后，标志位置1，读取后标志位自动清零
  */
u8 Serial_GetRxFlag(void)
{
	if (Serial_RxFlag == 1)			//如果标志位为1
	{
		Serial_RxFlag = 0;
		return 1;					//则返回1，并自动清零标志位
	}
	return 0;						//如果标志位为0，则返回0
}
/************************************接收上位机数据*******************************************/
void USART1_analyzer_data(void)
{
  u8 tmp;
	int i = 0;
  
  // 检查接收缓冲区是否有数据
  if(!rxd_scomm1.cnt)
    return;

  // 从接收缓冲区中读取数据
  tmp = rxd_scomm1.buffer[rxd_scomm1.read_pt];

  // 判断读取到的数据是否等于帧头 0xFF
  if(tmp == 0xFF)
  {
    // 读取下一个字节
    tmp = rxd_scomm1.buffer[(rxd_scomm1.read_pt + 1) % USART1_BUF_SIZE];

    // 判断下一个字节是否等于0xFE
    if(tmp == 0xFE)
    {
      // 如果帧头正确且接收缓冲区中的数据数量大于等于6
      if(rxd_scomm1.cnt >= 6)
      {
        // 将接收到的数据放在Serial_RxPacket数组中
        for(i = 0; i < 6; i++)
        {
          Serial_RxPacket1[i] = rxd_scomm1.buffer[(rxd_scomm1.read_pt  + i) % USART1_BUF_SIZE];
//					Serial_RxPacket1[i] = rxd_scomm1.buffer[i];
					if(i == 5)
					{
						Serial_RxFlag = 1;
					}
        }
					
        // 减少接收缓冲区中的数据数量
        rxd_scomm1.cnt -= 6;
        
        // 更新读取指针，指向下一个数据
        rxd_scomm1.read_pt = (rxd_scomm1.read_pt + 6) % USART1_BUF_SIZE;
      }
    }
  }
}




//-----------------------------------------------------------------------------
void USART1_tx_data(u8 *pt, u8 len)
{
  while(len--)
  {
    txd_scomm1.buffer[txd_scomm1.write_pt] = *pt++;
    txd_scomm1.write_pt = (txd_scomm1.write_pt + 1) % USART1_BUF_SIZE;
    NVIC_DisableIRQ(USART1_IRQn);
    txd_scomm1.cnt++;
    NVIC_EnableIRQ(USART1_IRQn);
  }

  if(txd_scomm1.cnt)
    USART_IntConfig(HT_USART1,  USART_INT_TXDE , ENABLE);
}

//-----------------------------------------------------------------------------
void USART1_test(void)
{
  u8 i,test_array[8];
  for(i=0; i<8; i++)
  {
    test_array[i] = i;
  }
  USART1_tx_data(test_array, 8);
}

//-----------------------------------------------------------------------------



