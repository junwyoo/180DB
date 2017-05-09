//
// Created by Amaael Antonini on 5/5/17.
//

#ifndef REAL_TIME_TRIAL_CONSTANTS_H
#define REAL_TIME_TRIAL_CONSTANTS_H


#define FILES 12        // number of files for training
#define ACTIVITIES 5    // number of activities
// TO DO replace this next lines for new below
//#define FILES 28        // number of files for training
//#define ACTIVITIES 7    // number of activities


#define DATA 7          // number of columns in file time acceleration x, y, z, gyro x, y, z
#define FEATURES 27     // number of features to use



#define BUFF_SIZE 1024  // buffer for chars
#define STRIDES 200     // buffer for strides

#define AUX 1

#define SLOTS 4         // split strides into slots
#define PRINT 100000    // buffer

#define NULL 0          // 0 char


#define WAIT 30         // time to wait before starting to train
#define RUN 20          // time collecting training data each file
#define NORM 1000000.0f // normalizer for data collection
#define IDLE 40         // idle time collecting data

#define TEST_SIZE 10
#define MAX_FILES 1000

#define NUM_CLASSIFIERS 27
#define NUM_MOTIONS 4

extern const int who;

extern const char * main_path;

extern const char * names[];

extern const char * activities_file;

extern const char * train_speeds[];
extern const char *train_name;
extern const char * test_name;



extern const int activities[];
extern const int speeds_outputs[];
extern const int speeds_start[];
extern const int speeds_end[];

extern const char * user_names[];
extern const char * main_path;
extern const char * class_files[];

extern const char * paths[];
extern const char * paths_out[];
extern const char * stride_path;
extern const char * train_file;
extern const char * test_dir;
extern const int train_times[];

#endif //REAL_TIME_TRIAL_CONSTANTS_H
