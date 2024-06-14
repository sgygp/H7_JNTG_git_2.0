/*
 * Set.cpp
 *
 *  Created on: 2021年9月1日
 *      Author: Administrator
 */

#include "Set.h"
Set::Set() {
	// TODO 自动生成的构造函数存根

}

Set::~Set() {
	// TODO 自动生成的析构函数存根
}

void Set::init() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
	  HAL_GPIO_WritePin(PORTS_POWER_OUT,PINS_POWER_OUT, GPIO_PIN_SET);
	  /*Configure GPIO pins : PCPin PCPin PCPin */
	  GPIO_InitStruct.Pin = PINS_POWER_OUT;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(PORTS_POWER_OUT, &GPIO_InitStruct);


	  HAL_GPIO_WritePin(PORTS_POWER_BATT,PINS_POWER_BATT, GPIO_PIN_SET);
	  /*Configure GPIO pins : PCPin PCPin PCPin */
	  GPIO_InitStruct.Pin = PINS_POWER_BATT;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(PORTS_POWER_BATT, &GPIO_InitStruct);


	  GPIO_InitStruct.Pin = PINS_OSPI;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(PORTS_OSPI, &GPIO_InitStruct);

	  setOn_BATT();
	  setOff_tft();
	  setOn_out();
	  setOn_OSPI();
//	  HAL_Delay(50);
}

void Set::setOn_out() {
	  HAL_GPIO_WritePin(PORTS_POWER_OUT,PINS_POWER_OUT, GPIO_PIN_RESET);
}
void Set::setOff_out() {
	  HAL_GPIO_WritePin(PORTS_POWER_OUT,PINS_POWER_OUT, GPIO_PIN_SET);
}
void Set::setOff_tft() {
	  HAL_GPIO_WritePin(PORTS_POWER_TFT,PINS_POWER_TFT, GPIO_PIN_SET);
	  GPIO_InitTypeDef GPIO_InitStruct;
	  memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
	  GPIO_InitStruct.Pin = PINS_POWER_TFT;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(PORTS_POWER_TFT, &GPIO_InitStruct);
}

void Set::setOn_tft() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));
	  GPIO_InitStruct.Pin = PINS_POWER_TFT;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
	  GPIO_InitStruct.Pull = GPIO_PULLUP;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(PORTS_POWER_TFT, &GPIO_InitStruct);
	  HAL_GPIO_WritePin(PORTS_POWER_TFT,PINS_POWER_TFT, GPIO_PIN_RESET);
}

void Set::setOn_BATT() {
	  HAL_GPIO_WritePin(PORTS_POWER_BATT,PINS_POWER_BATT, GPIO_PIN_RESET);
}
void Set::setOn_OSPI() {
	  HAL_GPIO_WritePin(PORTS_OSPI,PINS_OSPI, GPIO_PIN_SET);
}
void Set::setOff_OSPI() {
	  HAL_GPIO_WritePin(PORTS_OSPI,PINS_OSPI, GPIO_PIN_RESET);
}

void Set::setUSBinput() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));

	  /*Configure GPIO pins : PCPin PCPin PCPin */
	  GPIO_InitStruct.Pin = PINS_USB;
	  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_OD;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(PORTS_USB, &GPIO_InitStruct);


	  HAL_GPIO_WritePin(PORTS_USB, PINS_USB, GPIO_PIN_RESET);
}
bool Set::getKaijiInput() {
	  GPIO_InitTypeDef GPIO_InitStruct;
	  memset(&GPIO_InitStruct,0,sizeof(GPIO_InitStruct));

	  /*Configure GPIO pins : PCPin PCPin PCPin */
	  GPIO_InitStruct.Pin = PINS_INPUT_KAIJI;
	  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
	  GPIO_InitStruct.Pull = GPIO_NOPULL;
	  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
	  HAL_GPIO_Init(PORTS_INPUT_KAIJI, &GPIO_InitStruct);


	  if(HAL_GPIO_ReadPin(PORTS_INPUT_KAIJI, PINS_INPUT_KAIJI)==GPIO_PIN_SET)
		  return true;
	  else
		  return false;
}
