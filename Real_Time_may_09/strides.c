#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"


void strides(char ** input_files, float ***data, float ***sigma, float ***mean,
             int *data_lengths, int **strides_t2, int *count_t2, int **strides_t1, int *count_t1) {
    // receive arrays with data pre allocated, return strides
    int rv;
    int i, j, k;
    char **output_files;
    char **strides_out;
    char **parameters;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *record;



    for (i = 0; i < FILES; i++) {
//        printf("Starting\n");
        fp = fopen(input_files[i], "r");
        if (fp == NULL) {
            fprintf(stderr,
                    "Failed to read from file \'%s\'.\n",
                    input_files[i]
            );
            exit(EXIT_FAILURE);
        }
        rewind(fp);
        read = getline(&line, &len, fp); //discard header of file
        j = 0;
        double time_ref;
        time_ref = 0;
        if ((read = getline(&line, &len, fp)) != -1) //get first line to zero time
        {
            record = strtok(line, ",");
            time_ref = atof(record);
            k = 0;
            data[i][k++][j] = 0;
//            printf("time: %f, ref:%f\n", data[i][0][j], time_ref);
//            printf("real value ref: %20.10f\n", strtod(record, NULL));
            record = strtok(NULL, ",");

            while (record != NULL) {
//                if (k == 1)
//                    record = strtok(NULL, ",");
                data[i][k++][j] = (float) atof(record);
//                printf("%lf\n", data[i][k-1][j]);
                record = strtok(NULL, ",");

            }
            j++;
        }

        while ((read = getline(&line, &len, fp)) != -1) {

            record = strtok(line, ",");
            k = 0;
            data[i][k++][j] = (float) (atof(record) - time_ref);
//            printf("time: %2.10lf, value: %2.10lf\t", data[i][0][j], (float) (atof(record) - time_ref));
//            printf("%lf\n", time_ref);
            record = strtok(NULL, ",");

            while (record != NULL) {
//                if (k == 1)
//                    record = strtok(NULL, ",");
                data[i][k++][j] = (float) atof(record);
                record = strtok(NULL, ",");

            }
            j++;

        }
        data_lengths[i] = j;
        fclose(fp);
    }
    for (i = PRINT; i < 0 * FILES; i++) {
        for (j = 0; j < data_lengths[i]; j += 3000) {
            printf("%20.10lf,%20.10lf,%10.10lf,%10.10lf,%10.10lf,%10.10lf,%10.10lf\n",
                   data[i][0][j],
                   data[i][1][j],
                   data[i][2][j],
                   data[i][3][j],
                   data[i][4][j],
                   data[i][5][j],
                   data[i][6][j]

            );
        }
    }
    // print data to determine propper values
    for (i = PRINT; i < FILES; i++) {
        printf("File #: %d\n", i);
        for (j = 0; j < data_lengths[i]; j += 100) {
            for (k = 0; k < 7; k++) {
                printf("%5.15lf\t", data[i][k][j]);
            }
            printf("\n");


        }
    }
//    printf("offset\n");
    float off_set;
    for (i = 0; i < 0; i++) {
        off_set = data[i][0][0];
//        printf("offset: %f\n", off_set);
        for (j = 0; j < data_lengths[i]; j++) {
//            printf("time: %f\n", data[i][0][j]);
            data[i][0][j] = data[i][0][j] - off_set;
        }
    }
    //// Testing floats
    printf("offset\n");
    for (i = 0; i < 0; i++) {
        off_set = data[i][0][0];
//        printf("offset: %f\n", off_set);
        for (j = 0; j < data_lengths[i]; j++) {
//            printf("time: %f\n", data[i][0][j]);
            data[i][0][j] = data[i][0][j] - off_set;
        }
    }


    /// testing floats end

    printf("mean and sigma\n");
    for (i = 0; i < FILES; i++) {
//        printf("i: %d\n", i);
        for (j = 0; j < DATA - 1; j++) {

            sample_mean(data[i][j + 1], mean[i][j], 3000, data_lengths[i]);
//            printf("mean done: %d\n", j);
            sample_sigma(data[i][j + 1], mean[i][j], sigma[i][j], 3000, data_lengths[i]);
//            printf("sigma done: %d\n", j);
            center_data(data[i][j + 1], mean[i][j], sigma[i][j], 3000, data_lengths[i]);
//            printf("center done: %d\n", j);
        }
//        printf("next_file\n");
        for (j = PRINT; j < data_lengths[i]; j += 2900) {
            printf("mean:   %15.10lf,%15.10lf,%10.10lf,%10.10lf,%10.10lf,%10.10lf\n",
                   mean[i][0][j],
                   mean[i][1][j],
                   mean[i][2][j],
                   mean[i][3][j],
                   mean[i][4][j],
                   mean[i][5][j]
            );
            printf("sigma:   %15.10lf,%15.10lf,%10.10lf,%10.10lf,%10.10lf,%10.10lf\n",
                   sigma[i][0][j],
                   sigma[i][1][j],
                   sigma[i][2][j],
                   sigma[i][3][j],
                   sigma[i][4][j],
                   sigma[i][5][j]
            );
        }
    }


    for (i = 0; i < FILES; i++) {
        count_t1[i] = temp_strides(data[i], strides_t1[i], sigma[i][1], sigma[i][5], 4, 500, 3, 0.2, data_lengths[i]);

        count_t2[i] = select_strides(data[i][0], strides_t1[i], strides_t2[i], 0.4, count_t1[i]);

        for (j = 0; j < STRIDES; j++)
            strides_t1[i][j] = 0;
        for (j = PRINT; j < count_t2[i]; j++) {
            printf("%d, ", strides_t2[i][j]);
        }

        count_t1[i] = peak_strides(data[i][0], data[i][6], strides_t2[i], strides_t1[i], sigma[i][5], 1.4, count_t2[i],
                                   data_lengths[i]);

        count_t2[i] = select_strides(data[i][0], strides_t1[i], strides_t2[i], 0.01, count_t1[i]);

        count_t1[i] = outlier(data[i][0], strides_t2[i], strides_t1[i], .65, count_t2[i]);

        count_t2[i] = outlier(data[i][0], strides_t1[i], strides_t2[i], 0.65, count_t1[i]);

        count_t2[i] = select_strides(data[i][0], strides_t1[i], strides_t2[i], 0.3, count_t1[i]);

        count_t1[i] = select_strides(data[i][0], strides_t2[i], strides_t1[i], 0.3, count_t2[i]);
        if(i < 0)
        {
            printf("\n");
            printf("%s\t%d\n", names[i], i);
            for(j = 0; j < count_t2[i]; j++)
            {
                printf("%f\t", data[i][0][strides_t2[i][j]]);
            }
            printf("\n");
        }

    }
    printf("last attempt\n");
    for (i = PRINT; i < FILES; i++) {
        printf("file: %d\t%d\n", i, count_t1[i]);
        printf("[\n");
        for (j = 0; j < count_t2[i]; j++) {
            printf("%d, ", strides_t2[i][j]);
        }
        printf("\n");
        printf("],\n");
    }
    for (i = 0; i < FILES; i++)
        move_back(strides_t2[i], 100, count_t2[i]);
//        move_back(strides_t1[i], 100, count_t1[i]);
}


