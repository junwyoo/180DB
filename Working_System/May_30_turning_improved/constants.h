//
// Created by Amaael Antonini on 5/5/17.
//

#ifndef REAL_TIME_TRIAL_CONSTANTS_H
#define REAL_TIME_TRIAL_CONSTANTS_H


#define FILES 38       // number of files for training
#define TURNING 38
#define NO_TURN 20
#define TURN_RIGHT 9
#define TURN_LEFT 9
#define TURN_FEATURES 37 // 24 -> 8
//#define NUM_CLASSIFIERS 77
// Num classifiers = 27 + 12 * slots
#define NUM_MOTIONS 5
#define NUM_SPEEDS 4
#define NUM_TURNS 3
#define ACTIVITIES 7    // number of activities
// TO DO replace this next lines for new below
//#define FILES 28        // number of files for training
//#define ACTIVITIES 7    // number of activities
#define MAX_DATA 20000


#define DATA 7         // number of columns in file time acceleration x, y, z, gyro x, y, z
#define FEATURES 75     // number of features to use



#define BUFF_SIZE 1024  // buffer for chars
#define STRIDES 200     // buffer for strides

#define AUX 1

#define SLOTS 8        // split strides into slots
#define PRINT 100000    // buffer

#define NULL 0          // 0 char


#define WAIT 30         // time to wait before starting to train
#define RUN 20          // time collecting training data each file
#define NORM 1000000.0f // normalizer for data collection
#define IDLE 40         // idle time collecting data

#define TEST_SIZE 10
#define MAX_FILES 1000

extern const int who;
extern const char *turn_output[];
extern const char * main_path;

extern const char * names[];

extern const char * activities_file;

extern const char * train_speeds[];
extern const char *train_name;
extern const char * test_name;

extern const char * speed_output[];
extern const char * activities_output[];

extern const char *standard_speeds[];
extern const char *walk_speeds[];
extern const char *two_staires_speeds[];


extern const int no_turn[];
extern const int turn_right[];
extern const int turn_left[];
extern const int turn_out[];
extern const int activities[];

extern const int speeds_outputs[];
extern const int speeds_start[];
extern const int speeds_end[];
extern const int total_speeds[];
extern const int turns[];

extern const char * user_names[];
extern const char * main_path;
extern const char * class_files[];

extern const char * paths[];
extern const char * paths_out[];
extern const char * stride_path;
extern const char * train_file;
extern const char * test_dir;
extern const char * turning_name;
extern const int train_times[];


//#define FILES 28       // number of files for training
//#define TURNING 12
//#define NO_TURN 20
//#define TURN_FEATURES 8
//#define NUM_CLASSIFIERS 27
//#define NUM_MOTIONS 5
//#define NUM_SPEEDS 4
//#define NUM_TURNS 3
//#define ACTIVITIES 5    // number of activities
//// TO DO replace this next lines for new below
////#define FILES 28        // number of files for training
////#define ACTIVITIES 7    // number of activities
//#define MAX_DATA 20000
//
//
//#define DATA 7         // number of columns in file time acceleration x, y, z, gyro x, y, z
//#define FEATURES 39     // number of features to use
//
//
//
//#define BUFF_SIZE 1024  // buffer for chars
//#define STRIDES 200     // buffer for strides
//
//#define AUX 1
//
//#define SLOTS 4         // split strides into slots
//#define PRINT 100000    // buffer
//
//#define NULL 0          // 0 char
//
//
//#define WAIT 30         // time to wait before starting to train
//#define RUN 20          // time collecting training data each file
//#define NORM 1000000.0f // normalizer for data collection
//#define IDLE 40         // idle time collecting data
//
//#define TEST_SIZE 10
//#define MAX_FILES 1000
//
//extern const int who;
//extern const char *turn_output[];
//extern const char * main_path;
//
//extern const char * names[];
//
//extern const char * activities_file;
//
//extern const char * train_speeds[];
//extern const char *train_name;
//extern const char * test_name;
//
//extern const char * speed_output[];
//extern const char * activities_output[];
//extern const int activities[];
//extern const int speeds_outputs[];
//extern const int speeds_start[];
//extern const int speeds_end[];
//extern const int turns[];
//
//extern const char * user_names[];
//extern const char * main_path;
//extern const char * class_files[];
//
//extern const char * paths[];
//extern const char * paths_out[];
//extern const char * stride_path;
//extern const char * train_file;
//extern const char * test_dir;
//extern const char * turning_name;
//extern const int train_times[];

#endif //REAL_TIME_TRIAL_CONSTANTS_H




