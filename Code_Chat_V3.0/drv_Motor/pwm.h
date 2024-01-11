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
typedef int (*set_alpha_drv)(TIM_HandleTypeDef htim, int Channel);

typedef struct Motor_drv_struct
{
	int Channel_Forward;
	int Channel_Backward;
	int sens;	//il faut inverser les sens pour bouger de façons rectiligne
	int alpha;
	int old_alpha;
	float vitesse;
	int position;
	int old_position;
	int old_sens;

	TIM_HandleTypeDef Timer_Forward,Timer_Backward;
	pwm_start_drv pwm_start;
	pwm_stop_drv pwm_stop;
	void (*update)(struct Motor_drv_struct);
} Motor_drv_struct;


int pwm_start(TIM_HandleTypeDef htim, int Channel);
int pwm_stop(TIM_HandleTypeDef htim, int Channel);

void update_motor(struct Motor_drv_struct);

void pwm_start_test(void);
void pwm_stop_test(void);
void set_pwm_alpha(int alpha);
void motor_Init(struct Motor_drv_struct *moteur1, struct Motor_drv_struct *moteur2);

#endif /* DRV_MOTOR_PWM_H_ */
