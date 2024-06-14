/*
 * Set.h
 *
  *      Author: Administrator
 */

#ifndef SET_H_
#define SET_H_
#include "main.h"

#define PORTS_POWER_OUT             GPIOA
#define PINS_POWER_OUT               GPIO_PIN_8

#define PORTS_POWER_TFT             GPIOB
#define PINS_POWER_TFT               GPIO_PIN_0

#define PORTS_POWER_BATT          GPIOC
#define PINS_POWER_BATT               GPIO_PIN_4

#define PORTS_USB             			GPIOE
#define PINS_USB              	 			GPIO_PIN_3

#define PORTS_INPUT_KAIJI             GPIOC
#define PINS_INPUT_KAIJI               GPIO_PIN_13

#define PORTS_OSPI           				GPIOD
#define PINS_OSPI               			GPIO_PIN_14

class Set {
public:
	Set();
	virtual ~Set();
	void init();
	static void setOn_out();
	static void setOff_out();
	static void setOn_tft();
	static void setOff_tft();
	static void setOn_BATT();
	static void setOn_OSPI();
	static void setOff_OSPI();
	static void setUSBinput();
	static bool getKaijiInput();

};

#endif /* SET_H_ */
