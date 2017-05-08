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

void gather_data(char *file_name, int hold_time, int record_time)
{

    int i;
    int safe_guard = 0;
    double time_start, time_end, time_wait, time_ref;
    struct timeval time_before, time_after;
    float a_res, g_res, m_res;
    FILE *fp;

    time_wait = 0;
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
 
    
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
        time_wait = time_before.tv_sec - time_ref;
        time_start = time_before.tv_sec + time_before.tv_usec / NORM;
        usleep(100);
    }
    fclose(fp);
}
