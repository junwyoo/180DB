//
// Created by Amaael Antonini on 3/22/17.
//

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <math.h>

#ifndef FILE_EXAMPLE_CLASSIFIERS_H
#define FILE_EXAMPLE_CLASSIFIERS_H

void fake_data(const char **names);

void collect_training_data();

void train_network(const char * train_name);

void test_neural_network(float **class_v, int n);

void test_data(const char *test_name);

int test_classifiers(float ***classifiers, const char * name, int * n);

void training_file(float ***classifiers, const char * file_name, int *row_counts, const int *activities, int inputs, int s, int n, int a);


int temp_strides(float **data, int *out, float *sigma_y, float *sigma_z, float max_accel,
                 float max_gyro, float max_time, float min_time, int n);

void train_parameters(float ****classifiers, int *** counts, int **dimensions, int * n);

int select_strides(float * times, int * strides, int * out, float min_time, int n);

void sample_mean(float * data, float * out, int chunk, int n);


void sample_sigma(float * data, float * mean, float * out, int chunk, int n);

void sample_sigma_2(float * data, float * mean, float ** out, int chunk, int n);

void center_data(float * data, float * mean, float * sigma, int chunk, int n);

int peak_strides(float * time, float * data, int * strides, int *out, float *sigma, float weight, int sn,  int n);

void move_back(int * strides, int offset, int n);

void sigma_stride(float * data, int ** stride, float * level, float * sigma_low, float * sigma_high, int n);
//void center_data(float * data, float * center, int n);

void peaks(float * data, int ** strides, float ** max_peak, float **max_trough, int n);

void mean_slot(float * data, int ** strides, float * level, float ** mean_low, float ** mean_high, float slots, int n);

int outlier(float * data, int * strides, int * out, float min_diff, int n);


#endif //FILE_EXAMPLE_CLASSIFIERS_H
