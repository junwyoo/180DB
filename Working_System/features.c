#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"




void features(float **data, float **mean, float **sigma, int *strides, int stride_count,
              float **classifiers, float **turning, int *rows)
{

    int i, j, k, l, cnt;
    float *sigma_high_xa, *sigma_low_xa;
    float *sigma_high_ya, *sigma_low_ya;
    float *sigma_high_zg, *sigma_low_zg;

    float **troughs_xa, **peaks_xa;
    float **troughs_ya, **peaks_ya;
    float **troughs_zg, **peaks_zg;


    float **mean_low_xa, **mean_high_xa;
    float **mean_low_ya, **mean_high_ya;
    float **mean_low_zg, **mean_high_zg;

    float *sigma_high_xg, *sigma_low_xg;
    float **troughs_xg, **peaks_xg;
    float **mean_high_xg, **mean_low_xg;

    int **stride_all;
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
    //stride_float = (float **) malloc(STRIDES * sizeof(float *));
    num_strides = 0;

    sigma_high_xg = (float *) malloc(sizeof(float) * STRIDES);
    sigma_low_xg = (float *) malloc(sizeof(float) * STRIDES);
    troughs_xg = (float **) malloc(sizeof(float *) * STRIDES);
    peaks_xg = (float **) malloc(sizeof(float *) * STRIDES);
    mean_high_xg = (float **) malloc(sizeof(float *) * STRIDES);
    mean_low_xg = (float **) malloc(sizeof(float *) * STRIDES);

    for (j = 0; j < STRIDES; j++) {
        mean_high_xa[j] = (float *) malloc(SLOTS * sizeof(float));
        mean_low_xa[j] = (float *) malloc(SLOTS * sizeof(float));
        mean_high_ya[j] = (float *) malloc(SLOTS * sizeof(float));
        mean_low_ya[j] = (float *) malloc(SLOTS * sizeof(float));
        mean_high_zg[j] = (float *) malloc(SLOTS * sizeof(float));
        mean_low_zg[j] = (float *) malloc(SLOTS * sizeof(float));


        peaks_xa[j] = (float *) malloc(4 * sizeof(float));
        troughs_xa[j] = (float *) malloc(4 * sizeof(float));
        peaks_ya[j] = (float *) malloc(4 * sizeof(float));
        troughs_ya[j] = (float *) malloc(4 * sizeof(float));
        peaks_zg[j] = (float *) malloc(4 * sizeof(float));
        troughs_zg[j] = (float *) malloc(4 * sizeof(float));

        stride_all[j] = (int *) malloc(4 * sizeof(int));
        //istride_float[j] = (float *) malloc(STRIDES * sizeof(float));

        peaks_xg[j] = (float *) malloc(4 * sizeof(float));
        troughs_xg[j] = (float *) malloc(4 * sizeof(float));
        mean_high_xg[j] = (float *) malloc(SLOTS * sizeof(float));
        mean_low_xg[j] = (float *) malloc(SLOTS * sizeof(float));
    }


    cnt = 0;
    for(j = 1; j < stride_count; j++)
    {
        if(data[0][strides[j]] - data[0][strides[j-1]] < 4)
        {
            stride_all[cnt][0] = strides[j-1];
            stride_all[cnt][1] = strides[j];
            stride_all[cnt][2] = strides[j]-strides[j-1];
            num_strides++;
            cnt++;
        }
    }

//    printf("after stride all\n");
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




    // strides_all
    // first dimension  activities,
    // second dimension is stride index
    // third dimension 0 start of stride, 1 is end of stride, 2 is period

    // num_stride[i] contains the number of strides

//    printf("before parameters\n");
    for(i = 0; i < FILES; i++)
    {
        sigma_stride(data[1], stride_all, sigma[0], sigma_low_xa, sigma_high_xa, num_strides);
        sigma_stride(data[2], stride_all, sigma[1], sigma_low_ya, sigma_high_ya, num_strides);
        sigma_stride(data[6], stride_all, sigma[5], sigma_low_zg, sigma_high_zg, num_strides);
        sigma_stride(data[4], stride_all, sigma[3], sigma_low_xg, sigma_high_xg, num_strides);

//        printf("before peaks\n");
        peaks(data[1], stride_all, peaks_xa, troughs_xa, num_strides);
        peaks(data[2], stride_all, peaks_ya, troughs_ya, num_strides);
        peaks(data[6], stride_all, peaks_zg, troughs_zg, num_strides);
        peaks(data[4], stride_all, peaks_xg, troughs_xg, num_strides);

        mean_slot(data[1], stride_all, mean[0], mean_low_xa, mean_high_xa, 2.0, num_strides);
        mean_slot(data[2], stride_all, mean[1], mean_low_ya, mean_high_ya, 2.0, num_strides);
        mean_slot(data[6], stride_all, mean[5], mean_low_zg, mean_high_zg, 2.0, num_strides);
        mean_slot(data[4], stride_all, mean[3], mean_low_xg, mean_high_xg, 2.0, num_strides);
        for(j = PRINT; j < num_strides; j++)
        {
            printf("%f\t%f\t%f\t%f\n", peaks_xa[j][0], peaks_xa[j][1], peaks_xa[j][2], peaks_xa[j][3]);
            printf("%f\t%f\t%f\t%f\n\n", troughs_xa[j][0], troughs_xa[j][1], troughs_xa[j][2], troughs_xa[j][3]);
        }
    }
//    printf("after parameters\n");

    for (j = 0; j < num_strides; j++) {

        l = 0;
        classifiers[j][l++] = stride_all[j][2];
//            printf("%f\t", classifiers[j][l-1]);
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = peaks_xa[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }

        for(k = 0; k < 4; k++){
            classifiers[j][l++] = troughs_xa[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = peaks_ya[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }

        for(k = 0; k < 4; k++){
            classifiers[j][l++] = troughs_ya[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = peaks_zg[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        for(k = 0; k < 4; k++){
            classifiers[j][l++] = troughs_zg[j][k];
//                printf("%f\t", classifiers[j][l-1]);
        }
        classifiers[j][l++] = sigma_high_xa[j];
//            printf("%f\t", classifiers[j][l-1]);
        classifiers[j][l++] = sigma_low_xa[j];

//            printf("%f\t", classifiers[j][l-1]);
//            printf("\n");
    }

//    printf("after classifiers\n");

//    out = (float ***) malloc(FILES * sizeof(float **));
//    rows = (int *) malloc(FILES * sizeof(int));
//    for(i = 0; i < FILES; i++) {
//        rows[i] = num_strides[i]-1;
////        classifiers[i] = (float **) malloc(sizeof(float*) * (num_strides[i]-1));
////        printf("%s\n", names[i]);
//        for (j = 0; j < num_strides[i]-1; j++) {
////            classifiers[i][j] = (float *) malloc(sizeof(float) * FEATURES);
//            l = 0;
//            classifiers[i][j][l++] = stride_all[i][j][2];
////            printf("%f\t", out[i][j][l-1]);
//            for(k = 0; k < 2; k++){
//                classifiers[i][j][l++] = peaks_xa[i][j][k];
////                printf("%f\t", out[i][j][l-1]);
//            }
//
//            for(k = 0; k < 2; k++){
//                classifiers[i][j][l++] = troughs_xa[i][j][k];
//
////                printf("%f\t", out[i][j][l-1]);
//            }
//
//            for(k = 0; k < 2; k++){
//                classifiers[i][j][l++] = peaks_ya[i][j][k];
////                printf("%f\t", out[i][j][l-1]);
//            }
//
//            for(k = 0; k < 2; k++){
//                classifiers[i][j][l++] = troughs_ya[i][j][k];
////                printf("%f\t", out[i][j][l-1]);
//            }
//            for(k = 0; k < 2; k++){
//                classifiers[i][j][l++] = peaks_zg[i][j][k];
////                printf("%f\t", out[i][j][l-1]);
//            }
//            for(k = 0; k < 2; k++){
//                classifiers[i][j][l++] = troughs_zg[i][j][k];
////                printf("%f\t", out[i][j][l-1]);
//            }
//            classifiers[j][l++] = mean_low_xa[j][k];
//            classifiers[j][l++] = mean_low_xa[j][k];
//            classifiers[j][l++] = mean_low_ya[j][k];
//            classifiers[j][l++] = mean_low_ya[j][k];
//            classifiers[j][l++] = mean_low_zg[j][k];
//            classifiers[j][l++] = mean_low_zg[j][k];
//            classifiers[j][l++] = mean_high_xa[j][k];
//            classifiers[j][l++] = mean_high_xa[j][k];
//            classifiers[j][l++] = mean_high_ya[j][k];
//            classifiers[j][l++] = mean_high_ya[j][k];
//            classifiers[j][l++] = mean_high_zg[j][k];
//            classifiers[j][l++] = mean_high_zg[j][k];
//            classifiers[i][j][l++] = sigma_high_xa[i][j];
////            printf("%f\t", out[i][j][l-1]);
//            classifiers[i][j][l++] = sigma_low_xa[i][j];
////            printf("%f\t", out[i][j][l-1]);
////            printf("\n");
//        }
//
//    }
//    printf("after classifiers loading\n");


//    printf("before turning\n");


    for(j = PRINT; j < TURNING; j++)
    {
        for(k = 0; k < TURN_FEATURES; k++){
            printf("%f", turning[j][k]);
        }
        printf("round: %d\n", j);
    }

//    printf("before turning\n");
    for (j = 0; j < num_strides; j++) {
        l = 0;

//            printf("%d\n", num_strides[turns[i]]-1);
        turning[j][l++] = sigma_low_xg[j];
        turning[j][l++] = sigma_high_xg[j];
        turning[j][l++] = peaks_xg[j][0];
        turning[j][l++] = troughs_xg[j][0];
        turning[j][l++] = mean_low_xg[j][0];
        turning[j][l++] = mean_low_xg[j][1];
        turning[j][l++] = mean_high_xg[j][0];
        turning[j][l++] = mean_high_xg[j][1];
//            turning[i][j][l++] = 0 /* turning fieature 5 */;
        if(PRINT == 0)
        {
            printf("%f\t%f\t%f\t%f\n", mean_low_xg[j][0]/1024,
                   mean_low_xg[j][1]/1024, mean_high_xg[j][0]/1024, mean_high_xg[j][1]/1024);
        }

    }
//    printf("before free\n");




    for(j = 0; j < STRIDES; j++)
    {
        free(mean_high_xa[j]);
        free(mean_high_ya[j]);
        free(mean_high_zg[j]);
        free(mean_high_xg[j]);
        free(mean_low_xa[j]);
        free(mean_low_ya[j]);
        free(mean_low_zg[j]);
        free(mean_low_xg[j]);

        free(peaks_xa[j]);
        free(peaks_ya[j]);
        free(peaks_zg[j]);
        free(peaks_xg[j]);
        free(troughs_xa[j]);
        free(troughs_ya[j]);
        free(troughs_zg[j]);
        free(troughs_xg[j]);
        free(stride_all[j]);

    }

    free(mean_high_xa);
    free(mean_high_ya);
    free(mean_high_zg);
    free(mean_high_xg);
    free(mean_low_xa);
    free(mean_low_ya);
    free(mean_low_zg);
    free(mean_low_xg);

    free(peaks_xa);
    free(peaks_ya);
    free(peaks_zg);
    free(peaks_xg);
    free(troughs_xa);
    free(troughs_ya);
    free(troughs_zg);
    free(troughs_xg);

    free(sigma_high_xa);
    free(sigma_low_xa);
    free(sigma_high_ya);
    free(sigma_low_ya);
    free(sigma_high_zg);
    free(sigma_low_zg);
    free(sigma_high_xg);
    free(sigma_low_xg);

    free(stride_all);
    //printf("features out, strides: %d\n", num_strides);
    *rows = num_strides;

}

