/*
 * controle.c
 *
 *  Created on: Nov 30, 2023
 *      Author: Gilles
 */


#include "control.h"
#define ROWS_M1 2
#define COLS_M1 10
#define COLS_M2 1

MotorControl_HandleTypeDef MotorControlLeft;
MotorControl_HandleTypeDef MotorControlRight;


void motorControlInit(MotorControl_HandleTypeDef *MotorControl,Motor_HandleTypeDef *Motor,Codeur_HandleTypeDef *hcodeur,int kp,int ki){
	MotorControl->Motor = Motor;
	MotorControl->hcodeur = hcodeur;
	MotorControl->b0=4*ki+(kp*(Te^2))/2;
	MotorControl->b1=6*ki-(kp*(Te^2));
	MotorControl->b2=4*ki+(kp*(Te^2))/2;
	for(int i=0 ; i<32;i++){
		MotorControl->speed_command[i]=0;
		MotorControl->speed_codeur[i]=0;
		MotorControl->speed_error[i]=0;
	}
	MotorControl->time_index = 0;
}


void motorControl(MotorControl_HandleTypeDef *MotorControl){
	int speed = Get_speed(MotorControl->hcodeur);

	int setpoint = MotorControl->setpoint; // La consigne de vitesse doit être définie ou passée à la fonction


	MotorControl->speed_error[MotorControl->time_index] = setpoint - speed;

	// Application de l'équation PID fournie
	MotorControl->speed_command[MotorControl->time_index] =
			(2 * MotorControl->Te * MotorControl->speed_error[MotorControl->time_index]+ 2 * MotorControl->speed_error[(MotorControl->time_index - 1) % 32]
			 - MotorControl->speed_error[(MotorControl->time_index - 2) % 32]- MotorControl->b1 * MotorControl->speed_command[(MotorControl->time_index - 1) % 32]
			 - MotorControl->b2 * MotorControl->speed_command[(MotorControl->time_index - 2) % 32])/ MotorControl->b0;

	update_motor(MotorControl->Motor, MotorControl->speed_command[MotorControl->time_index]);

	// Mise à jour de l'index de temps pour la prochaine exécution
	MotorControl->time_index = (MotorControl->time_index + 1) % 32;
}

//---------------------------------------------------------------------------------------------------------------------------------------//*

void follow(int lidarbuffer,MotorControl_HandleTypeDef *MotorControlLeft,MotorControl_HandleTypeDef *MotorControlRight){
	//Method one
	int weights[ROWS_M1][COLS_M1] = {
			// M1
			{15, 8, 6, 4, 2, -2, -4, -6, -8, -15},
			// M2
			{-15, -8, -6, -4, -2, 2, 4, 6, 8, 15}
	};
	int inputs = lidarbuffer;
	int bias = 5;
	int result[ROWS_M1];
	matrixProduct(result, weights, inputs, bias);

	//method two
	result[0] = (x-180)*(maxspeed/180) + bias;
	result[1] = (180-x)*(maxspeed/180) + bias;

	//Send command
	MotorControlRight->setpoint = result[0];
	MotorControlLeft->setpoint = result[1];
	motorControl(MotorControlRight);
	motorControl(MotorControlLeft);
	printf("M1 = %f\nM2 = %f\n", result[0], result[1]);
	return 0;
}


void matrixProduct(int result[ROWS_M1], int weights[ROWS_M1][COLS_M1], int inputs[COLS_M1], int bias) {
	for (int i = 0; i < ROWS_M1; ++i) {
		result[i] = 0.0;
		for (int j = 0; j < COLS_M1; ++j) {
			result[i] += weights[i][j] * inputs[j];
		}
		result[i] += bias;
	}
}
