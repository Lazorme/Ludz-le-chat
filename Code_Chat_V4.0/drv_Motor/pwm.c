/*
 * pwm.c
 *
 *  Created on: Nov 30, 2023
 *      Author: serva
 */

#include "pwm.h"


//Gestion de l'allumage des moteurs
int pwm_start(TIM_HandleTypeDef htim, int Channel)
{
	return HAL_TIM_PWM_Start(&htim, Channel);
}

int pwm_stop(TIM_HandleTypeDef htim, int Channel)
{
	return HAL_TIM_PWM_Stop(&htim, Channel);
}

//Initialisation des moteurs
void motor_Init(struct Motor_drv_struct *moteur1, struct Motor_drv_struct *moteur2){

	moteur1->Channel_Backward = TIM_CHANNEL_1;
	moteur1->Channel_Forward = TIM_CHANNEL_2;
	moteur1->Timer_Backward = htim15;
	moteur1->Timer_Forward = htim15;
	moteur1->pwm_start = pwm_start;
	moteur1->pwm_stop = pwm_stop;
	moteur1->update = update_motor;
	moteur1->old_alpha =0;

	moteur2->Channel_Backward = TIM_CHANNEL_1;
	moteur2->Channel_Forward = TIM_CHANNEL_1;
	moteur2->Timer_Backward = htim16;
	moteur2->Timer_Forward = htim17;
	moteur2->pwm_start = pwm_start;
	moteur2->pwm_stop = pwm_stop;
	moteur2->update = update_motor;
	moteur2->old_alpha =0;
}

//Update alpha du moteur
void update_motor(struct Motor_drv_struct motor)
{
	if(motor.old_sens == motor.sens){
		if (motor.old_alpha <= motor.alpha)	//permet de faire une rampe sur le pwm
		{
			for (int i = motor.old_alpha; i<=motor.alpha;i++){	//application de la rampe
				if (motor.sens == 1)
				{
					__HAL_TIM_SetCompare(&motor.Timer_Backward,motor.Channel_Backward,i);
					__HAL_TIM_SetCompare(&motor.Timer_Forward,motor.Channel_Forward,0);
				}
				if (motor.sens == 2)
				{
					__HAL_TIM_SetCompare(&motor.Timer_Backward,motor.Channel_Backward,0);
					__HAL_TIM_SetCompare(&motor.Timer_Forward,motor.Channel_Forward,i);
				}
			}
		}
		else if (motor.old_alpha >= motor.alpha)
		{
			for (int i = motor.old_alpha; i>=motor.alpha;i--){	//application de la rampe
				if (motor.sens == 1)
				{
					__HAL_TIM_SetCompare(&motor.Timer_Backward,motor.Channel_Backward,i);
					__HAL_TIM_SetCompare(&motor.Timer_Forward,motor.Channel_Forward,0);
				}
				if (motor.sens == 2)
				{
					__HAL_TIM_SetCompare(&motor.Timer_Backward,motor.Channel_Backward,0);
					__HAL_TIM_SetCompare(&motor.Timer_Forward,motor.Channel_Forward,i);
				}
			}
		}
	}

	else{
		for (int i = motor.old_alpha; i>100;i--){	//application de la rampe pour le mettre à l'arret
			if (motor.old_sens == 1)
			{
				__HAL_TIM_SetCompare(&motor.Timer_Backward,motor.Channel_Backward,i);
				__HAL_TIM_SetCompare(&motor.Timer_Forward,motor.Channel_Forward,0);
			}
			if (motor.old_sens == 2)
			{
				__HAL_TIM_SetCompare(&motor.Timer_Backward,motor.Channel_Backward,0);
				__HAL_TIM_SetCompare(&motor.Timer_Forward,motor.Channel_Forward,i);
			}
		}

		for (int i = 100; i<=motor.alpha;i++){	//application de la rampe pour le démarrer dans l'autre sens
			if (motor.sens == 1)
			{
				__HAL_TIM_SetCompare(&motor.Timer_Backward,motor.Channel_Backward,i);
				__HAL_TIM_SetCompare(&motor.Timer_Forward,motor.Channel_Forward,0);
			}
			if (motor.sens == 2)
			{
				__HAL_TIM_SetCompare(&motor.Timer_Backward,motor.Channel_Backward,0);
				__HAL_TIM_SetCompare(&motor.Timer_Forward,motor.Channel_Forward,i);
			}
		}

	}



	motor.old_alpha = motor.alpha;	//permet de stocker l'ancien alpha pour réaliser la futur rampe
}

