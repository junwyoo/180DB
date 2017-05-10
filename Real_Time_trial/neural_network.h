#ifndef NEURAL_NETWORK_H_ 
#define NEURAL_NETWORK_H_


/* 
Trains neural network by taking in a training file as an input. 

The "outputs" parameter is used to determine number of outputs of our neural network; this number
will usually be set to the either the number of motions or number of speeds belonging to a
particular motion.

The "layers" parameter contains the number of layers the neural network will hold.
*/
void train_network(const char *training_file, char* output_file, int inputs, int outputs);

/* Tests neural network, takes in:
	- Classifier values, first dimension is strides, second dimension is classifier values
	- Number of rows of classifiers
 */
void test_neural_network(float **class_v, int num_rows_classifiers, char* name)

#endif // NEURAL_NETWORK_H_
