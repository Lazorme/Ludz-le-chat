/*
 * pwm.c
 *
 *  Created on: Nov 30, 2023
 *      Author: serva
 */

#include "pwm.h"
#include "tim.h"

int aTemp; //variable temporaire pour modifier la valeur du alpha en échelon


int pwm_start(TIM_HandleTypeDef htim, int Channel)
{
	return HAL_TIM_PWM_Start(&htim, Channel);
}

int pwm_stop(TIM_HandleTypeDef htim, int Channel)
{
	return HAL_TIM_PWM_Stop(&htim, Channel);
}

void update_motor(MotorControl_HandleTypeDef *motor)
{
	if (motor.sens == 1)
	{
		__HAL_TIM_SetCompare(&motor->Motor.Timer_Backward,motor->Motor.Channel_Backward,motor->Motor.alpha);
		__HAL_TIM_SetCompare(&motor->Motor.Timer_Forward,motor->Motor.Channel_Forward,0);
	}
	if (motor.sens == 2)
	{
		__HAL_TIM_SetCompare(&motor->Motor.Timer_Backward,motor->Motor.Channel_Backward,0);
		__HAL_TIM_SetCompare(&motor->Motor.Timer_Forward,motor->Motor.Channel_Forward,motor->Motor.alpha);
	}
}

void motor_Init(Motor_HandleTypeDef *moteur1,Motor_HandleTypeDef *moteur2){

	moteur1->Channel_Backward = TIM_CHANNEL_1;
	moteur1->Channel_Forward = TIM_CHANNEL_2;
	moteur1->Timer_Backward = htim15;
	moteur1->Timer_Forward = htim15;

	moteur2->Channel_Backward = TIM_CHANNEL_1;
	moteur2->Channel_Forward = TIM_CHANNEL_1;
	moteur2->Timer_Backward = htim16;
	moteur2->Timer_Forward = htim17;

}
