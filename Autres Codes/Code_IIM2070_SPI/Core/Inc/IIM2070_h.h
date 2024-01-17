/**
 * @file IIM2070_h
 * @brief Header file for the IIM2070 module.
 * 
 * This file contains the declarations of the SPI driver functions for the gyroscope and accelerometer
 * of the IIM2070 module. It also includes the necessary libraries for the module.
 * 
 * @author GILLES Baptiste
 */

#ifndef IIM2070_H_
#define IIM2070_H_

#include <stdint.h>
#include <stdbool.h>

typedef enum IIM2070_register_enum
{
	bank_select       = 0x1F, //15:0
	whoami            = 0x0E, //7:0 , default value: 0xF3 bank 1
	soft_reset        = 0x18, //1:0 , default value: 0x0
	en_accel_selftest = 0x11, //bits 11:10 , default value: 0x0 bank 1
	en_gyro_selftest  = 0x12, //bits 12:11 , default value: 0x0 bank 1

} IIM2070_register_t;

typedef enum IIM2070_data_enum
{
	IIM2070_accel_x = 0x04, //15:0
	IIM2070_accel_y = 0x05, //15:0
	IIM2070_accel_z = 0x06, //15:0
	IIM2070_gyro_x  = 0x00, //15:0
	IIM2070_gyro_y  = 0x01, //15:0
	IIM2070_gyro_z  = 0x02, //15:0
	IIM2070_temp_1  = 0x03, //15:0
	IIM2070_temp_2  = 0x07, //15:0
	fixed_value     = 0x0B, //15:0 Fixed value 0xAA55
} IIM2070_data_t;

typedef enum IIM2070_filter_enum //Output filter setting for the gyroscope’s and accelerometer’s x,y,z axis
{
	IIM2070_filter_x = 0x0E, //13:8 x
	IIM2070_filter_y_z = 0x0C, //15:0 ,5:0 y, 11:6 z
	senitivity = 0x14, //accel_fs_sel[2:0] 2g: 0x00, 4g: 0x01, 8g: 0x02, 16g: 0x03 | gyro_fs_sel[3:0] 250dps: 0x00, 500dps: 0x01, 1000dps: 0x02, 2000dps: 0x03
} IIM2070_filter_t;

// define SPI driver functions for IIM2070
typedef int (*IIM2070_transmit_receive_drv_t)(uint8_t *p_data_RX,uint8_t *p_data_TX, uint16_t size);

typedef struct IIM2071_SPI_drv_struct
{
	IIM2070_transmit_receive_drv_t transmit_receive;
} IIM2070_I2C_drv_t;

typedef struct h_IIM2070_struct
{
	// Chip ID
	uint8_t whoami;
	uint16_t fixed_value;
	uint8_t bank_select;

	// Filter
	uint8_t filter_x;
	uint8_t filter_y;
	uint8_t filter_z;

	// driver SPI
	IIM2070_I2C_drv_t SPI_drv;

	//Accel, Gyro, Temp
	float accel_x;
	float accel_y;
	float accel_z;
	float gyro_x;
	float gyro_y;
	float gyro_z;
	float temp_1;
	float temp_2;

	// Data available
	uint8_t data_available;

	// Skipped data
	uint32_t skipped_data;

} h_IIM2070_t;


// Define SPI driver functions for gyroscope
int IIM2070_init(h_IIM2070_t * h_IIM2070);
uint8_t calculateCRC (uint8_t *Input_data);
int IIM2070_Read_Gyro(h_IIM2070_t *h_IIM2070);
int IIM2070_Read_Accel(h_IIM2070_t *h_IIM2070);
//void gyroscope_read_data(int16_t* x, int16_t* y, int16_t* z);
//void accelerometer_read_data(int16_t* x, int16_t* y, int16_t* z);

#endif /* IIM2070_H_ */

