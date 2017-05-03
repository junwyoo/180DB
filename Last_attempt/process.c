#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"

#define BUFF_SIZE 1024
#define STRIDES 200
#define FILES 12
#define DATA 7
#define NULL 0
#define SLOTS 4
#define FEATURES 27
#define PRINT 100000
const char * names[] = {"AA2", "DA2"};
const char * file_names[] = {
        "walk_speed_1_50sec_32m",
        "walk_speed_2_35sec_32m",
        "walk_speed_3_25sec_32m",
        "walk_speed_4_15sec_32m",
        "slow_run",
        "medium_run",
        "fast_run",
        "stairs_up",
        "stairs_down",
        "low_jump",
        "medium_jump",
        "high_jump"};

const char * main_path = "";
const char * class_files[] = {"Classifiers/AA_file/", "Classifiers/DA_file/"};

//const char * paths[] = {"Data_sets_3/AA2_data/", "Data_sets_3/DA2_data/"};
const char * paths[] = {"Data_sets/AA2_data/", "Data_sets/DA2_data/"};
const char * paths_out[] = {"Data_sets_out/AA2_data/", "Data_sets_out/DA2_data/"};
const char * stride_path = "Data_strides_out/";

void train_parameters(float ****classifiers, int *** counts, int **dimensions, int * n){
    int rv;
    int db;
    int i, j, k;
    char **input_files;
    char **output_files;
    char **strides_out;
    char **parameters;
    float ***data;
    float ***sigma;
    float ***mean;
    int ** strides_t1, *count_t1;
    int ** strides_t2, *count_t2;
    int N_SAMPLES;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    ssize_t read;
    char *record;
    int *data_lengths;
    db = 0;

    count_t1 = (int*) malloc(sizeof(int) * FILES);
    count_t2 = (int*) malloc(sizeof(int) * FILES);
    strides_t1 = (int**) malloc(sizeof(int*) * FILES);
    strides_t2 = (int**) malloc(sizeof(int*) * FILES);
    for(i = 0; i < FILES; i++)
    {
        strides_t1[i] = (int*) malloc(sizeof(int) * STRIDES);
        strides_t2[i] = (int*) malloc(sizeof(int) * STRIDES);
    }
    data_lengths = (int*) malloc(sizeof(int) * FILES);
    data = (float ***) malloc(FILES * sizeof(float **));
    mean = (float ***) malloc(FILES * sizeof(float **));
    sigma = (float ***) malloc(FILES * sizeof(float **));
    input_files = (char **) malloc(FILES * sizeof(char *));
    for (i = 0; i < FILES; i++) {
        input_files[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(input_files[i], 0, BUFF_SIZE);
        snprintf(input_files[i],
                 BUFF_SIZE,
                 "%s%s%s_%s.csv",main_path, paths[1], file_names[i], names[1]);
        data[i] = (float **) malloc(sizeof(float*) * DATA);
        mean[i] = (float **) malloc(sizeof(float*) * DATA);
        sigma[i] = (float **) malloc(sizeof(float*) * DATA);
    }


    for (i = 0; i < FILES; i++)
    {
//        printf("Starting\n");
        fp = fopen(input_files[i], "r");
        if (fp == NULL) {
            fprintf(stderr,
                    "Failed to read from file \'%s\'.\n",
                    input_files[i]
            );
            exit(EXIT_FAILURE);
        }
//        printf("File opened\n");
        /* count the number of lines in the file */
        read = getline(&line, &len, fp); //discard header of file
        N_SAMPLES = 0;
        while ((read = getline(&line, &len, fp)) != -1) {
//            if(N_SAMPLES<100)
//                printf("%s\n", line);
            N_SAMPLES++;
        }
//        *lengths[i] = N_SAMPLES;
//        printf("samples counted: %d\n", N_SAMPLES);
        data_lengths[i] = N_SAMPLES;
        for(j = 0; j < DATA; j++)
        {
            data[i][j] = (float *) malloc(sizeof(float) * N_SAMPLES);
            mean[i][j] = (float *) malloc(sizeof(float) * N_SAMPLES);
            sigma[i][j] = (float *) malloc(sizeof(float) * N_SAMPLES);
        }
//        printf("data initialized\n");
        rewind(fp);
        read = getline(&line, &len, fp); //discard header of file
        j = 0;
        //        printf("file rewined\n");

        double time_ref;
        time_ref = 0;
        if((read = getline(&line, &len, fp)) != -1) //get first line to zero time
        {
            record = strtok(line, ",");
            time_ref = atof(record);
            k = 0;
            data[i][k++][j] = 0;
//            printf("time: %f, ref:%f\n", data[i][0][j], time_ref);
//            printf("real value ref: %20.10f\n", strtod(record, NULL));
            record = strtok(NULL, ",");

            while (record != NULL) {
                if(k == 1)
                    record = strtok(NULL, ",");
                data[i][k++][j] = (float) atof(record);
//                printf("%lf\n", data[i][k-1][j]);
                record = strtok(NULL, ",");

            }
            j++;
        }
//        printf("loop 1 done\n");


        N_SAMPLES = 0;
        while ((read = getline(&line, &len, fp)) != -1) {

            record = strtok(line, ",");
            k = 0;
            data[i][k++][j] = (float) (atof(record) - time_ref);
//            printf("time: %2.10lf, value: %2.10lf\t", data[i][0][j], (float) (atof(record) - time_ref));
//            printf("%lf\n", time_ref);
            record = strtok(NULL, ",");

            while (record != NULL) {
                if(k == 1)
                    record = strtok(NULL, ",");
                data[i][k++][j] = (float) atof(record);
                record = strtok(NULL, ",");

            }
            j++;
        }
        fclose(fp);
//        *lengths[i] = N_SAMPLES;
//        printf("samples counted: %d\n", N_SAMPLES);
        //rewind(fp);
//        printf("%d\n", i);

    }
    for(i = PRINT; i < 0*FILES; i++) {
        for (j = 0; j < data_lengths[i]; j+=3000) {
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
    for(i = PRINT; i < FILES; i++)
    {
        printf("File #: %d\n", i);
        for(j = 0; j < data_lengths[i]; j+= 100)
        {
            for(k = 0; k < 7; k++)
            {
                printf("%5.15lf\t", data[i][k][j]);
            }
            printf("\n");


        }
    }
//    printf("offset\n");
    float off_set;
    for(i = 0; i < 0; i++)
    {
        off_set = data[i][0][0];
//        printf("offset: %f\n", off_set);
        for(j = 0; j < data_lengths[i]; j++)
        {
//            printf("time: %f\n", data[i][0][j]);
            data[i][0][j] = data[i][0][j]- off_set;
        }
    }
    //// Testing floats
    printf("offset\n");
    for(i = 0; i < 0; i++)
    {
        off_set = data[i][0][0];
//        printf("offset: %f\n", off_set);
        for(j = 0; j < data_lengths[i]; j++)
        {
//            printf("time: %f\n", data[i][0][j]);
            data[i][0][j] = data[i][0][j]- off_set;
        }
    }


    /// testing floats end

    printf("mean and sigma\n");
    for(i = 0; i < FILES; i++)
    {
//        printf("i: %d\n", i);
        for(j = 0; j < DATA-1; j++)
        {

            sample_mean(data[i][j+1], mean[i][j], 3000, data_lengths[i]);
//            printf("mean done: %d\n", j);
            sample_sigma(data[i][j+1], mean[i][j], sigma[i][j], 3000, data_lengths[i]);
//            printf("sigma done: %d\n", j);
            center_data(data[i][j+1], mean[i][j], sigma[i][j], 3000, data_lengths[i]);
//            printf("center done: %d\n", j);
        }
//        printf("next_file\n");
        for (j = PRINT; j < data_lengths[i]; j+=2900)
        {
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
//        printf("%d\n", count_t1[i]);
//        printf("[\n");
//        for(j = 0; j < count_t1[i]; j++)
//        {
//            printf("%d, ", strides_t1[i][j]);
//
//        }
//        printf("],\n");
//        printf("\n");
        count_t2[i] = select_strides(data[i][0], strides_t1[i], strides_t2[i], 0.4, count_t1[i]);
//        printf("%d\n", count_t2[i]);
//        printf("[\n");
        for(j = 0; j < STRIDES; j++)
            strides_t1[i][j] = 0;
        for(j = PRINT; j < count_t2[i]; j++)
        {
            printf("%d, ", strides_t2[i][j]);
        }
//        printf("],\n");
//        printf("\n");
        count_t1[i] = peak_strides(data[i][0], data[i][6], strides_t2[i], strides_t1[i], sigma[i][5], 1.4, count_t2[i], data_lengths[i]);
//        printf("strides temp: %d, selected: %d\n", count_t1[i], count_t2[i]);
//        count_t2[i] = select_strides(data[i][0], strides_t2[i], strides_t1[i], 0.4, count_t1[i]);
//        count_t1[i] = count_t2[i];
        count_t2[i] = select_strides(data[i][0], strides_t1[i], strides_t2[i], 0.01, count_t1[i]);
//        printf("before outlier\n");
        count_t1[i] = outlier(data[i][0], strides_t2[i], strides_t1[i], 0.65, count_t2[i]);
//        printf("second\n");
        count_t2[i] = outlier(data[i][0], strides_t1[i], strides_t2[i], 0.65, count_t1[i]);
//        for(j = 0; j < count_t1[i]; j++)
//        {
//            strides_t2[i][j] = strides_t1[i][j];
//        }
//        count_t2[i] = count_t1[i];
    }
    printf("last attempt\n");
    for (i = PRINT; i < FILES; i++)
    {
        printf("%d\n", count_t1[i]);
        printf("[\n");
        for(j = 0; j < count_t2[i]; j++)
        {
            printf("%d, ", strides_t2[i][j]);
        }
        printf("\n");
        printf("],\n");
    }
    for(i = 0; i < FILES; i++)
        move_back(strides_t2[i], 100, count_t2[i]);
//        move_back(strides_t1[i], 100, count_t1[i]);


    float ** sigma_high_xa, ** sigma_low_xa;
    float ** sigma_high_ya, ** sigma_low_ya;
    float ** sigma_high_zg, ** sigma_low_zg;

    float *** troughs_xa, *** peaks_xa;
    float *** troughs_ya, *** peaks_ya;
    float *** troughs_zg, *** peaks_zg;


    float *** mean_low_xa, *** mean_high_xa;
    float *** mean_low_ya, *** mean_high_ya;
    float *** mean_low_zg, *** mean_high_zg;

    int *** stride_all;
    float *** stride_float;
    int *num_strides;

    sigma_high_xa = (float **) malloc(FILES * sizeof(float *));
    sigma_low_xa = (float **) malloc(FILES * sizeof(float *));
    sigma_high_ya = (float **) malloc(FILES * sizeof(float *));
    sigma_low_ya = (float **) malloc(FILES * sizeof(float *));
    sigma_high_zg = (float **) malloc(FILES * sizeof(float *));
    sigma_low_zg = (float **) malloc(FILES * sizeof(float *));

    mean_high_xa = (float ***) malloc(FILES * sizeof(float **));
    mean_low_xa = (float ***) malloc(FILES * sizeof(float **));
    mean_high_ya = (float ***) malloc(FILES * sizeof(float **));
    mean_low_ya = (float ***) malloc(FILES * sizeof(float **));
    mean_high_zg = (float ***) malloc(FILES * sizeof(float **));
    mean_low_zg = (float ***) malloc(FILES * sizeof(float **));

    num_strides = (int *) malloc(FILES * sizeof(int ));
    stride_all = (int ***) malloc(FILES * sizeof(int **));
    stride_float = (float ***) malloc(FILES * sizeof(float **));

    peaks_xa = (float ***) malloc(FILES * sizeof(float **));
    peaks_ya = (float ***) malloc(FILES * sizeof(float **));
    peaks_zg = (float ***) malloc(FILES * sizeof(float **));

    troughs_xa = (float ***) malloc(FILES * sizeof(float **));
    troughs_ya = (float ***) malloc(FILES * sizeof(float **));
    troughs_zg = (float ***) malloc(FILES * sizeof(float **));


    for(i = 0; i < FILES; i++) {
        num_strides[i] = 0;
    }


    for(i = 0; i < FILES; i++)
    {
        sigma_high_xa[i] = (float *) malloc(sizeof(float) * STRIDES);
        sigma_low_xa[i] = (float *) malloc(sizeof(float) * STRIDES);
        sigma_high_ya[i] = (float *) malloc(sizeof(float) * STRIDES);
        sigma_low_ya[i] = (float *) malloc(sizeof(float) * STRIDES);
        sigma_high_zg[i] = (float *) malloc(sizeof(float) * STRIDES);
        sigma_low_zg[i] = (float *) malloc(sizeof(float) * STRIDES);

        mean_high_xa[i] = (float **) malloc(STRIDES * sizeof(float *));
        mean_low_xa[i] = (float **) malloc(STRIDES * sizeof(float *));
        mean_high_ya[i] = (float **) malloc(STRIDES * sizeof(float *));
        mean_low_ya[i] = (float **) malloc(STRIDES * sizeof(float *));
        mean_high_zg[i] = (float **) malloc(STRIDES * sizeof(float *));
        mean_low_zg[i] = (float **) malloc(STRIDES * sizeof(float *));

        peaks_xa[i] = (float **) malloc(STRIDES * sizeof(float *));
        troughs_xa[i] = (float **) malloc(STRIDES * sizeof(float *));
        peaks_ya[i] = (float **) malloc(STRIDES * sizeof(float *));
        troughs_ya[i] = (float **) malloc(STRIDES * sizeof(float *));
        peaks_zg[i] = (float **) malloc(STRIDES * sizeof(float *));
        troughs_zg[i] = (float **) malloc(STRIDES * sizeof(float *));

        stride_all[i] = (int **) malloc(STRIDES * sizeof(int *));
        stride_float[i] = (float **) malloc(STRIDES * sizeof(float *));

    }

    for(i = 0; i < FILES; i++)
    {
        for(j = 0; j < STRIDES; j++)
        {
            mean_high_xa[i][j] = (float *) malloc(SLOTS * sizeof(float ));
            mean_low_xa[i][j] = (float *) malloc(SLOTS * sizeof(float ));
            mean_high_ya[i][j] = (float *) malloc(SLOTS * sizeof(float ));
            mean_low_ya[i][j] = (float *) malloc(SLOTS * sizeof(float ));
            mean_high_zg[i][j] = (float *) malloc(SLOTS * sizeof(float ));
            mean_low_zg[i][j] = (float *) malloc(SLOTS * sizeof(float ));

            peaks_xa[i][j] = (float *) malloc(4 * sizeof(float ));
            troughs_xa[i][j] = (float *) malloc(4 * sizeof(float ));
            peaks_ya[i][j] = (float *) malloc(4 * sizeof(float ));
            troughs_ya[i][j] = (float *) malloc(4 * sizeof(float ));
            peaks_zg[i][j] = (float *) malloc(4 * sizeof(float ));
            troughs_zg[i][j] = (float *) malloc(4 * sizeof(float ));

            stride_all[i][j] = (int *) malloc(4 * sizeof(int ));
            stride_float[i][j] = (float *) malloc(STRIDES * sizeof(float));

        }
    }
    int cnt;
    cnt = 0;
    for(i = 0; i < FILES; i++)
    {
        cnt = 0;
        for(j = 1; j < count_t2[i]; j++)
        {
            if(data[i][0][strides_t2[i][j]] - data[i][0][strides_t2[i][j-1]] < 3)
            {
                stride_all[i][cnt][0] = strides_t2[i][j-1];
                stride_all[i][cnt][1] = strides_t2[i][j];
                stride_all[i][cnt][2] = strides_t2[i][j]-strides_t2[i][j-1];
                num_strides[i]++;
                cnt++;
            }
        }
        for(j = 0; j < num_strides[i]-1; j++)
        {
            if(stride_all[i][j][1] == stride_all[i][j+1][0])
            {
                stride_all[i][j][3] = 1;
            }
            else
            {
                stride_all[i][j][3] = -1;
            }
        }
        for(j = 1000; j < num_strides[i]; j++)
        {
            printf("stride: %d, end: %d, period: %d, next: %d\n", stride_all[i][j][0],
                   stride_all[i][j][1], stride_all[i][j][2], stride_all[i][j][3]);
        }

    }

    for(i = 0; i < FILES; i++)
    {
        sigma_stride(data[i][1], stride_all[i], sigma[i][0], sigma_low_xa[i], sigma_high_xa[i], num_strides[i]);
        sigma_stride(data[i][2], stride_all[i], sigma[i][1], sigma_low_ya[i], sigma_high_ya[i], num_strides[i]);
        sigma_stride(data[i][6], stride_all[i], sigma[i][5], sigma_low_zg[i], sigma_high_zg[i], num_strides[i]);

//        printf("before peaks\n");
        peaks(data[i][1], stride_all[i], peaks_xa[i], troughs_xa[i], num_strides[i]);
        peaks(data[i][2], stride_all[i], peaks_ya[i], troughs_ya[i], num_strides[i]);
        peaks(data[i][6], stride_all[i], peaks_zg[i], troughs_zg[i], num_strides[i]);
        for(j = PRINT; j < num_strides[i]-1; j++)
        {
            printf("%f\t%f\t%f\t%f\n", peaks_xa[i][j][0], peaks_xa[i][j][1], peaks_xa[i][j][2], peaks_xa[i][j][3]);
            printf("%f\t%f\t%f\t%f\n\n", troughs_xa[i][j][0], troughs_xa[i][j][1], troughs_xa[i][j][2], troughs_xa[i][j][3]);
        }
    }

    printf("done strides\n");
    strides_out = (char **) malloc(FILES * sizeof(char *));
    output_files = (char **) malloc(FILES * sizeof(char *));
    parameters = (char **) malloc(FILES * sizeof(char *));
    for (i = 0; i < FILES; i++) {
        output_files[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(output_files[i], 0, BUFF_SIZE);
        snprintf(output_files[i],
                 BUFF_SIZE,
                 "%s%s%s_%s_2.csv",main_path, paths_out[1], file_names[i], names[1]);
        strides_out[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(strides_out[i], 0, BUFF_SIZE);
        snprintf(strides_out[i],
                 BUFF_SIZE,
                 "%s%s%s_%s_strides.csv",main_path, stride_path, file_names[i], names[1]);
        parameters[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(parameters[i], 0, BUFF_SIZE);
        snprintf(parameters[i],
                 BUFF_SIZE,
                 "%s%s%s_%s.csv",main_path, class_files[1], file_names[i], names[1]);
//        printf("mem_set_out\n");

    }


    float *** out;
    int **d1, *d2;

    int l;
    out = (float ***) malloc(FILES * sizeof(float **));
    d1 = (int **) malloc(FILES * sizeof(int*));
    d2 = (int *) malloc(FILES * sizeof(int));
    for(i = 0; i < FILES; i++) {
        d2[i] = num_strides[i]-1;
        d1[i] = (int *) malloc((num_strides[i]-1) * sizeof(int));
        out[i] = (float **) malloc(sizeof(float*) * (num_strides[i]-1));
//        printf("%s\n", file_names[i]);
        for (j = 0; j < num_strides[i]-1; j++) {
            d1[i][j] = FEATURES;
            out[i][j] = (float *) malloc(sizeof(float) * FEATURES);
            l = 0;
            out[i][j][l++] = stride_all[i][j][2];
//            printf("%f\t", out[i][j][l-1]);
            for(k = 0; k < 4; k++){
                out[i][j][l++] = peaks_xa[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }

            for(k = 0; k < 4; k++){
                out[i][j][l++] = troughs_xa[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }
            for(k = 0; k < 4; k++){
                out[i][j][l++] = peaks_ya[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }

            for(k = 0; k < 4; k++){
                out[i][j][l++] = troughs_ya[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }
            for(k = 0; k < 4; k++){
                out[i][j][l++] = peaks_zg[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }
            for(k = 0; k < 4; k++){
                out[i][j][l++] = troughs_zg[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }
            out[i][j][l++] = sigma_high_xa[i][j];
//            printf("%f\t", out[i][j][l-1]);
            out[i][j][l++] = sigma_low_xa[i][j];
//            printf("%f\t", out[i][j][l-1]);
//            printf("\n");
        }

    }
    for(i = PRINT; i < FILES; i++)
    {
        printf("%s\n", file_names[i]);
        for (j = 0; j < num_strides[i]-1; j++){
            for(k = 0; k < FEATURES; k++){
                printf("%f\t", out[i][j][k]);
            }
            printf("\n");
        }
    }
    *classifiers = out;
    *counts = d1;
    *dimensions = d2;
    *n = FILES;


    // Free memory
    printf("free memory\n");
    for(i = 0; i < FILES; i++) {
        for(k = 0; k < DATA; k++) {
            free(data[i][k]);
            free(mean[i][k]);
            free(sigma[i][k]);
        }
        free(data[i]);
        free(strides_t1[i]);
        free(strides_t2[i]);
        free(mean[i]);
        free(sigma[i]);
        free(input_files[i]);
    }

    free(data);
    free(strides_t1);
    free(strides_t2);
    free(count_t1);
    free(count_t2);
    free(mean);
    free(sigma);
    free(data_lengths);
    free(input_files);


    for(i = 0; i < FILES; i++)
    {
        for(j = 0; j < STRIDES; j++)
        {
            free(mean_high_xa[i][j]);
            free(mean_high_ya[i][j]);
            free(mean_high_zg[i][j]);
            free(mean_low_xa[i][j]);
            free(mean_low_ya[i][j]);
            free(mean_low_zg[i][j]);

            free(peaks_xa[i][j]);
            free(peaks_ya[i][j]);
            free(peaks_zg[i][j]);
            free(troughs_xa[i][j]);
            free(troughs_ya[i][j]);
            free(troughs_zg[i][j]);

            free(stride_all[i][j]);
            free(stride_float[i][j]);

        }
        free(mean_high_xa[i]);
        free(mean_high_ya[i]);
        free(mean_high_zg[i]);
        free(mean_low_xa[i]);
        free(mean_low_ya[i]);
        free(mean_low_zg[i]);

        free(peaks_xa[i]);
        free(peaks_ya[i]);
        free(peaks_zg[i]);
        free(troughs_xa[i]);
        free(troughs_ya[i]);
        free(troughs_zg[i]);

        free(stride_all[i]);
        free(stride_float[i]);
    }
    free(mean_high_xa);
    free(mean_high_ya);
    free(mean_high_zg);
    free(mean_low_xa);
    free(mean_low_ya);
    free(mean_low_zg);

    free(peaks_xa);
    free(peaks_ya);
    free(peaks_zg);
    free(troughs_xa);
    free(troughs_ya);
    free(troughs_zg);

    free(stride_all);
    free(stride_float);
    free(num_strides);

}
