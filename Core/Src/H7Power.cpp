/*
 * H7Power.cpp
 *
 *  Created on: 2024年4月9日
 *      Author: Administrator
 */

#include "H7Power.h"
#include "string.h"
#include "myFile.h"
#include "Set.h"

RTC_HandleTypeDef H7Power::hrtc;
void H7Power::init(/*int sleepSec*/) {
	//首先使能备份域
	HAL_PWR_EnableBkUpAccess();

	memset(&hrtc,0,sizeof(hrtc));
	hrtc.Instance = RTC;
	hrtc.Init.HourFormat = RTC_HOURFORMAT_24;
	hrtc.Init.AsynchPrediv = 127;
	hrtc.Init.SynchPrediv = 255;
	hrtc.Init.OutPut = RTC_OUTPUT_DISABLE;
	hrtc.Init.OutPutPolarity = RTC_OUTPUT_POLARITY_HIGH;
	hrtc.Init.OutPutType = RTC_OUTPUT_TYPE_OPENDRAIN;
	hrtc.Init.OutPutRemap = RTC_OUTPUT_REMAP_NONE;
	if (HAL_RTC_Init(&hrtc) != HAL_OK)
	{
		Error_Handler();
	}
	HAL_PWREx_EnableBkUpReg(); //使能备份寄存器
	if(HAL_RTCEx_BKUPRead(&hrtc,30)!=0x55aa55aa)
	{
		RTC_DateTypeDef date;
		memset(&date,0,sizeof(date));
		RTC_TimeTypeDef time;
		memset(&time,0,sizeof(time));
		date.Year=24;
		date.Month=4;
		date.Date=29;
		date.WeekDay=RTC_WEEKDAY_SUNDAY;
		time.Hours=9;
		time.Minutes=40;
		time.Seconds=0;
		HAL_RTC_SetTime(&hrtc,&time,RTC_FORMAT_BIN);
		HAL_RTC_SetDate(&hrtc,&date,RTC_FORMAT_BIN);
		HAL_RTCEx_BKUPWrite(&hrtc,30,0x55aa55aa);//总共32个寄存器
		//以下测试备份RAM
		/*	__HAL_RCC_BKPRAM_CLK_SLEEP_DISABLE();
		__HAL_RCC_BKPRAM_CLK_ENABLE();
		u32 addr=SRD_BKPSRAM_BASE;

		for(int i=0;i<128;i++)
		{
		 *(u8 *)(addr+i)=i*2;
		}

		for(int i=0;i<128;i++)
		{
			backup_buf[i]=*(u8 *)(addr+i);
		}
		 */

	}
	HAL_PWREx_DisableBkUpReg();



	/*
	if (HAL_RTCEx_SetWakeUpTimer_IT(&hrtc, 2000*sleepSec, RTC_WAKEUPCLOCK_RTCCLK_DIV16) != HAL_OK)//wakeup time  = 20000 *(16/32000) = 10s
	{
		Error_Handler();
	}
	//必须先清除闹钟中断标志，否则一进入待机立即唤醒
	__HAL_RTC_WAKEUPTIMER_CLEAR_FLAG(&hrtc, RTC_FLAG_WUTF);
	 */
	//可以不用开中断
	//	HAL_NVIC_SetPriority(RTC_WKUP_IRQn, 0, 1);
	//	HAL_NVIC_EnableIRQ(RTC_WKUP_IRQn);


}

void H7Power::getRTCtime() {
	RTC_TimeTypeDef time;
	RTC_DateTypeDef date;
	HAL_RTC_GetTime(&hrtc,&time,RTC_FORMAT_BIN);
	HAL_RTC_GetDate(&hrtc,&date,RTC_FORMAT_BIN);
	time_[0]=date.Year+2000;
	time_[1]=date.Month;
	time_[2]=date.Date;
	time_[3]=time.Hours;
	time_[4]=time.Minutes;
	time_[5]=time.Seconds;

}

u32 H7Power::getWakeUpTimer() {

	return HAL_RTCEx_GetWakeUpTimer(&hrtc);
}

extern "C" void HAL_RTCEx_WakeUpTimerEventCallback(RTC_HandleTypeDef *hrtc)
{
	__HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);
}


extern "C" void  HAL_RTC_MspInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_CLK_ENABLE();

	RCC_PeriphCLKInitTypeDef PeriphClkInitStruct ;
	memset(&PeriphClkInitStruct,0,sizeof(PeriphClkInitStruct));

	PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_RTC;
	PeriphClkInitStruct.RTCClockSelection = RCC_RTCCLKSOURCE_LSE;
	if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/* Peripheral clock enable */
	__HAL_RCC_RTC_ENABLE();
	//__HAL_RCC_SYSCFG_CLK_ENABLE();

}
extern "C" void  HAL_RTC_MspDeInit(RTC_HandleTypeDef *hrtc)
{
	__HAL_RCC_RTC_CLK_DISABLE();

}
extern PCD_HandleTypeDef hpcd_USB_OTG_HS;

void H7Power::sleep() {
	//	return;
	Set::setOff_tft();
	for(int i=0;i<100;i++)//3秒
	{
		HAL_Delay(100);
		if(HAL_GPIO_ReadPin(GPIOC,GPIO_PIN_13) ==GPIO_PIN_RESET)
			break;
	}
	HAL_PWREx_DisableUSBVoltageDetector();
	HAL_PCD_Stop(&hpcd_USB_OTG_HS);
	HAL_PWREx_ClearWakeupFlag(PWR_WAKEUP_FLAG4);
	//	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN4_HIGH);
	//	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN4_HIGH);
	HAL_PWR_DisableWakeUpPin(PWR_WAKEUP_PIN4_LOW);
	HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN4_LOW);
	HAL_Delay(50);
	HAL_PWR_EnterSTANDBYMode();
}

void H7Power::setRTCtime() {
	HAL_PWREx_EnableBkUpReg(); //使能备份寄存器
	RTC_DateTypeDef date;
	memset(&date,0,sizeof(date));
	RTC_TimeTypeDef time;
	memset(&time,0,sizeof(time));
	date.Year=time_[0]%100;
	date.Month=time_[1];
	date.Date=time_[2];
	date.WeekDay=RTC_WEEKDAY_SUNDAY;
	time.Hours=time_[3];
	time.Minutes=time_[4];
	time.Seconds=time_[5];
	HAL_RTC_SetTime(&hrtc,&time,RTC_FORMAT_BIN);
	HAL_RTC_SetDate(&hrtc,&date,RTC_FORMAT_BIN);
	HAL_PWREx_DisableBkUpReg();
}

void H7Power::run(u32 ms) {
	static u32 fileTime=0;
	if(!bGaiTime)
		return;
	if(fileTime==0)
		fileTime=ms;
	else
	{
		if((ms-fileTime)>3000)
		{
			u32 ftime=MyFile::getFileTime();
			if(ftime!=kaijiTime)
			{
				kaijiTime=ftime;
				u16 tt;
				ftime+=3;//2秒后改时间
				tt=ftime;
				time_[3]=tt>>11;
				time_[4]=(tt>>5)&0x3f;
				time_[5]=(tt&0x1f)<<1;

				tt=ftime>>16;
				time_[0]=(tt>>9)+1980;
				time_[1]=(tt>>5)&0x0f;
				time_[2]=tt&0x1f;

				setRTCtime();
			}
			bGaiTime=0;
			fileTime=0;
		}
	}
}
