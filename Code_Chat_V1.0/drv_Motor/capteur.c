///*
// * capteur.c
// *
// *  Created on: Dec 7, 2023
// *      Author: serva
// */
//
//#include "capteur.h"
//
//#include "stdio.h"
//#include "cmsis_os.h"
//#include "main.h"
//
//CAPT capteurs;
//uint16_t GPIO_Pin_mem;
//TaskHandle_t TaskHandle_CAPT;
//SemaphoreHandle_t sem_capt = NULL;
//
//void capt_task(){
//	for(;;){
//		xSemaphoreTake(sem_capt, portMAX_DELAY);
//		switch (GPIO_Pin_mem)
//		{
//		case BUMPER_1_Pin 	: capteurs.CONTACT_1 = TRUE; CAPT_BTN_MAP.Pressed = BUMPER; break;
//		case BUMPER_2_Pin 	: capteurs.CONTACT_2 = TRUE; CAPT_BTN_MAP.Pressed = BUMPER; break;
//		case BUMPER_3_Pin 	: capteurs.CONTACT_3 = TRUE; CAPT_BTN_MAP.Pressed = BUMPER; break;
//		case BUMPER_4_Pin 	: capteurs.CONTACT_4 = TRUE; CAPT_BTN_MAP.Pressed = BUMPER; break;
//		case BTN_1_Pin 		: capteurs.BTN_1 = TRUE;CAPT_BTN_MAP.Pressed = BTN; break;
//		case BTN_2_Pin 		: capteurs.BTN_2 = TRUE;CAPT_BTN_MAP.Pressed = BTN; break;
//		case BTN_3_Pin 		: capteurs.BTN_3 = TRUE;CAPT_BTN_MAP.Pressed = BTN; break;
//		case BORDURE_1_Pin 	: capteurs.BORDURE_1 = TRUE;CAPT_BTN_MAP.Pressed = BORDURE; break;
//		case BORDURE_2_Pin 	: capteurs.BORDURE_2 = TRUE;CAPT_BTN_MAP.Pressed = BORDURE; break;
//		//case BORDURE_3_Pin : CAPT_BTN_MAP.BORDURE_3 = TRUE; break;
//		}
//	}
//}
//
//int capt_init()
//{
//	sem_capt = xSemaphoreCreateBinary();
//	BaseType_t xReturned;
//
//	printf("Loading btn_ISR task with priority 3\n\r");
//
//	xReturned = xTaskCreate(
//			capt_task,
//			"capt_task",
//			256,
//			NULL,
//			3,
//			&TaskHandle_CAPT);
//
//	if(xReturned==pdPASS){
//		printf("Task loaded\r\n");
//		return 0;
//	}
//	else {
//		printf("Error loading task\r\n");
//		return 1;
//	}
//}
//
//// contact1 gauche
//// contact2 devant
//// contact3 derrière
//// contact4 droite
//void HAL_GPIO_EXTI_Rising_Callback(uint16_t GPIO_Pin)
//{
//	BaseType_t higher_priority_task_woken = pdFALSE;
//	GPIO_Pin_mem = GPIO_Pin;
//	//capt_btn_task();
//	xSemaphoreGiveFromISR(sem_capt, &higher_priority_task_woken);
//	portYIELD_FROM_ISR(higher_priority_task_woken);
//
//	/*switch (GPIO_Pin)
//	{
//	case Bouton1_Pin :
//		HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
//		break;
//
//	case Bouton2_Pin :
//		HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
//		break;
//
//	case Bouton3_Pin :
//		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
//		break;
//
//	case Contact1_Pin :
//		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
//		break;
//	case Contact2_Pin :
//		HAL_GPIO_TogglePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin);
//		break;
//
//	case Contact3_Pin :
//		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
//		HAL_GPIO_TogglePin(LED_WHITE_GPIO_Port, LED_WHITE_Pin);
//		break;
//	case Contact4_Pin :
//		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
//		HAL_GPIO_TogglePin(LED_YELLOW_GPIO_Port, LED_YELLOW_Pin);
//		break;
//	case Bordure1_Pin :
//		HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
//		capteurs.BORDURE_1 = TRUE;
//
//		break;
//
//	case Bordure2_Pin :
//		HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
//		capteurs.BORDURE_2 = TRUE;
//
//		break;
//	 */
//}
//
