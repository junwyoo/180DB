#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include "prompt_collect.h"

int checkCSV(char *username, const char *activity_name, int record_time)
{   

  	char *file_name = (char*) malloc(sizeof(char)*BUFF_SIZE);
  	memset(file_name, 0, sizeof(char)*BUFF_SIZE);
 
	// CSV file format:
	// Train_Data_Set_<name>/<activity name>_<name>.csv 

	sprintf(file_name, "Train_Data_Set_%s/%s_%s.csv", username, activity_name, username);
			
	if(access(file_name, F_OK) == -1){
		printf("%s file not found, will be created in data gathering process.\n", file_name);    
		
		printf("Beginning data gathering for %s...\n", file_name);
		gather_data(file_name, HOLD_TIME, record_time);
		
	}

	return 0;
}
