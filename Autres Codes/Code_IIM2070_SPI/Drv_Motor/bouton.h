/*
 * bouton.h
 *
 *  Created on: Dec 7, 2023
 *      Author: serva
 */

#ifndef INC_BORDURE_BUMPER_H_
#define INC_BORDURE_BUMPER_H_

#include "stdint.h"
#include "gpio.h"

enum BOOL {FALSE, TRUE};
enum BTN_TYPE {NONE, BUMPER, BTN, BORDURE};

struct CAPT_BTN_MAP_struct {
	enum BTN_TYPE Pressed;
	enum BOOL BUMPER_1;
	enum BOOL BUMPER_2;
	enum BOOL BUMPER_3;
	enum BOOL BUMPER_4;
	enum BOOL BTN_1;
	enum BOOL BTN_2;
	enum BOOL BTN_3;
	enum BOOL BORDURE_1;
	enum BOOL BORDURE_2;

};

//CAPT_BTN_MAP_struct CAPT_BTN_MAP_RESET = {NONE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

//void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);

#endif /* INC_BORDURE_BUMPER_H_ */
