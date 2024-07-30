#ifndef __U_H
#define __U_H


extern uint8_t Serial_TxPacket1[];
extern uint8_t Serial_RxPacket1[];
uint8_t Serial_GetRxFlag1(void);
void USART2_IRQHandler(void);
void USART2_Init(void);

#endif
