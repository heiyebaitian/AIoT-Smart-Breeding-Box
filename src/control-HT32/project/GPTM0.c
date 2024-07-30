//-----------------------------------------------------------------------------
#include "GPTM0.h"

//-----------------------------------------------------------------------------
#define GPTM0_TIMER_BASE            ((long long)SystemCoreClock * 10/1000)

//-----------------------------------------------------------------------------
vu32 gptm0_ct;

//-----------------------------------------------------------------------------
void GPTM0_Configuration(void)
{
  u32 wCRR = 0, wPSCR = 0;
  TM_TimeBaseInitTypeDef TM_TimeBaseInitStruct;
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{ 0 }};
  CKCUClock.Bit.GPTM0      = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  wCRR = GPTM0_TIMER_BASE - 1;
  while ((wCRR / (wPSCR + 1)) > 0xFFFF)
  {
    wPSCR++;
  }
  wCRR = wCRR / (wPSCR + 1);

  TM_TimeBaseInitStruct.CounterReload = wCRR;
  TM_TimeBaseInitStruct.Prescaler = wPSCR;
  TM_TimeBaseInitStruct.RepetitionCounter = 0;
  TM_TimeBaseInitStruct.CounterMode = TM_CNT_MODE_UP;
  TM_TimeBaseInitStruct.PSCReloadTime = TM_PSC_RLD_IMMEDIATE;
  TM_TimeBaseInit(HT_GPTM0, &TM_TimeBaseInitStruct);

  TM_ClearFlag(HT_GPTM0, TM_INT_UEV);
  TM_IntConfig(HT_GPTM0, TM_INT_UEV, ENABLE);
  NVIC_EnableIRQ(GPTM0_IRQn);   // interrupt enable

  TM_Cmd(HT_GPTM0, ENABLE);
}


//-----------------------------------------------------------------------------
void GPTM0_IRQHandler(void)
{
  TM_ClearFlag(HT_GPTM0, TM_INT_UEV);
  gptm0_ct++;
}

//-----------------------------------------------------------------------------


