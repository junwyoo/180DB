#include "fann.h"

/* Note that output files must have a .net suffix, e.g. "TRAINING.net" */
void train_network(const char *training_file, char *output_file, int inputs, int outputs)
{
    const unsigned int num_input = inputs;
    const unsigned int num_output = outputs;
    const unsigned int num_layers = 3;
    const unsigned int num_neurons_hidden = 9;
    const float desired_error = (const float) 0.01;
    const unsigned int max_epochs = 5000;
    const unsigned int epochs_between_reports = 100;

    struct fann *ann = fann_create_standard(num_layers, num_input,
        num_neurons_hidden, num_output);

    fann_set_activation_function_hidden(ann, FANN_SIGMOID_SYMMETRIC);
    fann_set_activation_function_output(ann, FANN_SIGMOID_SYMMETRIC);

    fann_train_on_file(ann, training_file, max_epochs,
        epochs_between_reports, desired_error);

    fann_save(ann, output_file);

    fann_destroy(ann);

    return 0;
}
