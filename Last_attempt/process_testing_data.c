#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
//#include <sys/time.h>
#include "classifiers.h"

#define BUFF_SIZE 1024
#define STRIDES 200
#define DATA 7
#define NULL 0
#define SLOTS 4
#define FEATURES 27
#define PRINT 100000

int test_classifiers(float ***classifiers, const char * name, int *n){

    int i, j, k;
    char *input_files;
    float **data;
    float **sigma;
    float **mean;
    int * strides_t1, count_t1;
    int * strides_t2, count_t2;
    int N_SAMPLES;
    FILE *fp;
    char *line = NULL;
    size_t len = 0;
    char *record;
    int data_lengths;


    count_t1 = (int) malloc(sizeof(int));
    count_t2 = (int) malloc(sizeof(int));
    strides_t1 = (int*) malloc(sizeof(int) * STRIDES);
    strides_t2 = (int*) malloc(sizeof(int) * STRIDES);
    data = (float **) malloc(sizeof(float*) * DATA);
    mean = (float **) malloc(sizeof(float*) * DATA);
    sigma = (float **) malloc(sizeof(float*) * DATA);

    input_files = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(input_files, 0, BUFF_SIZE);
    snprintf(input_files,
             BUFF_SIZE,
             "%s", name);



//        printf("Starting\n");
    fp = fopen(input_files, "r");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to read from file \'%s\'.\n",
                input_files
        );
        return -1;
//        exit(EXIT_FAILURE);
    }
//        printf("File opened\n");
    /* count the number of lines in the file */
    getline(&line, &len, fp); //discard header of file
    N_SAMPLES = 0;
    while (getline(&line, &len, fp) != -1) {
//            if(N_SAMPLES<100)
//                printf("%s\n", line);
        N_SAMPLES++;
    }
//        *lengths[i] = N_SAMPLES;
//        printf("samples counted: %d\n", N_SAMPLES);
    data_lengths = N_SAMPLES;
    for(j = 0; j < DATA; j++)
    {
        data[j] = (float *) malloc(sizeof(float) * N_SAMPLES);
        mean[j] = (float *) malloc(sizeof(float) * N_SAMPLES);
        sigma[j] = (float *) malloc(sizeof(float) * N_SAMPLES);
    }
//        printf("data initialized\n");
    rewind(fp);
    getline(&line, &len, fp); //discard header of file
    j = 0;
    //        printf("file rewined\n");

    double time_ref;
    time_ref = 0;
    if(getline(&line, &len, fp) != -1) //get first line to zero time
    {
        record = strtok(line, ",");
        time_ref = atof(record);
        k = 0;
        data[k++][j] = 0;
//            printf("time: %f, ref:%f\n", data[i][0][j], time_ref);
//            printf("real value ref: %20.10f\n", strtod(record, NULL));
        record = strtok(NULL, ",");

        while (record != NULL) {
            if(k == 1)
                record = strtok(NULL, ",");
            data[k++][j] = (float) atof(record);
//                printf("%lf\n", data[i][k-1][j]);
            record = strtok(NULL, ",");

        }
        j++;
    }
//        printf("loop 1 done\n");

    while (getline(&line, &len, fp) != -1) {

        record = strtok(line, ",");
        k = 0;
        data[k++][j] = (float) (atof(record) - time_ref);
//            printf("time: %2.10lf, value: %2.10lf\t", data[i][0][j], (float) (atof(record) - time_ref));
//            printf("%lf\n", time_ref);
        record = strtok(NULL, ",");

        while (record != NULL) {
            if(k == 1)
                record = strtok(NULL, ",");
            data[k++][j] = (float) atof(record);
            record = strtok(NULL, ",");

        }
        j++;
    }
    fclose(fp);
//        *lengths[i] = N_SAMPLES;
//        printf("samples counted: %d\n", N_SAMPLES);


    for (j = PRINT; j < data_lengths; j+=3000) {
        printf("%20.10lf,%20.10lf,%10.10lf,%10.10lf,%10.10lf,%10.10lf,%10.10lf\n",
               data[0][j],
               data[1][j],
               data[2][j],
               data[3][j],
               data[4][j],
               data[5][j],
               data[6][j]

        );
    }
    // print data to determine propper values
    for(j = PRINT; j < data_lengths; j+= 100)
    {
        for(k = 0; k < 7; k++)
        {
            printf("%5.15lf\t", data[k][j]);
        }
        printf("\n");


    }
//    printf("offset\n");
    float off_set;
    off_set = data[0][0];
//        printf("offset: %f\n", off_set);
    for(j = 0; j < data_lengths; j++)
    {
//            printf("time: %f\n", data[i][0][j]);
        data[0][j] = data[0][j]- off_set;
    }



    /// testing floats end

//    printf("mean and sigma\n");
    for(j = 0; j < DATA-1; j++)
    {

        sample_mean(data[j+1], mean[j], 3000, data_lengths);
//            printf("mean done: %d\n", j);
        sample_sigma(data[j+1], mean[j], sigma[j], 3000, data_lengths);
//            printf("sigma done: %d\n", j);
        center_data(data[j+1], mean[j], sigma[j], 3000, data_lengths);
//            printf("center done: %d\n", j);
    }
//        printf("next_file\n");
    for (j = PRINT; j < data_lengths; j+=2900)
    {
        printf("mean:   %15.10lf,%15.10lf,%10.10lf,%10.10lf,%10.10lf,%10.10lf\n",
               mean[0][j],
               mean[1][j],
               mean[2][j],
               mean[3][j],
               mean[4][j],
               mean[5][j]
        );
        printf("sigma:   %15.10lf,%15.10lf,%10.10lf,%10.10lf,%10.10lf,%10.10lf\n",
               sigma[0][j],
               sigma[1][j],
               sigma[2][j],
               sigma[3][j],
               sigma[4][j],
               sigma[5][j]
        );
    }

    count_t1 = temp_strides(data, strides_t1, sigma[1], sigma[5], 4, 500, 3, 0.2, data_lengths);
//        printf("%d\n", count_t1[i]);
//        printf("[\n");
//        for(j = 0; j < count_t1[i]; j++)
//        {
//            printf("%d, ", strides_t1[i][j]);
//
//        }
//        printf("],\n");
//        printf("\n");
    count_t2 = select_strides(data[0], strides_t1, strides_t2, 0.4, count_t1);
//        printf("%d\n", count_t2[i]);
//        printf("[\n");
    for(j = 0; j < STRIDES; j++)
        strides_t1[j] = 0;
    for(j = PRINT; j < count_t2; j++)
    {
        printf("%d, ", strides_t2[j]);
    }
//        printf("],\n");
//        printf("\n");
    count_t1 = peak_strides(data[0], data[6], strides_t2, strides_t1, sigma[5], 1.4, count_t2, data_lengths);
//        printf("strides temp: %d, selected: %d\n", count_t1[i], count_t2[i]);
//        count_t2[i] = select_strides(data[i][0], strides_t2[i], strides_t1[i], 0.4, count_t1[i]);
//        count_t1[i] = count_t2[i];
    count_t2 = select_strides(data[0], strides_t1, strides_t2, 0.01, count_t1);
//        printf("before outlier\n");
    count_t1 = outlier(data[0], strides_t2, strides_t1, 0.65, count_t2);
//        printf("second\n");
    count_t2 = outlier(data[0], strides_t1, strides_t2, 0.65, count_t1);
//        for(j = 0; j < count_t1[i]; j++)
//        {
//            strides_t2[i][j] = strides_t1[i][j];
//        }
//        count_t2[i] = count_t1[i];
//    printf("last attempt\n");
    if(PRINT< 2)
    {
        printf("%d\n", count_t1);
        printf("[\n");
        for(j = 0; j < count_t2; j++)
        {
            printf("%d, ", strides_t2[j]);
        }
        printf("\n");
        printf("],\n");
    }
    move_back(strides_t2, 100, count_t2);
//        move_back(strides_t1[i], 100, count_t1[i]);


    float * sigma_high_xa, * sigma_low_xa;
    float * sigma_high_ya, * sigma_low_ya;
    float * sigma_high_zg, * sigma_low_zg;

    float ** troughs_xa, ** peaks_xa;
    float ** troughs_ya, ** peaks_ya;
    float ** troughs_zg, ** peaks_zg;


    float ** mean_low_xa, ** mean_high_xa;
    float ** mean_low_ya, ** mean_high_ya;
    float ** mean_low_zg, ** mean_high_zg;

    int ** stride_all;
    float ** stride_float;
    int num_strides;

    sigma_high_xa = (float *) malloc(sizeof(float) * STRIDES);
    sigma_low_xa = (float *) malloc(sizeof(float) * STRIDES);
    sigma_high_ya = (float *) malloc(sizeof(float) * STRIDES);
    sigma_low_ya = (float *) malloc(sizeof(float) * STRIDES);
    sigma_high_zg = (float *) malloc(sizeof(float) * STRIDES);
    sigma_low_zg = (float *) malloc(sizeof(float) * STRIDES);

    mean_high_xa = (float **) malloc(STRIDES * sizeof(float *));
    mean_low_xa = (float **) malloc(STRIDES * sizeof(float *));
    mean_high_ya = (float **) malloc(STRIDES * sizeof(float *));
    mean_low_ya = (float **) malloc(STRIDES * sizeof(float *));
    mean_high_zg = (float **) malloc(STRIDES * sizeof(float *));
    mean_low_zg = (float **) malloc(STRIDES * sizeof(float *));

    peaks_xa = (float **) malloc(STRIDES * sizeof(float *));
    troughs_xa = (float **) malloc(STRIDES * sizeof(float *));
    peaks_ya = (float **) malloc(STRIDES * sizeof(float *));
    troughs_ya = (float **) malloc(STRIDES * sizeof(float *));
    peaks_zg = (float **) malloc(STRIDES * sizeof(float *));
    troughs_zg = (float **) malloc(STRIDES * sizeof(float *));

    stride_all = (int **) malloc(STRIDES * sizeof(int *));
    stride_float = (float **) malloc(STRIDES * sizeof(float *));
    num_strides = 0;

    for(j = 0; j < STRIDES; j++)
    {
        mean_high_xa[j] = (float *) malloc(SLOTS * sizeof(float ));
        mean_low_xa[j] = (float *) malloc(SLOTS * sizeof(float ));
        mean_high_ya[j] = (float *) malloc(SLOTS * sizeof(float ));
        mean_low_ya[j] = (float *) malloc(SLOTS * sizeof(float ));
        mean_high_zg[j] = (float *) malloc(SLOTS * sizeof(float ));
        mean_low_zg[j] = (float *) malloc(SLOTS * sizeof(float ));

        peaks_xa[j] = (float *) malloc(4 * sizeof(float ));
        troughs_xa[j] = (float *) malloc(4 * sizeof(float ));
        peaks_ya[j] = (float *) malloc(4 * sizeof(float ));
        troughs_ya[j] = (float *) malloc(4 * sizeof(float ));
        peaks_zg[j] = (float *) malloc(4 * sizeof(float ));
        troughs_zg[j] = (float *) malloc(4 * sizeof(float ));

        stride_all[j] = (int *) malloc(4 * sizeof(int ));
        stride_float[j] = (float *) malloc(STRIDES * sizeof(float));

    }
    int cnt;
    cnt = 0;
    for(j = 1; j < count_t2; j++)
    {
        if(data[0][strides_t2[j]] - data[0][strides_t2[j-1]] < 3)
        {
            stride_all[cnt][0] = strides_t2[j-1];
            stride_all[cnt][1] = strides_t2[j];
            stride_all[cnt][2] = strides_t2[j]-strides_t2[j-1];
            num_strides++;
            cnt++;
        }
    }
    for(j = 0; j < num_strides-1; j++)
    {
        if(stride_all[j][1] == stride_all[j+1][0])
        {
            stride_all[j][3] = 1;
        }
        else
        {
            stride_all[j][3] = -1;
        }
    }
    for(j = 1000; j < num_strides; j++)
    {
        printf("stride: %d, end: %d, period: %d, next: %d\n", stride_all[j][0],
               stride_all[j][1], stride_all[j][2], stride_all[j][3]);
    }



    sigma_stride(data[1], stride_all, sigma[0], sigma_low_xa, sigma_high_xa, num_strides);
    sigma_stride(data[2], stride_all, sigma[1], sigma_low_ya, sigma_high_ya, num_strides);
    sigma_stride(data[6], stride_all, sigma[5], sigma_low_zg, sigma_high_zg, num_strides);

//        printf("before peaks\n");
    peaks(data[1], stride_all, peaks_xa, troughs_xa, num_strides);
    peaks(data[2], stride_all, peaks_ya, troughs_ya, num_strides);
    peaks(data[6], stride_all, peaks_zg, troughs_zg, num_strides);
    for(j = PRINT; j < num_strides-1; j++)
    {
        printf("%f\t%f\t%f\t%f\n", peaks_xa[j][0], peaks_xa[j][1], peaks_xa[j][2], peaks_xa[j][3]);
        printf("%f\t%f\t%f\t%f\n\n", troughs_xa[j][0], troughs_xa[j][1], troughs_xa[j][2], troughs_xa[j][3]);
    }

//    printf("done strides\n");

    float ** out;
    int *d1, d2;

    int l;

    d2 = num_strides-1;
    d1 = (int *) malloc((num_strides-1) * sizeof(int));
    out = (float **) malloc(sizeof(float*) * (num_strides-1));


    for (j = 0; j < num_strides-1; j++) {
        d1[j] = FEATURES;
        out[j] = (float *) malloc(sizeof(float) * FEATURES);
        l = 0;
        out[j][l++] = stride_all[j][2];
//            printf("%f\t", out[i][j][l-1]);
        for(k = 0; k < 4; k++){
            out[j][l++] = peaks_xa[j][k];
//                printf("%f\t", out[i][j][l-1]);
        }

        for(k = 0; k < 4; k++){
            out[j][l++] = troughs_xa[j][k];
//                printf("%f\t", out[i][j][l-1]);
        }
        for(k = 0; k < 4; k++){
            out[j][l++] = peaks_ya[j][k];
//                printf("%f\t", out[i][j][l-1]);
        }

        for(k = 0; k < 4; k++){
            out[j][l++] = troughs_ya[j][k];
//                printf("%f\t", out[i][j][l-1]);
        }
        for(k = 0; k < 4; k++){
            out[j][l++] = peaks_zg[j][k];
//                printf("%f\t", out[i][j][l-1]);
        }
        for(k = 0; k < 4; k++){
            out[j][l++] = troughs_zg[j][k];
//                printf("%f\t", out[i][j][l-1]);
        }
        out[j][l++] = sigma_high_xa[j];
//            printf("%f\t", out[i][j][l-1]);
        out[j][l] = sigma_low_xa[j];
//            printf("%f\t", out[i][j][l-1]);
//            printf("\n");
    }


//    printf("%s\n", input_files);
    for (j = 0; j < num_strides-1; j++){
        for(k = 0; k < FEATURES; k++){
            printf("%f\t", out[j][k]);
        }
        printf("\n");
    }

    // free memory

    // Free memory
    for(i = 0; i < DATA; i++) {
        free(data[i]);
        free(mean[i]);
        free(sigma[i]);
    }

    free(data);
    free(strides_t1);
    free(strides_t2);
    free(mean);
    free(sigma);
    free(input_files);


    for(i = 0; i < STRIDES; i++)
    {
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

    *classifiers = out;
    *n = d2;
    return 1;
}