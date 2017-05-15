#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include <string.h>
#include "floatfann.h"
#include "fann.h"
#include "constants.h"

//#define NUM_CLASSIFIERS 27
#define NUM_MOTIONS 5
//#define NUM_SPEEDS 4
//#define BUFF_SIZE 1024

void create_names(char *name, char ***c, char **turn){
    char **dot_net_name;
    char *dot_turn;
    int i;
    dot_net_name = (char **) malloc(sizeof(char*) * (ACTIVITIES + 1));
    dot_turn = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(dot_turn, 0, sizeof(char) * BUFF_SIZE);
    for (i = 0; i < ACTIVITIES + 1; i++) {
        dot_net_name[i] = (char *) malloc(sizeof(char) * BUFF_SIZE);

        memset(dot_net_name[i], 0, sizeof(char) * BUFF_SIZE);
    }
    sprintf(dot_net_name[0], "%s%s_%s/%s_%s.net", main_path, train_file, name, activities_file, name);
    sprintf(dot_turn, "%s%s_%s/%s_%s.net", main_path, train_file, name, turning_name, name);
    for (i = 0; i < ACTIVITIES; i++) {
       sprintf(dot_net_name[i+1], "%s%s_%s/%s_%s.net", main_path, train_file, name, train_speeds[i], name);
    }
 
    *c = dot_net_name;
}
/*
NOTE: Directory containing all the .net files. NOTE: Name MUST end with a '/', e.g. "home/directory/", NOT
"home/directory"
*/
void test_neural_network(float **class_v, float **turning, int num_rows_classifiers, char* net_files_directory, char* name){
    int i, j, motion, speed, turn;//, turn_left, turn_right;
    float max;
    
    //names of files
    char* TRAINING = (char*)malloc(sizeof(char)*1024);
    char* WALKING = (char*)malloc(sizeof(char)*1024);
    char* RUNNING = (char*)malloc(sizeof(char)*1024);
    char* JUMPING = (char*)malloc(sizeof(char)*1024);
    char* STAIR_ASCENT = (char*)malloc(sizeof(char)*1024);
    char* STAIR_DESCENT = (char*)malloc(sizeof(char)*1024);
    char* TURNING_ANN = (char*)malloc(sizeof(char)*1024);
    //char *TURNING_RIGHT = (char*)malloc(sizeof(char)*1024);

//    sprintf(TRAINING,"%sTRAINING_%s.net", net_files_directory, name);
//    sprintf(WALKING,"%sWALKING_%s.net", net_files_directory, name);
//    sprintf(RUNNING,"%sRUNNING_%s.net", net_files_directory, name);
//    sprintf(JUMPING,"%sJUMPING_%s.net", net_files_directory, name);
//    sprintf(STAIR_ASCENT,"%sSTAIR_ASCENT_%s.net", net_files_directory, name);
//    sprintf(STAIR_DESCENT,"%sSTAIR_DESCENT_%s.net", net_files_directory, name);
//    sprintf(TURNING_LEFT,"%sTURNING_LEFT_%s.net", net_files_directory, name);
//    sprintf(TURNING_RIGHT,"%sTURNING_RIGHT_%s.net", net_files_directory, name);

    sprintf(TRAINING,"%s_%s.net",activities_file, name);
    sprintf(WALKING,"%s_%s.net",train_speeds[0], name);
    sprintf(RUNNING,"%s_%s.net", train_speeds[1], name);
    sprintf(STAIR_ASCENT,"%s_%s.net", train_speeds[2], name);
    sprintf(STAIR_DESCENT,"%s_%s.net", train_speeds[3], name);
    sprintf(JUMPING,"%s_%s.net", train_speeds[4], name);

    sprintf(TURNING_ANN,"%s_%s.net", turning_name, name);
//    sprintf(TURNING_RIGHT,"%sTURNING_RIGHT_%s.net", net_files_directory, name);
    // calc_out returns an array of outputs, with the number of outputs being equal to number 
    // of neurons in output layer
    fann_type *calc_out_motion, *calc_out_speed, *calc_out_turning;//_left, *calc_out_turning_right;
    fann_type input[NUM_CLASSIFIERS];
    fann_type turn_input[TURN_FEATURES];
    
    // Our artificial neural networks for motions and speeds
    struct fann *motion_ann, 
                *walking_ann, 
                *running_ann, 
                *stair_ascent_ann, 
                *stair_descent_ann, 
                *jumping_ann,
                *turning_ann;

    // Will help determine motion
    motion_ann = fann_create_from_file(TRAINING);

    // These will help determine speed corresponding to a specific motion
    walking_ann         = fann_create_from_file(WALKING);
    running_ann         = fann_create_from_file(RUNNING);
    stair_ascent_ann    = fann_create_from_file(STAIR_ASCENT);
    stair_descent_ann   = fann_create_from_file(STAIR_DESCENT);
    jumping_ann         = fann_create_from_file(JUMPING);
    turning_ann    = fann_create_from_file(TURNING_ANN);
    //turning_right_ann   = fann_create_from_file(TURNING_RIGHT);

    // Go through all n rows for class_v
    for (i=0; i < num_rows_classifiers; i++) {
        max = -100;

        // Populate our input array in preparation for neural network
        for (j=0; j < NUM_CLASSIFIERS; j++){
            input[j] = class_v[i][j];
        }
        for(j = 0; j < TURN_FEATURES; j++)
        {
            turn_input[j] = turning[i][j];

        }

        calc_out_motion = fann_run(motion_ann, input);
	//calc_out_turning = fann_run(turning_ann, input);
        motion = -1;
        /* Check for each motion */
        for (j = 0; j < NUM_MOTIONS; j++) {

            if (calc_out_motion[j] > max) {
                max = calc_out_motion[j];
                motion = j;
            }
        }
	if(max < -1.1){

	printf("max: %f\tmotion: %s\n", max, train_speeds[motion]);
	}
        speed = -1.0;
        /* Now that motion is determined, find speed */
        switch(motion){
            case 0:
                calc_out_speed = fann_run(walking_ann, input);
                //calc_out_turning = fann_run(turning_ann, turn_input);
               // calc_out_turning_right = fann_run(turning_right_ann, input);
                break;
            case 1:
                calc_out_speed = fann_run(running_ann, input);
                //calc_out_turning = fann_run(turning_ann, turn_input);
//                calc_out_turning_right = fann_run(turning_right_ann, input);
                break;
            case 2:
                calc_out_speed = fann_run(stair_ascent_ann, input);
                break;
            case 3:
                calc_out_speed = fann_run(stair_descent_ann, input);
                break;
            case 4:
                calc_out_speed = fann_run(jumping_ann, input);
                break;
	    default:
		break;
	}
	calc_out_turning = fann_run(turning_ann, turn_input);

        /* Classify our form of motion */
        max = -100;
        for (j = 0; j < NUM_SPEEDS; j++){

            if (calc_out_speed[j] > max) {
                max = calc_out_speed[j];
                speed = j;
            }
        }

        /* If we are walking or running, check if we are turning also */
       //if (motion == 0 || motion == 1){
            max = -100;
            for (j = 0; j < NUM_TURNS; j++){
                // check calc_out for left and right turns, return the maximum
                if (calc_out_turning[j] > max) {
                    max = calc_out_turning[j];
                    turn = j;
                }  
            }
	   if(max < -1.1)
		    printf("turn: %f\n", max);
//            /*if (turn_right > turn_left)
//                turn = turn_right;
//            else
//                turn = turn_left;*/
    if(motion < 0 || speed < 0)
        continue;

    printf("Motion: %s --> Speed: %s\t%s\n", activities_output[motion], speed_output[speed], turn_output[turn]);
       
       //	} else     
           // printf("Motion : %d --> Speed : %d\n", motion, speed);
    }

    
    fann_destroy(motion_ann);
    fann_destroy(walking_ann);
    fann_destroy(running_ann);
    fann_destroy(stair_ascent_ann);
    fann_destroy(stair_descent_ann);
    fann_destroy(jumping_ann);
    fann_destroy(turning_ann);
}

