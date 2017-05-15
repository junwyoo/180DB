#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"

int main(int argc, char **argv) {
    int i, j, k;

    float ***classifiers;
    classifiers = (float ***) malloc(sizeof ( float **) * FILES);
    for(i = 0; i < FILES; i++)
    {
        classifiers[i] = (float **) malloc(sizeof ( float *) * STRIDES);
    }
    for(i = 0; i < FILES; i++)
    {
        for(j = 0; j < FEATURES; j++)
        {
            classifiers[i][j] = (float *) malloc(sizeof ( float) * FEATURES);
            memset(classifiers[i][j], 0, sizeof(float) * FEATURES);
        }

    }
    float ***turning;
    turning = (float ***) malloc(sizeof ( float **) * TURNING);

    for(i = 0; i < TURNING; i++)
    {
        turning[i] = (float **) malloc(sizeof ( float *) * STRIDES);
    }
    for(i = 0; i < TURNING; i++)
    {
        for(j = 0; j < STRIDES; j++)
        {
            turning[i][j] = (float *) malloc(sizeof (float) * TURN_FEATURES);
            memset(turning[i][j], 0, sizeof(float) * TURN_FEATURES);
        }
    }
    float ***turning_all;
    turning_all = (float ***) malloc(sizeof ( float **) * FILES);

    for(i = 0; i < FILES; i++)
    {
        turning_all[i] = (float **) malloc(sizeof ( float *) * STRIDES);
    }
    for(i = 0; i < FILES; i++)
    {
        for(j = 0; j < STRIDES; j++)
        {
            turning_all[i][j] = (float *) malloc(sizeof (float) * TURN_FEATURES);
            memset(turning_all[i][j], 0, sizeof(float) * TURN_FEATURES);
        }
    }
    // printf("Turning memory\n");
    // pointer to 3 d array with first D1 is activities,
    // second D2 is rows per activity,
    // third D3 is number of classifies (27 for now)
    int *dimen;
    dimen = (int *) malloc(sizeof (int) * FILES);
    memset(dimen, 0, sizeof(int) * FILES);
    int input;
    input = FEATURES;
    // when training, there will be sum o n rows if dim[i] for i from 0 to n-1 rows
    // dim[i] is the truth value or element 1 in row of -1 -1 -1 1(ith) -1

    // example follows

    int speeds;
//    float **test_array;
    int num_of_test_classifers;
    int truth_value;
    char **dot_text_names;
    char **dot_net_name;
    char **dot_csv_name;
    int dot_net_files;

    char *dot_text_turning;
    char *dot_net_turning;

    char *name;
    name = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(name, 0, sizeof(char) * BUFF_SIZE);

    sprintf(name, "AA");
//    sprintf(name, argv[1]);

    dot_net_turning = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(dot_net_turning, 0, sizeof(char) * BUFF_SIZE);
    dot_text_turning = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(dot_text_turning, 0, sizeof(char) * BUFF_SIZE);
    sprintf(dot_net_turning, "%s%s_%s/%s_%s.net", main_path, train_file, name, turning_name, name);
    sprintf(dot_text_turning,"%s%s_%s/%s_%s.txt", main_path, train_file, name, turning_name, name);


    dot_text_names = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    dot_net_name = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    dot_csv_name = (char **) malloc(sizeof(char*) * (FILES));

    for (i = 0; i < ACTIVITIES + 1; i++) {
        dot_net_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        dot_text_names[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_net_name[i], 0, sizeof(char) * BUFF_SIZE);
        memset(dot_text_names[i], 0, sizeof(char) * BUFF_SIZE);
    }
    for (i = 0; i < FILES; i++) {
        dot_csv_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_csv_name[i], 0, sizeof(char) * BUFF_SIZE);
        sprintf(dot_csv_name[i], "%s%s_%s/%s_%s.csv", main_path, train_file, name, names[i], name);
    }

    printf("if loop\n");

                                            // path, folder user
    sprintf(dot_net_name[0], "%s%s_%s/%s_%s.net", main_path, train_file, name, activities_file, name);
    sprintf(dot_text_names[0],"%s%s_%s/%s_%s_%s", main_path, train_file, name, activities_file, name, train_name);


    for (i = 0; i < ACTIVITIES; i++) {
        sprintf(dot_net_name[i+1], "%s%s_%s/%s_%s.net", main_path, train_file, name, train_speeds[i], name);
        sprintf(dot_text_names[i+1],"%s%s_%s/%s_%s_%s", main_path, train_file, name, train_speeds[i], name, train_name);
        // now lists of .net and txt names are ready
    }
    dot_net_files = 1;
    for (i = 0; i < ACTIVITIES + 1; i++) {
        if (access(dot_net_name[i], F_OK) == -1)
            dot_net_files = -1;
    }
    if(access(dot_net_turning, F_OK) == -1)
        dot_net_files = -1;
    int success = 1;
    if (dot_net_files == -1) {
        printf("Training is required, proceeding to trainging\n");
        // funlion call
//        success = checkCSV(name);
    }
    if(success != 1)
    {
	    printf("fail to collect files, will exit\n");
        exit(1);
    }

    float ***data;
    float ***sigma;
    float ***mean;
    int ** strides_t1, *count_t1;
    int ** strides_t2, *count_t2;
    int *data_lengths;

    count_t1 = (int*) malloc(sizeof(int) * FILES);
    count_t2 = (int*) malloc(sizeof(int) * FILES);
    memset(count_t2, 0, sizeof(int) * FILES);
    memset(count_t1, 0, sizeof(int) * FILES);
    strides_t1 = (int**) malloc(sizeof(int*) * FILES);
    strides_t2 = (int**) malloc(sizeof(int*) * FILES);
    for(i = 0; i < FILES; i++)
    {
        strides_t1[i] = (int*) malloc(sizeof(int) * STRIDES);
        strides_t2[i] = (int*) malloc(sizeof(int) * STRIDES);
        memset(strides_t2[i], 0, sizeof(int) * FILES);
        memset(strides_t1[i], 0, sizeof(int) * FILES);
    }
    data_lengths = (int*) malloc(sizeof(int) * FILES);
    data = (float ***) malloc(FILES * sizeof(float **));
    mean = (float ***) malloc(FILES * sizeof(float **));
    sigma = (float ***) malloc(FILES * sizeof(float **));
    for (i = 0; i < FILES; i++) {
        data[i] = (float **) malloc(sizeof(float*) * DATA);
        mean[i] = (float **) malloc(sizeof(float*) * DATA);
        sigma[i] = (float **) malloc(sizeof(float*) * DATA);
    }
    for (i = 0; i < FILES; i++) {
        for(j = 0; j < DATA; j++)
        {
            data[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            mean[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            sigma[i][j] = (float *) malloc(sizeof(float) * MAX_DATA);
            memset(data[i][j], 0, sizeof(float) * MAX_DATA);
            memset(sigma[i][j], 0, sizeof(float) * MAX_DATA);
            memset(mean[i][j], 0, sizeof(float) * MAX_DATA);
        }

    }

    if (dot_net_files == -1) {
        // also if data returned success
        printf("after training\n");
        // add name to train parameters function
        for(i = 0; i < FILES; i++)
        {
            strides(dot_csv_name[i], data[i], sigma[i], mean[i],
            &data_lengths[i], strides_t2[i], strides_t1[i], &count_t2[i]);

//            printf("done strides, now features\n");
//            printf("i: %d\t", i);
            features(data[i], mean[i], sigma[i], strides_t2[i], count_t2[i],
                     classifiers[i], turning_all[i], &dimen[i]);
        }
        for(i = 0; i < TURNING; i++)
        {
            turning[i] = turning_all[turns[i]];
        }

        printf("before text file\n");
        training_file(classifiers, dot_text_names[0], dimen, activities, input, 0, NO_TURN, 5);
        turning_file(turning, dot_text_turning, dimen);
        for (i = 0; i < ACTIVITIES; i++) {
            speeds = speeds_end[i] - speeds_start[i];
//            printf("%s\n", dot_text_names[i+1]);
            training_file(classifiers, dot_text_names[i+1], dimen, speeds_outputs, input, speeds_start[i], speeds_end[i],
                          speeds);
            printf("after file\n");
        }
        //train neural_network

        for(i = PRINT; i < TURNING; i++) {
            printf("%d\n", count_t2[turns[i]]);
            for(j = 0; j < count_t2[turns[i]]; j++)
            {
                for(k = 0; k < TURN_FEATURES; k++){
                    printf("%f\t", turning[i][j][k]);
                }
                printf("\n");
            }
        }
	    train_network(dot_text_turning, dot_net_turning, TURN_FEATURES, 3);
	    train_network(dot_text_names[0], dot_net_name[0], FEATURES, 5);
        for (i = 1; i < ACTIVITIES + 1; i++) {
            train_network(dot_text_names[i], dot_net_name[i], FEATURES, 4);
        }
        for (i = 0; i < FILES; i++) {
            for (j = 0; j < dimen[i]; j++) {
                free(classifiers[i][j]);
            }
            free(classifiers[i]);
        }
        free(classifiers);
        free(dimen);
        main2(2, "");
    }
}
