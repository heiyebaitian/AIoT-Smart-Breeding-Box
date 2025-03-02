#ifndef __MOTOR_H
#define __MOTOR_H

void Motor_Init(void);
void Motor_SetSpeed(int8_t Speed);

uint8_t Motor_Water(uint16_t staus);
uint8_t Motor_Fan(uint16_t staus);
uint8_t Motor_Light(uint16_t staus);

#endif
