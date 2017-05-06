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

int main()
{
	int i=0;
	char *filename = malloc(sizeof(char*)*BUFF_SIZE);
	
	signal(SIGINT, int_handler);	

	while(run_flag){
	sprintf(filename, "test_%ld.csv", time(NULL));
	printf("\tWriting %d.\n", i);
	FILE* fp = fopen(filename, "w");
	printf("Opened file %s\n", filename);
	fprintf(fp, "%d", i);
	i++;
	sleep(1);
        }
}



