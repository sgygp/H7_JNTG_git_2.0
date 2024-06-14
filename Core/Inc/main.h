/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.h
 * @brief          : Header for main.c file.
 *                   This file contains the common defines of the application.
 ******************************************************************************
 * @attention
 *
 * Copyright (c) 2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H
#include "stm32h7xx_hal.h"
#include "kiss_fft.h"
#define nFFT 32768
#define QUCHUDIPIN_H 1000
#define FENGGESHU 20

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wunused-parameter"
#pragma GCC diagnostic ignored "-Wmissing-declarations"
#pragma GCC diagnostic ignored "-Wreturn-type"

#ifdef __cplusplus
extern "C" {
#endif

extern  kiss_fft_cpx 	cx_in[nFFT];
extern  kiss_fft_cpx 	cx_out[nFFT];
extern kiss_fft_cpx 	st_tmp[nFFT+34];
extern uint16_t 		adcData[nFFT];
extern 		float *fftJieguo;
extern 		float *fftJieguoTMP;
extern 		float *shiyuShuju;
extern u32 time_[6];
extern uint16_t bufLCD[240*135];
extern u8 *_bmp;
extern char 			nameItem[32][20];
extern char 			zhushiItem[32][128];
extern char 			canshuTxt[4096];
extern u8 				work[4096];
extern u8 		bCeDianchi;
extern u8 		bGaiTime;
extern u32				kaijiTime;
extern u8 		bUSB;
void SystemClock_Config(void);
void MPU_Config(void);
void Error_Handler(void);

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
