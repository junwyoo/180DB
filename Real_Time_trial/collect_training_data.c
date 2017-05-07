#include <stdio.h>
#include <mraa/i2c.h>
#include <signal.h>
#include <mraa/pwm.h>
#include <mraa/aio.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include "LSM9DS0.h"
#include "constants.h"




void collect_training_data(char * user)
{
    sig_atomic_t volatile run_flag = 1;

    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            run_flag = 0;
    }
    char** file_names;
    file_names = (char**)malloc(FILES*sizeof(char*));
    int i = 0;
    for(i = 0; i < FILES; i++) {
        file_names[i] = (char *) malloc(BUFF_SIZE * sizeof(char));
        memset(file_names[i], 0, BUFF_SIZE);
    }
    FILE *fp;

    double time_start, time_end, time_wait, time_ref;
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
    time_wait = 0;
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    if (user == "") {
        for(i = 0; i < FILES; i++)
        {
            snprintf(file_names[i], BUFF_SIZE, "%s_%d.csv", names[i], time_ref);
        }
    }
    else
    {
        for(i = 0; i < FILES; i++)
        {
            snprintf(file_names[i], BUFF_SIZE, "%s_%s.csv", names[i], user);
        }
    }
    while(time_wait < IDLE)
    {
        printf("idling: %d\n", IDLE - (int) time_wait);;
        usleep(1000000);
        gettimeofday(&time_before, NULL);
        time_end = time_before.tv_sec;
        time_wait = time_end - time_ref;
    }
    mag = mag_init();
    set_mag_ODR(mag, M_ODR_125);
    set_mag_scale(mag, m_scale);
    m_res = calc_mag_res(m_scale);
    i = 0;
    while(i < FILES && run_flag) {
        time_wait = 0;
        gettimeofday(&time_before, NULL);
        time_ref = time_before.tv_sec;
        int safe_guard;
        safe_guard = 0;
        printf("next %s\tin:\t%d\n", names[files], (WAIT));
        while (time_wait < WAIT && safe_guard < 10000)
        {
            usleep(1000000);
            printf("%0.0f\t%s\n", (WAIT - time_wait), names[i]);
            safe_guard++;
            gettimeofday(&time_before, NULL);
            time_end = time_before.tv_sec;
            time_wait = time_end - time_ref;
            //   usleep(1000000);
        }
        printf("Attempting to write to file \'%s\'.\n", file_names[i]);
        fp = fopen(file_names[i], "w");
        if (fp == NULL) {
            fprintf(stderr,
                    "Failed to write to file \'%s\'.\n",
                    file_names[i]
            );
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
                " Acceleration z, Gyro x, Gyro y, Gyro z\n");
        time_wait = 0;
        gettimeofday(&time_before, NULL);
        time_ref = time_before.tv_sec;
        while(time_wait < RUN & run_flag)
        {
            gettimeofday(&time_before, NULL);
            ad = read_accel(accel, a_res);
            gd = read_gyro(gyro, g_res);

            time_wait = time_before.tv_sec - time_ref;

            time_start = time_before.tv_sec + time_before.tv_usec / NORM;
            fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

            usleep(100);
        }

        fclose(fp);
        i++;
    }
    return 0;
}







