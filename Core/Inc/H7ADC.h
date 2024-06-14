/*
 * H7ADC.h
 *
 *  Created on: 2024年4月1日
 *      Author: Administrator
 */

#ifndef ADCH7_H_
#define ADCH7_H_
#include "main.h"
#define ADC_CONVERTED_DATA_BUFFER_SIZE   ((uint32_t) 512)   /* Size of array aADCxConvertedData[] */

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* hadc);
void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc);

class H7ADC {
public:
	 __IO uint16_t AdcValue;
	static ALIGN_32BYTES (uint16_t   aADCxConvertedData[ADC_CONVERTED_DATA_BUFFER_SIZE]);
	static ADC_HandleTypeDef hadc1;
	static DMA_HandleTypeDef hdma_adc1;
	static bool bAD_OK1;
	static bool bAD_OK2;
	static u32 heZhi;
	static u32 heCishu;

	static void setCedianchi(bool b,int pinlv);
	static void BSP_AdcStart(void);
	void run();
	static void init();
	static int curLEN;
};

#endif /* ADCH7_H_ */
