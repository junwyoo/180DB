#ifndef PROMPT_COLLECT_H_ 
#define PROMPT_COLLECT_H_

#define ACTIVITIES 38
#define HOLD_TIME 10
#define BUFF_SIZE 1024
#define FILES 38
#define NORM 1000000.0f

int checkCSV(char *username, const char *activity_name, int record_time);
void gather_data(char *file_name, int hold_time, int record_time);

extern const char * activity_names[];
extern const int record_time[]; 

#endif
