/*
 * anjian.h
 *
 *  Created on: 2019年11月21日
 *      Author: Administrator
 */

#ifndef ANJIAN_H_
#define ANJIAN_H_
#include "main.h"

#define PPORT1 GPIOB
#define PPIN1  GPIO_PIN_7
#define PPIN2  GPIO_PIN_9
#define PPIN3  GPIO_PIN_8

#define PPORT2 GPIOE
#define PPIN4  GPIO_PIN_1
#define PPIN5  GPIO_PIN_0

#define PPORT3 GPIOC
#define PPIN6  GPIO_PIN_13

#define ENTER			1
#define UP				2
#define DOWN			4
#define RIGHT			8
#define LEFT			16
#define ESC				32

#define KEYLEN 20

class Anjian {
public:
	Anjian();
public:
	static u8 m_pJianW;
	static u8 m_pJianR ;
	static u32 m_bufJian[KEYLEN] ;
	static void init();
	static u32 GetKey();
	static void PutKey(u32 key);
	static bool run();
	static bool doKey(GPIO_TypeDef *port,uint16_t pin,u8 key,u8 &mode,int &jishu,GPIO_PinState normal);
};

#endif /* ANJIAN_H_ */
