/*
 * controle.c
 *
 *  Created on: Nov 30, 2023
 *      Author: Gilles
 */


#include "control.h" // For fabs function
#include <stdint.h>// Pour les types de données standard
#include "drv_LIDAR.h"
#include "pwm.h"

// Définir des constantes pour améliorer la lisibilité
#define ANGLE_MAX 180
#define VITESSE_LIMITE 999
#define COEFFICIENT_ANGLE 2// Equivalent à 190.0 / 180.0
#define VITESSE_CONSTANTE 400



int angle_inverse(int angle) {
    int angle_oppose = 180 - angle;
    // Ajustement pour s'assurer que l'angle reste dans l'intervalle [0, 360]
    if (angle_oppose < 0) {
        angle_oppose += 360;
    }
    return angle_oppose;
}



void ajusterAlphaMoteurs(int angle,int vitesse, Motor_drv_struct *motor1, Motor_drv_struct *motor2) {

    int alpha1, alpha2;

    if (angle > ANGLE_MAX) {
        alpha1 = (ANGLE_MAX - angle) * COEFFICIENT_ANGLE + VITESSE_CONSTANTE + vitesse/2;
        alpha2 = 200 + vitesse;
    } else {
        alpha2 = (angle - ANGLE_MAX) * COEFFICIENT_ANGLE + VITESSE_CONSTANTE + vitesse/2;
        alpha1 = 200 + vitesse;
    }

    // Vérifier la limite de vitesse
    if (alpha1 > VITESSE_LIMITE || alpha2 > VITESSE_LIMITE) {
        alpha1 = VITESSE_LIMITE;
        alpha2 = VITESSE_LIMITE;
    }

    motor1->alpha = alpha1;
    motor2->alpha = alpha2;
}









//// Define the PID structure
//typedef struct {
//    float kp; // Proportional gain
//    float ki; // Integral gain
//    float kd; // Derivative gain
//    float integral;
//    float previous_error;
//} PID;

//// Initialize the PID controller
//void initializePID(PID *pid, float kp, float ki, float kd) {
//    pid->kp = kp;
//    pid->ki = ki;
//    pid->kd = kd;
//    pid->integral = 0.0f;
//    pid->previous_error = 0.0f;
//}
//
//// PID function to adjust the angle
//int adjustAnglePID(PID *pid, int alpha, int target_angle, float dt) {
//    // Calculate the error
//    float error = target_angle - alpha;
//
//    // Calculate the integral of the error
//    pid->integral += error * dt;
//
//    // Calculate the derivative of the error
//    float derivative = (error - pid->previous_error) / dt;
//
//    // PID adjustment
//    float adjustment = pid->kp * error + pid->ki * pid->integral + pid->kd * derivative;
//
//    // Update the previous error
//    pid->previous_error = error;
//
//    // Calculate the new angle and clamp it to the range [0, 999]
//    int new_alpha = alpha + (int)adjustment;
//    if (new_alpha > 999) new_alpha = 999;
//    else if (new_alpha < 0) new_alpha = 0;
//
//    return new_alpha;
//}



