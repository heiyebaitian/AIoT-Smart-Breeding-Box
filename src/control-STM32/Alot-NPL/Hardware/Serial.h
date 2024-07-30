#ifndef __SERIAL_H
#define __SERIAL_H

#include <stdio.h>

extern uint8_t Serial_TxPacket[];
extern uint16_t Serial_RxPacket[];
extern uint8_t Serial_RxPacket_Water[];				//水泵开关
extern uint8_t Serial_RxPacket_Fan[];				//风扇开关
extern uint8_t Serial_RxPacket_Light[];				//灯开关

void Serial_Init(void);
void Serial_SendByte(uint8_t Byte);
void Serial_SendArray(uint8_t *Array, uint16_t Length);
void Serial_SendString(char *String);
void Serial_SendNumber(uint32_t Number, uint8_t Length);
void Serial_Printf(char *format, ...);

void Serial_SendPacket(void);
uint8_t Serial_GetRxFlag(void);
void Send_Soil_moisture(uint16_t AD_Value1, uint16_t Length);

//育种箱专用
void Serial_SendArray1(uint16_t *Array, uint16_t Length);
void Serial_SendByte1(uint16_t Byte);
#endif

