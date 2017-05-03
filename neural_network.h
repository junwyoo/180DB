#ifndef NEURAL_NETWORK_H_ 
#define NEURAL_NETWORK_H_

/* Trains neural network by taking in a training file as an input. */
void train_network(const char *training_file);

/* Tests neural network, takes in:
	- Classifier values
	- Number of rows of classifiers
 */
void test_neural_network(float **class_v, int num_rows_classifiers);

#endif // NEURAL_NETWORK_H_