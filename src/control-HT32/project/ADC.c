//-----------------------------------------------------------------------------
#include "ADC.h"

//-----------------------------------------------------------------------------
vu8 adc_result_update;
vu16 adc_result[14];

//-----------------------------------------------------------------------------
void ADC_Configuration(void)
{
  CKCU_PeripClockConfig_TypeDef CKCUClock = {{0}};
  CKCUClock.Bit.ADC   = 1;
  CKCUClock.Bit.AFIO  = 1;
  CKCU_PeripClockConfig(CKCUClock, ENABLE);

  ADC_Reset(HT_ADC);
  CKCU_SetADCPrescaler(CKCU_ADCPRE_DIV64);    // Setting ADCLK frequency

  AFIO_GPxConfig(GPIO_PA, AFIO_PIN_0, AFIO_FUN_ADC);  // Config AFIO mode

  ADC_RegularChannelConfig(HT_ADC, ADC_CH_0, 0);


  ADC_RegularGroupConfig(HT_ADC, ONE_SHOT_MODE, 1, 1);  // Mode, Length , SubLength
  ADC_RegularTrigConfig(HT_ADC, ADC_TRIG_SOFTWARE);     //Trigger mode
  ADC_SamplingTimeConfig(HT_ADC, 0);
  ADC_IntConfig(HT_ADC, ADC_INT_CYCLE_EOC, ENABLE);
  ADC_Cmd(HT_ADC, ENABLE);
  NVIC_EnableIRQ(ADC_IRQn);
  ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);   // Trigger to start
}


//-----------------------------------------------------------------------------
void ADC_IRQHandler(void)
{
  if(ADC_GetIntStatus(HT_ADC, ADC_INT_CYCLE_EOC))
  {
    ADC_ClearIntPendingBit(HT_ADC, ADC_FLAG_CYCLE_EOC);
    adc_result[0] = HT_ADC->DR[0] & 0x0FFF;
    adc_result_update = 1;
    ADC_SoftwareStartConvCmd(HT_ADC, ENABLE);   // Trigger to NEXT start?
  }
}


//-----------------------------------------------------------------------------


