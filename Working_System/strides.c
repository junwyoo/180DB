#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"


void strides(char * input_file, float **data, float **sigma, float **mean,
             int *data_length, int *strides_t2, int *strides_t1, int *rows) {
    // receive arrays with data pre allocated, return strides
    int i, j, k;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    char *record;
    int data_size;
    int count_t2;
    int count_t1;
    int last_period;



    fp = fopen(input_file, "r");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to read from file \'%s\'.\n",
                input_file
        );
        exit(EXIT_FAILURE);
    }
    getline(&line, &len, fp); //discard header of file
    j = 0;
    double time_ref;
    time_ref = 0;
    if ((getline(&line, &len, fp)) != -1) //get first line to zero time
    {
        record = strtok(line, ",");
        time_ref = atof(record);
        k = 0;
        data[k++][j] = 0;
//            printf("time: %f, ref:%f\n", data[i][0][j], time_ref);
//            printf("real value ref: %20.10f\n", strtod(record, NULL));
        record = strtok(NULL, ",");

        while (record != NULL) {
//                if (k == 1)
//                    record = strtok(NULL, ",");
            data[k++][j] = (float) atof(record);
//                printf("%lf\n", data[i][k-1][j]);
            record = strtok(NULL, ",");

        }
        j++;
    }

    while ((getline(&line, &len, fp)) != -1) {

        record = strtok(line, ",");
        k = 0;
        data[k++][j] = (float) (atof(record) - time_ref);
//            printf("time: %2.10lf, value: %2.10lf\t", data[i][0][j], (float) (atof(record) - time_ref));
//            printf("%lf\n", time_ref);
        record = strtok(NULL, ",");

        while (record != NULL) {
//                if (k == 1)
//                    record = strtok(NULL, ",");
            data[k++][j] = (float) atof(record);
            record = strtok(NULL, ",");

        }
        j++;

    }
    *data_length = j;
    data_size = j;
    fclose(fp);







    count_t2 = 0;
    count_t1 = 0;
//    printf("mean and sigma\n");
    for (i = 0; i < FILES; i++) {
//        printf("i: %d\n", i);
        for (j = 0; j < DATA - 1; j++) {

            sample_mean(data[j + 1], mean[j], 3000, data_size);
//            printf("mean done: %d\n", j);
            sample_sigma(data[j + 1], mean[j], sigma[j], 3000, data_size);
//            printf("sigma done: %d\n", j);
            center_data(data[j + 1], mean[j], sigma[j], 3000, data_size);
//            printf("center done: %d\n", j);
        }
    }



    for (i = 0; i < FILES; i++) {
        count_t1 = temp_strides(data, strides_t1, sigma[1], sigma[5], 4, 500, 3, 0.2, data_size);

        count_t2 = select_strides(data[0], strides_t1, strides_t2, 0.4, count_t1);

        for (j = 0; j < STRIDES; j++)
            strides_t1[j] = 0;
        for (j = PRINT; j < count_t2; j++) {
            printf("%d, ", strides_t2[j]);
        }

        count_t1 = peak_strides(data[0], data[6], strides_t2, strides_t1, sigma[5], 1.4, count_t2,
                                   data_size);

        count_t2 = select_strides(data[0], strides_t1, strides_t2, 0.01, count_t1);

        count_t1 = outlier(data[0], strides_t2, strides_t1, .65, count_t2);

        count_t2 = outlier(data[0], strides_t1, strides_t2, 0.65, count_t1);

        count_t2 = select_strides(data[0], strides_t1, strides_t2, 0.3, count_t1);

        count_t1 = select_strides(data[0], strides_t2, strides_t1, 0.3, count_t2);

    }
//    printf("last attempt\n");
    for (i = PRINT; i < FILES; i++) {
        printf("file: %d\t%d\n", i, count_t1);
        printf("[\n");
        for (j = 0; j < count_t2; j++) {
            printf("%d, ", strides_t2[j]);
        }
        printf("\n");
        printf("],\n");
    }

    move_back(strides_t2, 100, count_t2);
    if(count_t2 > 1)
    {
        last_period = strides_t2[count_t2-1] - strides_t2[count_t2-2];


        if(data_size > strides_t2[count_t2] + last_period)
        {
            strides_t2[count_t2] = strides_t2[count_t2-1] + last_period;
            count_t2++;

        }
    }



//        move_back(strides_t1[i], 100, count_t1[i]);
    *rows = count_t2;
}


