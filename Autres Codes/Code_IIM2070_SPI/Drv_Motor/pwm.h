/*
 * pwm.h
 *
 *  Created on: Nov 30, 2023
 *      Author: serva
 */

#ifndef DRV_MOTOR_PWM_H_
#define DRV_MOTOR_PWM_H_

#include "tim.h"
#include "gpio.h"

#define alpha_MAX 999
#define PWM_MAX 100

typedef int (*pwm_start_drv)(TIM_HandleTypeDef htim, int Channel);
typedef int (*pwm_stop_drv)(TIM_HandleTypeDef htim, int Channel);


typedef struct __Motor_HandleTypeDef
{
	int Channel_Forward;
	int Channel_Backward;
	int sens;
	int alpha;
	int vitesse;
	int position;
	int old_position;
	TIM_HandleTypeDef Timer_Backward;
	TIM_HandleTypeDef Timer_Forward;
} Motor_HandleTypeDef;

extern Motor_HandleTypeDef motor1;
extern Motor_HandleTypeDef motor2;

int pwm_start(TIM_HandleTypeDef htim, int Channel);
int pwm_stop(TIM_HandleTypeDef htim, int Channel);
void update_motor(Motor_HandleTypeDef);
void motor_Init(Motor_HandleTypeDef *moteur1, Motor_HandleTypeDef *moteur2);

#endif /* DRV_MOTOR_PWM_H_ */
