//
// Created by Amaael Antonini on 4/11/17.
//

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "constants.h"



//void Functions::Read_file() {
//
//    int i, j, rv;
//    int walk_speed;
//    int *output;
//    int N_SAMPLES;
//    float **all_data;
//    char *ifile_name;
//    FILE *fp;
//    char *line = NULL;
//    char buffer[1024];
//    char *record, *csv_line;
//    size_t len = 0;
//    ssize_t read;
//
//    ifile_name = (char *) malloc(sizeof(char) * BUFF_SIZE);
//    memset(ifile_name, 0, BUFF_SIZE);
//    snprintf(ifile_name,
//             BUFF_SIZE,
//             "/Users/amaaelantonini/Documents/Winter 2017/EE 180DA/File_Example/testing_file_girl_2"
//    );
//    fp = fopen(ifile_name, "r");
//    if (fp == NULL) {
//        fprintf(stderr,
//                "Failed to read from file \'%s\'.\n",
//                ifile_name
//        );
//        exit(EXIT_FAILURE);
//    }
//
//    read = getline(&line, &len, fp); //discard header of file
//    N_SAMPLES = 0;
//    while ((read = getline(&line, &len, fp)) != -1) {
//        N_SAMPLES++;
//    }
//    rewind(fp);
//
//    all_data = (float **) malloc(sizeof(float **) * N_SAMPLES);
//    for (i = 0; i < N_SAMPLES; i++) {
//        all_data[i] = (float *) malloc(sizeof(float) * num_input);
//        for (j = 0; j < num_input; j++)
//            printf("%f ", all_data[i][j]);
//        printf("next: %d\n", i);
//    }
//    read = getline(&line, &len, fp); //discard header of file
//    j = 0;
//    i = 0;
//    while ((read = getline(&line, &len, fp)) != -1) {
//        j = 0;
//        printf("%s\n", line);
//        record = strtok(line, ",");
//        printf("%s, %d\n", record, i);
//        while (record != NULL) {
//            printf("record : %s\n", record);
//            printf("converted : %f\n", atof(record));
//            all_data[i][j++] = (float) atof(record);
//            record = strtok(NULL, ",");
//        }
//        ++i;
//    }
//    fclose(fp);
//}

void fake_data(const char **names) {
    struct timeval current;
    int start, time_wait, over, files, offset;
    over = 0;
    time_wait = 0;
    files = 0;
    gettimeofday(&current, 0);
    start = (int)current.tv_sec;
//    printf("start: %d, elapsed: %d\n", start, time_wait);
    while(time_wait < IDLE)
    {

        printf("idling: %d\n", IDLE - (int) time_wait);
        usleep(1000000);
        gettimeofday(&current, 0);
        time_wait = (int) current.tv_sec;
        time_wait -= start;


    }


    gettimeofday(&current, 0);
    start = (int)current.tv_sec;


    while(files < FILES)
    {

        time_wait = 0;
        offset = files * WAIT + files * RUN;

        while(time_wait < WAIT)
        {
            printf("next %s\tin:\t%d\n", names[files], (WAIT - time_wait));
            usleep(1000000);
            gettimeofday(&current, 0);
            time_wait = (int) current.tv_sec - start - offset;


        }

        time_wait = 0;
        offset = files * WAIT + files * RUN + WAIT;

        while(time_wait < RUN)
        {
            printf("collecting\t%s\t%d\n",names[files],  (WAIT - time_wait));
            usleep(1000000);
            gettimeofday(&current, 0);
            time_wait = (int) current.tv_sec - start - offset;


        }
        files++;

    }
}




