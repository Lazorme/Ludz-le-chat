/*
 * bouton.c
 *
 *  Created on: Dec 7, 2023
 *      Author: serva
 */

#include "bouton.h"

struct CAPT_BTN_MAP_struct CAPT_BTN_MAP;


void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	switch (GPIO_Pin)
	{
	case Bouton1_Pin :
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		break;

	case Bouton2_Pin :
		HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
		break;

	case Bouton3_Pin :
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		break;

	case Contact1_Pin :
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		break;
	case Contact2_Pin :
		HAL_GPIO_TogglePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin);
		break;

	case Contact3_Pin :
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_GPIO_TogglePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin);
		break;
	case Contact4_Pin :
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
		break;
	case Bordure1_Pin :
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_GPIO_TogglePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin);
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		break;

	case Bordure2_Pin :
		HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
		HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_GREEN_Pin);
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		break;

	}

	/*	if(BUMPER_1_Pin <= GPIO_Pin <= BUMPER_4_Pin)
	{
		CAPT_BTN_MAP.Pressed = BUMPER;
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		switch (GPIO_Pin)
		{
			case BUMPER_1_Pin : CAPT_BTN_MAP.BUMPER_1 = TRUE; break;
			case BUMPER_2_Pin : CAPT_BTN_MAP.BUMPER_2 = TRUE; break;
			case BUMPER_3_Pin : CAPT_BTN_MAP.BUMPER_3 = TRUE; break;
			case BUMPER_4_Pin : CAPT_BTN_MAP.BUMPER_4 = TRUE; break;
		}
	}

	else *//*if(BTN_1_Pin <= GPIO_Pin <=BTN_3_Pin)
	{
		CAPT_BTN_MAP.Pressed = BTN;
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		switch (GPIO_Pin)
		{
			case BTN_1_Pin : CAPT_BTN_MAP.BTN_1 = TRUE; break;
			case BTN_2_Pin : CAPT_BTN_MAP.BTN_2 = TRUE; break;
			case BTN_3_Pin : CAPT_BTN_MAP.BTN_3 = TRUE; break;
		}
	}

	else if(BORDURE_1_Pin <= GPIO_Pin <= BORDURE_2_Pin)
		{
			CAPT_BTN_MAP.Pressed = BORDURE;
			switch (GPIO_Pin)
			{
				case BORDURE_1_Pin : CAPT_BTN_MAP.BORDURE_1 = TRUE; break;
				case BORDURE_2_Pin : CAPT_BTN_MAP.BORDURE_2 = TRUE; break;
				//case BORDURE_3_Pin : CAPT_BTN_MAP.BORDURE_3 = TRUE; break;
			}
		}

		else CAPT_BTN_MAP = CAPT_BTN_MAP_RESET;
	 */
}

