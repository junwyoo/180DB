#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <mraa/i2c.h>
#include <mraa/pwm.h>
#include <mraa/aio.h>
#include <math.h>
#include "LSM9DS0.h"
#include "constants.h"


static volatile int run_flag = 1;

void int_handler(int sig)
{
	run_flag = 0;
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"Error: incorrect number of arguments\n");
		exit(1);
	}

	char *username = argv[1];
	char *filename = malloc(sizeof(char*)*BUFF_SIZE);
	
	printf("Testing with name (%s)\n",username);

    char *dir = (char*) malloc(sizeof(char)*BUFF_SIZE);
    sprintf(dir,"./%s%s","Train_Data_Set_", username);   
    mkdir(dir, 0777);

	int i = 0, stop = 0, file_number = 0;
	double time_start, time_wait, time_ref;
    struct timeval time_before, time_after;

	mraa_i2c_context accel, gyro, mag;
    float a_res, g_res, m_res;
    accel_scale_t a_scale;
    gyro_scale_t g_scale;
    mag_scale_t m_scale;
    data_t ad, gd, md;

    a_scale = A_SCALE_16G;
    g_scale = G_SCALE_500DPS;
    m_scale = M_SCALE_8GS;

    accel = accel_init();
    set_accel_ODR(accel, A_ODR_100);
    set_accel_scale(accel, a_scale);
    a_res = calc_accel_res(a_scale);

    gyro = gyro_init();
    set_gyro_ODR(gyro, G_ODR_190_BW_70);
    set_gyro_scale(gyro, g_scale);
    g_res = calc_gyro_res(g_scale);
    mag = mag_init();
    set_mag_ODR(mag, M_ODR_125);
    set_mag_scale(mag, m_scale);
    m_res = calc_mag_res(m_scale);

	signal(SIGINT, int_handler);

	while(stop < MAX_FILES && run_flag){
		time_wait = 0;
        stop++;


		sprintf(filename, "%s/test_%s_%ld.csv", dir, username, time(NULL));
		printf("\tTest file collection will begin.\n");

		//gather_data(filename,10,15); //this is for real testing
		FILE *fp = fopen(filename,"w"); //this is for dummy testing
		printf("Attempting to open file %s\n", filename);

		if(fp != NULL){

			fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
                " Acceleration z, Gyro x, Gyro y, Gyro z\n");
        	
        	gettimeofday(&time_before, NULL);
        	time_ref = time_before.tv_sec;
        	
        	while(time_wait < TEST_SIZE && run_flag)
        	{
            	ad = read_accel(accel, a_res);
            	gd = read_gyro(gyro, g_res);
            	gettimeofday(&time_before, NULL);
            	time_start = time_before.tv_sec + time_before.tv_usec / NORM;
            	time_wait += (int) (time_start - time_ref);
            	
            	fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);
            }
            usleep(100);
		}
		
		fclose(fp);

		printf("Testing file %s has been created.\n",filename);
		sleep(1);
	}
	
	return 0;
}
