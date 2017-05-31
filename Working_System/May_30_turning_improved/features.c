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
    float *sigma_high_za, *sigma_low_za;
    float **troughs_za, **peaks_za;
    float **mean_high_za, **mean_low_za;
    float *sigma_high_yg, *sigma_low_yg;
    float **troughs_yg, **peaks_yg;
    float **mean_high_yg, **mean_low_yg;

    int **stride_all;
    int num_strides;

//    float sigma_high_xa[SLOTS][STRIDES];
    
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

    sigma_high_za = (float *) malloc(sizeof(float) * STRIDES);
    sigma_low_za = (float *) malloc(sizeof(float) * STRIDES);
    troughs_za = (float **) malloc(sizeof(float *) * STRIDES);
    peaks_za = (float **) malloc(sizeof(float *) * STRIDES);
    mean_high_za = (float **) malloc(sizeof(float *) * STRIDES);
    mean_low_za = (float **) malloc(sizeof(float *) * STRIDES);

    sigma_high_yg = (float *) malloc(sizeof(float) * STRIDES);
    sigma_low_yg = (float *) malloc(sizeof(float) * STRIDES);
    troughs_yg = (float **) malloc(sizeof(float) * STRIDES);
    peaks_yg = (float **) malloc(sizeof(float *) * STRIDES);
    mean_high_yg = (float **) malloc(sizeof(float *) * STRIDES);
    mean_low_yg = (float **) malloc(sizeof(float *) * STRIDES);

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
        peaks_za[j] = (float *) malloc(4 * sizeof(float));
        troughs_za[j] = (float *) malloc(4 * sizeof(float));
        mean_high_za[j] = (float *) malloc(SLOTS * sizeof(float));
        mean_low_za[j] = (float *) malloc(SLOTS * sizeof(float));
	peaks_yg[j] = (float *) malloc(4 * sizeof(float));
	troughs_yg[j] = (float *) malloc(4* sizeof(float));
	mean_high_yg[j] = (float *) malloc(SLOTS * sizeof(float));
	mean_low_yg[j] = (float *) malloc(SLOTS * sizeof(float));

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
        sigma_stride(data[3], stride_all, sigma[2], sigma_low_za, sigma_high_za, num_strides);
        sigma_stride(data[6], stride_all, sigma[5], sigma_low_zg, sigma_high_zg, num_strides);
        sigma_stride(data[4], stride_all, sigma[3], sigma_low_xg, sigma_high_xg, num_strides);
	sigma_stride(data[5], stride_all, sigma[4], sigma_low_yg, sigma_high_yg, num_strides);

//        printf("before peaks\n");
        peaks(data[1], stride_all, peaks_xa, troughs_xa, num_strides);
        peaks(data[2], stride_all, peaks_ya, troughs_ya, num_strides);
        peaks(data[3], stride_all, peaks_za, troughs_za, num_strides);
        peaks(data[6], stride_all, peaks_zg, troughs_zg, num_strides);
        peaks(data[4], stride_all, peaks_xg, troughs_xg, num_strides);

        mean_slot(data[1], stride_all, mean[0], mean_low_xa, mean_high_xa, SLOTS, num_strides);
        mean_slot(data[2], stride_all, mean[1], mean_low_ya, mean_high_ya, SLOTS, num_strides);
        mean_slot(data[3], stride_all, mean[2], mean_low_za, mean_high_za, SLOTS, num_strides);
        mean_slot(data[6], stride_all, mean[5], mean_low_zg, mean_high_zg, SLOTS, num_strides);
        mean_slot(data[4], stride_all, mean[3], mean_low_xg, mean_high_xg, SLOTS, num_strides);
	mean_slot(data[5], stride_all, mean[4], mean_low_yg, mean_high_yg, SLOTS, num_strides);
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
	for(k = 0; k < SLOTS; k++)
	{
//        classifiers[j][l++] = mean_low_xa[j][k];
//        classifiers[j][l++] = mean_low_xa[j][k];
//        classifiers[j][l++] = mean_low_ya[j][k];
//        classifiers[j][l++] = mean_low_ya[j][k];
//        classifiers[j][l++] = mean_low_zg[j][k];
//        classifiers[j][l++] = mean_low_zg[j][k];
        classifiers[j][l++] = mean_high_xa[j][k];
        classifiers[j][l++] = mean_high_xa[j][k];
        classifiers[j][l++] = mean_high_ya[j][k];
        classifiers[j][l++] = mean_high_ya[j][k];
        classifiers[j][l++] = mean_high_zg[j][k];
        classifiers[j][l++] = mean_high_zg[j][k];
	}

        classifiers[j][l++] = sigma_high_xa[j];
//            printf("%f\t", classifiers[j][l-1]);
        classifiers[j][l++] = sigma_low_xa[j];
//            printf("%f\t", classifiers[j][l-1]);

    }

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
/*        turning[j][l++] = sigma_low_za[j];
        turning[j][l++] = sigma_high_za[j];
        turning[j][l++] = peaks_za[j][0];
	turning[j][l++] = troughs_za[j][0];
*/
//        for(k = 0; k < SLOTS; k++){
//            turning[j][l++] = mean_low_za[j][k];
//        }
/*       for(k = 0; k < SLOTS; k++){
            turning[j][l++] = mean_high_za[j][k];
        }
*/

        turning[j][l++] = sigma_low_xg[j];
        turning[j][l++] = sigma_high_xg[j];
	turning[j][l++] = peaks_xg[j][0];
        turning[j][l++] = troughs_xg[j][0];

        turning[j][l++] = sigma_low_zg[j];
        turning[j][l++] = sigma_high_zg[j];
	turning[j][l++] = peaks_zg[j][0];
        turning[j][l++] = troughs_zg[j][0];
	
	int diff =  peaks_zg[j][3] - peaks_zg[j][1];
	if(diff >= 0)
		turning[j][l++] = diff;
	else
		turning[j][l++] = -diff;
	
	turning[j][l++] = (peaks_xg[j][1] - troughs_xg[j][1]) + (peaks_xg[j][3] - troughs_xg[j][1]);




	int si;	
	int n_cross_high_xg = 0;
	int n_cross_low_xg = 0;
	for(si = stride_all[j][0]; si < stride_all[j][1]; si++)
	{
		if(data[4][si] == sigma_high_xg[j])
			n_cross_high_xg++;
		else if(data[4][si] == sigma_low_xg[j])
			n_cross_low_xg++;
	}
	turning[j][l++] = n_cross_high_xg;
	turning[j][l++] = n_cross_low_xg;


//        for(k = 0; k < SLOTS; k++){
//            turning[j][l++] = mean_low_xg[j][k];
//        }

	for(k = 0; k < SLOTS; k++){
            turning[j][l++] = mean_high_xg[j][k];
        }



//////////////////////////////////////////////////////////////////////////	
/*	float *xg_mean;
	xg_mean = (float*) malloc(sizeof(float) * FILES);
	memset(xg_mean, 0 , STRIDES);
	for(k = 0; k < SLOTS; k++){
	    xg_mean[j] += mean_low_xg[j][k] + mean_high_xg[j][k];
	}
	turning[j][l++] = xg_mean[j];
	*/
//////////////////////////////////////////////////////////////////////////
	turning[j][l++] = (peaks_zg[j][0])*(peaks_zg[j][0])-(peaks_zg[j][2])*(peaks_zg[j][2]); // no turn shape has sharpe higher peak1 than peak2

	turning[j][l++] = sigma_low_xa[j];
	turning[j][l++] = sigma_high_xa[j];

	turning[j][l++] = sigma_low_zg[j];
	turning[j][l++] = sigma_high_zg[j];
	turning[j][l++] = peaks_zg[j][0];

	turning[j][l++] = sigma_low_yg[j];
	turning[j][l++] = sigma_high_yg[j];
	turning[j][l++] = peaks_yg[j][0];
	
	for(k = 0; k< SLOTS; k++){
		turning[j][l++] = mean_high_yg[j][k];
	}
	
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
        free(mean_high_za[j]);
        free(mean_low_xa[j]);
        free(mean_low_ya[j]);
        free(mean_low_zg[j]);
        free(mean_low_xg[j]);
        free(mean_low_za[j]);

        free(peaks_xa[j]);
        free(peaks_ya[j]);
        free(peaks_za[j]);
        free(peaks_zg[j]);
        free(peaks_xg[j]);
        free(troughs_xa[j]);
        free(troughs_ya[j]);
        free(troughs_za[j]);
        free(troughs_zg[j]);
        free(troughs_xg[j]);
        free(stride_all[j]);

    }

    free(mean_high_xa);
    free(mean_high_ya);
    free(mean_high_za);
    free(mean_high_zg);
    free(mean_high_xg);
    free(mean_low_xa);
    free(mean_low_ya);
    free(mean_low_za);
    free(mean_low_zg);
    free(mean_low_xg);

    free(peaks_xa);
    free(peaks_ya);
    free(peaks_za);
    free(peaks_zg);
    free(peaks_xg);
    free(troughs_xa);
    free(troughs_ya);
    free(troughs_za);
    free(troughs_zg);
    free(troughs_xg);

    free(sigma_high_xa);
    free(sigma_low_xa);
    free(sigma_high_ya);
    free(sigma_low_ya);
    free(sigma_high_za);
    free(sigma_low_za);
    free(sigma_high_zg);
    free(sigma_low_zg);
    free(sigma_high_xg);
    free(sigma_low_xg);

    free(stride_all);
    printf("features out, strides: %d\n", num_strides);
    *rows = num_strides;

}

