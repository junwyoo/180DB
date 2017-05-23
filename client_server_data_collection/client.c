#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "prompt_collect.h"

const char * activity_names[] = {
/* 1 */		"walk_speed_1_50sec_32m",
/* 2 */		"walk_speed_2_35sec_32m",
/* 3 */		"walk_speed_3_25sec_32m",
/* 4 */		"walk_speed_4_15sec_32m",
/* 5 */		"slow_run",
/* 6 */		"medium_run",
/* 7 */		"fast_run",
/* 8 */		"slow_stairs_up",
/* 9 */		"medium_stairs_up",
/* 10 */	"fast_stairs_up",
/* 11 */	"slow_stairs_down",
/* 12 */	"medium_stairs_down",
/* 13 */	"fast_stairs_down",
/* 14 */	"slow_two_stairs_up",
/* 15 */	"fast_two_stairs_up",
/* 16 */	"slow_two_stairs_down",
/* 17 */	"fast_two_stairs_down",
/* 18 */	"low_jump",
/* 19 */	"medium_jump",
/* 20 */	"high_jump",
/* 21 */	"walk_turn_right_1",
/* 22 */	"walk_turn_right_2",
/* 23 */	"walk_turn_right_3",
/* 24 */	"walk_turn_left_1",
/* 25 */	"walk_turn_left_2",
/* 26 */	"walk_turn_left_3",
/* 27 */	"run_turn_right_1",
/* 28 */	"run_turn_right_2",
/* 29 */	"run_turn_right_3",
/* 30 */	"run_turn_left_1",
/* 31 */	"run_turn_left_2",
/* 32 */	"run_turn_left_3",
/* 33 */	"jump_turn_right_1",
/* 34 */	"jump_turn_right_2",
/* 35 */	"jump_turn_right_3",
/* 36 */	"jump_turn_left_1",
/* 37 */	"jump_turn_left_2",
/* 38 */	"jump_turn_left_3"
};

const int record_time[] = {

		20,//"walk_speed_1_50sec_32m",
		20,//"walk_speed_2_35sec_32m",
		15,//"walk_speed_3_25sec_32m",
		10,//"walk_speed_4_15sec_32m",
		15,//"slow_run",
		15,//"medium_run",
		10,//"fast_run",
		20,//"slow_stairs_up",
		15,//"medium_stairs_up",
		10,//"fast_stairs_up",
		20,//"slow_stairs_down",
		15,//"medium_stairs_down",
		10,//"fast_stairs_down",
		15,//"slow_two_stairs_up",
		10,//"fast_two_stairs_up",
		15,//"slow_two_stairs_down",
		10,//"fast_two_stairs_down",
		15,//"low_jump",
		15,//"medium_jump",
		10,//"high_jump",
		20,//"walk_turn_right_1",
		15,//"walk_turn_right_2",
		10,//"walk_turn_right_3",
		20,//"walk_turn_left_1",
		15,//"walk_turn_left_2",
		10,//"walk_turn_left_3",
		15,//"run_turn_right_1",
		15,//"run_turn_right_2",
		10,//"run_turn_right_3",
		15,//"run_turn_left_1",
		15,//"run_turn_left_2",
		10,//"run_turn_left_3",
		15,//"jump_turn_right_1",
		15,//"jump_turn_right_2",
		10,//"jump_turn_right_3",
		15,//"jump_turn_left_1",
		15,//"jump_turn_left_2",
		10//"jump_turn_left_3"

};

void error(const char *msg)
{
	perror(msg);
	exit(0);
}

int client(char* SERVER_IP_ADDRESS, int portno, char *username){
	
	int client_socket_fd, n;
	struct sockaddr_in serv_addr;
	struct hostent *server;
	char buffer[256];
	
	int completed_activities = 0;
	int read_result = 0;
	int write_result = 0;

	// setup the socket
	client_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	// check if the socket was created successfully. If it wasnt, display an error and exit
	if(client_socket_fd < 0) {
		error("ERROR opening socket");
	}

	// check if the IP entered by the user is valid 
	server = gethostbyname(SERVER_IP_ADDRESS);
	if (server == NULL) {
		fprintf(stderr,"ERROR, no such host\n");
		exit(0);
	}

	// clear our the serv_addr buffer
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	// set up the socket 
	serv_addr.sin_family = AF_INET;
	memcpy((char *)&serv_addr.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
	
	// We sleep 2 seconds waiting for server to latch to its own port; we do this primarily for
	// automation purposes
	sleep(2);
	
	serv_addr.sin_port = htons(portno);
	printf("Attempting to connect to server port %d...\n", portno);
	while (connect(client_socket_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		printf("ERROR on binding. Attempting to bind to %d.\n", portno + 5);
		portno += 5;
		serv_addr.sin_port = htons(portno);
	}

	printf("Successfully connected to server port %d.\n", portno);

	// Create the directory to hold all our recordings of trainings
	char *directory = (char*) malloc(sizeof(char)*BUFF_SIZE);
  	memset(directory, 0, sizeof(char)*BUFF_SIZE);
  	sprintf(directory,"Train_Data_Set_%s",username);
	mkdir(directory, 0777);

	// Begin recording activites
	while (completed_activities != ACTIVITIES){
		printf("Waiting for signal to begin recording %s...\n", activity_names[completed_activities]);

		memset(buffer, 0, 256);

		read_result = read(client_socket_fd, buffer, 255);
		printf("read_result = %d\n", read_result);

		if (read_result <= 0){
			continue;
		} else {

			printf("Signal received. Beginning recording %s.\n", activity_names[completed_activities]);

			// Record data
			checkCSV(username, activity_names[completed_activities], record_time[completed_activities]);
		}

		memset(buffer, 0, 256);
		sprintf(buffer, "%s", activity_names[completed_activities]); // TODO: needs to be moved
		
		// Let server know we've finished recording	
		printf("%s recorded. Sending completetion signal to server.\n", activity_names[completed_activities]);

		write_result = write(client_socket_fd, buffer, strlen(buffer));

		if (write_result > 0)
			printf("Successfully sent completion signal to server for %s.\n", activity_names[completed_activities]);
		else {
			while (write_result <= 0){
				printf("Error writing to client_socket_fd. Attempting to resend.\n");
				write_result = write(client_socket_fd, buffer, strlen(buffer));
			}	
		} 

		completed_activities++;
	}

	// clean up the file descriptors
	close(client_socket_fd);
	return 0;
}

int main(int argc, char **argv){
	client(argv[1], atoi(argv[2]), argv[3]);
}
