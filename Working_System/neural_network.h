#ifndef NEURAL_NETWORK_H_ 
#define NEURAL_NETWORK_H_

/* Trains neural network by taking in a training file as an input. */
void train_network( char *training_file, char* output_file, int inputs, int outputs);

/* Tests neural network, takes in:
	- Classifier values
	- Number of rows of classifiers
	- Directory containing all the .net files. NOTE: Name MUST end with a '/', e.g. "home/directory/", NOT
	  "home/directory"
	- Name of person who we're personalizing the data for
 */
void test_neural_network(float **class_v, float **turning, int num_rows_classifiers, char* net_files_directory, char* name);

#endif // NEURAL_NETWORK_H_
