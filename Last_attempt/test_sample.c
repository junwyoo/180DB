//
// Created by Amaael Antonini on 4/12/17.
//

#include <stdio.h>
#include <mraa/i2c.h>
#include <signal.h>
#include <mraa/pwm.h>
#include <mraa/aio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "LSM9DS0.h"

#define BUFF_SIZE 1024
#define FILE_TIME
#define RUN 30
#define NORM 1000000.0f
#define FILES 12
#define IDLE 40
const char * test_name = "/Users/amaaelantonini/Google Drive/Local/Spring_2017/EE 180DB/Real_Time_trial/test";
const char * path = "/Users/amaaelantonini/Google Drive/Local/Spring_2017/EE 180DB/Real_Time_trial/";

//sig_atomic_t volatile run_flag = 1;
//
//void do_when_interrupted(int sig)
//{
//    if (sig == SIGINT)
//        run_flag = 0;
//}

void test_sample()
{
    char* file_name;
    file_name = (char*)malloc(FILES*sizeof(char));
    memset(file_name, 0, BUFF_SIZE);
    int i = 0;
    int file_number;
    file_number = 0;
    FILE *fp;

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
    int stop = 0;
    while(stop < 10 && run_flag) {
        time_wait = 0;
        spritf(file_name, "%s_%d.csv", test_name, file_number);
        fp = fopen(file_names[i], "w");
        if (fp == NULL) {
            fprintf(stderr,
                    "Failed to write to file \'%s\'.\n",
                    file_names[i]
            );
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "time_stamp before, time_stamp after, Acceleration x, Acceleration y,"
                " Acceleration z, Gyro x, Gyro y, Gyro z\n");
        while(time_wait < RUN && run_flag)
        {
            gettimeofday(&time_before, NULL);
            ad = read_accel(accel, a_res);
            gd = read_gyro(gyro, g_res);

            time_wait = time_before.tv_sec - time_ref;

            time_start = time_before.tv_sec + time_before.tv_usec / NORM;
//            time_end = time_after.tv_sec + time_after.tv_usec / NORM;
            fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

            usleep(100);
        }

        fclose(fp);
        file_number++;
    }
    return 0;
}








