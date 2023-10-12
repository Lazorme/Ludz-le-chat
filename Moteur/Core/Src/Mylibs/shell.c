/**
 *@file shell.c
 *@brief Shell permettant de controller le moteur
 *@date Oct 1, 2023
 *@author Nicolas
 *@author Tom
 *@author Baptiste
 *@author Antoine
 */

#include "usart.h"
#include "shell.h"
#include "pwm.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

uint8_t prompt[]="user@Nucleo-STM32G474RET6>>";
uint8_t started[]=
		"\r\n*-----------------------------*"
		"\r\n| Welcome on Nucleo-STM32G474 |"
		"\r\n*-----------------------------*"
		"\r\n";
uint8_t newline[]="\r\n";
uint8_t backspace[]="\b \b";
uint8_t cmdNotFound[]="Command not found\r\n";
uint8_t brian[]="Brian is in the kitchen\r\n";
uint8_t uartRxReceived;
uint8_t uartRxBuffer[UART_RX_BUFFER_SIZE];
uint8_t uartTxBuffer[UART_TX_BUFFER_SIZE];

char	 	cmdBuffer[CMD_BUFFER_SIZE];
int 		idx_cmd;
char* 		argv[MAX_ARGS];
int		 	argc = 0;
char*		token;
int 		newCmdReady = 0;
int			motorStart =0; //permet de connaitre l'état de fonctionnement du moteur (on ou off)

/**
 * @brief Fonction d'initialisation du Shell
 * @note Affiche un message d'accueil lors du lançement du programme
 */

void Shell_Init(void){
	memset(argv, NULL, MAX_ARGS*sizeof(char*));
	memset(cmdBuffer, NULL, CMD_BUFFER_SIZE*sizeof(char));
	memset(uartRxBuffer, NULL, UART_RX_BUFFER_SIZE*sizeof(char));
	memset(uartTxBuffer, NULL, UART_TX_BUFFER_SIZE*sizeof(char));

	HAL_UART_Receive_IT(&hlpuart1, uartRxBuffer, UART_RX_BUFFER_SIZE);
	HAL_UART_Transmit(&hlpuart1, started, strlen((char *)started), HAL_MAX_DELAY);
	HAL_UART_Transmit(&hlpuart1, prompt, strlen((char *)prompt), HAL_MAX_DELAY);
}

/**
 * @brief Cette fonction tourne en boucle afin de recevoir et d'exécuter les commandes du Shell
 */
void Shell_Loop(void){
	if(uartRxReceived){ //Condition verifiée lors de la réception d'un nouveau caractère UART
		switch(uartRxBuffer[0]){
		case ASCII_CR: //Nouvelle ligne, instruction à traiter
			HAL_UART_Transmit(&hlpuart1, newline, sizeof(newline), HAL_MAX_DELAY);
			cmdBuffer[idx_cmd] = '\0';
			argc = 0;
			token = strtok(cmdBuffer, " ");
			while(token!=NULL){
				argv[argc++] = token;
				token = strtok(NULL, " ");
			}
			idx_cmd = 0;
			newCmdReady = 1;
			break;
		case ASCII_BACK: //Suppression du dernier caractère
			cmdBuffer[idx_cmd--] = '\0';
			HAL_UART_Transmit(&hlpuart1, backspace, sizeof(backspace), HAL_MAX_DELAY);
			break;

		default: //Nouveau caractère
			cmdBuffer[idx_cmd++] = uartRxBuffer[0];
			HAL_UART_Transmit(&hlpuart1, uartRxBuffer, UART_RX_BUFFER_SIZE, HAL_MAX_DELAY);
		}
		uartRxReceived = 0;
	}

	if(newCmdReady){ //Condition vérifiant la validitée d'une commande
		if(strcmp(argv[0],"WhereisBrian?")==0){
			HAL_UART_Transmit(&hlpuart1, brian, sizeof(brian), HAL_MAX_DELAY);
		}
		else if(strcmp(argv[0],"help")==0){//Fonction help renvoyant la notice des fonctions spécifiées
			int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Print all available functions here\r\n");
			HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
			if(strcmp(argv[1],"speed")==0){
				int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Prend en argument un rapport cyclique entre 0 et 100\r\n");
				HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
			}
		}
		else if(strcmp(argv[0],"speed")==0){//Fonction permettant de choisir le sens de rotation et la vitesse du moteur
			if(atoi(argv[1])>=0){//La valeur de vitesse reçue étant une chaine de caractères ASCII, atoi permet de la convertir en entier
				if(atoi(argv[1])<=100){
					set_pwm_alpha(atoi(argv[1]));//Appelle à la fonction permettant de changer le rapport cyclique
					int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Nouveau alpha = %d\r\n",atoi(argv[1]));
					HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
				}
				else{//Sécuritée permettant de ne pas avoir un rapport cyclique supérieur à 75
					set_pwm_alpha(75);
					int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Nouveau alpha = 75\r\n");
					HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
				}
			}
			else{
				int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Erreur alpha doit être compris entre 0 et 100\r\n",atoi(argv[1]));
				HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
			}
		}

		else if(strcmp(argv[0],"start")==0){//Fonction permettant d'allumer les PWM
			if(motorStart == 0){
				pwm_start();
				set_pwm_alpha(50);
				int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Motor start\r\nalpha = 50\r\n");
				HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
				motorStart =1;
			}
			else{
				int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Motor is already on\r\n");
				HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
			}
		}

		else if(strcmp(argv[0],"stop")==0){//Fonction permettant d'éteindre les PWM
					if(motorStart == 1){
						pwm_stop();
						int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Motor stop\r\n");
						HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
						motorStart =0;
					}
					else{
						int uartTxStringLength = snprintf((char *)uartTxBuffer, UART_TX_BUFFER_SIZE, "Motor is already off\r\n");
						HAL_UART_Transmit(&hlpuart1, uartTxBuffer, uartTxStringLength, HAL_MAX_DELAY);
					}
				}

		else{
			HAL_UART_Transmit(&hlpuart1, cmdNotFound, sizeof(cmdNotFound), HAL_MAX_DELAY);
		}
		HAL_UART_Transmit(&hlpuart1, prompt, sizeof(prompt), HAL_MAX_DELAY);
		newCmdReady = 0;
	}
}

/**
 * @brief Fonction Callback appelée lors de la réception d'un nouveau caractère
 */
void HAL_UART_RxCpltCallback(UART_HandleTypeDef * huart){
	uartRxReceived = 1;
	HAL_UART_Receive_IT(&hlpuart1, uartRxBuffer, UART_RX_BUFFER_SIZE);
}
