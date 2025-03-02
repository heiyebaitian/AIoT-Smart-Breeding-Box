//-----------------------------------------------------------------------------
#ifndef __GPIO_H
#define __GPIO_H

#include "ht32.h"

//-----------------------------------------------------------------------------
#define IN_PA0_PORT           A
#define IN_PA0_PIN            0
#define IN_PA0_GPIO_CLK      STRCAT2(P,          IN_PA0_PORT)
#define IN_PA0_GPIO_ID       STRCAT2(GPIO_P,     IN_PA0_PORT)
#define IN_PA0_GPIO_PORT     STRCAT2(HT_GPIO,    IN_PA0_PORT)
#define IN_PA0_GPIO_PIN      STRCAT2(GPIO_PIN_,  IN_PA0_PIN)
#define IN_PA0_AFIO_MODE     (AFIO_MODE_GPIO)
#define IN_PA0_STATE         (GPIO_ReadInBit(IN_PA0_GPIO_PORT, IN_PA0_GPIO_PIN))

#define IN_PA3_PORT           A
#define IN_PA3_PIN            3
#define IN_PA3_GPIO_CLK      STRCAT2(P,          IN_PA3_PORT)
#define IN_PA3_GPIO_ID       STRCAT2(GPIO_P,     IN_PA3_PORT)
#define IN_PA3_GPIO_PORT     STRCAT2(HT_GPIO,    IN_PA3_PORT)
#define IN_PA3_GPIO_PIN      STRCAT2(GPIO_PIN_,  IN_PA3_PIN)
#define IN_PA3_AFIO_MODE     (AFIO_MODE_GPIO)
#define IN_PA3_STATE         (GPIO_ReadInBit(IN_PA3_GPIO_PORT, IN_PA3_GPIO_PIN))

#define IN_PA5_PORT           A
#define IN_PA5_PIN            5
#define IN_PA5_GPIO_CLK      STRCAT2(P,          IN_PA5_PORT)
#define IN_PA5_GPIO_ID       STRCAT2(GPIO_P,     IN_PA5_PORT)
#define IN_PA5_GPIO_PORT     STRCAT2(HT_GPIO,    IN_PA5_PORT)
#define IN_PA5_GPIO_PIN      STRCAT2(GPIO_PIN_,  IN_PA5_PIN)
#define IN_PA5_AFIO_MODE     (AFIO_MODE_GPIO)
#define IN_PA5_STATE         (GPIO_ReadInBit(IN_PA5_GPIO_PORT, IN_PA5_GPIO_PIN))

#define IN_PC9_PORT           C
#define IN_PC9_PIN            9
#define IN_PC9_GPIO_CLK      STRCAT2(P,          IN_PC9_PORT)
#define IN_PC9_GPIO_ID       STRCAT2(GPIO_P,     IN_PC9_PORT)
#define IN_PC9_GPIO_PORT     STRCAT2(HT_GPIO,    IN_PC9_PORT)
#define IN_PC9_GPIO_PIN      STRCAT2(GPIO_PIN_,  IN_PC9_PIN)
#define IN_PC9_AFIO_MODE     (AFIO_MODE_GPIO)
#define IN_PC9_STATE         (GPIO_ReadInBit(IN_PC9_GPIO_PORT, IN_PC9_GPIO_PIN))



//-----------------------------------------------------------------------------
#define OUT_PA2_PORT           A
#define OUT_PA2_PIN            2
#define OUT_PA2_GPIO_CLK      STRCAT2(P,          OUT_PA2_PORT)
#define OUT_PA2_GPIO_ID       STRCAT2(GPIO_P,     OUT_PA2_PORT)
#define OUT_PA2_GPIO_PORT     STRCAT2(HT_GPIO,    OUT_PA2_PORT)
#define OUT_PA2_GPIO_PIN      STRCAT2(GPIO_PIN_,  OUT_PA2_PIN)
#define OUT_PA2_AFIO_MODE     (AFIO_MODE_GPIO)
#define OUT_PA2_HIGH          OUT_PA2_GPIO_PORT->SRR    = OUT_PA2_GPIO_PIN
#define OUT_PA2_LOW           OUT_PA2_GPIO_PORT->RR     = OUT_PA2_GPIO_PIN
#define OUT_PA2_XOR           OUT_PA2_GPIO_PORT->DOUTR ^= OUT_PA2_GPIO_PIN

#define OUT_PA4_PORT           A
#define OUT_PA4_PIN            4
#define OUT_PA4_GPIO_CLK      STRCAT2(P,          OUT_PA4_PORT)
#define OUT_PA4_GPIO_ID       STRCAT2(GPIO_P,     OUT_PA4_PORT)
#define OUT_PA4_GPIO_PORT     STRCAT2(HT_GPIO,    OUT_PA4_PORT)
#define OUT_PA4_GPIO_PIN      STRCAT2(GPIO_PIN_,  OUT_PA4_PIN)
#define OUT_PA4_AFIO_MODE     (AFIO_MODE_GPIO)
#define OUT_PA4_HIGH          OUT_PA4_GPIO_PORT->SRR    = OUT_PA4_GPIO_PIN
#define OUT_PA4_LOW           OUT_PA4_GPIO_PORT->RR     = OUT_PA4_GPIO_PIN
#define OUT_PA4_XOR           OUT_PA4_GPIO_PORT->DOUTR ^= OUT_PA4_GPIO_PIN

#define OUT_PC4_PORT           C
#define OUT_PC4_PIN            4
#define OUT_PC4_GPIO_CLK      STRCAT2(P,          OUT_PC4_PORT)
#define OUT_PC4_GPIO_ID       STRCAT2(GPIO_P,     OUT_PC4_PORT)
#define OUT_PC4_GPIO_PORT     STRCAT2(HT_GPIO,    OUT_PC4_PORT)
#define OUT_PC4_GPIO_PIN      STRCAT2(GPIO_PIN_,  OUT_PC4_PIN)
#define OUT_PC4_AFIO_MODE     (AFIO_MODE_GPIO)
#define OUT_PC4_HIGH          OUT_PC4_GPIO_PORT->SRR    = OUT_PC4_GPIO_PIN
#define OUT_PC4_LOW           OUT_PC4_GPIO_PORT->RR     = OUT_PC4_GPIO_PIN
#define OUT_PC4_XOR           OUT_PC4_GPIO_PORT->DOUTR ^= OUT_PC4_GPIO_PIN

#define OUT_PC5_PORT           C
#define OUT_PC5_PIN            5
#define OUT_PC5_GPIO_CLK      STRCAT2(P,          OUT_PC5_PORT)
#define OUT_PC5_GPIO_ID       STRCAT2(GPIO_P,     OUT_PC5_PORT)
#define OUT_PC5_GPIO_PORT     STRCAT2(HT_GPIO,    OUT_PC5_PORT)
#define OUT_PC5_GPIO_PIN      STRCAT2(GPIO_PIN_,  OUT_PC5_PIN)
#define OUT_PC5_AFIO_MODE     (AFIO_MODE_GPIO)
#define OUT_PC5_HIGH          OUT_PC5_GPIO_PORT->SRR    = OUT_PC5_GPIO_PIN
#define OUT_PC5_LOW           OUT_PC5_GPIO_PORT->RR     = OUT_PC5_GPIO_PIN
#define OUT_PC5_XOR           OUT_PC5_GPIO_PORT->DOUTR ^= OUT_PC5_GPIO_PIN

#define OUT_PC8_PORT           C
#define OUT_PC8_PIN            8
#define OUT_PC8_GPIO_CLK      STRCAT2(P,          OUT_PC8_PORT)
#define OUT_PC8_GPIO_ID       STRCAT2(GPIO_P,     OUT_PC8_PORT)
#define OUT_PC8_GPIO_PORT     STRCAT2(HT_GPIO,    OUT_PC8_PORT)
#define OUT_PC8_GPIO_PIN      STRCAT2(GPIO_PIN_,  OUT_PC8_PIN)
#define OUT_PC8_AFIO_MODE     (AFIO_MODE_GPIO)
#define OUT_PC8_HIGH          OUT_PC8_GPIO_PORT->SRR    = OUT_PC8_GPIO_PIN
#define OUT_PC8_LOW           OUT_PC8_GPIO_PORT->RR     = OUT_PC8_GPIO_PIN
#define OUT_PC8_XOR           OUT_PC8_GPIO_PORT->DOUTR ^= OUT_PC8_GPIO_PIN



//-----------------------------------------------------------------------------
void GPIO_Configuration(void);

#endif

