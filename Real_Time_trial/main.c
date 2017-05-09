#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"


int main(int argc, char **argv) {

    float ***classifiers = NULL;
    // pointer to 3 d array with first D1 is activities,
    // second D2 is rows per activity,
    // third D3 is number of classifies (27 for now)
    int *dimen = NULL;
    int input;
    input = FEATURES;
    // when training, there will be sum o n rows if dim[i] for i from 0 to n-1 rows
    // dim[i] is the truth value or element 1 in row of -1 -1 -1 1(ith) -1

    // example follows
    int i, j, k;
    int speeds;
//    float **test_array;
    int num_of_test_classifers;
    int truth_value;
    char **training_name;
    char **dot_net_name;
    int dot_net_files;

    char *name;
    name = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(name, 0, sizeof(char) * BUFF_SIZE);

    sprintf(name, "AA");
//    sprintf(name, argv[1]);

    training_name = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    dot_net_name = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));

    for (i = 0; i < ACTIVITIES + 1; i++) {
        dot_net_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        training_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);
        memset(dot_net_name[i], 0, sizeof(char) * BUFF_SIZE);
        memset(dot_net_name[i], 0, sizeof(char) * BUFF_SIZE);
    }
    printf("if loop\n");

                                            // path, folder user
    sprintf(dot_net_name[0], "%s%s_%s/%s_%s.net", main_path, train_file, name, activities_file, name);
    sprintf(training_name[0],"%s%s_%s/%s_%s_%s", main_path, train_file, name, activities_file, name, train_name);
    for (i = 0; i < ACTIVITIES; i++) {
        sprintf(dot_net_name[i+1], "%s%s_%s/%s_%s.net", main_path, train_file, name, train_speeds[i], name);
        sprintf(training_name[i+1],"%s%s_%s/%s_%s_%s", main_path, train_file, name, train_speeds[i], name, train_name);
        // now lists of .net and txt names are ready
    }
    dot_net_files = 1;
    for (i = 0; i < ACTIVITIES + 1; i++) {
        if (access(dot_net_name[i], F_OK) == -1)
            dot_net_files = -1;
    }
    if (dot_net_files == -1) {
        printf("Training is required, proceeding to trainging\n");
        // function call
    }

    if (dot_net_files == -1) {
        // data returned success
        printf("after training\n");
        // add name to train parameters function
        train_parameters(&classifiers, &dimen);
        printf("before text file\n");
        training_file(classifiers, training_name[0], dimen, activities, input, 0, FILES, ACTIVITIES);

        for (i = 0; i < ACTIVITIES; i++) {
            speeds = speeds_end[i] - speeds_start[i];
            training_file(classifiers, training_name[i+1], dimen, speeds_outputs, input, speeds_start[i], speeds_end[i],
                          speeds);
        }
        //train neural_network

        for (i = 0; i < ACTIVITIES + 1; i++) {
            train_network(training_name[i]);
        }
        for (i = 0; i < FILES; i++) {
            for (j = 0; j < dimen[i]; j++) {
                free(classifiers[i][j]);
            }
            free(classifiers[i]);
        }
        free(classifiers);
        free(dimen);
    }
    consumer("a");
}
