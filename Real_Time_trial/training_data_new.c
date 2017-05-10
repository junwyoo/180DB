//
// Created by Amaael Antonini on 5/7/17.
//

#include <stdio.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "constants.h"
#include "classifiers.h"
#include "training_data_new.h"
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>

//#include "LSM9DS0.h"
//#include <mraa/pwm.h>
//#include <mraa/aio.h>
//#include <mraa/i2c.h>

//int main()
//{
//
//    char * fake_test;
//    fake_test = (char*)malloc(sizeof(char)*BUFF_SIZE);
//    memset(fake_test, 0, sizeof(char)*BUFF_SIZE);
//    sprintf(fake_test, "%s_%s.csv", main_path, names[0]);
//    gather_data(fake_test, 5, 1);
//}
void gather_data(char *file_name, int hold_time, int record_time)
{

//    sig_atomic_t volatile interrupt_flag = 1;
//
//
//    void do_when_interrupted(int sig)
//    {
//        if (sig == SIGINT)
//            interrupt_flag = 0;
//    }

    int i;

    double time_start, time_end, time_wait, time_ref;
    struct timeval time_before, time_after;
    float a_res, g_res, m_res;
    FILE *fp;

//    mraa_i2c_context accel, gyro, mag;
//    accel_scale_t a_scale;
//    gyro_scale_t g_scale;
//    mag_scale_t m_scale;
//    data_t ad, gd, md;
//
//    a_scale = A_SCALE_16G;
//    g_scale = G_SCALE_500DPS;
//    m_scale = M_SCALE_8GS;
//
//    accel = accel_init();
//    set_accel_ODR(accel, A_ODR_100);
//    set_accel_scale(accel, a_scale);
//    a_res = calc_accel_res(a_scale);
//
//    gyro = gyro_init();
//    set_gyro_ODR(gyro, G_ODR_190_BW_70);
//    set_gyro_scale(gyro, g_scale);
//    g_res = calc_gyro_res(g_scale);

//    mag = mag_init();
//    set_mag_ODR(mag, M_ODR_125);
//    set_mag_scale(mag, m_scale);
//    m_res = calc_mag_res(m_scale);


    time_wait = 0;
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    int safe_guard;
    safe_guard = 0;
    printf("next %s\tin:\t%d\n", file_name, (int) (hold_time-time_wait));
    while (time_wait <= hold_time && safe_guard < 10000)
    {

        printf("%d\t%s\n", (int) (hold_time - time_wait), file_name);
        usleep(1000000);
        safe_guard++;
        gettimeofday(&time_before, NULL);
        time_end = time_before.tv_sec;
        time_wait = time_end - time_ref;
    }
    printf("Attempting to write to file \'%s\'.\n", file_name);
    fp = fopen(file_name, "w");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to write to file \'%s\'.\n",
                file_name
        );
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
            " Acceleration z, Gyro x, Gyro y, Gyro z\n");
    time_wait = 0;
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    while(time_wait < record_time /*&& interrupt_flag*/)
    {
        gettimeofday(&time_before, NULL);


//            ad = read_accel(accel, a_res);
//            gd = read_gyro(gyro, g_res);

        time_wait = time_before.tv_sec - time_ref;

        time_start = time_before.tv_sec + time_before.tv_usec / NORM;
//            fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
//                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

        usleep(100);
    }
    fclose(fp);
}








