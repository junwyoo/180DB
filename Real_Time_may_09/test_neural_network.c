#include <unistd.h>
#include <mraa/aio.h>
#include <stdio.h>
#include <string.h>
#include "floatfann.h"
#include "fann.h"


//#define NUM_CLASSIFIERS 27
//#define NUM_MOTIONS 4

void test_neural_network(float **class_v, int n)
{	
    int i, j, motion;
    float max;					// comparison for results from fann_run
    fann_type *calc_out;			// will hold result for fann_run
    fann_type input[NUM_CLASSIFIERS];		// for our classifiers
    struct fann *ann;				// what we will use to test our neural network

    /////////////////////////////////////////////
    // AMAEEL READ
//    float **class_v;				// holds our classifier values
//    int n;					// number of rows of classifiers
    float *final;				// for converging over a complete sample (10 seconds)

    final = (float *) malloc(NUM_CLASSIFIERS * sizeof(float));
    memset(final, 0, NUM_CLASSIFIERS);
    
	    
	    // Check if file exists; -1 means file does not exists
    //while (access(ifile_name, F_OK) == -1) {
	// File does not exist; wait 5 seconds
	//sleep(5);
    //} else {
	// File exists, so populate our data structure
	//populate_neural_node(&n_node, ifile_name);
    //}


    ann = fann_create_from_file("TRAINING.net");
    printf("Entering for loop"); 
    
    // Go through all n rows for class_v
    for (i=0; i < n; i++) {
	max = -100;
        
	// Populate our input array in preparation for neural network
	for (j=0; j < NUM_CLASSIFIERS; j++){
		input[j] = class_v[i];
	}

	// calc_out returns an array of outputs, the number of whichbeing equal to number of neurons
	// in output layer
        calc_out = fann_run(ann, input);

	for (j = 0; j < NUM_CLASSIFIERS; j++){
		final[j] += calc_out[j];
	}

	/* Check for each motion */
        for (j = 0; j < NUM_MOTIONS; j++) {
	
            if (final[j] > max) {
                max = final[j];
                motion = j;
            }
        }
	
	printf("For strides row %d --> motion is %d\n", i, motion);
    }

    fann_destroy(ann);
}
