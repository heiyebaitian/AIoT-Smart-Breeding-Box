#include "ht32f5xxxx_01.h"


void delay_ms(uint32_t ms)
{
    uint32_t i, j;

    for (i = 0; i < ms; i++)
    {
        for (j = 0; j < 10000; j++) // 假设单片机主频为10MHz
        {
            __NOP(); // 空指令，用于延时
        }
    }
}