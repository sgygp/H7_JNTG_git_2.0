#include "usb_device.h"
#include "main.h"

#include "string.h"
#include <H7ADC.h>
//#include "H7UART.h"
#include "OW25Qxx.h"
#include "H7Power.h"
#include "Week.h"
#include "Screen.h"
#include "ff.h"
#include "Anjian.h"
#include "math.h"
#include "Set.h"


#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

kiss_fft_cpx 	cx_in[nFFT];
kiss_fft_cpx 	cx_out[nFFT];//FFT后，把cx_in中的加速度数据搬到后半部分来
kiss_fft_cpx 	st_tmp[nFFT+34];//fft后，后半部分包含幅值（平方和再开方）
uint16_t 		adcData[nFFT]		__attribute__((section(".RAM_CD")));;//ADC数据缓冲区
float *fftJieguo;
float *fftJieguoTMP;
float *shiyuShuju;
uint16_t 		bufLCD[240*135] 		__attribute__((section(".DTCMRAM1")));//液晶数据缓冲区
u8*				_bmp;//[nFFT]		 	__attribute__((section(".DTCMRAM2")));//读取图像文件的临时缓冲区
char 			nameItem[32][20] 		__attribute__((section(".DTCMRAM2")));
char 			zhushiItem[32][128] 	__attribute__((section(".DTCMRAM2")));
char 			canshuTxt[4096]		__attribute__((section(".DTCMRAM2")));
u8 				work[4096] 				__attribute__((section(".DTCMRAM2")));
u8 				bCeDianchi=1;
u8 				bGaiTime=0;
u32				kaijiTime=0;
u8					bUSB=0;
int main(void)
{

	__HAL_RCC_GPIOH_CLK_ENABLE();
	__HAL_RCC_GPIOA_CLK_ENABLE();
	__HAL_RCC_GPIOB_CLK_ENABLE();
	__HAL_RCC_GPIOC_CLK_ENABLE();
	__HAL_RCC_GPIOD_CLK_ENABLE();
	__HAL_RCC_GPIOE_CLK_ENABLE();
	Set m_set;
	m_set.init();

	MPU_Config();
	SCB_EnableDCache();
	SCB_EnableICache();
	HAL_Init();//初始化定时器
	SystemClock_Config();//配置时钟并重新初始化定时器
	HAL_Delay(200);

	fftJieguo=(float *)&st_tmp[nFFT/2];
	shiyuShuju=(float *)&cx_out[nFFT/2];
	fftJieguoTMP=(float *)&st_tmp;
	_bmp=(u8 *)&st_tmp;


	H7Power m_power;
	m_power.init();

	OW25Qxx m_o25;
	m_o25.init();
	FATFS fs[1];
	f_mount(&fs[0], "0:", 0); //加载驱动器

	Screen m_screen(fs);
	m_screen.init();
	m_screen.LOGO();
	MX_USB_DEVICE_Init();
	HAL_Delay(2500);
	Set::setUSBinput();

	Anjian::init();
	H7ADC m_ad;
	m_ad.init();
	m_ad.setCedianchi(true,10000);
 	u32 ms=0;
 	bool b;
	while (1)
	{
		m_ad.run();
		if(uwTick!=ms)//uwTick
		{
			b=Anjian::run();
			ms=uwTick;
			m_screen.run(ms,b);
			m_power.run(ms);//主要功能是判断是否修改了config文件，以便时钟校准
			if(ms%100==0)
				m_power.getRTCtime();
		}
	}

}


/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct;
	memset(&RCC_OscInitStruct,0,sizeof(RCC_OscInitStruct));

	RCC_ClkInitTypeDef RCC_ClkInitStruct ;
	memset(&RCC_ClkInitStruct,0,sizeof(RCC_ClkInitStruct));

	/** Supply configuration update enable
	 */
	HAL_PWREx_ConfigSupply(PWR_LDO_SUPPLY);

	/** Configure the main internal regulator output voltage
	 */
	__HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE0);

	while(!__HAL_PWR_GET_FLAG(PWR_FLAG_VOSRDY)) {}

	/*AXI clock gating */
	RCC->CKGAENR = 0xFFFFFFFF;


	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI48|RCC_OSCILLATORTYPE_HSE|RCC_OSCILLATORTYPE_LSE;
	RCC_OscInitStruct.HSEState = RCC_HSE_ON;
	RCC_OscInitStruct.LSEState = RCC_LSE_ON;
	RCC_OscInitStruct.HSI48State = RCC_HSI48_ON;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
	RCC_OscInitStruct.PLL.PLLM = 5;
	RCC_OscInitStruct.PLL.PLLN = 56;
	RCC_OscInitStruct.PLL.PLLP = 2;
	RCC_OscInitStruct.PLL.PLLQ = 2;
	RCC_OscInitStruct.PLL.PLLR = 2;
	RCC_OscInitStruct.PLL.PLLRGE = RCC_PLL1VCIRANGE_2;
	RCC_OscInitStruct.PLL.PLLVCOSEL = RCC_PLL1VCOWIDE;
	RCC_OscInitStruct.PLL.PLLFRACN = 0;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2
			|RCC_CLOCKTYPE_D3PCLK1|RCC_CLOCKTYPE_D1PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.SYSCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_HCLK_DIV1;
	RCC_ClkInitStruct.APB3CLKDivider = RCC_APB3_DIV2;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_APB1_DIV2;
	RCC_ClkInitStruct.APB2CLKDivider = RCC_APB2_DIV2;
	RCC_ClkInitStruct.APB4CLKDivider = RCC_APB4_DIV2;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_6) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/* MPU Configuration */

void MPU_Config(void)
{
	MPU_Region_InitTypeDef MPU_InitStruct ;
	memset(&MPU_InitStruct,0,sizeof(MPU_InitStruct));

	/* Disables the MPU */
	HAL_MPU_Disable();

	/** Initializes and configures the Region and the memory to be protected

	MPU_InitStruct.Enable = MPU_REGION_ENABLE;
	MPU_InitStruct.Number = MPU_REGION_NUMBER0;
	MPU_InitStruct.BaseAddress = 0x0;
	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	MPU_InitStruct.SubRegionDisable = 0x87;
	MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
	MPU_InitStruct.AccessPermission = MPU_REGION_NO_ACCESS;
	MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_DISABLE;
	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);*/

	/* 配置AXI SRAM的MPU属性为Write back, Read allocate，Write allocate */
	MPU_InitStruct.Enable           = MPU_REGION_ENABLE;
	MPU_InitStruct.BaseAddress      = 0x24000000;
	MPU_InitStruct.Size             = MPU_REGION_SIZE_1MB;
	MPU_InitStruct.SubRegionDisable = 0x00;
	//	MPU_InitStruct.BaseAddress = 0x0;
	//	MPU_InitStruct.Size = MPU_REGION_SIZE_4GB;
	//	MPU_InitStruct.SubRegionDisable = 0x87;

	MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
	MPU_InitStruct.Number           = MPU_REGION_NUMBER0;
	MPU_InitStruct.TypeExtField     = MPU_TEX_LEVEL1;
	MPU_InitStruct.IsBufferable     = MPU_ACCESS_BUFFERABLE;
	MPU_InitStruct.IsCacheable      = MPU_ACCESS_CACHEABLE;
	MPU_InitStruct.IsShareable      = MPU_ACCESS_NOT_SHAREABLE;
	//	MPU_InitStruct.IsShareable = MPU_ACCESS_SHAREABLE;
	//	MPU_InitStruct.IsCacheable = MPU_ACCESS_NOT_CACHEABLE;
	//	MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;

	HAL_MPU_ConfigRegion(&MPU_InitStruct);


	HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);

}
void Error_Handler(void)
{
	NVIC_SystemReset();
#if 0
	__disable_irq();
	while (1)
	{
	}
#endif
}


#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
	//trace_printf("Wrong parameters value: file %s on line %d\r\n", file, line) ;
}
#endif /* USE_FULL_ASSERT */
