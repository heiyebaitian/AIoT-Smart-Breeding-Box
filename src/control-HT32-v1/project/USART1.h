//-----------------------------------------------------------------------------
#ifndef __USART1_H
#define __USART1_H


//-----------------------------------------------------------------------------
#include "ht32.h"

//-----------------------------------------------------------------------------
#define USART1_BUF_SIZE 256
#define USART1_FIFO_LEN 1

//-----------------------------------------------------------------------------
typedef struct
{
  u8 buffer[USART1_BUF_SIZE];
  u16 write_pt;
  u16 read_pt;
  u16 cnt;
}_USART1_STRUCT;

//-----------------------------------------------------------------------------
void USART1_Configuration(void);
void USART1_init_buffer (void);
void USART1_analyzer_data(void);
void USART1_tx_data(u8 *pt, u8 len);
void USART1_test(void);

u8 Serial_GetRxFlag(void);

extern u8  Serial_RxPacket1[];
//-----------------------------------------------------------------------------
#endif

