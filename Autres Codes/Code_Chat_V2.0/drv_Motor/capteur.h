/*
 * capteur.h
 *
 *  Created on: Dec 7, 2023
 *      Author: serva
 */

#ifndef DRV_MOTOR_BOUTON_H_
#define DRV_MOTOR_BOUTON_H_

#include "stdint.h"
#include "gpio.h"

enum BOOL {FALSE, TRUE};

typedef struct CAPT_struct {
	enum BOOL CONTACT_1;
	enum BOOL CONTACT_2;
	enum BOOL CONTACT_3;
	enum BOOL CONTACT_4;
	enum BOOL BTN_1;
	enum BOOL BTN_2;
	enum BOOL BTN_3;
	enum BOOL BORDURE_1;
	enum BOOL BORDURE_2;
} CAPT;

//CAPT_BTN_MAP_struct CAPT_BTN_MAP_RESET = {NONE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE, FALSE};

//void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin);

#endif /* INC_BORDURE_BUMPER_H_ */
