/**
 * @file IIM2070_c
 * @brief Source file for the IIM2070 module.
 * 
 * This file contains the definitions of the SPI driver functions for the gyroscope and accelerometer
 * 
 * @author GILLES Baptiste
 */

#include "IIM2070_h.h"
#include <stdio.h>
#include <stdlib.h>
#include "gpio.h"

//Define
#define DATA_SIZE 24
uint8_t RW_WRITE_MSG = 0x80;
uint8_t RW_READ_MSG = 0x0;
float Fs =655.35; //Default value of resolution
float Fs_a =16.384;


// CRC
uint8_t calculateCRC (uint8_t *Input_data)
{
	// Variables
	uint8_t  CRC_h = 0xFF;
	uint8_t  CRC_New = 0x00;
	for (int i = 23; i >= 0; i--) {
		CRC_New = ((CRC_h << 1) & 0xFE) | ((CRC_h >> 7) & 0x01);
		CRC_New ^= Input_data[i];

		CRC_h = CRC_New;
	}
	// Final XOR
	for (int i = 0; i < 8; i++) {
		CRC_h ^= 0x01;
	}
	return CRC_h;
}

#include "spi.h"


int IIM2070_init(h_IIM2070_t * h_IIM2070)
{
	// Init SPI driver
	uint8_t addr = fixed_value << 2;
	//1st byte is write bit ,register address and return status (concatenated)
	uint8_t bufferTX[4]={RW_READ_MSG|addr,0x00,0x00,0x00};//2nd & 3rd byte are data to write (or read), 4th byte is CRC
	uint8_t bufferRX[4];


	for(int i=0;i<4;i++){
		printf("calibRX %2d = 0x%x\r\n",i,bufferTX[i]);
	}
	//BANK 0
	//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	//	HAL_SPI_TransmitReceive(&hspi1, bufferTXbank0,bufferRXbank0, 4, HAL_MAX_DELAY);
	//	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	//	if ((bufferRXbank0[1]<<8|bufferRXbank0[2]) == 0x0000)
	//		{
	//			printf("Error: IIM2070 not initialized_bank 0 error\n\r");
	//			for(int j=0;j<4;j++){
	//				printf("calibbank0 %2d = 0x%x\r\n",j,bufferRXbank0[j]);
	//			}
	//		}
	//	HAL_Delay(20);

	//ID
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_TransmitReceive(&hspi1, bufferTX, bufferRX, 4, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	if ((bufferRX[1]<<8|bufferRX[2]) != 0xAA55)
	{
		printf("Error: IIM2070 not initialized\n\r");
		for(int j=0;j<4;j++){
			printf("calibTX %2d = 0x%x\r\n",j,bufferRX[j]);
		}
	}
	h_IIM2070->fixed_value = bufferRX[1]<<8|bufferRX[2];
	printf("ID =0x%x\r\n",h_IIM2070->fixed_value);
	return 0;
}

int IIM2070_Read_Gyro(h_IIM2070_t *h_IIM2070)
{
	uint8_t g_x = IIM2070_gyro_x << 2;
//	uint8_t g_y = IIM2070_gyro_y << 2;
//	uint8_t g_z = IIM2070_gyro_z << 2;

	uint8_t bufferTX_x[4]={RW_READ_MSG|g_x,0x00,0x00,0xF1};
	uint8_t bufferTX_y[4]={0x04,0x00,0x00,0xDC};
	uint8_t bufferTX_z[4]={0x08,0x00,0x00,0xAB};
	uint8_t bufferTX[4]={0x5C,0x00,0x00,0xD8}; //send nothing

	uint8_t bufferRX_x[4];
	uint8_t bufferRX_y[4];
	uint8_t bufferRX_z[4];

	//Send commnand to G_x and receive nothing
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_Transmit(&hspi1,bufferTX_x,4, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);


	// Send command to G_y and read G_x
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_TransmitReceive(&hspi1, bufferTX_y, bufferRX_x, 4, HAL_MAX_DELAY);
	printf("calibX = 0x%x\r\n",bufferRX_x[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	h_IIM2070->gyro_x = ((bufferRX_x[1]<<8|bufferRX_x[2])*Fs)/(1<<15);
	printf("G_x = %f_dps\r\n",h_IIM2070->gyro_x);

	// Send command to G_z and read G_y
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_TransmitReceive(&hspi1, bufferTX_z, bufferRX_y, 4, HAL_MAX_DELAY);
	printf("calibY = 0x%x\r\n",bufferRX_y[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	h_IIM2070->gyro_y = ((bufferRX_y[1]<<8|bufferRX_y[2])*Fs)/(1<<15);
	printf("G_y = %f_dps\r\n",h_IIM2070->gyro_y);

	//Read G_z
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_TransmitReceive(&hspi1,bufferTX,bufferRX_z, 4, HAL_MAX_DELAY);
	printf("calibZ = 0x%x\r\n",bufferRX_z[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	h_IIM2070->gyro_z = ((bufferRX_z[1]<<8|bufferRX_z[2])*Fs)/(1<<15);
	printf("G_z = %f_dps\r\n",h_IIM2070->gyro_z);
	HAL_Delay(20);
	return 0;
}

int IIM2070_Read_Accel(h_IIM2070_t *h_IIM2070)
{
	uint8_t a_x = IIM2070_accel_x << 2;
	uint8_t a_y = IIM2070_accel_y << 2;
	uint8_t a_z = IIM2070_accel_z << 2;

	uint8_t bufferTX_x[4]={RW_READ_MSG|a_x,0x00,0x00,0x45};
	uint8_t bufferTX_y[4]={RW_READ_MSG|a_y,0x00,0x00,0x68};
	uint8_t bufferTX_z[4]={RW_READ_MSG|a_z,0x00,0x00,0x1F};
	uint8_t bufferTX[4]={0x5C,0x00,0x00,0xD8}; //send nothing

	uint8_t bufferRX[4];
	uint8_t bufferRX_x[4];
	uint8_t bufferRX_y[4];
	uint8_t bufferRX_z[4];

	//Send commnand to A_x and receive nothing
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_TransmitReceive(&hspi1,bufferTX_x,bufferRX,4, HAL_MAX_DELAY);
	printf("calib = 0x%x\r\n",bufferRX[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);


	// Send command to A_y and read A_x
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_TransmitReceive(&hspi1, bufferTX_y, bufferRX_x, 4, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	printf("calibX = 0x%x\r\n",bufferRX_x[0]);
	h_IIM2070->accel_x = (bufferRX_x[1]<<8|bufferRX_x[2])*Fs_a/(1<<15);
	printf("A_x = %f_g\r\n",h_IIM2070->accel_x);

	// Send command to A_z and read A_y
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_TransmitReceive(&hspi1, bufferTX_z, bufferRX_y, 4, HAL_MAX_DELAY);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	printf("calibY = 0x%x\r\n",bufferRX_y[0]);
	h_IIM2070->accel_y = ((bufferRX_y[1]<<8|bufferRX_y[2])*Fs_a)/(1<<15);
	printf("A_y = %f_g\r\n",h_IIM2070->accel_y);

	//Read A_z
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 0);
	HAL_SPI_TransmitReceive(&hspi1,bufferTX,bufferRX_z, 4, HAL_MAX_DELAY);
	printf("calibZ = 0x%x\r\n",bufferRX_z[0]);
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_10, 1);
	h_IIM2070->accel_z = ((bufferRX_z[1]<<8|bufferRX_z[2])*Fs_a)/(1<<15);
	printf("A_z = %f_g\r\n",h_IIM2070->accel_z);
	HAL_Delay(20);
	return 0;
}
