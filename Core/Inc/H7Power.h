/*
 * H7Power.h
 *
 *  Created on: 2024年4月9日
 *      Author: Administrator
 */

#ifndef H7POWER_H_
#define H7POWER_H_
#include "main.h"

class H7Power {
public:
	static RTC_HandleTypeDef hrtc;
	void init(/*int sleepSec*/);
	u32 getWakeUpTimer();
	void setRTCtime();
	static void getRTCtime();
	static void sleep();
	void run(u32 ms);
};

#endif /* H7POWER_H_ */
