/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * \author Tom CHARBONNEAU
 * \author Baptiste GILLES
 * \author Antoine SERVAN
 *
 * @attention
 *
 * Copyright (c) 2024 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 ******************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "cmsis_os.h"
#include "adc.h"
#include "dma.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdio.h"
#include "math.h"
#include "drv_LIDAR.h"
#include "string.h"
#include "pwm.h"
#include "sensor.h"
#include "control.h"
#include "capteur.h"

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define STACK_SIZE 256 //! Taille du stack fournie aux taches par FreeRTOS

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

int time=0;//! Compte le temps entre deux appuis sur une moustache pour l'anti-rebound

h_LIDAR_t h_LIDAR; //! Le handel du LIDAR

Motor_drv_struct motor1; //! Le handel du moteur 1
Motor_drv_struct motor2; //! Le handel du moteur 2

Codeur_HandleTypeDef hCodeurLeft; //! Le handel du codeur gauche
Codeur_HandleTypeDef hCodeurRight; //! Le handel du codeur droit

int angle; //! Angle effectué avec le robot le plus proche
int vitesse; //! Distance du robot le plus proche

int hunter = 1; //! Etat du robot : 1 chat et 0 souris
int alpha1,alpha2; //! Rapports cycliques des moteurs

SemaphoreHandle_t SemHalfCallBack; //! Semaphore Handle pour la réception DMA de moitiée
SemaphoreHandle_t SemClpCallBack; //! Semaphore Handle pour la réception DMA totale

int distance_min=0; //! Distance du cluster le plus proche
int idx_min = 0; //! Index du custer le plus proche
int frame_start = 0; //! Indice du début de trame
int frame_end = 0; //! Indice de fin de trame

TaskHandle_t xHandleLIDAR = NULL; //! Handle de la tache gérant le LIDAR
TaskHandle_t xHandleMOTOR = NULL; //! Handle de la tache gérant les moteurs

BaseType_t ret; //! Valeur retournée lors de la création des taches

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
void MX_FREERTOS_Init(void);
/* USER CODE BEGIN PFP */

/**
 * Redirige les printf sur l'UART1
 */
int __io_putchar(int chr)
{
	HAL_UART_Transmit(&huart1, (uint8_t*)&chr, 1, HAL_MAX_DELAY);
	return chr;
}
/**
 * Polling transmit sur l'UART4
 */
int uart_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit(&huart4, p_data, size, HAL_MAX_DELAY);
	return 0;
}
/**
 * Interrupt transmit sur l'UART4
 */
int uart_it_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit_IT(&huart4, p_data, size);
	return 0;
}
/**
 * DMA transmit sur l'UART4
 */
int uart_dma_transmit(uint8_t *p_data, uint16_t size){
	HAL_UART_Transmit_DMA(&huart4, p_data, size);
	return 0;
}
/**
 * Polling receive sur l'UART4
 */
int uart_poll_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive(&huart4, p_data, size, HAL_MAX_DELAY);
	return 0;
}
/**
 * Interrupt receive sur l'UART4
 */
int uart_it_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive_IT(&huart4, p_data, size);
	return 0;
}
/**
 * DMA receive sur l'UART4
 */
int uart_dma_receive(uint8_t *p_data, uint16_t size){
	HAL_UART_Receive_DMA(&huart4, p_data, size);
	return 0;
}

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/**
 * Tache du LIDAR
 */
void TaskLIDAR (void * pvParameters){
	for(;;){
		xSemaphoreTake(SemHalfCallBack, portMAX_DELAY); //! Semaphore du DMA Half Callback
		for(int i=0; i<DATA_BUFF_SIZE; i++){
			if(i==DATA_BUFF_SIZE/2){
				xSemaphoreTake(SemClpCallBack, portMAX_DELAY);
			}
			/**
			 * Réponse à la commande SCAN pour connaitre le début et la fin des trames
			 */
			if(i==0){
				if((h_LIDAR.data_buff[i]==0xA5) && (h_LIDAR.data_buff[i+1]==0x5A) && (h_LIDAR.data_buff[i+2]==0x05) && (h_LIDAR.data_buff[i+3]==0x00) && (h_LIDAR.data_buff[i+4]==0x00) && (h_LIDAR.data_buff[i+5]==0x40) && (h_LIDAR.data_buff[i+6]==0x81)){
					printf("Scan Command Reply\r\n");
					h_LIDAR.processing.idx=0;
					i=6;
					frame_start=7;
					frame_end=frame_start+4;
				}
			}
			/**
			 * Séparations des différentes trames et récupérations des valeurs
			 */
			if(i==frame_start){
				h_LIDAR.processing.PH=h_LIDAR.data_buff[i];
			}
			else if(i==frame_start+1){
				h_LIDAR.processing.PH=h_LIDAR.processing.PH|(h_LIDAR.data_buff[i]<<8);
			}
			else if(i==frame_start+2){
				h_LIDAR.processing.CT=h_LIDAR.data_buff[i];
			}
			/**
			 * Calcul du nouvel index de fin de trame
			 */
			else if(i==frame_start+3){
				frame_end=frame_start+9+2*h_LIDAR.data_buff[i];
				h_LIDAR.processing.LSN=h_LIDAR.data_buff[i];
			}
			else if(i==frame_start+4){
				h_LIDAR.processing.FSA=h_LIDAR.data_buff[i];
			}
			else if(i==frame_start+5){
				h_LIDAR.processing.FSA=h_LIDAR.processing.FSA|(h_LIDAR.data_buff[i]<<8);
			}
			else if(i==frame_start+6){
				h_LIDAR.processing.LSA=h_LIDAR.data_buff[i];
			}
			else if(i==frame_start+7){
				h_LIDAR.processing.LSA=h_LIDAR.processing.LSA|(h_LIDAR.data_buff[i]<<8);
			}
			else if(i==frame_start+8){
				h_LIDAR.processing.CS=h_LIDAR.data_buff[i];
			}
			else if(i==frame_start+9){
				h_LIDAR.processing.CS=h_LIDAR.processing.CS|(h_LIDAR.data_buff[i]<<8);
			}
			else if(i==frame_end){
				h_LIDAR.processing.frame_buff[h_LIDAR.processing.idx++]=h_LIDAR.data_buff[i];
				if(frame_end-frame_start>11){
					LIDAR_process_frame(&h_LIDAR);  //! On récupère les différents points
					medianFilter(&h_LIDAR);  //! On filtre ces points
					find_clusters(&h_LIDAR);  //! On regroupe ces points
				}
				h_LIDAR.processing.idx=0;
				frame_start=frame_end+1;
				frame_end=frame_start+5;
			}
			else{
				h_LIDAR.processing.frame_buff[h_LIDAR.processing.idx++]=h_LIDAR.data_buff[i];
			}
		}
		frame_start=frame_start-DATA_BUFF_SIZE;
		frame_end=frame_end-DATA_BUFF_SIZE;

		/**
		 * Envoi du/des points sur l'UART pour pouvoir les tracer via le programme python
		 */
		printf("#\r\n");
		distance_min=1000;
		for(int i=0 ; i<h_LIDAR.processing.cluster_cnt ; i++){
			if(h_LIDAR.processing.clusters[i].distance_moyenne<distance_min && h_LIDAR.processing.clusters[i].distance_moyenne !=0){
				distance_min = h_LIDAR.processing.clusters[i].distance_moyenne;
				idx_min=i;
			}
			//printf("%d,%d\r\n",h_LIDAR.processing.clusters[i].angle_moyen,h_LIDAR.processing.clusters[i].distance_moyenne);
		}
		printf("%d,%d\r\n",h_LIDAR.processing.clusters[idx_min].angle_moyen,h_LIDAR.processing.clusters[idx_min].distance_moyenne);

		vitesse = h_LIDAR.processing.clusters[idx_min].distance_moyenne;
		angle = h_LIDAR.processing.clusters[idx_min].angle_moyen;
		//
		//		if ( hunter == 1){
		//			ajusterAlphaMoteurs(angle,vitesse,&motor1,&motor2);
		//		}
		//		else{
		//			angle = angle_inverse(angle);
		//			ajusterAlphaMoteurs(angle,vitesse, &motor1,&motor2);
		//		}

		/**
		 * Calcul des rapports cycliques des moteurs pour suivre ou fuir le robot le plus proche en vonction de la valeur de hunter
		 */
		if (hunter == 1){
			if (angle > 180){
				alpha1 = (180 - angle )*2 + 400 + vitesse;
				alpha2 = 300 + vitesse/3;
				if (alpha1 > 900 ){
					alpha1 = 900;
				}
			}
			else{
				alpha2 = (angle -180)*2 + 400 + vitesse;
				alpha1 = 300 + vitesse/3 ;
				if (alpha2 > 900){
					alpha2 = 900;
				}
			}
			motor2.sens = SENS_MOTOR_2;
			motor1.sens = SENS_MOTOR_1;
			motor1.alpha = alpha1;
			motor2.alpha = alpha2;
		}
		else{
			angle = angle_inverse(angle);
			if (angle > 180){
				alpha1 = (180 - angle )*2 + 400 + vitesse;
				alpha2 = 300 + vitesse/3;
				if (alpha1 > 900 ){
					alpha1 = 900;
				}
			}
			else{
				alpha2 = (angle -180)*2 + 400 + vitesse;
				alpha1 = 300 + vitesse/3 ;
				if (alpha2 > 900){
					alpha2 = 900;
				}
			}

			motor2.sens = SENS_MOTOR_1;
			motor1.sens =SENS_MOTOR_2;
			motor1.alpha =   alpha2;
			motor2.alpha =   alpha1;
		}
	}
}

/**
 * Tache appliquant les rapports cyclique calculés dans la tache LIDAR aux moteurs toutes les 50 ms
 */
void TaskMOTOR (void * pvParameters){
	for(;;){
		update_motor(motor1);
		update_motor(motor2);
		vTaskDelay(50);
	}
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main(void)
{
	/* USER CODE BEGIN 1 */

	/* USER CODE END 1 */

	/* MCU Configuration--------------------------------------------------------*/

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	HAL_Init();

	/* USER CODE BEGIN Init */

	/* USER CODE END Init */

	/* Configure the system clock */
	SystemClock_Config();

	/* USER CODE BEGIN SysInit */

	/* USER CODE END SysInit */

	/* Initialize all configured peripherals */
	MX_GPIO_Init();
	MX_DMA_Init();
	MX_ADC1_Init();
	MX_TIM1_Init();
	MX_TIM3_Init();
	MX_TIM14_Init();
	MX_TIM15_Init();
	MX_TIM16_Init();
	MX_TIM17_Init();
	MX_USART1_UART_Init();
	MX_USART4_UART_Init();
	MX_SPI1_Init();
	MX_TIM6_Init();
	/* USER CODE BEGIN 2 */
	h_LIDAR.serial_drv.transmit=uart_transmit;
	h_LIDAR.serial_drv.it_transmit=uart_it_transmit;
	h_LIDAR.serial_drv.dma_transmit=uart_dma_transmit;
	h_LIDAR.serial_drv.poll_receive=uart_poll_receive;
	h_LIDAR.serial_drv.it_receive=uart_it_receive;
	h_LIDAR.serial_drv.dma_receive=uart_dma_receive;

	printf("\r\n===== Driver_LIDAR =====\r\n");

	LIDAR_stop(&h_LIDAR);
	HAL_Delay(1000);
	LIDAR_get_info(&h_LIDAR); //! Récupère les infos du LIDAR
	LIDAR_get_health_stat(&h_LIDAR); //! Récupère l'état du LIDAR

	ret = xTaskCreate(TaskLIDAR,"TaskLIDAR",STACK_SIZE,(void *) NULL,1,&xHandleLIDAR); //! Création de la tache LIDAR
	if (ret != pdPASS)
	{
		printf("Error creating TaskLIDAR\r\n");
		Error_Handler();
	}
	printf("Task LIDAR created\r\n");

	ret = xTaskCreate(TaskMOTOR,"TaskMOTOR",STACK_SIZE,(void *) NULL,1,&xHandleMOTOR); //! Création de la tache MOTEUR
	if (ret != pdPASS)
	{
		printf("Error creating TaskMOTOR\r\n");
		Error_Handler();
	}
	printf("Task MOTOR created\r\n");

	SemHalfCallBack = xSemaphoreCreateBinary();
	SemClpCallBack = xSemaphoreCreateBinary();

	capt_init(); //! Initialise la tache capt

	LIDAR_start(&h_LIDAR); //! Démarrage du LIDAR

	/**
	 * Initialisation
	 */
	motor_Init(&motor1,&motor2);
	motor1.sens = SENS_MOTOR_1;
	motor1.old_sens = SENS_MOTOR_1;
	motor1.pwm_start(motor1.Timer_Backward,motor1.Channel_Backward);
	motor1.pwm_start(motor1.Timer_Forward,motor1.Channel_Forward);
	motor2.sens = SENS_MOTOR_2;
	motor2.old_sens = SENS_MOTOR_2;
	motor2.pwm_start(motor2.Timer_Backward,motor2.Channel_Backward);
	motor2.pwm_start(motor2.Timer_Forward,motor2.Channel_Forward);

	/**
	 * Codeur
	 */
	codeur_init(&hCodeurLeft,&htim3,0);
	codeur_init(&hCodeurRight,&htim1,0);

	vTaskStartScheduler(); //! Lancement du scheduler
	/* USER CODE END 2 */

	/* Call init function for freertos objects (in freertos.c) */
	MX_FREERTOS_Init();
	/* Start scheduler */
	osKernelStart();

	/* We should never get here as control is now taken by the scheduler */
	/* Infinite loop */
	/* USER CODE BEGIN WHILE */

	while (1)
	{
		/* USER CODE END WHILE */
		/* USER CODE BEGIN 3 */
	}
	/* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config(void)
{
	RCC_OscInitTypeDef RCC_OscInitStruct = {0};
	RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

	/** Configure the main internal regulator output voltage
	 */
	HAL_PWREx_ControlVoltageScaling(PWR_REGULATOR_VOLTAGE_SCALE1);

	/** Initializes the RCC Oscillators according to the specified parameters
	 * in the RCC_OscInitTypeDef structure.
	 */
	RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
	RCC_OscInitStruct.HSIState = RCC_HSI_ON;
	RCC_OscInitStruct.HSIDiv = RCC_HSI_DIV1;
	RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
	RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
	RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
	RCC_OscInitStruct.PLL.PLLM = RCC_PLLM_DIV1;
	RCC_OscInitStruct.PLL.PLLN = 8;
	RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
	RCC_OscInitStruct.PLL.PLLR = RCC_PLLR_DIV2;
	if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
	{
		Error_Handler();
	}

	/** Initializes the CPU, AHB and APB buses clocks
	 */
	RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
			|RCC_CLOCKTYPE_PCLK1;
	RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
	RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
	RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

	if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
	{
		Error_Handler();
	}
}

/* USER CODE BEGIN 4 */

/**
 * Complete Callback de la réception DMA de l'UART
 */
void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t higher_priority_task_woken = pdFALSE;
	xSemaphoreGiveFromISR(SemHalfCallBack,&higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

/**
 * Half Callback de la réception DMA de l'UART
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
	BaseType_t higher_priority_task_woken = pdFALSE;
	xSemaphoreGiveFromISR(SemClpCallBack,&higher_priority_task_woken);
	portYIELD_FROM_ISR(higher_priority_task_woken);
}

/* USER CODE END 4 */

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM7 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
	/* USER CODE BEGIN Callback 0 */
	time++; //! Compte le nombre d'appels au SysTick
	/* USER CODE END Callback 0 */
	if (htim->Instance == TIM7) {
		HAL_IncTick();
	}
	/* USER CODE BEGIN Callback 1 */

	/* USER CODE END Callback 1 */
}

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler(void)
{
	/* USER CODE BEGIN Error_Handler_Debug */
	/* User can add his own implementation to report the HAL error return state */
	__disable_irq();
	while (1)
	{
	}
	/* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
 * @brief  Reports the name of the source file and the source line number
 *         where the assert_param error has occurred.
 * @param  file: pointer to the source file name
 * @param  line: assert_param error line source number
 * @retval None
 */
void assert_failed(uint8_t *file, uint32_t line)
{
	/* USER CODE BEGIN 6 */
	/* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
	/* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
