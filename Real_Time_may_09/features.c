#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"




void features(float ***data, float ***mean, float ***sigma, int **strides, int *stride_count,
              float ***classifiers, int *dimen, float ***turning)
{

    int i, j, k;
    float ** sigma_high_xa, ** sigma_low_xa;
    float ** sigma_high_ya, ** sigma_low_ya;
    float ** sigma_high_zg, ** sigma_low_zg;


    float *** troughs_xa, *** peaks_xa;
    float *** troughs_ya, *** peaks_ya;
    float *** troughs_zg, *** peaks_zg;




    float *** mean_low_xa, *** mean_high_xa;
    float *** mean_low_ya, *** mean_high_ya;
    float *** mean_low_zg, *** mean_high_zg;


    // turning features
    float ** sigma_high_xg, ** sigma_low_xg;
    float *** troughs_xg, *** peaks_xg;
    float *** mean_low_xg, *** mean_high_xg;

    int *** stride_all;
    float *** stride_float;
    int *num_strides;

    sigma_high_xa = (float **) malloc(FILES * sizeof(float *));
    sigma_low_xa = (float **) malloc(FILES * sizeof(float *));
    sigma_high_ya = (float **) malloc(FILES * sizeof(float *));
    sigma_low_ya = (float **) malloc(FILES * sizeof(float *));
    sigma_high_zg = (float **) malloc(FILES * sizeof(float *));
    sigma_low_zg = (float **) malloc(FILES * sizeof(float *));

    sigma_high_xg = (float **) malloc(FILES * sizeof(float *));
    sigma_low_xg = (float **) malloc(FILES * sizeof(float *));
    troughs_xg = (float ***) malloc(FILES * sizeof(float **));
    peaks_xg = (float ***) malloc(FILES * sizeof(float **));
    mean_high_xg = (float ***) malloc(FILES * sizeof(float **));
    mean_low_xg = (float ***) malloc(FILES * sizeof(float **));

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

        // turning features
        sigma_high_xg[i] = (float *) malloc(sizeof(float) * STRIDES);
        sigma_low_xg[i] = (float *) malloc(sizeof(float) * STRIDES);
        troughs_xg[i] = (float **) malloc(sizeof(float *) * STRIDES);
        peaks_xg[i] = (float **) malloc(sizeof(float *) * STRIDES);
        mean_high_xg[i] = (float **) malloc(sizeof(float *) * STRIDES);
        mean_low_xg[i] = (float **) malloc(sizeof(float *) * STRIDES);

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

            mean_high_xg[i][j] = (float *) malloc(SLOTS * sizeof(float ));
            mean_low_xg[i][j] = (float *) malloc(SLOTS * sizeof(float ));
            peaks_xg[i][j] = (float *) malloc(4 * sizeof(float ));
            troughs_xg[i][j] = (float *) malloc(4 * sizeof(float ));

        }
    }
//    printf("after initializing\n");
    int cnt;
    cnt = 0;
    for(i = 0; i < FILES; i++)
    {
        cnt = 0;
        for(j = 1; j < stride_count[i]; j++)
        {
            if(data[i][0][strides[i][j]] - data[i][0][strides[i][j-1]] < 4)
            {
                stride_all[i][cnt][0] = strides[i][j-1];
                stride_all[i][cnt][1] = strides[i][j];
                stride_all[i][cnt][2] = strides[i][j]-strides[i][j-1];
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

        for(j = PRINT; j < num_strides[i]; j++)
        {
            printf("stride: %d, end: %d, period: %d, next: %d\n", stride_all[i][j][0],
                   stride_all[i][j][1], stride_all[i][j][2], stride_all[i][j][3]);
        }

    }

    // strides_all
    // first dimension  activities,
    // second dimension is stride index
    // third dimension 0 start of stride, 1 is end of stride, 2 is period

    // num_stride[i] contains the number of strides

    for(i = 0; i < FILES; i++)
    {
        sigma_stride(data[i][1], stride_all[i], sigma[i][0], sigma_low_xa[i], sigma_high_xa[i], num_strides[i]);
        sigma_stride(data[i][2], stride_all[i], sigma[i][1], sigma_low_ya[i], sigma_high_ya[i], num_strides[i]);
        sigma_stride(data[i][6], stride_all[i], sigma[i][5], sigma_low_zg[i], sigma_high_zg[i], num_strides[i]);
        sigma_stride(data[i][4], stride_all[i], sigma[i][3], sigma_low_xg[i], sigma_high_xg[i], num_strides[i]);

//        printf("before peaks\n");
        peaks(data[i][1], stride_all[i], peaks_xa[i], troughs_xa[i], num_strides[i]);
        peaks(data[i][2], stride_all[i], peaks_ya[i], troughs_ya[i], num_strides[i]);
        peaks(data[i][6], stride_all[i], peaks_zg[i], troughs_zg[i], num_strides[i]);
        peaks(data[i][4], stride_all[i], peaks_xg[i], troughs_xg[i], num_strides[i]);
        for(j = PRINT; j < num_strides[i]-1; j++)
        {
            printf("%f\t%f\t%f\t%f\n", peaks_xa[i][j][0], peaks_xa[i][j][1], peaks_xa[i][j][2], peaks_xa[i][j][3]);
            printf("%f\t%f\t%f\t%f\n\n", troughs_xa[i][j][0], troughs_xa[i][j][1], troughs_xa[i][j][2], troughs_xa[i][j][3]);
        }
    }

//    float *** out;

    int l;
//    out = (float ***) malloc(FILES * sizeof(float **));
//    dimen = (int *) malloc(FILES * sizeof(int));
    for(i = 0; i < FILES; i++) {
        dimen[i] = num_strides[i]-1;
//        classifiers[i] = (float **) malloc(sizeof(float*) * (num_strides[i]-1));
//        printf("%s\n", names[i]);
        for (j = 0; j < num_strides[i]-1; j++) {
//            classifiers[i][j] = (float *) malloc(sizeof(float) * FEATURES);
            l = 0;
            classifiers[i][j][l++] = stride_all[i][j][2];
//            printf("%f\t", out[i][j][l-1]);
            for(k = 0; k < 4; k++){
                classifiers[i][j][l++] = peaks_xa[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }

            for(k = 0; k < 4; k++){
                classifiers[i][j][l++] = troughs_xa[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }
            for(k = 0; k < 4; k++){
                classifiers[i][j][l++] = peaks_ya[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }

            for(k = 0; k < 4; k++){
                classifiers[i][j][l++] = troughs_ya[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }
            for(k = 0; k < 4; k++){
                classifiers[i][j][l++] = peaks_zg[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }
            for(k = 0; k < 4; k++){
                classifiers[i][j][l++] = troughs_zg[i][j][k];
//                printf("%f\t", out[i][j][l-1]);
            }
            classifiers[i][j][l++] = sigma_high_xa[i][j];
//            printf("%f\t", out[i][j][l-1]);
            classifiers[i][j][l++] = sigma_low_xa[i][j];
//            printf("%f\t", out[i][j][l-1]);
//            printf("\n");
        }

    }
//    printf("after classifiers loading\n");
    for(i = PRINT; i < FILES; i++)
    {
        printf("%s\n", names[i]);
        for (j = 0; j < num_strides[i]; j++){
            for(k = 0; k < FEATURES; k++){
                printf("%f\t", classifiers[i][j][k]);
            }
            printf("\n");
        }
    }

    for(i = 0; i < FILES; i++) {
        stride_count[i] = num_strides[i]-1;
        printf("%s\t%d\t%d\n", names[i], i, num_strides[i]);
    }

    printf("before turning\n");

    for(i = PRINT; i < TURNING; i++) {
        printf("%s\t", names[i]);
        printf("file: %d\tstrides: %d\n", turns[i], num_strides[turns[i]]-1);
        for (j = 0; j < num_strides[turns[i]]-1; j++)
        printf("sigma low: %f\t sigma high: %f\t peak: %f\t trough: %f\t\n",
               sigma_low_xg[turns[i]][j], sigma_high_xg[turns[i]][j], peaks_xg[turns[i]][j][0], troughs_xg[turns[i]][j][0]);

    }

    for(i = PRINT; i < TURNING; i++) {
        for(j = 0; j < TURNING; j++)
        {
            for(k = 0; k < TURN_FEATURES; k++){
                printf("%f", turning[i][j][k]);
            }
            printf("round: %d\n", j);
        }
    }

    for(i = 0; i < TURNING; i++) {
        printf("%s\t", names[i]);
        printf("file: %d\tstrides: %d\n", turns[i], num_strides[turns[i]]-1);
        for (j = 0; j < num_strides[turns[i]]; j++) {
            l = 0;

//            printf("%d\n", num_strides[turns[i]]-1);
            turning[i][j][l++] = sigma_low_xg[turns[i]][j];
            turning[i][j][l++] = sigma_high_xg[turns[i]][j];
            turning[i][j][l++] = peaks_xg[turns[i]][j][0];
            turning[i][j][l++] = troughs_xg[turns[i]][j][0];
//            turning[i][j][l++] = 0 /* turning fieature 5 */;

        }
        printf("last\n");

    }
    printf("before free\n");
//    // Free memory
//    for(i = 0; i < FILES; i++) {
//        for(k = 0; k < DATA; k++) {
//            free(data[i][k]);
//            free(mean[i][k]);
//            free(sigma[i][k]);
//        }
//        free(data[i]);
//        free(mean[i]);
//        free(sigma[i]);
//    }
//
//    free(data);
//    free(mean);
//    free(sigma);


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
    printf("features out\n");

}

