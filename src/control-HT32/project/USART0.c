//-----------------------------------------------------------------------------
#include "USART0.h"
#include "Control.h"

u16  Serial_RxPacket[17];

   
//-----------------------------------------------------------------------------
__ALIGN4 _USART0_STRUCT rxd_scomm0;
__ALIGN4 _USART0_STRUCT txd_scomm0;

//-----------------------------------------------------------------------------
void USART0_Configuration(void)
{
  USART_InitTypeDef USART_InitStruct;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.USART0   = 1;
  CKCUClock.Bit.AFIO     = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_2, AFIO_FUN_USART_UART);  // Config AFIO mode
  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_3, AFIO_FUN_USART_UART);  // Config AFIO mode

  USART_InitStruct.USART_BaudRate = 9600;
  USART_InitStruct.USART_WordLength = USART_WORDLENGTH_8B;
  USART_InitStruct.USART_StopBits = USART_STOPBITS_1;
  USART_InitStruct.USART_Parity = USART_PARITY_NO;
  USART_InitStruct.USART_Mode = USART_MODE_NORMAL;
  USART_Init(HT_USART0, &USART_InitStruct);
  USART_RxCmd(HT_USART0, ENABLE);
  USART_TxCmd(HT_USART0, ENABLE);

  USART_RXTLConfig(HT_USART0, USART_RXTL_04);
  USART_TXTLConfig(HT_USART0, USART_TXTL_04);
  USART_IntConfig(HT_USART0, USART_INT_RXDR | USART_INT_TOUT, ENABLE);
  HT_USART0->TPR = 0x80 | 40;

  NVIC_EnableIRQ(USART0_IRQn);
  USART0_init_buffer();
}

//-----------------------------------------------------------------------------
void USART0_init_buffer (void)
{
  rxd_scomm0.read_pt = 0;
  rxd_scomm0.write_pt = 0;
  rxd_scomm0.cnt = 0;

  txd_scomm0.read_pt = 0;
  txd_scomm0.write_pt = 0;
  txd_scomm0.cnt = 0;
}

//-----------------------------------------------------------------------------
void USART0_IRQHandler(void)
{
  // Tx, move data from buffer to UART FIFO
  if ((HT_USART0->SR) & USART_FLAG_TXC)
  {
    if (!txd_scomm0.cnt)
    {
      USART_IntConfig(HT_USART0, USART_INT_TXDE, DISABLE);
    }
    else
    {
      u16 i;
      for (i = 0; i < USART0_FIFO_LEN; i++)   //have FIFO?
      {
        USART_SendData(HT_USART0, txd_scomm0.buffer[txd_scomm0.read_pt]);
        txd_scomm0.read_pt = (txd_scomm0.read_pt + 1) % USART0_BUF_SIZE;
        txd_scomm0.cnt--;
        if (!txd_scomm0.cnt)
          break;
      }
    }
  }

  // Rx, move data from UART FIFO to buffer
  if ((HT_USART0->SR) & USART_FLAG_RXDR)
  {
    while(USART_GetFIFOStatus(HT_USART0, USART_FIFO_RX))
    {
      rxd_scomm0.buffer[rxd_scomm0.write_pt] = USART_ReceiveData(HT_USART0);

      rxd_scomm0.write_pt = (rxd_scomm0.write_pt + 1) % USART0_BUF_SIZE;
      rxd_scomm0.cnt++;
    }
  }

  if ((HT_USART0->SR) & USART_FLAG_TOUT)
  {
    USART_ClearFlag(HT_USART0, USART_FLAG_TOUT);
    while(USART_GetFIFOStatus(HT_USART0, USART_FIFO_RX))
    {
      rxd_scomm0.buffer[rxd_scomm0.write_pt] = USART_ReceiveData(HT_USART0);

      rxd_scomm0.write_pt = (rxd_scomm0.write_pt + 1) % USART0_BUF_SIZE;
      rxd_scomm0.cnt++;
    }
  }
}

//-----------------------------------------------------------------------------
//void USART0_analyzer_data(void)
//{
//  u8 tmp;
//  
//  // 检查接收缓冲区是否有数据
//  if(!rxd_scomm0.cnt)
//    return;

//  //...to be add

//  // 从接收缓冲区中读取数据
//  tmp = rxd_scomm0.buffer[rxd_scomm0.read_pt];

//  // 判断读取到的数据是否不等于 0xAA
//  if( (tmp != 0xAA) )            //for example
//  {
//    // 如果数据不等于 0xAA，则禁用 USART0 中断
//    NVIC_DisableIRQ(USART0_IRQn);
//    
//    // 减少接收缓冲区中的数据数量
//    rxd_scomm0.cnt--;
//    
//    // 重新启用 USART0 中断
//    NVIC_EnableIRQ(USART0_IRQn);
//    
//    // 更新读取指针，指向下一个数据
//    rxd_scomm0.read_pt = (rxd_scomm0.read_pt + 1) % USART0_BUF_SIZE;
//    
//    return;
//  }
//  // 如果读取到的数据等于 0xAA，并且接收缓冲区中的数据数量大于等于 8
//  else if(rxd_scomm0.cnt >= 8)    //for example
//  {
//    // 减少接收缓冲区中的数据数量
//    rxd_scomm0.cnt--;
//    
//    // 更新读取指针，指向下一个数据
//    rxd_scomm0.read_pt = (rxd_scomm0.read_pt + 1) % USART0_BUF_SIZE;
//  }
//}

/****************************接收气体传感器数据****************************************/
void USART0_analyzer_data(void)
{
	
  u8 tmp;
	int i = 0;
  
  // 检查接收缓冲区是否有数据
  if(!rxd_scomm0.cnt)
    return;

  // 从接收缓冲区中读取数据
  tmp = rxd_scomm0.buffer[rxd_scomm0.read_pt];

  // 判断读取到的数据是否等于帧头 0x3c
  if(tmp == 0x3c)
  {
    // 读取下一个字节
    tmp = rxd_scomm0.buffer[(rxd_scomm0.read_pt + 2) % USART0_BUF_SIZE];

		
    // 判断下一个字节是否等于0x02
    
      // 如果帧头正确且接收缓冲区中的数据数量大于等于17
      if(rxd_scomm0.cnt >= 17)
      {
				
				 for(i = 0; i < 17; i++)
        {
          Box_Configuration[ 7 + i] = rxd_scomm0.buffer[(rxd_scomm0.read_pt  + i) % USART0_BUF_SIZE];
      
				}

        // 减少接收缓冲区中的数据数量
        rxd_scomm0.cnt--;
        
        // 更新读取指针，指向下一个数据
        rxd_scomm0.read_pt = (rxd_scomm0.read_pt + 1) % USART0_BUF_SIZE;
      }
    
  }
}


//-----------------------------------------------------------------------------
void USART0_tx_data(u8 *pt, u8 len)
{
  while(len--)
  {
    txd_scomm0.buffer[txd_scomm0.write_pt] = *pt++;
    txd_scomm0.write_pt = (txd_scomm0.write_pt + 1) % USART0_BUF_SIZE;
    NVIC_DisableIRQ(USART0_IRQn);
    txd_scomm0.cnt++;
    NVIC_EnableIRQ(USART0_IRQn);
  }

  if(txd_scomm0.cnt)
    USART_IntConfig(HT_USART0,  USART_INT_TXDE , ENABLE);
}

//-----------------------------------------------------------------------------
void USART0_test(void)
{
  u8 i,test_array[8];
  for(i=0; i<8; i++)
  {
    test_array[i] = i;
  }
  USART0_tx_data(test_array, 8);
}

//-----------------------------------------------------------------------------


