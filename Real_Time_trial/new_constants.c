//
// Created by Amaael Antonini on 5/6/17.
//

#include "new_constants.h"
const char * main_path = "/Users/amaaelantonini/Google Drive/Local/Spring_2017/EE 180DB/Real_time_trial/";
const char * paths[] = {"Data_sets/AA2_data/", "Data_sets/DA2_data/"};
const char * user_names[] = {"AA2", "DA2"};

const char * class_files[] = {"Classifiers/AA_file/", "Classifiers/DA_file/"};
const char * paths_out[] = {"Data_sets_out/AA2_data/", "Data_sets_out/DA2_data/"};
const char * stride_path = "Data_strides_out/";

const char *train_name = "testing.txt";
const char * test_name = "test";
const char * activities_file = "activities";


const char * train_speeds[] = {"walk", "run", "stairs_up", "stairs_down", "jump", "turn_right", "turn_left"};

const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 6, 6, 6, 6};
const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3, 0, 1, 2, 3};
const int speeds_start[]  =     {0, 4,  8, 12, 16, 20, 24};
const int speeds_end[] =        {4, 8, 12, 16, 20, 24, 28};

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
        "turning_speed_1",
        "turning_speed_2",
        "turning_speed_3",
        "turning_speed_4"
};
