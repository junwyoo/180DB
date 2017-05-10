#ifndef PROMPT_COLLECT_H_
#define PROMPT_COLLECT_H_

#define BUFF_SIZE 1024
#define FILES 28

/*
const char * activity_names[];
const int hold_time[FILES];
const int record_time[FILES];
*/
//check CSV files and collect data according to user's decision
//returns 1 if all the CSV files are there.
//returns -1 if one or more CSV files are missing.

int checkCSV(char *username); 

#endif // PROMPT_COLLECT_H_
