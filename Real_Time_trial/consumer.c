#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"



void consumer(char * name) {

    char *fake_test;
    fake_test = (char *) malloc(sizeof(char) * BUFF_SIZE);
    const char *test_file_name;
    test_file_name = (char*)malloc(sizeof(char)*BUFF_SIZE);
    int truth_value;


    int i;
    float **test_array;
    int num_of_test_classifers;

    // consumer code
    int loops;
    int check;
    check = 0;
    loops = 0;
    int l = 0;
    i = 0;

    //memset(test_file_name, 0, sizeof(char)*BUFF_SIZE);

//    while (loops < CYCLES && run_flag){
    while (loops < FILES ) {
        memset(fake_test, 0, sizeof(char)*BUFF_SIZE);
        sprintf(fake_test, "%s%s%s_%s.csv", main_path, paths[0], names[l++], user_names[0]);

        sprintf(test_file_name,"%s%s/%s_%d.csv", main_path, test_dir, test_name, loops);
        printf(test_file_name);
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