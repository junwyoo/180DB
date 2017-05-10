#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFF_SIZE 100

static volatile int run_flag = 1;

void int_handler(int sig)
{
	run_flag = 0;
}

int main(int argc, char **argv)
{
	if(argc != 2)
	{
		fprintf(stderr,"Error: incorrect number of arguments\n");
		exit(1);
	}

	char *username = argv[1];
	char *filename = malloc(sizeof(char*)*BUFF_SIZE);
	

	signal(SIGINT, int_handler);	
	
	printf("Testing with name (%s)\n",username);

	while(run_flag){
	sprintf(filename, "test_%s_%ld.csv", username, time(NULL));
	printf("\tTest file collection will begin.\n");

	printf("Opened file %s\n", filename);

	//gather_data(filename,10,15); //this is for real testing
	FILE *fp = fopen(filename,"w"); //this is for dummy testing
	if(fp!=NULL)
	{
		fprintf(fp,"filename: %s\n", filename);
	}
	fclose(fp);

	printf("Testing file %s has been created.\n",filename);
	sleep(1);
        }
	return 0;
}
