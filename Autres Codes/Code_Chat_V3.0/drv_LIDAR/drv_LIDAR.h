#ifndef DRV_LIDAR_H_
#define DRV_LIDAR_H_

#include <stdint.h>
#include "math.h"

#define INFO_BUFF_SIZE 27
#define HEALTH_BUFF_SIZE 10
#define CMD_BUFF_SIZE 2
#define DATA_BUFF_SIZE 4000
#define FRAME_BUFF_SIZE 90	//Au max on a 0x28=40 points par packet (80o) + le header (10o)
#define POINT_BUFF_SIZE 360 //Les 360° autour du LIDAR
#define NB_DEGRES 360
#define CLUSTER_SEUIL 150

typedef enum LIDAR_command_enum
{
	CMD_BEGIN   = 0xA5, //Premier octet de la commande
	CMD_START   = 0x60, //Start scanning and export point cloud data
	CMD_STOP    = 0x65, //Stop and stop scanning
	CMD_INFO    = 0x90, //Get device information (model, firmware, hardware version)
	CMD_HEALTH  = 0x91, //Get device health status
	CMD_RESTART = 0x80  //Soft restart
} LIDAR_command_t;

//transmission en blockage
typedef int (* LIDAR_transmit_drv_t)(uint8_t *p_data, uint16_t size);
//transmission en interruption
typedef int (* LIDAR_it_transmit_drv_t)(uint8_t *p_data, uint16_t size);
//transmission en DMA
typedef int (* LIDAR_dma_transmit_drv_t)(uint8_t *p_data, uint16_t size);
//réception en polling
typedef int (* LIDAR_poll_receive_drv_t)(uint8_t *p_data, uint16_t size);
//réception en interruption
typedef int (* LIDAR_it_receive_drv_t)(uint8_t *p_data, uint16_t size);
//réception en DMA
typedef int (* LIDAR_DMA_receive_drv_t)(uint8_t *p_data, uint16_t size);

typedef struct LIDAR_serial_drv_struct
{
	LIDAR_transmit_drv_t transmit;
	LIDAR_transmit_drv_t it_transmit;
	LIDAR_transmit_drv_t dma_transmit;
	LIDAR_poll_receive_drv_t poll_receive;
	LIDAR_it_receive_drv_t it_receive;
	LIDAR_DMA_receive_drv_t dma_receive;
} LIDAR_serial_drv_t;

typedef struct LIDAR_health_stat_struct
{
	uint16_t start_sign;
	uint32_t lenght;
	uint8_t mode;
	uint8_t type_code;
	uint8_t status_code;
	uint16_t error_code;
}LIDAR_health_stat_t;

typedef struct LIDAR_device_info_struct
{
	uint16_t start_sign;
	uint32_t lenght;
	uint8_t mode;
	uint8_t type_code;
	uint8_t model;
	char firmware[6];
	uint8_t hardware;
	char serial[17];
}LIDAR_device_info_t;

// Structure pour stocker les informations sur un cluster
typedef struct LIDAR_cluster_struct {
	int angle_moyen;
	int distance_moyenne;
	int count; // Nombre de valeurs dans le cluster
} LIDAR_cluster_t;

typedef struct LIDAR_processing_struct
{
	//Header
	uint16_t PH;

	//CT Package Type / fréquence
	uint8_t CT;

	//LSN nombre de points de la trame
	uint8_t LSN;

	//FSA Start Angle
	uint16_t FSA;

	//LSA End Angle
	uint16_t LSA;

	//CS Check Code
	uint16_t CS;

	//Frame index
	uint8_t idx;

	//Buffer contenant les trames reçu en DMA
	uint8_t receive_buff[DATA_BUFF_SIZE];

	//Buffer contenant une frame
	uint8_t frame_buff[FRAME_BUFF_SIZE];

	//Buffer contenant les points
	int point_buff[POINT_BUFF_SIZE];

	//Buffer discret
	int filtred_buff[POINT_BUFF_SIZE];

	//Buffer cluster
	LIDAR_cluster_t clusters[100]; //Je limite pas le fait qu'il puisse y avoir plus de 50 clusters ça peut pauser problème
	int cluster_cnt;               //Maintenant si

}LIDAR_processing_t;

typedef struct h_LIDAR_struct
{
	//driver UART
	LIDAR_serial_drv_t serial_drv;

	//health status
	LIDAR_health_stat_t health_stat;

	//device info
	LIDAR_device_info_t device_info;

	//variables propres au lidar
	uint8_t info_buff[INFO_BUFF_SIZE];
	uint8_t health_buff[HEALTH_BUFF_SIZE];
	uint8_t data_buff[DATA_BUFF_SIZE];

	//Traitement des trames
	LIDAR_processing_t processing;


} h_LIDAR_t;

int LIDAR_start(h_LIDAR_t * h_LIDAR);
int LIDAR_stop(h_LIDAR_t * h_LIDAR);
int LIDAR_get_info(h_LIDAR_t * h_LIDAR);
int LIDAR_get_health_stat(h_LIDAR_t * h_LIDAR);
int LIDAR_restart(h_LIDAR_t * h_LIDAR);
void LIDAR_process_frame(h_LIDAR_t *LIDAR);
int calculer_distance_moyenne(int distances[], int debut, int fin);
void find_clusters(h_LIDAR_t * LIDAR);
void medianFilter(h_LIDAR_t * LIDAR);

#endif /* DRV_LIDAR_H_ */


