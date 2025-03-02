//-----------------------------------------------------------------------------
#ifndef __USART0_H
#define __USART0_H


//-----------------------------------------------------------------------------
#include "ht32.h"

//-----------------------------------------------------------------------------
#define USART0_BUF_SIZE 256
#define USART0_FIFO_LEN 1

//-----------------------------------------------------------------------------
typedef struct
{
  u8 buffer[USART0_BUF_SIZE];
  u16 write_pt;
  u16 read_pt;
  u16 cnt;
}_USART0_STRUCT;

//-----------------------------------------------------------------------------
void USART0_Configuration(void);
void USART0_init_buffer (void);
void USART0_analyzer_data(void);
void USART0_tx_data(u8 *pt, u8 len);
void USART0_test(void);

extern u16  Serial_RxPacket[17];
//-----------------------------------------------------------------------------
#endif

