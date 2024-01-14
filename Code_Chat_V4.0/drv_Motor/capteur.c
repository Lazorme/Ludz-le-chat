/*
 * capteur.c
 *
 *  Created on: Dec 7, 2023
 *      Author: serva
 */

#include "capteur.h"
#include "pwm.h"

#include "stdio.h"
#include "cmsis_os.h"

CAPT capteurs;
uint16_t GPIO_Pin_mem;
TaskHandle_t TaskHandle_CAPT;
SemaphoreHandle_t sem_capt = NULL;
extern Motor_drv_struct motor1, motor2;
extern int time;

extern int hunter; //1 hunter, 0 souris

extern TaskHandle_t xHandleMOTOR;
int lastdebounce_time = 0;

void capt_task(){
	for(;;){
		xSemaphoreTake(sem_capt, portMAX_DELAY);
		switch (GPIO_Pin_mem)
		{
		case Contact1_Pin 	:
			capteurs.CONTACT_1 = TRUE;
			HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
			break;

		case Contact2_Pin 	:
			capteurs.CONTACT_2 = TRUE;
			HAL_GPIO_TogglePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin);
			break;

		case Contact3_Pin 	:
			capteurs.CONTACT_3 = TRUE;
			HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
			break;

		case Contact4_Pin 	:
			capteurs.CONTACT_4 = TRUE;
			//HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
			break;

		case Bouton1_Pin 		:
			capteurs.BTN_1 = TRUE;
			break;

		case Bouton2_Pin 		:
			capteurs.BTN_2 = TRUE;
			break;

		case Bouton3_Pin 		:
			capteurs.BTN_3 = TRUE;
			break;

		case Bordure1_Pin 	:
			capteurs.BORDURE_1 = TRUE;
			break;

		case Bordure2_Pin 	:
			capteurs.BORDURE_2 = TRUE;
			break;
		}

		if (time - lastdebounce_time >2000){	//permet de changer d'état seulement toute les 2sec
			if (capteurs.CONTACT_1 || capteurs.CONTACT_2 || capteurs.CONTACT_3 || capteurs.CONTACT_4)
			{
				hunter = 1-hunter; //changement d'état pour passer de hunter à souris ou l'inverse
				if(hunter){	//si hunter led bleue allumée
					HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);

					HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_RESET);
				}
				else{ //si souris led jaune allumée
					HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_RESET);
					HAL_GPIO_WritePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin, GPIO_PIN_SET);
				}



				lastdebounce_time = time;
			}
			capteurs.CONTACT_1 = FALSE;
			capteurs.CONTACT_2 = FALSE;
			capteurs.CONTACT_3 = FALSE;
			capteurs.CONTACT_4 = FALSE;
		}


		vTaskSuspend(xHandleMOTOR);
		if (capteurs.BORDURE_1)
		{
			HAL_GPIO_WritePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin, GPIO_PIN_SET);


			//mettre le code pour reculer et tourner
			motor1.sens = SENS_MOTOR_2;
			motor2.sens = SENS_MOTOR_1;
			motor1.alpha = PUISSANCE_MOTEUR_BORDURE;
			motor2.alpha = PUISSANCE_MOTEUR_BORDURE;
			motor1.update(motor1);
			motor2.update(motor2);

			vTaskDelay(1000);
			motor1.sens = SENS_MOTOR_2;
			motor2.sens = SENS_MOTOR_2;
			motor1.update(motor1);
			motor2.update(motor2);
			vTaskDelay(1600);
			HAL_GPIO_WritePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin, GPIO_PIN_SET);
			capteurs.BORDURE_1 = FALSE;
		}
		if (capteurs.BORDURE_2)
		{
			HAL_GPIO_WritePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin, GPIO_PIN_SET);


			//mettre le code pour reculer et tourner
			motor1.sens = SENS_MOTOR_1;
			motor2.sens = SENS_MOTOR_2;
			motor1.alpha = PUISSANCE_MOTEUR_BORDURE;
			motor2.alpha = PUISSANCE_MOTEUR_BORDURE;
			motor1.update(motor1);
			motor2.update(motor2);
			vTaskDelay(1000);
			motor1.sens = SENS_MOTOR_2;
			motor2.sens = SENS_MOTOR_2;
			motor1.update(motor1);
			motor2.update(motor2);
			vTaskDelay(1600);
			HAL_GPIO_WritePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin, GPIO_PIN_SET);
			capteurs.BORDURE_2 = FALSE;
		}
		vTaskResume(xHandleMOTOR);


	}
}

int capt_init()
{
	sem_capt = xSemaphoreCreateBinary();
	BaseType_t xReturned;

	printf("Loading Capt_ISR task with priority 3\n\r");

	xReturned = xTaskCreate(
			capt_task,
			"capt_task",
			256,
			NULL,
			3,
			&TaskHandle_CAPT);
	HAL_GPIO_WritePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin, GPIO_PIN_SET);
	if(xReturned==pdPASS){
		printf("Task loaded\r\n");
		return 0;
	}
	else {
		printf("Error loading task\r\n");
		HAL_GPIO_WritePin(LED_RED_GPIO_Port, LED_RED_Pin, GPIO_PIN_SET);
		return 1;
	}

}

// contact1 gauche
// contact2 devant
// contact3 derrière
// contact4 droite
void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
{
	BaseType_t higher_priority_task_woken = pdFALSE;
	GPIO_Pin_mem = GPIO_Pin;
	//capt_btn_task();
	xSemaphoreGiveFromISR(sem_capt, &higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);

	/*switch (GPIO_Pin)
	{
	case Bouton1_Pin :
		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
		break;

	case Bouton2_Pin :
		HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
		break;

	case Bouton3_Pin :
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		break;

	case Contact1_Pin :
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		break;
	case Contact2_Pin :
		HAL_GPIO_TogglePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin);
		break;

	case Contact3_Pin :
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		HAL_GPIO_TogglePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin);
		break;
	case Contact4_Pin :
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
		break;
	case Bordure1_Pin :
		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
		capteurs.BORDURE_1 = TRUE;

		break;

	case Bordure2_Pin :
		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
		capteurs.BORDURE_2 = TRUE;

		break;
	 */
}

