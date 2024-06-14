/*
 * H7ADC.cpp
 *
 *  Created on: 2024年4月1日
 *      Author: Administrator
 */
#include "H7ADC.h"
#include "string.h"
#include "../KissFFT/kiss_fft.h"
ALIGN_32BYTES  (uint16_t H7ADC::aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]);
DMA_HandleTypeDef H7ADC::hdma_adc1;
int H7ADC::curLEN=0;
bool H7ADC::bAD_OK1=false;
bool H7ADC::bAD_OK2=false;
ADC_HandleTypeDef H7ADC::hadc1;
u32 H7ADC::heZhi;
u32 H7ADC::heCishu;
void H7ADC::setCedianchi(bool b,int pinlv)
{
	ADC_ChannelConfTypeDef sConfig;
	bCeDianchi=b;
	HAL_ADC_Stop_DMA(&hadc1);
	HAL_ADC_Stop(&hadc1);
	if(b)
	{
		sConfig.Channel = ADC_CHANNEL_8;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
		sConfig.SingleDiff = ADC_SINGLE_ENDED;
		sConfig.OffsetNumber = ADC_OFFSET_NONE;
		sConfig.Offset = 0;
		sConfig.OffsetSignedSaturation = DISABLE;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}
		curLEN=-1;
		heZhi=0;
	}else
	{
		sConfig.Channel = ADC_CHANNEL_5;
		sConfig.Rank = ADC_REGULAR_RANK_1;
		sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
		sConfig.SingleDiff = ADC_SINGLE_ENDED;
		sConfig.OffsetNumber = ADC_OFFSET_NONE;
		sConfig.Offset = 0;
		sConfig.OffsetSignedSaturation = DISABLE;
		if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
		{
			Error_Handler();
		}

		curLEN=0;
	}
	TIM_HandleTypeDef  htim ;
	TIM_OC_InitTypeDef sConfig1 ;
	memset(&htim,0,sizeof(htim));
	memset(&sConfig1,0,sizeof(sConfig1));

	int a=SystemCoreClock/pinlv;
	int Prescaler=1;
	if(a>65000)
	{
		Prescaler=a/65000+1;
	}
	int arr=SystemCoreClock/Prescaler/pinlv;

	htim.Instance = TIM4;
	htim.Init.Period            = arr-1;//10K
	htim.Init.Prescaler         = Prescaler-1;
	htim.Init.ClockDivision     = 0;
	htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
	htim.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&htim);

	sConfig1.OCMode     = TIM_OCMODE_PWM1;
	sConfig1.OCPolarity = TIM_OCPOLARITY_LOW;
	/* 占空比50% */
	sConfig1.Pulse = (htim.Init.Period+1)/2;

	if(HAL_TIM_OC_ConfigChannel(&htim, &sConfig1, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
	/* 启动OC1 */
	if(HAL_TIM_OC_Start(&htim, TIM_CHANNEL_4) != HAL_OK)
	{
		Error_Handler();
	}
//	init();
	BSP_AdcStart();
}
void H7ADC::init() {

	memset(&hadc1,0,sizeof(hadc1));
	memset(&hdma_adc1,0,sizeof(hdma_adc1));

	/* DMA controller clock enable */
	__HAL_RCC_DMA1_CLK_ENABLE();

	hdma_adc1.Instance = DMA1_Stream0;
	hdma_adc1.Init.Request = DMA_REQUEST_ADC1;
	hdma_adc1.Init.Direction = DMA_PERIPH_TO_MEMORY;
	hdma_adc1.Init.PeriphInc = DMA_PINC_DISABLE;
	hdma_adc1.Init.MemInc = DMA_MINC_ENABLE;
	hdma_adc1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
	hdma_adc1.Init.MemDataAlignment = DMA_MDATAALIGN_HALFWORD;
	hdma_adc1.Init.Mode = DMA_CIRCULAR;
	hdma_adc1.Init.Priority = DMA_PRIORITY_LOW;
	hdma_adc1.Init.FIFOMode = DMA_FIFOMODE_DISABLE;
	if (HAL_DMA_Init(&hdma_adc1) != HAL_OK)
	{
		Error_Handler();
	}

	__HAL_LINKDMA(&hadc1,DMA_Handle,hdma_adc1);

	/* DMA interrupt init */
	/* DMA1_Stream0_IRQn interrupt configuration */
	HAL_NVIC_SetPriority(DMA1_Stream0_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(DMA1_Stream0_IRQn);


	hadc1.Instance = ADC1;
	hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
	hadc1.Init.Resolution = ADC_RESOLUTION_16B;
	hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
	//hadc1.Init.ScanConvMode = ADC_SCAN_ENABLE;
	hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
	hadc1.Init.LowPowerAutoWait = DISABLE;
	hadc1.Init.ContinuousConvMode = DISABLE;
	//	hadc1.Init.ContinuousConvMode = ENABLE;
	hadc1.Init.NbrOfConversion = 1;
	//hadc1.Init.NbrOfConversion = 2;
	hadc1.Init.DiscontinuousConvMode = DISABLE;
	hadc1.Init.ExternalTrigConv = ADC_EXTERNALTRIG_T4_CC4;
	hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_RISING;
	hadc1.Init.ConversionDataManagement = ADC_CONVERSIONDATA_DMA_CIRCULAR;
	hadc1.Init.Overrun = ADC_OVR_DATA_PRESERVED;
	hadc1.Init.LeftBitShift = ADC_LEFTBITSHIFT_NONE;
	hadc1.Init.OversamplingMode = DISABLE;
	if (HAL_ADC_Init(&hadc1) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure the ADC multi-mode
	 */
	ADC_MultiModeTypeDef multimode;
	ADC_ChannelConfTypeDef sConfig;
	memset(&multimode,0,sizeof(multimode));
	memset(&sConfig,0,sizeof(sConfig));
	multimode.Mode = ADC_MODE_INDEPENDENT;
	if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
	{
		Error_Handler();
	}

	/** Configure Regular Channel
	 */
	/*
	sConfig.Channel = ADC_CHANNEL_5;
	sConfig.Rank = ADC_REGULAR_RANK_1;
	sConfig.SamplingTime = ADC_SAMPLETIME_64CYCLES_5;
	sConfig.SingleDiff = ADC_SINGLE_ENDED;
	sConfig.OffsetNumber = ADC_OFFSET_NONE;
	sConfig.Offset = 0;
	sConfig.OffsetSignedSaturation = DISABLE;
	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	{
		Error_Handler();
	}
	 */
	//	sConfig.Channel = ADC_CHANNEL_19;
	//	sConfig.Rank = ADC_REGULAR_RANK_2;
	//	if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
	//	{
	//		Error_Handler();
	//	}

}
void H7ADC::run() {
	if(H7ADC::bAD_OK1)
	{
		H7ADC::bAD_OK1=false;
		if(bCeDianchi)
		{
			H7ADC::heCishu++;
			for(u32 i=0;i<ADC_CONVERTED_DATA_BUFFER_SIZE/2;i++)
				H7ADC::heZhi+=H7ADC::aADCxConvertedData[i];

		}else
		{
			if(curLEN<0)
				return;
			memcpy((u8*)&adcData[curLEN],(u8*)&H7ADC::aADCxConvertedData[0],ADC_CONVERTED_DATA_BUFFER_SIZE);
			curLEN+=(ADC_CONVERTED_DATA_BUFFER_SIZE/2);
		}
	}
	if(H7ADC::bAD_OK2)
	{
		H7ADC::bAD_OK2=false;
		if(bCeDianchi)
		{
			H7ADC::heCishu++;
			for(u32 i=0;i<ADC_CONVERTED_DATA_BUFFER_SIZE/2;i++)
				H7ADC::heZhi+=H7ADC::aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE/2+i];
		}else
		{
			if(curLEN<0)
				return;
			memcpy((u8*)&adcData[curLEN],(u8*)&H7ADC::aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE/2],ADC_CONVERTED_DATA_BUFFER_SIZE);
			curLEN+=(ADC_CONVERTED_DATA_BUFFER_SIZE/2);
		}
	}
	if(bCeDianchi)
		return;

	if(curLEN>=nFFT)
	{
		curLEN=-1;
		setCedianchi(1,10000);
	}

}

void H7ADC::BSP_AdcStart(void) {
	HAL_ADCEx_Calibration_Start(&hadc1, ADC_CALIB_OFFSET, ADC_SINGLE_ENDED);

	HAL_ADC_Start_DMA(&hadc1,
			(uint32_t *)&aADCxConvertedData,
			ADC_CONVERTED_DATA_BUFFER_SIZE
	);
}
extern "C" void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the first half of the ADC converted data buffer: 32 bytes
		 使数据缓存无效，以便在ADC转换数据缓冲区的下半部分获得SRAM的更新内容:32字节*/
	SCB_InvalidateDCache_by_Addr((uint32_t *) &H7ADC::aADCxConvertedData[0], ADC_CONVERTED_DATA_BUFFER_SIZE);
	H7ADC::bAD_OK1=true;
}

/**
 * @brief  Conversion DMA half-transfer callback in non-blocking mode
 * @param  hadc: ADC handle
 * @retval None
 */
extern "C" void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc)
{
	/* Invalidate Data Cache to get the updated content of the SRAM on the second half of the ADC converted data buffer: 32 bytes */
	SCB_InvalidateDCache_by_Addr((uint32_t *) &H7ADC::aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE/2], ADC_CONVERTED_DATA_BUFFER_SIZE);
	H7ADC::bAD_OK2=true;
}


extern "C" void HAL_ADC_MspInit(ADC_HandleTypeDef* adcHandle)
{
	GPIO_InitTypeDef GPIO_InitStruct ;
	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct;
	memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
	memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));

	if(adcHandle->Instance==ADC1)
	{
		/* USER CODE BEGIN ADC1_MspInit 0 */
		RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ;
		memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));
		PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_ADC;
		PeriphClkInitStruct.PLL3.PLL3M = 5;
		PeriphClkInitStruct.PLL3.PLL3N = 36;
		PeriphClkInitStruct.PLL3.PLL3P = 2;
		PeriphClkInitStruct.PLL3.PLL3Q = 2;
		PeriphClkInitStruct.PLL3.PLL3R = 5;
		PeriphClkInitStruct.PLL3.PLL3RGE = RCC_PLL3VCIRANGE_2;
		PeriphClkInitStruct.PLL3.PLL3VCOSEL = RCC_PLL3VCOWIDE;
		PeriphClkInitStruct.PLL3.PLL3FRACN = 0;
		PeriphClkInitStruct.AdcClockSelection = RCC_ADCCLKSOURCE_PLL3;
		if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
		{
			Error_Handler();
		}
		__HAL_RCC_ADC12_CLK_ENABLE();

		GPIO_InitStruct.Pin = GPIO_PIN_1;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

		GPIO_InitStruct.Pin = GPIO_PIN_5;
		GPIO_InitStruct.Mode = GPIO_MODE_ANALOG;
		GPIO_InitStruct.Pull = GPIO_NOPULL;
		HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

	}
}

extern "C" void HAL_ADC_MspDeInit(ADC_HandleTypeDef* adcHandle)
{

	if(adcHandle->Instance==ADC1)
	{
		/* USER CODE BEGIN ADC1_MspDeInit 0 */

		/* USER CODE END ADC1_MspDeInit 0 */
		/* Peripheral clock disable */
		__HAL_RCC_ADC12_CLK_DISABLE();

		/**ADC1 GPIO Configuration
    PA5     ------> ADC1_INP19
		 */
		HAL_GPIO_DeInit(GPIOA, GPIO_PIN_5);

		/* ADC1 DMA DeInit */
		HAL_DMA_DeInit(adcHandle->DMA_Handle);
		/* USER CODE BEGIN ADC1_MspDeInit 1 */

		/* USER CODE END ADC1_MspDeInit 1 */
	}
}


extern "C" void HAL_TIM_Base_MspInit(TIM_HandleTypeDef *htim)
{
	/* TIM6 时钟使能 */
	if(htim->Instance==TIM4)
		__HAL_RCC_TIM4_CLK_ENABLE();
}

/*
 *********************************************************************************************************
 *	函 数 名: HAL_TIM_Base_MspDeInit
 *	功能说明: 复位定时器时钟
 *	形    参: htim  TIM_HandleTypeDef类型结构体指针变量
 *	返 回 值: 无
 *********************************************************************************************************
 */
extern "C" void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef *htim)
{
	/*##-1- 复位外设 ##################################################*/
	if(htim->Instance==TIM4)
	{
		__HAL_RCC_TIM4_FORCE_RESET();
		__HAL_RCC_TIM4_RELEASE_RESET();
	}
}

/*
 *********************************************************************************************************
 *	函 数 名: TIM6_Config
 *	功能说明: 配置TIM6作为DAC触发源
 *	形    参: _freq : 采样频率，单位Hz
 *	返 回 值: 无
 *********************************************************************************************************
 */
/*
static void TIM6_Config(uint32_t _freq)
{
	static TIM_HandleTypeDef  htim;
	TIM_MasterConfigTypeDef sMasterConfig;

	##-1- Configure the TIM peripheral #######################################
	 Time base configuration
	htim.Instance = TIM6;

	htim.Init.Period            = (SystemCoreClock / 2) / _freq - 1;
	htim.Init.Prescaler         = 0;
	htim.Init.ClockDivision     = 0;
	htim.Init.CounterMode       = TIM_COUNTERMODE_UP;
	htim.Init.RepetitionCounter = 0;
	HAL_TIM_Base_Init(&htim);

	 TIM6 TRGO selection
	sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
	sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;

	HAL_TIMEx_MasterConfigSynchronization(&htim, &sMasterConfig);

	##-2- Enable TIM peripheral counter ######################################
	HAL_TIM_Base_Start(&htim);
}
 */
