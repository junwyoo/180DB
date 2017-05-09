#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFF_SIZE 100
 
static volatile int run_flag = 1;

void sig_handler(int sig)
{
	run_flag = 0;
}

int main()
{
	signal(SIGINT, sig_handler);

	while(run_flag){	
	system("ls test_*.csv > filename_list.txt");
	printf("Generated filename_list.txt\n");
	sleep(1);
	
	FILE* fp = fopen("filename_list.txt", "r");
	if(fp==NULL)
		fprintf(stderr,"ERROR: Failed opening\n");
	else{
		printf("Successfully opened.\n");
		char *buf = malloc(sizeof(char*)*BUFF_SIZE);
		fscanf(fp, "%s", buf);
		fprintf(stdout, "\tRead file contents:%s\n",buf);
		sleep(2);
		}
	fclose(fp);	
	}		
	exit(0);
}