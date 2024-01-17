/*
 * sensor.h
 *
 *  Created on: Nov 30, 2023
 *      Author: servan, gilles
 */

#ifndef DRV_MOTOR_ENCODER_H_
#define DRV_MOTOR_ENCODER_H_

#include "tim.h"
#include <stdio.h>

typedef struct __Codeur_HandleTypedef{
	TIM_HandleTypeDef *htimEncodeur;
	int frequency;
}Codeur_HandleTypeDef;

extern Codeur_HandleTypeDef hCodeurLeft;
extern Codeur_HandleTypeDef hCodeurRight;

void codeur_init(Codeur_HandleTypeDef *hcodeur, TIM_HandleTypeDef *htimEncodeur, float frequency);
int Get_speed(Codeur_HandleTypeDef *hcodeur);

#endif /* DRV_MOTOR_ENCODER_H_ */

