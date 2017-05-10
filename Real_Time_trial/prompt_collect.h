#ifndef PROMPT_COLLECT_H_
#define PROMPT_COLLECT_H_

/*
#define BUFF_SIZE 1024
#define FILES 28

<<<<<<< HEAD
<<<<<<< HEAD
=======
>>>>>>> parent of 7f5b522... fixed makefile
const char * activity_names[] = { // changed from "names"
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
        "turning_left_speed_4" //28 csv files
};

const int hold_time[FILES] = {
  10,//"walk_speed_1_50sec_32m",
  10,//"walk_speed_2_35sec_32m",
  10,//"walk_speed_3_25sec_32m",
  10,//"walk_speed_4_15sec_32m",
  10,//"slow_run",
  10,//"medium_slow_run",
  10,//"medium_fast_run",
  10,//"fast_run",
  10,//"slow_stairs_up",
  10,//"medium_slow_stairs_up",
  10,//"medium_fast_stairs_up",
  10,//"fast_stairs_up",
  10,//"slow_stairs_down",
  10,//"medium_slow_stairs_down",
  10,//"medium_fast_stairs_down",
  10,//"fast_stairs_down",
  10,//"low_jump",
  10,//"medium_low_jump",
  10,//"medium_high_jump",
  10,//"high_jump",
  10,//"turning_right_speed_1",
  10,//"turning_right_speed_2",
  10,//"turning_right_speed_3",
  10,//"turning_right_speed_4",
  10,//"turning_left_speed_1",
  10,//"turning_left_speed_2",
  10,//"turning_left_speed_3",
  10//"turning_left_speed_4"
};
const int record_time[FILES] = {
  15,//"walk_speed_1_50sec_32m",
  15,//"walk_speed_2_35sec_32m",
  10,//"walk_speed_3_25sec_32m",
  10,//"walk_speed_4_15sec_32m",

  15,//"slow_run",
  15,//"medium_slow_run",
  10,//"medium_fast_run",
  10,//"fast_run",
  
  15,//"slow_stairs_up",
  15,//"medium_slow_stairs_up",
  10,//"medium_fast_stairs_up",
  10,//"fast_stairs_up",
  15,//"slow_stairs_down",
  15,//"medium_slow_stairs_down",
  10,//"medium_fast_stairs_down",
  10,//"fast_stairs_down",

  15,//"low_jump",
  15,//"medium_low_jump",
  15,//"medium_high_jump",
  15,//"high_jump",

  15,//"turning_right_speed_1",
  15,//"turning_right_speed_2",
  10,//"turning_right_speed_3",
  10,//"turning_right_speed_4",

  15,//"turning_left_speed_1",
  15,//"turning_left_speed_2",
  10,//"turning_left_speed_3",
  10//"turning_left_speed_4"
};
<<<<<<< HEAD
*/

=======
/*
const char * activity_names[];
const int hold_time[FILES];
const int record_time[FILES];
*/
>>>>>>> 7f5b522a382a4aa6663da45f7628f2b81f257614
=======


>>>>>>> parent of 7f5b522... fixed makefile
//check CSV files and collect data according to user's decision
//returns 1 if all the CSV files are there.
//returns -1 if one or more CSV files are missing.

int checkCSV(char *username); 

#endif // PROMPT_COLLECT_H_
