/*
 * control.h
 *
 *  Created on: Nov 30, 2023
 *      Author: serva
 */

#ifndef DRV_MOTOR_Controle_H_
#define DRV_MOTOR_Controle_H_

#include "tim.h"
#include "gpio.h"
#include "sensor.h"
#include "pwm.h"

typedef struct __MotorControl_HandleTypedef{
	Codeur_HandleTypeDef hcodeur;
	Motor_HandleTypeDef Motor;
	int b0;
	int b1;
	int b2;
	int time_index;
	int speed_command[32];
	int speed_codeur[32];
	int speed_error[32];
	int setpoint;
}MotorControl_HandleTypeDef;


extern MotorControl_HandleTypeDef MotorControlLeft;
extern MotorControl_HandleTypeDef MotorControlRight;

void motorControlInit(MotorControl_HandleTypeDef *MotorControl,Motor_HandleTypeDef *Motor,Codeur_HandleTypeDef *hcodeur,int k,int ki);
void motorControl(MotorControl_HandleTypeDef *MotorControl);
void follow(int lidarbuffer,MotorControl_HandleTypeDef *MotorControlLeft,MotorControl_HandleTypeDef *MotorControlRight);
void matrixProduct(int result[ROWS_M1], int weights[ROWS_M1][COLS_M1], int inputs[COLS_M1], int bias);

#endif /* DRV_MOTOR_Control_H_ */
