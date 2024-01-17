/*
 * sensor.c
 *
 *  Created on: Nov 30, 2023
 *      Author: servan, gilles
 */

#include "sensor.h"


Codeur_HandleTypeDef hCodeurLeft;
Codeur_HandleTypeDef hcodeurRight;


void codeur_init(Codeur_HandleTypeDef *hcodeur, TIM_HandleTypeDef *htimEncodeur, int frequency){
	HAL_TIM_Encoder_Start_IT(&htim3,TIM_CHANNEL_ALL);
	HAL_TIM_Encoder_Start_IT(&htim1,TIM_CHANNEL_ALL);
	hcodeur->htimEncodeur = htimEncodeur;
	hcodeur->frequency = frequency;
	HAL_TIM_Base_Start(hcodeur->htimEncodeur);
}

int Get_speed(Codeur_HandleTypeDef *hcodeur){
		uint32_t counter = __HAL_TIM_GET_COUNTER(hcodeur->htimEncodeur);
		__HAL_TIM_SET_COUNTER(hcodeur->htimEncodeur,32768);
		//hcodeur->speed = (counter-32768)/4;
		return (counter-32768)/4;
}
