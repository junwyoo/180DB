#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "prompt_collect.h"

/*
Process:
1) Establish server port number
2) Connect with clients using server port number
	- Once all connections are established, verify you're connected with the right clients
3) Once verification is complete, have server send signal to clients to start data collection process
4) Ensure processes are synchronized 
*/

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
	exit(1);
}

void relay_skip_activity(char* buffer, int *client_socket_fd, int num_clients){

	int j;
	int write_results[num_clients];
	memset(write_results, 0, sizeof(int)*num_clients);

	memset(buffer, 0, 256);
	sprintf(buffer, "n");

	for (j = 0; j < num_clients; j++){
		write_results[j] = write(client_socket_fd[j], buffer, strlen(buffer));
			
		if (write_results[j] <= 0){
			printf("Error writing skip recording message to client socket %d. Attempting to resend.\n", client_socket_fd[j]);
			while (write_results[j] <= 0){
				write_results[j] = write(client_socket_fd[j], buffer, strlen(buffer));
			}
		}

		printf("Successfuly wrote skip recording message to client %d with socket %d.\n", j, client_socket_fd[j]);
	}
}

int main(int argc, char *argv[])
{
	// error check command line arguments
	if (argc != 4) {
		fprintf(stderr,"Usage: ./server <PORT_NUMBER> <NUM_CLIENTS> <USERNAME>\n");
		exit(1);
	}

	int server_socket_fd, portno;
	socklen_t clilen;
	char buffer[256];
	struct sockaddr_in serv_addr, cli_addr;
	int n;
	
	int i = 0, j = 0;
	int accepted_sockets = 0;	// We want this to be equal to NUM_CLIENTS
	int completed_activities = 0;
	char input;

	int NUM_CLIENTS = atoi(argv[2]);
	int client_socket_fd[NUM_CLIENTS];
	memset(client_socket_fd, 0, sizeof(int)*NUM_CLIENTS);

	// Will hold result of each read and write call to each of the clients
	int read_results[NUM_CLIENTS];
	int write_results[NUM_CLIENTS];
	memset(read_results, 0, sizeof(int)*NUM_CLIENTS);
	memset(write_results, 0, sizeof(int)*NUM_CLIENTS);

	// Create the directory to hold all our recordings of trainings
	char *directory = (char*) malloc(sizeof(char)*BUFF_SIZE);
  	memset(directory, 0, sizeof(char)*BUFF_SIZE);
  	sprintf(directory,"Train_Data_Set_%s",argv[3]);
	mkdir(directory, 0777);

	char *file_name = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *cmd = (char*) malloc(sizeof(char)*BUFF_SIZE);
  	memset(cmd, 0, sizeof(char)*BUFF_SIZE);

	// setup socket
	server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket_fd < 0) {	
		error("ERROR opening socket");
	}

	// setup server information
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	// port number must be 1000 and 8000
	portno = atoi(argv[1]);
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// bind the socket to an address
	printf("Attempting to bind server to port %d...\n", atoi(argv[1]));
	while (bind(server_socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("ERROR on binding. Attempting to bind to %d.\n", portno + 5);
		portno += 5;
		serv_addr.sin_port = htons(portno);
	}

	printf("Successfully binded server to port %d\n", portno);

	// listen for incoming connections
	// accept at most 5 connections before refusing them
	printf("Listening for incoming connections...\n");
	listen(server_socket_fd, 10);
	clilen = sizeof(cli_addr);

	// block until there is a new connection. When one is received, accept it
	while (accepted_sockets != NUM_CLIENTS){
		client_socket_fd[i] = accept(server_socket_fd, (struct sockaddr *) &cli_addr, &clilen);
		if (client_socket_fd[i] < 0){
			error("ERROR on accept");
			continue;
		} else {
			// Make sure to print something verifying connection
			accepted_sockets++;
			printf("Added client %d with socket %d.\n", accepted_sockets, client_socket_fd[i]);
			i++;
		}

	}

	printf("Successfully connected to all %d clients.\n", NUM_CLIENTS);

	while (completed_activities != ACTIVITIES){

		memset(file_name, 0, sizeof(char)*BUFF_SIZE);
		sprintf(file_name, "Train_Data_Set_%s/%s_%s.csv", argv[3], activity_names[completed_activities], argv[3]);

		printf("Train for %s? [y/n]\n", activity_names[completed_activities]);
		
		// Obtain user input to record (or not record) activity
		input = getchar();

		while (input != 'y' && input != 'n'){
			printf("Input 'y' or 'n'.\n");
			input = getchar();
		}

		// If user input n, skip current activity, and let clients know to skip activity
		if (input == 'n'){
			printf("Skipping training for %s.\n", activity_names[completed_activities]);
			completed_activities++;
			relay_skip_activity(buffer, client_socket_fd, NUM_CLIENTS);
			continue;
		}

		if(access(file_name, F_OK) == -1){
			printf("%s file not found, will be created in data gathering process.\n", file_name);
		} else {
			printf("File %s_%s already exists. Overwrite? [y/n]\n", activity_names[completed_activities], argv[3]);
		
			input = getchar();
			while (input != 'y' && input != 'n'){
				printf("Input 'y' or 'n'.\n");
				input = getchar();
			}

			if (input == 'n'){
				printf("Retaining existing version of %s_%s.\n", activity_names[completed_activities], argv[3]);
				return 0;
			} else {
				sprintf(cmd, "rm ./%s", file_name);
				system(cmd);
			}
		}

		printf("Sending signal to begin recording %s...\n", activity_names[completed_activities]);

		memset(buffer, 0, 256);
		sprintf(buffer, "%s", activity_names[completed_activities]);

		for (j = 0; j < NUM_CLIENTS; j++){
			write_results[j] = write(client_socket_fd[j], buffer, strlen(buffer));
			
			if (write_results[j] <= 0){
				printf("Error writing to client socket %d. Attempting to resend.\n", client_socket_fd[j]);
				while (write_results[j] <= 0){
					write_results[j] = write(client_socket_fd[j], buffer, strlen(buffer));
				}
			}

			printf("Successfuly wrote to client %d with socket %d.\n", j, client_socket_fd[j]);
		}

		/*
		for (i=0; i<NUM_CLIENTS; i++){
			printf("write_results[%d] = %d\n", i, write_results[i]);
		}*/

		printf("Successfully sent record signal to all clients for %s.\nWaiting for response from all clients...\n", activity_names[completed_activities]);
		
		gather_data(file_name, HOLD_TIME, record_time[completed_activities]);
		//checkCSV(argv[3], activity_names[completed_activities], record_time[completed_activities], 1);

		for (j = 0; j < NUM_CLIENTS; j++){
			
			memset(buffer, 0, 256);
			read_results[j] = read(client_socket_fd[j], buffer, 255);

			if (read_results[j] <= 0){
				//printf("Error reading from client socket %d. Attempting to re-read.\n", client_socket_fd[j]);
				j--;
				continue;
			}

			printf("Successfully read from client %d with socket %d.\n", j, client_socket_fd[j]);
		}

		printf("All clients successfully recorded activity for %s.\n", activity_names[completed_activities]);
		completed_activities++;
	}

	printf("All activities recorded. Closing sockets.");
	for (i = 0; i < NUM_CLIENTS; i++){
		close(client_socket_fd[i]);
	}

	close(server_socket_fd);
	return 0; 
}
