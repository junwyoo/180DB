#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h> 
#include "server_client.h"

// Make sure that write_message is a 256 bytes
int client_send_write(char *write_message, int client_socket_fd){
    
	char buffer[256];
	memset(buffer, 0, sizeof(char)*256);
	sprintf(buffer, "%s", write_message);

    int write_result = write(client_socket_fd, buffer, strlen(buffer));

	if (write_result > 0)
		return write_result;
	else {
		while (write_result <= 0){
			printf("Error writing to client_socket_fd. Attempting to resend.\n");
			write_result = write(client_socket_fd, buffer, strlen(buffer));
		}	
	}

	return 0;
}

// Connects client to server IP address and port, returns the client socket file descriptor
int client_connect(char *SERVER_IP_ADDRESS){
	
	int client_socket_fd, n;
	int portno = 1000;

	struct sockaddr_in serv_addr;
	struct hostent *server;
	
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
	
	serv_addr.sin_port = htons(portno);
	printf("Attempting to connect to server port %d...\n", portno);
	while (connect(client_socket_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
		printf("ERROR on binding. Attempting to bind to %d.\n", portno + 5);
		portno += 5;
		serv_addr.sin_port = htons(portno);
	}

	printf("Successfully connected to server port %d.\n", portno);

	return client_socket_fd;
}

// Runs client-side training protocol, in synchronization with server
int client_training(int client_socket_fd, char *SERVER_IP_ADDRESS, char **activities, int num_activities, char *username){
	
	int completed_activities = 0;
	int read_result = 0;
	int write_result = 0;

	char *file_name = (char *) malloc(sizeof(char) * 256);
	char *buffer = (char *) malloc(sizeof(char) * 256);
	char *cmd = (char*) malloc(sizeof(char) * 256);

	while (completed_activities != num_activities){
		printf("Waiting for signal to begin recording %s...\n", activities[completed_activities]);

		memset(file_name, 0, sizeof(char) * 256);
		sprintf(file_name, "%s_%s/%s_%s.csv", client_training_directory, username, activities[completed_activities], username);
		
		memset(buffer, 0, 256);

		read_result = read(client_socket_fd, buffer, 255);
		//printf("read_result = %d\n", read_result);

		if (read_result <= 0){
			continue;
		} else {
			
			if (strcmp(buffer, "skip") == 0){
				printf("Skipping training for %s.\n", activities[completed_activities]);
				completed_activities++;
				continue;
			} else if (strcmp(buffer, "done") == 0){
				return 2;
			}

			printf("Signal received. Letting server know to begin recording %s.\n", activities[completed_activities]);
			client_send_write(activities[completed_activities], client_socket_fd);

			// Record data
			gather_data(file_name, HOLD_TIME, record_time[completed_activities]);
		}

		// move file to server so that server can process data there
		memset(cmd, 0, sizeof(char) * 256);
		// ACTION: You may need to change directory below from dcs
		sprintf(cmd, "scp %s root@%s:/home/root/dcs2/%s_%s", file_name, SERVER_IP_ADDRESS, client_training_directory, username);
		printf("Moving data file to server...\n");
		system(cmd);

		// Let server know we've finished recording	
		printf("%s recorded. Sending completetion signal to server.\n", activities[completed_activities]);
		client_send_write(activities[completed_activities], client_socket_fd);

		completed_activities++;
	}

	printf("Training cycle completed.\n");

	free(file_name);
	free(buffer);
	free(cmd);

	return 1;
}

int client_testing(int client_socket_fd, char *SERVER_IP_ADDRESS, char *username){
	
	int read_result = 0;
	int write_result = 0;

	char *file_path = (char *) malloc(sizeof(char) * 256);
	char *buffer = (char *) malloc(sizeof(char) * 256);
	char *cmd = (char*) malloc(sizeof(char) * 256);

	while (1){
		printf("Waiting for signal to begin recording...\n");

		memset(file_path, 0, sizeof(char) * 256);
		memset(buffer, 0, 256);

		read_result = read(client_socket_fd, buffer, 255);
		//printf("read_result = %d\n", read_result);

		if (read_result <= 0){
			continue;
		} else {
			
			sprintf(file_path, "%s_%s/%s.csv", client_testing_directory, username, buffer);

			printf("Signal received. Beginning recording %s.\n");

			// Record data
			gather_data(file_path, 0, TESTING_RECORD_TIME);
		}

		// move file to server so that server can process data there
		memset(cmd, 0, sizeof(char) * 256);
		// ACTION: You may need to change directory below from dcs
		printf("Transferring file to server...\n");
		sprintf(cmd, "scp %s root@%s:/home/root/dcs2/%s_%s", file_path, SERVER_IP_ADDRESS, client_testing_directory, username);
		system(cmd);
		
		// Let server know we've finished recording	
		printf("Motion recorded. Sending completetion signal to server.\n");
		client_send_write(buffer, client_socket_fd);

	}

	printf("Testing cycle completed.\n");

	free(file_path);
	free(buffer);
	free(cmd);

	return 1;
}

void record_missing_training_files(char *SERVER_IP_ADDRESS, int client_socket_fd, char* username){
	
	int read_result, write_result;

	char *buffer = (char *) malloc(sizeof(char) * 256);
	char *cmd = (char *) malloc(sizeof(char) * 256);
	char *file_name = (char *) malloc(sizeof(char) * 256);
	
	
	// breaks out of loop when signal received is "done"
	while (1){
		memset(buffer, 0, 256);
		
		printf("Waiting for record signal from server...\n");
		read_result = read(client_socket_fd, buffer, 255);

		if (read_result <= 0)
			continue;
	
		// If we reached here, we've read from server successfully 
		if (strcmp(buffer, "n") == 0){
			printf("Skip recording received.\n");
			continue;
		} else if (strcmp(buffer, "done") == 0)
			break;

		memset(file_name, 0, sizeof(char) * 256);
		sprintf(file_name, "%s_%s/%s_%s.csv", client_training_directory, username, buffer, username);

		// Record data
		//int obtain_record_time(char *activity, char **activities, int num_activities, int *recording_times)
		gather_data(file_name, HOLD_TIME, obtain_record_time(buffer, activity_names, ACTIVITIES, record_time));

		// move file to server so that server can process data there
		memset(cmd, 0, sizeof(char) * 256);
		// ACTION: You may need to change directory below from dcs
		sprintf(cmd, "scp %s root@%s:/home/root/dcs/%s_%s", file_name, SERVER_IP_ADDRESS, client_training_directory, username);
		system(cmd);
		
		// Let server know we've finished recording	
		printf("%s recorded. Sending completetion signal to server.\n", buffer);
		client_send_write(buffer, client_socket_fd);
	}

	printf("Finished recording all missing activities.\n");
	free(buffer);
}

// If user wants to re-record, we go through all relevant motions listed in activity_names
int check_record_again(int client_socket_fd, char* SERVER_IP_ADDRESS, char* username){
	
	char *buffer = (char*) malloc(sizeof(char)*BUFF_SIZE);
	memset(buffer, 0, 256);

	int read_result;
		
	printf("Waiting for record signal from server...\n");

	while(1){
		read_result = read(client_socket_fd, buffer, 255);

		if (read_result <= 0)
			continue;
	
		// If we reached here, we've read from server successfully 
		if (strcmp(buffer, "y") == 0)
			client_training(client_socket_fd, SERVER_IP_ADDRESS, activity_names, ACTIVITIES, username);
		if (strcmp(buffer, "n") == 0)
			return 2;
	}

	printf("Re-recording process complete.\n");
	return 1;
}

int main(int argc, char **argv){

	// error check command line arguments
	if (argc != 4) {
		fprintf(stderr,"Usage: ./client <SERVER_IP_ADDRESS> <PORT_NUMBER> <USERNAME>\n");
		exit(1);
	}

	int train_result, test_result;

	// Create the directory to hold all our recordings of training/testing data
	char *directory = (char*) malloc(sizeof(char)*BUFF_SIZE);
  	memset(directory, 0, sizeof(char)*BUFF_SIZE);
  	sprintf(directory,"%s_%s", client_training_directory, argv[3]);
  	mkdir(directory, 0777);
  	
  	// Initiate public/private key pair with server
  	//setup_scp_without_auth_client(char* SERVER_IP_ADDRESS);

	// Connect client to server IP address and port
  	int client_socket_fd = client_connect(argv[1]);
  	
  	//int client_training(int client_socket_fd, char *SERVER_IP_ADDRESS, char **activities, int num_activities, char *username)
  	train_result = client_training(client_socket_fd, argv[1], activity_names, ACTIVITIES, argv[3]);

  	// A returned value of 2 means that we have no missing training files
  	if (train_result != 2){
  		printf("Checking for missed training files from server...\n");
  		record_missing_training_files(argv[1], client_socket_fd, argv[3]);
  	}

  	train_result = 0;
  	while (train_result != 2)
  		train_result = check_record_again(client_socket_fd, argv[1], argv[3]);

  	printf("Training process completed. Entering testing mode...");
  	memset(directory, 0, sizeof(char)*BUFF_SIZE);
  	sprintf(directory,"%s_%s", client_testing_directory, argv[3]);
  	mkdir(directory, 0777);

  	test_result = client_testing(client_socket_fd, argv[1], argv[3]);

	// clean up the file descriptors
	close(client_socket_fd);

	return 0;
}
