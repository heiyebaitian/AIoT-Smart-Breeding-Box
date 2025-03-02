//-----------------------------------------------------------------------------
#include "GPIO.h"

//-----------------------------------------------------------------------------
void GPIO_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.PA    = 1;
  CKCUClock.Bit.PC    = 1;
  CKCUClock.Bit.AFIO  = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);


  GPIO_DirectionConfig    (IN_PA0_GPIO_PORT, IN_PA0_GPIO_PIN, GPIO_DIR_IN);
  GPIO_InputConfig        (IN_PA0_GPIO_PORT, IN_PA0_GPIO_PIN, ENABLE);
  GPIO_PullResistorConfig (IN_PA0_GPIO_PORT, IN_PA0_GPIO_PIN, GPIO_PR_UP);

  GPIO_DirectionConfig    (IN_PA3_GPIO_PORT, IN_PA3_GPIO_PIN, GPIO_DIR_IN);
  GPIO_InputConfig        (IN_PA3_GPIO_PORT, IN_PA3_GPIO_PIN, ENABLE);
  GPIO_PullResistorConfig (IN_PA3_GPIO_PORT, IN_PA3_GPIO_PIN, GPIO_PR_UP);

  GPIO_DirectionConfig    (IN_PA5_GPIO_PORT, IN_PA5_GPIO_PIN, GPIO_DIR_IN);
  GPIO_InputConfig        (IN_PA5_GPIO_PORT, IN_PA5_GPIO_PIN, ENABLE);
  GPIO_PullResistorConfig (IN_PA5_GPIO_PORT, IN_PA5_GPIO_PIN, GPIO_PR_UP);

  GPIO_DirectionConfig    (IN_PC9_GPIO_PORT, IN_PC9_GPIO_PIN, GPIO_DIR_IN);
  GPIO_InputConfig        (IN_PC9_GPIO_PORT, IN_PC9_GPIO_PIN, ENABLE);
  GPIO_PullResistorConfig (IN_PC9_GPIO_PORT, IN_PC9_GPIO_PIN, GPIO_PR_UP);


  GPIO_ClearOutBits       (OUT_PA2_GPIO_PORT, OUT_PA2_GPIO_PIN);
  GPIO_DirectionConfig    (OUT_PA2_GPIO_PORT, OUT_PA2_GPIO_PIN, GPIO_DIR_OUT);

  GPIO_ClearOutBits       (OUT_PA4_GPIO_PORT, OUT_PA4_GPIO_PIN);
  GPIO_DirectionConfig    (OUT_PA4_GPIO_PORT, OUT_PA4_GPIO_PIN, GPIO_DIR_OUT);

  GPIO_ClearOutBits       (OUT_PC4_GPIO_PORT, OUT_PC4_GPIO_PIN);
  GPIO_DirectionConfig    (OUT_PC4_GPIO_PORT, OUT_PC4_GPIO_PIN, GPIO_DIR_OUT);

  GPIO_ClearOutBits       (OUT_PC5_GPIO_PORT, OUT_PC5_GPIO_PIN);
  GPIO_DirectionConfig    (OUT_PC5_GPIO_PORT, OUT_PC5_GPIO_PIN, GPIO_DIR_OUT);

  GPIO_ClearOutBits       (OUT_PC8_GPIO_PORT, OUT_PC8_GPIO_PIN);
  GPIO_DirectionConfig    (OUT_PC8_GPIO_PORT, OUT_PC8_GPIO_PIN, GPIO_DIR_OUT);
}

//-----------------------------------------------------------------------------


