#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFF_SIZE 100
 
static volatile int run_flag = 1;

void sig_handler(int sig)
{
	run_flag = 0;
}

int main(int argc, char** argv)
{
	signal(SIGINT, sig_handler);
	
	if(argc != 2)
	{
		printf("Error: incorrect number of arguments\n");
		exit(1);
	}

	char *username = argv[1];
	char *cmd = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *dir = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *lineptr = (char*)malloc(sizeof(char)*BUFF_SIZE);
	size_t len = 0;	
	FILE *fp;

	sprintf(dir,"%s_trash",username);	
	mkdir(dir, 0777);
	while(run_flag)
	{
		sprintf(cmd,"ls test_%s_*.csv > filename_list.txt", username);
		system(cmd);
		printf("Generated filename_list.txt\n");
		sleep(1);
	
		fp = fopen("filename_list.txt", "r");
		if(fp==NULL)
			fprintf(stderr,"ERROR: Failed opening\n");
		else{
			printf("Successfully opened.\n");
			fprintf(stdout, "\tReading file contents:\n"); 
			while(getline(&lineptr,&len,fp) != -1)
			{
			  fprintf(stdout, "%s",lineptr);
			  lineptr[strlen(lineptr)-1] = '\0';
			  sprintf(cmd,"mv %s %s/%s",lineptr,dir,lineptr);
			  system(cmd);	  
			}
			sleep(2);
			}
		fclose(fp);	
	}		
	exit(0);
}
