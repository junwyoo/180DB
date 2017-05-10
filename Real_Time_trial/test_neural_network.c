#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include <string.h>
#include "floatfann.h"
#include "fann.h"
#include "constants.h"


void test_neural_network(float **class_v, int num_rows_classifiers, char* name){
    int i, j, motion, speed;
    float max;
    
    //names of files
    char* TRAINING = (char*)malloc(sizeof(char)*1024);
    char* WALKING = (char*)malloc(sizeof(char)*1024);
    char* RUNNING = (char*)malloc(sizeof(char)*1024);
    char* JUMPING = (char*)malloc(sizeof(char)*1024);
    char* STAIR_ASCENT = (char*)malloc(sizeof(char)*1024);
    char* STAIR_DESCENT = (char*)malloc(sizeof(char)*1024);

    sprintf(TRAINING,"TRAINING_%s.net", name);
    sprintf(WALKING,"WALKING_%s.net", name);
    sprintf(RUNNING,"RUNNING_%s.net", name);
    sprintf(JUMPING,"JUMPING_%s.net", name);
    sprintf(STAIR_ASCENT,"STAIR_ASCENT_%s.net", name);
    sprintf(STAIR_DESCENT,"STAIR_DESCENT_%s.net", name);    
    
    // calc_out returns an array of outputs, with the number of outputs being equal to number 
    // of neurons in output layer
    fann_type *calc_out_motion, *calc_out_speed;
    fann_type input[NUM_CLASSIFIERS];
    
    // Our artificial neural networks for motions and speeds
    struct fann *motion_ann, 
                *walking_ann, 
                *running_ann, 
                *stair_ascent_ann, 
                *stair_descent_ann, 
                *jumping_ann;

    // Will help determine motion
    motion_ann = fann_create_from_file(TRAINING);

    // These will help determine speed corresponding to a specific motion
    walking_ann         = fann_create_from_file(WALKING);
    running_ann         = fann_create_from_file(RUNNING);
    stair_ascent_ann    = fann_create_from_file(STAIR_ASCENT);
    stair_descent_ann   = fann_create_from_file(STAIR_DESCENT);
    jumping_ann         = fann_create_from_file(JUMPING);

    // Go through all n rows for class_v
    for (i=0; i < num_rows_classifiers; i++) {
        max = -100;

        // Populate our input array in preparation for neural network
        for (j=0; j < NUM_CLASSIFIERS; j++){
            input[j] = class_v[i];
        }

        calc_out_motion = fann_run(motion_ann, input);

        /* Check for each motion */
        for (j = 0; j < NUM_MOTIONS; j++) {

            if (calc_out_motion[j] > max) {
                max = calc_out_motion[j];
                motion = j;
            }
        }

        /* Now that motion is determined, find speed */
        switch(motion){
            case 1:
                calc_out_speed = fann_run(walking_ann, input);
                break;
            case 2:
                calc_out_speed = fann_run(running_ann, input);
                break;
            case 3:
                calc_out_speed = fann_run(stair_ascent_ann, input);
                break;
            case 4:
                calc_out_speed = fann_run(stair_descent_ann, input);
                break;
            case 5:
                calc_out_speed = fann_run(jumping_ann, input);
                break;
            default:
                printf("Incorrect motion value, cannot be classified.");
        }

        max = -100;

        /* NOTE: Make sure the value should be NUM_SPEEDS here. */
        for (j = 0; j < NUM_SPEEDS; j++){

            if (calc_out_speed[j] > max) {
                max = calc_out_speed[j];
                speed = j;
            }
        }
    
        printf("Motion : %d --> Speed is %d\n", motion, speed);
    }

    
    fann_destroy(motion_ann);
    fann_destroy(walking_ann);
    fann_destroy(running_ann);
    fann_destroy(stair_ascent_ann);
    fann_destroy(stair_descent_ann);
    fann_destroy(jumping_ann);
}

