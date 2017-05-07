#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"


//const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 2, 3, 4, 4, 4};
//const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 0, 0, 0, 1, 2};
//const int speeds_start[]  =     {0, 4, 7, 8, 9};
//const int speeds_end[] =        {4, 7, 8, 9, 12};
int main(int argc, char **argv) {

    char * fake_test;
    fake_test = (char*)malloc(sizeof(char)*BUFF_SIZE);
    memset(fake_test, 0, sizeof(char)*BUFF_SIZE);
    sprintf(fake_test, "%s%s%s_%s.csv", main_path, paths[0], names[0], user_names[0]);

    //char * test_file_name;
//    test_file_name = (char*)malloc(sizeof(char)*BUFF_SIZE);
//    memset(test_file_name, 0, BUFF_SIZE);

    float ***classifiers = NULL;
    // pointer to 3 d array with first D1 is activities,
    // second D2 is rows per activity,
    // third D3 is number of classifies (27 for now)
    int **counts = NULL;
    // pointer to 2 d array with first D1 is holds number of activities (12 total for now)
    // second D number of rows
    // each element is number of classifiers in each row, (constant of 27, not necessary)
    int *dimen = NULL;
    // dime is a pointer to 1 d array with first D1 number of activities (12 for now)
    // second D2 is rows per activity
    int n;
    // n is integer with number of activities (12 for now)
    int input;
    input = FEATURES;
    // when training, there will be sum o n rows if dim[i] for i from 0 to n-1 rows
    // dim[i] is the truth value or element 1 in row of -1 -1 -1 1(ith) -1

    // example follows
    int i, j, k;
    int speeds;
    float **test_array;
    int num_of_test_classifers;
    int truth_value;
    char *training_name;
    training_name = (char *) malloc(sizeof(char) * BUFF_SIZE);
//    if(access("training.txt", F_OK) == -1 || (argv[1] == "y" || argv[1] == "Y")){
    printf("if loop\n");
    if(access("/Users/amaaelantonini/Google Drive/Local/Spring_2017/EE 180DB/Real_Time_trial/testing.txt", F_OK) == -1 ){
        printf("training\n");
        collect_training_data("", "fake, main56");
        train_parameters(&classifiers, &counts, &dimen, &n);
        memset(training_name, 0, sizeof(char)*BUFF_SIZE);
        sprintf(training_name, "%s%s_%s", main_path, activities_file, train_name);
        printf("before training file\n");
        training_file(classifiers, training_name, dimen, activities, input, 0, n, ACTIVITIES);
        for(i = 0; i < ACTIVITIES; i++)
        {
            memset(training_name, 0, sizeof(char)*BUFF_SIZE);
            sprintf(training_name, "%s%s_%s", main_path, train_speeds[i], train_name);
            speeds = speeds_end[i] - speeds_start[i];
            training_file(classifiers, training_name, dimen, speeds_outputs, input, speeds_start[i], speeds_end[i], speeds);
        }
        //train neural_network
        train_network(train_name);
        for(i = 0; i < n; i++){
            for(j = 0; j < dimen[i]; j++){
                free(classifiers[i][j]);
            }
            free(classifiers[i]);
            free(counts[i]);
        }
        free(classifiers);
        free(dimen);
    }

//    test_data(test_name2);
//     Check if file exists; -1 means file does not exists
    int loops;
    int check;
    check = 0;
    loops = 0;
    int l = 0;
    const char *test_file_name;
    test_file_name = (char*)malloc(sizeof(char)*BUFF_SIZE);
    //memset(test_file_name, 0, sizeof(char)*BUFF_SIZE);


//    while (loops < CYCLES && run_flag){
    while (loops < FILES ) {
        memset(fake_test, 0, sizeof(char)*BUFF_SIZE);
        sprintf(fake_test, "%s%s%s_%s.csv", main_path, paths[0], names[l++], user_names[0]);


        sprintf(test_file_name, "%s%s_%d.csv", main_path, test_name, loops);
        // delete next line
        test_data(main_path, test_name);

        if(access(fake_test, F_OK) == -1){
            // File does not exist
            sleep(1);
            printf("%s, sleeping\n", fake_test);
        }

        else {
            // File exists, so populate our data structure

            truth_value = test_classifiers(&test_array, fake_test, &num_of_test_classifers);
            printf("%s\t", test_file_name);
            check = remove(test_file_name);
            printf("%d\n", check);
            test_neural_network(test_array, num_of_test_classifers);
            for(i = 0; i < num_of_test_classifers; i++)
                free(test_array[i]);
            free(test_array);
        }
        loops++;
    }


}