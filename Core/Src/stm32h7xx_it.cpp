#include <H7UART.h>
#include "main.h"
#include "H7ADC.h"
#include "H7Power.h"
#include "Tft114.h"
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;
extern "C" void SysTick_Handler(void)
{
	HAL_IncTick();
}
extern "C" void DMA1_Stream0_IRQHandler(void)
{
	HAL_DMA_IRQHandler(&H7ADC::hdma_adc1);
}

extern "C" void USART1_IRQHandler(void)
{
	UartIRQ(&H7UART::g_tUart1);
}

extern "C" void RTC_WKUP_IRQHandler(void)
{
	HAL_RTCEx_WakeUpTimerIRQHandler(&H7Power::hrtc);
}
extern "C" void PWR_WKUP_INT_IRQHandler(void)
{
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0);
}
extern "C" void OTG_HS_IRQHandler(void)
{
  /* USER CODE BEGIN OTG_HS_IRQn 0 */

  /* USER CODE END OTG_HS_IRQn 0 */
  HAL_PCD_IRQHandler(&hpcd_USB_OTG_HS);
  /* USER CODE BEGIN OTG_HS_IRQn 1 */

  /* USER CODE END OTG_HS_IRQn 1 */
}
extern "C" void BDMA2_Channel0_IRQHandler(void)
{
  /* USER CODE BEGIN BDMA2_Channel0_IRQn 0 */

  /* USER CODE END BDMA2_Channel0_IRQn 0 */
  HAL_DMA_IRQHandler(&Tft114::hdma_spi6_tx);
  /* USER CODE BEGIN BDMA2_Channel0_IRQn 1 */

  /* USER CODE END BDMA2_Channel0_IRQn 1 */
}



extern "C" void NMI_Handler(void)
{
	while (1)
	{
	}
}

/**
 * @brief This function handles Hard fault interrupt.
 */
extern "C" void HardFault_Handler(void)
{
	while (1)
	{
	}
}
extern "C" void MemManage_Handler(void)
{
	while (1)
	{
	}
}

extern "C" void BusFault_Handler(void)
{
	while (1)
	{
	}
}

extern "C" void UsageFault_Handler(void)
{
	while (1)
	{
	}
}
extern "C" void SVC_Handler(void)
{
}

extern "C" void DebugMon_Handler(void)
{
}

extern "C" void PendSV_Handler(void)
{
}
