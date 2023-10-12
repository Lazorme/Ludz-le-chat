/**
 *@file pwm.c
 *@brief Ce fichier contient les fonctionnement nécéssaires à la génération des signaux PWM pour controler le moteur.
 *@date Oct 2, 2023
 *@author Tom
 *@author Baptiste
 *@author Antoine
*/

#include "pwm.h"
#include "tim.h"
#include "gpio.h"

/**
 * @def alpha_MAX
 * Résolution maximal du rapport cyclique
 */
#define alpha_MAX 999
int aTemp;

/**
 * @brief Démarre le PWM.
 *
 * Cette fonction démarre le PWM sur les canaux 1 et 2 du Timer 1.
 * Elle active la sortie PWM et la sortie complémentaire PWM (PWMN) sur ces canaux.
 *
 * @note Cette fonction doit être appelée après avoir configuré les paramètres PWM appropriés.
 */

void pwm_start(void){
	HAL_GPIO_WritePin(GPIOA, EN_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, ENU_Pin, 1);
	HAL_GPIO_WritePin(GPIOB, ENV_Pin, 1);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
	//HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_1);

	HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);
	//HAL_TIMEx_PWMN_Start(&htim1, TIM_CHANNEL_2);
	aTemp=50;

}
void pwm_stop(void){
	set_pwm_alpha(50);
	HAL_GPIO_WritePin(GPIOA, EN_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, ENU_Pin, 0);
	HAL_GPIO_WritePin(GPIOB, ENV_Pin, 0);
}
/**
 * @brief Configure le rapport cyclique des signaux PWM sur les canaux 1 et 2 du Timer 1 en fonction du rapport cyclique alpha.
 *
 * Cette fonction configure le rapport cyclique des signaux PWM sur les canaux 1 et 2 du Timer 1 en fonction
 * d'un paramètre alpha fourni en pourcentage. Le paramètre alpha doit être compris entre 0 et 100, où 0
 * signifie un rapport cyclique de 0% sur le canal 1 et 100 signifie un rapport cyclique de 0% sur le canal 2.
 *
 * @param alpha La valeur du parameter alpha en pourcentage (entre 0 et 100).
 *
 * @note Assurez-vous que le Timer 1 est configuré correctement pour le fonctionnement PWM avant d'appeler cette fonction.
 */

void set_pwm_alpha(int alpha){
	int a1;
	int a2;
	int aTransi=aTemp;

	if (alpha > aTransi){
		while(aTransi != alpha){

			aTransi++;
			a1=alpha_MAX*aTransi/100;
			a2=alpha_MAX*(100-aTransi)/100;
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,a1);
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,a2);
			HAL_Delay(50);
		}
	}

	if (alpha < aTransi){
		while(aTransi != alpha){

			aTransi--;
			a1=alpha_MAX*aTransi/100;
			a2=alpha_MAX*(100-aTransi)/100;
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_1,a1);
			__HAL_TIM_SetCompare(&htim1,TIM_CHANNEL_2,a2);
			HAL_Delay(50);
		}
	}
	aTemp = alpha;
}

















































































































































































































