#include "drv_LIDAR.h"
#include "stdio.h"

//Start scanning and export point cloud data
//Sustained response
int LIDAR_start(h_LIDAR_t * h_LIDAR){
	uint8_t cmd_buff[CMD_BUFF_SIZE]={CMD_BEGIN,CMD_START};
	h_LIDAR->serial_drv.dma_receive(h_LIDAR->data_buff,DATA_BUFF_SIZE);
	h_LIDAR->serial_drv.transmit(cmd_buff,CMD_BUFF_SIZE);
	return 0;
}

//Stop and stop scanning
//No answer
int LIDAR_stop(h_LIDAR_t * h_LIDAR){
	uint8_t cmd_buff[CMD_BUFF_SIZE]={CMD_BEGIN,CMD_STOP};
	h_LIDAR->serial_drv.transmit(cmd_buff,CMD_BUFF_SIZE);
	return 0;
}

//Get device information
//Single response
int LIDAR_get_info(h_LIDAR_t * h_LIDAR){
	uint8_t cmd_buff[CMD_BUFF_SIZE]={CMD_BEGIN,CMD_INFO}; //Buffer de commande
	h_LIDAR->serial_drv.transmit(cmd_buff,CMD_BUFF_SIZE);
	h_LIDAR->serial_drv.poll_receive(h_LIDAR->info_buff,INFO_BUFF_SIZE);

	h_LIDAR->device_info.start_sign=(h_LIDAR->info_buff[0]<<8)|h_LIDAR->info_buff[1];
	h_LIDAR->device_info.lenght=(h_LIDAR->info_buff[2])|(h_LIDAR->info_buff[3]<<8)|(h_LIDAR->info_buff[4]<<16);
	h_LIDAR->device_info.mode=h_LIDAR->info_buff[5];
	h_LIDAR->device_info.type_code=h_LIDAR->info_buff[6];
	h_LIDAR->device_info.model=h_LIDAR->info_buff[7];
	snprintf(h_LIDAR->device_info.firmware,6,"%d.%d",h_LIDAR->info_buff[8],h_LIDAR->info_buff[9]);
	h_LIDAR->device_info.hardware=h_LIDAR->info_buff[10];
	snprintf(h_LIDAR->device_info.serial,17,"%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x%x",h_LIDAR->info_buff[11],h_LIDAR->info_buff[12],h_LIDAR->info_buff[13],h_LIDAR->info_buff[14],h_LIDAR->info_buff[15],h_LIDAR->info_buff[16],h_LIDAR->info_buff[17],h_LIDAR->info_buff[18],h_LIDAR->info_buff[19],h_LIDAR->info_buff[20],h_LIDAR->info_buff[21],h_LIDAR->info_buff[22],h_LIDAR->info_buff[23],h_LIDAR->info_buff[24],h_LIDAR->info_buff[25],h_LIDAR->info_buff[26]);

	printf("Start sign : %x\r\n",h_LIDAR->device_info.start_sign);
	printf("Length : %ld\r\n",h_LIDAR->device_info.lenght);
	printf("Mode : %x\r\n",h_LIDAR->device_info.mode);
	printf("Type code : %x\r\n",h_LIDAR->device_info.type_code);
	printf("Model %x\r\n",h_LIDAR->device_info.model);
	printf("Firmware version : %s\r\n",h_LIDAR->device_info.firmware);
	printf("Hardware version : %d\r\n",h_LIDAR->device_info.hardware);
	printf("Serial number : %s\r\n",h_LIDAR->device_info.serial);

	return 0;
}

//Get device health status
//Single response
int LIDAR_get_health_stat(h_LIDAR_t * h_LIDAR){
	uint8_t cmd_buff[CMD_BUFF_SIZE]={CMD_BEGIN,CMD_HEALTH}; //Buffer de commande
	h_LIDAR->serial_drv.transmit(cmd_buff,2);
	h_LIDAR->serial_drv.poll_receive(h_LIDAR->health_buff,HEALTH_BUFF_SIZE);

	h_LIDAR->health_stat.start_sign=(h_LIDAR->health_buff[0]<<8)|h_LIDAR->health_buff[1];
	h_LIDAR->health_stat.lenght=(h_LIDAR->health_buff[2])|(h_LIDAR->health_buff[3]<<8)|(h_LIDAR->health_buff[4]<<16);
	h_LIDAR->health_stat.mode=h_LIDAR->health_buff[5];
	h_LIDAR->health_stat.type_code=h_LIDAR->health_buff[6];
	h_LIDAR->health_stat.status_code=h_LIDAR->health_buff[7];
	h_LIDAR->health_stat.error_code=h_LIDAR->health_buff[8]|(h_LIDAR->health_buff[9]<<8);

	printf("Start sign : %x\r\n",h_LIDAR->health_stat.start_sign);
	printf("Length : %ld\r\n",h_LIDAR->health_stat.lenght);
	printf("Mode : %x\r\n",h_LIDAR->health_stat.mode);
	printf("Type code : %x\r\n",h_LIDAR->health_stat.type_code);
	printf("Status code : %x\r\n",h_LIDAR->health_stat.status_code);
	printf("Error code : %x\r\n",h_LIDAR->health_stat.error_code);

	return 0;
}

//Soft restart
//No response
int LIDAR_restart(h_LIDAR_t * h_LIDAR){
	uint8_t cmd_buff[CMD_BUFF_SIZE]={CMD_BEGIN,CMD_RESTART};
	h_LIDAR->serial_drv.transmit(cmd_buff,CMD_BUFF_SIZE);
	return 0;
}

void LIDAR_process_frame(h_LIDAR_t * LIDAR){
	uint16_t Si;
	int Di;
	int Ai;
	int AngleFSA=(LIDAR->processing.FSA>>1); //64
	int AngleLSA=(LIDAR->processing.LSA>>1);
	int index;
	for(int i=0;i<LIDAR->processing.idx/2;i++){
		Si=LIDAR->processing.frame_buff[2*i]|(LIDAR->processing.frame_buff[2*i+1]<<8);
		Di=Si/4; //Distance du point i
		Ai=AngleFSA/64+i*(AngleLSA-AngleFSA)/64/(LIDAR->processing.LSN-1);
		index = round(Ai);

		if(Di>1500){ //On affiche pas les points trop loin
			LIDAR->processing.point_buff[index]=0;
		}
		else if(Di<40){
			LIDAR->processing.point_buff[index]=0;
		}
		else{
			LIDAR->processing.point_buff[index]=Di;
		}
	}
}

// Fonction pour calculer la distance moyenne
int calculer_distance_moyenne(int distances[], int debut, int fin) {
	int somme = 0;
	for (int i = debut; i <= fin; i++) {
		somme += distances[i];
	}
	return somme / (fin - debut + 1);
}

//Fonction pour regrouper les points proches en clusters
void find_clusters(h_LIDAR_t * LIDAR) {

	int * distances=LIDAR->processing.filtred_buff;
	int cluster_count = 0;

	// Parcourir les 360 degrés pour regrouper les valeurs en clusters
	int debut_cluster = 0;
	for (int i = 1; i < NB_DEGRES; i++) {
		if (fabs(distances[i] - distances[i - 1]) > CLUSTER_SEUIL) {
			// Calcul de la distance moyenne pour le cluster
			int distance_moyenne = calculer_distance_moyenne(distances, debut_cluster, i - 1);

			// Calcul de l'angle moyen pour le cluster
			int angle_moyen = (debut_cluster + i - 1) / 2;

			// Stockage des valeurs du cluster dans la structure
			LIDAR->processing.clusters[cluster_count].angle_moyen = angle_moyen;
			LIDAR->processing.clusters[cluster_count].distance_moyenne = distance_moyenne;
			LIDAR->processing.clusters[cluster_count].count = i - debut_cluster;

			debut_cluster = i;
			cluster_count++;
		}
	}

	// Traitement du dernier cluster
	int distance_moyenne = calculer_distance_moyenne(distances, debut_cluster, NB_DEGRES - 1);
	int angle_moyen = (debut_cluster + NB_DEGRES - 1) / 2;
	LIDAR->processing.clusters[cluster_count].angle_moyen = angle_moyen;
	LIDAR->processing.clusters[cluster_count].distance_moyenne = distance_moyenne;
	LIDAR->processing.clusters[cluster_count].count = NB_DEGRES - debut_cluster;
	cluster_count++;
	LIDAR->processing.cluster_cnt=cluster_count;
}

void medianFilter(h_LIDAR_t * LIDAR) {
	int * signal=LIDAR->processing.point_buff; //points non filtrés
	int signal_length=NB_DEGRES;
	int window[5];
	int i, j, k, middle;

	// La médiane se trouve au milieu de la fenêtre triée
	middle = 5 / 2;

	for (i = 0; i < signal_length; i++) {
		// Construire la fenêtre avec les données autour du point i
		for (j = 0; j < 5; j++) {
			int index = i - middle + j;
			// Gérer les bords du signal
			if (index < 0) index = 0;
			if (index >= signal_length) index = signal_length - 1;
			window[j] = signal[index];
		}

		// Trier la fenêtre pour trouver la valeur médiane
		for (j = 0; j < 5; j++) {
			for (k = j + 1; k < 5; k++) {
				if (window[j] > window[k]) {
					// Échange simple pour le tri
					int temp = window[j];
					window[j] = window[k];
					window[k] = temp;
				}
			}
		}

		// Stocker la médiane dans le signal filtré
		LIDAR->processing.filtred_buff[i] = window[middle];
	}
}
