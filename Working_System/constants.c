//
// Created by Amaael Antonini on 5/5/17.
//
#include "constants.h"
const char * main_path = "../";
const char * paths[] = {"Data/Train_Data_Set_AA/", "Data/Train_Data_Set_AA/"};
//const char * paths[] = {"Data_sets/AA2_data/", "Data_sets/DA2_data/"};
const char * user_names[] = {"AA2", "DA2"};
// output file names for parameters, currently not in use
const char * class_files[] = {"Classifiers/AA_file/", "Classifiers/DA_file/"};
const char * paths_out[] = {"Data_sets_out/AA2_data/", "Data_sets_out/DA2_data/"};
const char * stride_path = "Data_strides_out/";

// name for training and testing files, will be changed
const char *train_name = "training.txt";
const char * turning_name = "turning_file";
const char * test_name = "test";
const char * activities_file = "activities";
const int who = 0;

const char * train_file = "Train_Data_Set";
const char * test_dir = "Test_Dir";


//const char * train_speeds[] = {"walk", "run", "stairs_up", "stairs_down", "jump"};
//
//const char * names[] = {
//        "walk_speed_1_50sec_32m",
//        "walk_speed_2_35sec_32m",
//        "walk_speed_3_25sec_32m",
//        "walk_speed_4_15sec_32m",
//        "slow_run",
//        "medium_run",
//        "fast_run",
//        "stairs_up",
//        "stairs_down",
//        "low_jump",
//        "medium_jump",
//        "high_jump"
//};
//
//const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 2, 3, 4, 4, 4};
//const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 0, 0, 0, 1, 2};
//const int speeds_start[]  =     {0, 4, 7, 8, 9};
//const int speeds_end[] =        {4, 7, 8, 9, 12};

const char *turn_output[] = {"", "turn right", "turn left"};
const char *speed_output[] = {"slow      ", "medium slow", "medium fast", "fast      "};
const char * train_speeds[] = {"walk", "run", "stairs_up", "stairs_down", "jump", "turn_right", "turn_left"};
const char * activities_output[] = {"walk      ", "run       ", "stairs up ", "stairs down", "jump      "};
const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
const int speeds_start[]  =     {0, 4,  8, 12, 16, 20, 24};
const int speeds_end[] =        {4, 8, 12, 16, 20, 24, 28};
const int train_times[] =       {15, 15, 10, 10, 15, 15, 10, 10, 15, 15, 10, 10, 15, 15, 10, 10,
                                 15, 15, 10, 10, 15, 15, 10, 10, 15, 15, 10, 10};
const int turns[] = {0, 1, 2, 3, 20, 21, 22, 23, 24, 25, 26, 27};

const char * names[] = {
        "walk_speed_1_50sec_32m",
        "walk_speed_2_35sec_32m",
        "walk_speed_3_25sec_32m",
        "walk_speed_4_15sec_32m",
        "slow_run",
        "medium_slow_run",
        "medium_fast_run",
        "fast_run",
        "slow_stairs_up",
        "medium_slow_stairs_up",
        "medium_fast_stairs_up",
        "fast_stairs_up",
        "slow_stairs_down",
        "medium_slow_stairs_down",
        "medium_fast_stairs_down",
        "fast_stairs_down",
        "low_jump",
        "medium_low_jump",
        "medium_high_jump",
        "high_jump",
        "turning_right_speed_1",
        "turning_right_speed_2",
        "turning_right_speed_3",
        "turning_right_speed_4",
        "turning_left_speed_1",
        "turning_left_speed_2",
        "turning_left_speed_3",
        "turning_left_speed_4"
};
