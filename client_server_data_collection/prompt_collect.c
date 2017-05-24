#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>
#include "prompt_collect.h"

int checkCSV(char *username, const char *activity_name, int record_time, int server)
{   

  	char *file_name = (char*) malloc(sizeof(char)*BUFF_SIZE);
  	memset(file_name, 0, sizeof(char)*BUFF_SIZE);
	char *cmd = (char*) malloc(sizeof(char)*BUFF_SIZE);
  	memset(cmd, 0, sizeof(char)*BUFF_SIZE);
  	char input;
 
	// CSV file format:
	// Train_Data_Set_<name>/<activity name>_<name>.csv 

	sprintf(file_name, "Train_Data_Set_%s/%s_%s.csv", username, activity_name, username);
		
	// Check if file exists; record if it doesn't, prompt user for overwriting if it does exist	
	if(access(file_name, F_OK) == -1){
		printf("%s file not found, will be created in data gathering process.\n", file_name);
	} else {
		if (server == 1){
			printf("File %s_%s already exists. Overwrite? [y/n]\n", activity_name, username);
		
			input = getchar();
			while (input != 'y' && input != 'n'){
				printf("Input 'y' or 'n'.\n");
				input = getchar();
			}

			if (input == 'n'){
				printf("Retaining existing version of %s_%s.\n", activity_name, username);
				return 0;
			} else {
				sprintf(cmd, "rm ./%s", file_name);
				system(cmd);
			}
		}
	}
	
	printf("Beginning data gathering for %s...\n", file_name);
	gather_data(file_name, HOLD_TIME, record_time);
	
	free(file_name);
	free(cmd);
	return 1;
}
