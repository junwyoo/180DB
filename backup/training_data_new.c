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
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>

//#include "LSM9DS0.h"
//#include <mraa/pwm.h>
//#include <mraa/aio.h>
//#include <mraa/i2c.h>

void collect_training_data(char * folder, char *user)
{

    sig_atomic_t volatile interrupt_flag = 1;
    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            interrupt_flag = 0;
    }

    // determine which files are to be created and a directory for the files
    // assume training data is needed
    // create folder?? assume folder has been created?? chech if folder exist and then create it

    int hold;       // how long to wait before start recording
    int record;     // how long will the recording be
    int done_training;       // training is complete
    int current;    // index of the current file in use
    int *indexes;   // list of files indexes
    int repeat_all_session;       // restart training if requested
    int i;
    indexes = (int *) malloc(sizeof(int) * FILES);

    char** file_names;
    char *dir;
    dir = (char *) malloc(BUFF_SIZE * sizeof(char));
    memset(dir, 0, BUFF_SIZE);
    snprintf(dir, BUFF_SIZE, "%s_%s", train_file, user);
    file_names = (char**)malloc(FILES*sizeof(char*));

    double time_start, time_end, time_wait, time_ref;
    struct timeval time_before, time_after;
    float a_res, g_res, m_res;
    FILE *fp;


    for(i = 0; i < FILES; i++) {
        file_names[i] = (char *) malloc(BUFF_SIZE * sizeof(char));
        memset(file_names[i], 0, BUFF_SIZE);
        snprintf(file_names[i], BUFF_SIZE, "%s_%s/%s_%s.csv", train_file, user, names[i], user);//
        indexes[i] = i;
    }




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


    repeat_all_session = 1;
    while(repeat_all_session)
    {
        repeat_all_session = 0;
        snprintf(file_names[i], BUFF_SIZE, "%s_%s/%s_%s.csv", train_file, user, names[i], user);

        mkdir(dir, 0777);
        for(i = 0; i < FILES; i++)
        {
            if(access(file_names[i], F_OK) == -1 )
            {
                indexes[i] = -1;
            }
            else
            {
                printf()
            }
        }
        // determine if folder exist and create it otherwise
        // if folder created break
        // if folder existed determine which files are missing
        // set indexes[existing] to -1;
        // request if any overwrites
        // request which to overwrite if yes
        // make a list of the indexes to overwrite
        // set default hold and record???

        i = 0;
        done_training = 0;
        hold = 10; // set hold to 10????
        current = -1;

        while(interrupt_flag && done_training != 1) {

            // dialog to determine which file to train, how long to hold, how long to record
            for(i = 0; i < FILES; i++)
            {
                if(indexes[i] == i)
                {
                    printf("%d\t%s\n", i+1, names[i]);
                    // print the names of the files missing
                    // assume that indexes of not missing files are set to -1
                }
            }
            // prompt to enter the index of the file to record
            // prompt to enter hold and record time or default to standard
            // set current to the index to train: e.g i-1;

            if(current == 1)
            {
                printf("invalid entry, choose again\t");
                continue;
            }


            time_wait = 0;
            gettimeofday(&time_before, NULL);
            time_ref = time_before.tv_sec;
            int safe_guard;
            safe_guard = 0;
            printf("next %s\tin:\t%d\n", names[i], (int) (WAIT-time_wait));
            while (time_wait <= hold && safe_guard < 10000)
            {

                printf("%d\t%s\n", (int) (WAIT - time_wait), names[i]);
                usleep(1000000);
                safe_guard++;
                gettimeofday(&time_before, NULL);
                time_end = time_before.tv_sec;
                time_wait = time_end - time_ref;
            }
            usleep(1000000);
            printf("Attempting to write to file \'%s\'.\n", file_names[current]);
            fp = fopen(file_names[current], "w");
            if (fp == NULL) {
                fprintf(stderr,
                        "Failed to write to file \'%s\'.\n",
                        file_names[current]
                );
                exit(EXIT_FAILURE);
            }
            fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
                    " Acceleration z, Gyro x, Gyro y, Gyro z\n");
            time_wait = 0;
            gettimeofday(&time_before, NULL);
            time_ref = time_before.tv_sec;
            while(time_wait < RUN & interrupt_flag)
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
            // dialog to determine if recording is successful
            // determine if section is done_training
        }
        // dialog to restart data collecting
        // if need to restart, repeat_all_session = 1
    }
}


void gather_data(char *file_name, int hold_time, int record_time)
{
    sig_atomic_t volatile interrupt_flag = 1;
    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            interrupt_flag = 0;
    }
    int i = 0;
    FILE *fp;

    double time_start, time_wait, time_ref;
    struct timeval time_before;
    float a_res, g_res, m_res;

//    mraa_i2c_context accel, gyro, mag;
//    accel_scale_t a_scale;
//    gyro_scale_t g_scale;
//    mag_scale_t m_scale;
//    data_t ad, gd, md;
//
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
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    while(time_wait < TEST_SIZE && interrupt_flag) {

//        ad = read_accel(accel, a_res);
//        gd = read_gyro(gyro, g_res);

        gettimeofday(&time_before, NULL);
        time_start = time_before.tv_sec + time_before.tv_usec / NORM;
        time_wait += (int) (time_start - time_ref);
//        fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
//                time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

        usleep(100);
    }

    fclose(fp);
}


void collect_training_data(char * folder, char *user)
{

    sig_atomic_t volatile interrupt_flag = 1;
    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            interrupt_flag = 0;
    }

    // determine which files are to be created and a directory for the files
    // assume training data is needed
    // create folder?? assume folder has been created?? chech if folder exist and then create it

    int hold;       // how long to wait before start recording
    int record;     // how long will the recording be
    int done_training;       // training is complete
    int current;    // index of the current file in use
    int *indexes;   // list of files indexes
    int repeat_all_session;       // restart training if requested
    int i;
    indexes = (int *) malloc(sizeof(int) * FILES);

    char** file_names;
    char *dir;
    dir = (char *) malloc(BUFF_SIZE * sizeof(char));
    memset(dir, 0, BUFF_SIZE);
    snprintf(dir, BUFF_SIZE, "%s_%s", train_file, user);
    file_names = (char**)malloc(FILES*sizeof(char*));

    double time_start, time_end, time_wait, time_ref;
    struct timeval time_before, time_after;
    float a_res, g_res, m_res;
    FILE *fp;


    for(i = 0; i < FILES; i++) {
        file_names[i] = (char *) malloc(BUFF_SIZE * sizeof(char));
        memset(file_names[i], 0, BUFF_SIZE);
        snprintf(file_names[i], BUFF_SIZE, "%s_%s/%s_%s.csv", train_file, user, names[i], user);
        indexes[i] = i;
    }




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


    repeat_all_session = 1;
    while(repeat_all_session)
    {
        repeat_all_session = 0;
        snprintf(file_names[i], BUFF_SIZE, "%s_%s/%s_%s.csv", train_file, user, names[i], user);

        mkdir(dir, 0777);
        for(i = 0; i < FILES; i++)
        {
            if(access(file_names[i], F_OK) == -1 )
            {
                indexes[i] = -1;
            }
            else
            {
                printf()
            }
        }
        // determine if folder exist and create it otherwise
        // if folder created break
        // if folder existed determine which files are missing
        // set indexes[existing] to -1;
        // request if any overwrites
        // request which to overwrite if yes
        // make a list of the indexes to overwrite
        // set default hold and record???

        i = 0;
        done_training = 0;
        hold = 10; // set hold to 10????
        current = -1;

        while(interrupt_flag && done_training != 1) {

            // dialog to determine which file to train, how long to hold, how long to record
            for(i = 0; i < FILES; i++)
            {
                if(indexes[i] == i)
                {
                    printf("%d\t%s\n", i+1, names[i]);
                    // print the names of the files missing
                    // assume that indexes of not missing files are set to -1
                }
            }
            // prompt to enter the index of the file to record
            // prompt to enter hold and record time or default to standard
            // set current to the index to train: e.g i-1;

            if(current == 1)
            {
                printf("invalid entry, choose again\t");
                continue;
            }


            time_wait = 0;
            gettimeofday(&time_before, NULL);
            time_ref = time_before.tv_sec;
            int safe_guard;
            safe_guard = 0;
            printf("next %s\tin:\t%d\n", names[i], (int) (WAIT-time_wait));
            while (time_wait <= hold && safe_guard < 10000)
            {

                printf("%d\t%s\n", (int) (WAIT - time_wait), names[i]);
                usleep(1000000);
                safe_guard++;
                gettimeofday(&time_before, NULL);
                time_end = time_before.tv_sec;
                time_wait = time_end - time_ref;
            }
            usleep(1000000);
            printf("Attempting to write to file \'%s\'.\n", file_names[current]);
            fp = fopen(file_names[current], "w");
            if (fp == NULL) {
                fprintf(stderr,
                        "Failed to write to file \'%s\'.\n",
                        file_names[current]
                );
                exit(EXIT_FAILURE);
            }
            fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
                    " Acceleration z, Gyro x, Gyro y, Gyro z\n");
            time_wait = 0;
            gettimeofday(&time_before, NULL);
            time_ref = time_before.tv_sec;
            while(time_wait < RUN & interrupt_flag)
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
            // dialog to determine if recording is successful
            // determine if section is done_training
        }
        // dialog to restart data collecting
        // if need to restart, repeat_all_session = 1
    }
}







