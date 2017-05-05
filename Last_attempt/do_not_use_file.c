#include <stdio.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <sys/time.h>

#define BUFF_SIZE 1024
#define FILE_TIME
#define RUN 1
#define NORM 1000000.0f
#define FILES 12
#define IDLE 40

const char * path = "/Users/amaaelantonini/Google Drive/Local/Spring_2017/EE 180DB/Real_Time_trial/";

sig_atomic_t volatile run_flag = 1;

void do_when_interrupted(int sig)
{
    if (sig == SIGINT)
        run_flag = 0;
}

void collect_training_data(){
    printf("collecting fake training data\n");
}

void test_data(const char *test_name)
{
    char* file_name;
    file_name = (char*)malloc(BUFF_SIZE * sizeof(char));
    memset(file_name, 0, BUFF_SIZE);
    int i = 0;
    int file_number;
    file_number = 0;
    FILE *fp;

    double time_start, time_wait, time_ref;
    struct timeval time_before, time_after;

    float a_res, g_res, m_res;


    int stop = 0;
    while(stop < 1 && run_flag) {
        time_wait = 0;
        stop++;
        sprintf(file_name, "%s_%d.csv", test_name, file_number);
        fp = fopen(file_name, "w");
        if (fp == NULL) {
            fprintf(stderr,
                    "Failed to write to file \'%s\'.\n",
                    file_name
            );
            exit(EXIT_FAILURE);
        }
        fprintf(fp, "time_stamp before, time_stamp after, Acceleration x, Acceleration y,"
                " Acceleration z, Gyro x, Gyro y, Gyro z\n");
        gettimeofday(&time_before, NULL);
        time_ref = time_before.tv_sec;
        while(time_wait < RUN && run_flag)
        {
            gettimeofday(&time_before, NULL);
//            printf("%d", file_number);
            time_start = time_before.tv_sec;
            time_wait += (int) (time_start - time_ref);
//            time_end = time_after.tv_sec + time_after.tv_usec / NORM;
            time_wait++;
            usleep(100);
        }

        fclose(fp);
        printf("%d\n", file_number);
        file_number++;
    }
}









