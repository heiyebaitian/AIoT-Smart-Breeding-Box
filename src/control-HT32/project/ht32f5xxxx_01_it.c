//-----------------------------------------------------------------------------
#include "ht32.h"

//-----------------------------------------------------------------------------
void NMI_Handler(void)
{
  while (1);
}

//-----------------------------------------------------------------------------
void HardFault_Handler(void)
{
  while (1);
  //NVIC_SystemReset(); //For the final version
}

//-----------------------------------------------------------------------------
void SVC_Handler(void)
{
  while (1);
}

//-----------------------------------------------------------------------------
void PendSV_Handler(void)
{
  while (1);
}
