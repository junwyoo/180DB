#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <dirent.h>
#include <time.h>
#include "server_client.h"


/*
Process:
1) Establish server port number
2) Connect with clients using server port number
	- Once all connections are established, verify you're connected with the right clients
3) Once verification is complete, have server send signal to clients to start data collection process
4) Ensure processes are synchronized 
*/

// Make sure that write_message is a 256 bytes
int server_send_write(char *write_message, int *client_socket_fd, int num_clients){
    
    int write_results[num_clients], j;

    char buffer[256];
    memset(buffer, 0, 256);
    sprintf(buffer, "%s", write_message);

    for (j = 0; j < num_clients; j++){
        write_results[j] = write(client_socket_fd[j], buffer, strlen(buffer));
            
        if (write_results[j] <= 0){
            printf("Error writing to client socket %d. Attempting to resend.\n", client_socket_fd[j]);

            while (write_results[j] <= 0){
                write_results[j] = write(client_socket_fd[j], buffer, strlen(buffer));
            }
        }
    }

    return 1;
}

int read_from_clients(int *client_socket_fd, int num_clients){
	
	int read_results[num_clients], j;
	char buffer[256];

	for (j = 0; j < num_clients; j++){
		memset(buffer, 0, 256);
		read_results[j] = read(client_socket_fd[j], buffer, 255);

		if (read_results[j] <= 0){
			j--;
			continue;
		}
	}

	return 1;
}

void relay_skip_activity(int *client_socket_fd, int num_clients){

	int j;
	int write_results[num_clients];
	memset(write_results, 0, sizeof(int)*num_clients);

	server_send_write("skip", client_socket_fd, num_clients);

	printf("Successfuly wrote skip-recording message to all clients.\n");
	
}

// Latches server to a port and returns the server socket file descriptor
// Note that portno must be between 1000 and 8000
int server_connect(int portno){
	
	int server_socket_fd;
	struct sockaddr_in serv_addr;

	// setup socket
	server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (server_socket_fd < 0) {	
		error("ERROR opening socket");
	}

	// setup server information
	memset((char *) &serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);

	// bind the socket to an address
	printf("Attempting to bind server to port %d...\n", portno);
	while (bind(server_socket_fd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		printf("ERROR on binding. Attempting to bind to %d.\n", portno + 5);
		portno += 5;
		serv_addr.sin_port = htons(portno);
	}

	printf("Successfully binded server to port %d\n", portno);

	return server_socket_fd;
}

// Connects the server with clients, returns the client socket file descriptors
int *server_client_connect(int server_socket_fd, int num_clients){

	socklen_t clilen;
	struct sockaddr_in cli_addr;

	int *client_socket_fd = (int*) malloc(sizeof(int)*num_clients);
	memset(client_socket_fd, 0, sizeof(int)*num_clients);
	int accepted_sockets = 0;

	// Listen for incoming connections
	// accept at most 10 connections before refusing them
	printf("Listening for incoming connections...\n");
	listen(server_socket_fd, 10);
	clilen = sizeof(cli_addr);

	// block until there is a new connection. When one is received, accept it
	while (accepted_sockets != num_clients){
		client_socket_fd[accepted_sockets] = accept(server_socket_fd, (struct sockaddr *) &cli_addr, &clilen);
		if (client_socket_fd[accepted_sockets] < 0){
			error("ERROR on accept");
			continue;
		} else {
			// Make sure to print something verifying connection
			accepted_sockets++;
			printf("Added client %d with socket %d.\n", accepted_sockets, client_socket_fd[accepted_sockets]);
		}
	}

	return client_socket_fd;
}

// Runs server-side training protocol, in synchronization with client(s)
int server_training(int num_clients, char **activities, int num_activities, char *username, int *client_socket_fd){

	int completed_activities = 0;
	int j;

	char *file_name = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *buffer = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *cmd = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char input;

	// Will hold result of each read and write call to each of the clients
	int read_results[num_clients];
	memset(read_results, 0, sizeof(int)*num_clients);

	while (completed_activities != num_activities){

		memset(file_name, 0, sizeof(char)*BUFF_SIZE);
		sprintf(file_name, "%s_%s/%s_%s.csv", server_training_directory, username, activities[completed_activities], username);

		printf("Train for %s? [y/n]\n", activities[completed_activities]);
		
		// Obtain user input to record (or not record) activity
		input = obtain_yes_or_no_input();

		// If user input n, skip current activity, and let clients know to skip activity
		if (input == 'n'){
			printf("Skipping training for %s.\n", activities[completed_activities]);
			completed_activities++;
			relay_skip_activity(client_socket_fd, num_clients);
			continue;
		}

		if(access(file_name, F_OK) == -1){
			printf("%s file not found, will be created in data gathering process.\n", file_name);
		} else {
			printf("File %s_%s already exists. Overwrite? [y/n]\n", activities[completed_activities], username);
		
			input = getchar();
			while (input != 'y' && input != 'n'){
				printf("Input 'y' or 'n'.\n");
				input = getchar();
			}

			if (input == 'n'){
				printf("Retaining existing version of %s_%s.\n", activities[completed_activities], username);
				return 0;
			} else {
				sprintf(cmd, "rm ./%s", file_name);
				system(cmd);
			}
		}

		printf("Sending signal to begin recording %s...\n", activities[completed_activities]);
		server_send_write(activities[completed_activities], client_socket_fd, num_clients);
		
		printf("Successfully sent record signal to all clients for %s.\nWaiting for response from all clients to start recording...\n", activities[completed_activities]);
		read_from_clients(client_socket_fd, num_clients);

		printf("All responses accepted from clients; running data gathering process.\n");		
		gather_data(file_name, HOLD_TIME, obtain_record_time(activities[completed_activities], activity_names, num_activities, record_time));

		printf("Waiting for all clients to record activity for %s.\n", activities[completed_activities]);
		read_from_clients(client_socket_fd, num_clients);

		printf("All clients successfully recorded activity for %s.\n", activities[completed_activities]);
		completed_activities++;
	}

	free(file_name);
	free(buffer);
	free(cmd);

	printf("All activities recorded. Exiting.\n");

	return 1;
}

// Runs server-side testing protocol, in synchronization with client(s)
int server_testing(int num_clients, char *username, int *client_socket_fd){

	//int completed_activities = 0;
	int j, timestamp;

	char *file_path = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *file_name = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *buffer = (char*) malloc(sizeof(char)*BUFF_SIZE);
	char *cmd = (char*) malloc(sizeof(char)*BUFF_SIZE);

	// Will hold result of each read and write call to each of the clients
	int read_results[num_clients], write_results[num_clients];
	memset(read_results, 0, sizeof(int)*num_clients);
	memset(write_results, 0, sizeof(int)*num_clients);

	while (1){

		timestamp = (int)time(NULL);
		
		memset(file_path, 0, sizeof(char)*BUFF_SIZE);
		memset(file_name, 0, sizeof(char)*BUFF_SIZE);
		sprintf(file_path, "%s_%s/%d.csv", server_testing_directory, username, timestamp);
		sprintf(file_name, "%d.csv", username, timestamp);

		printf("Sending signal to begin recording %s...\n", file_name);

		memset(buffer, 0, 256);
		sprintf(buffer, "%d", timestamp);
		server_send_write(buffer, client_socket_fd, num_clients);

		printf("Successfully sent record signal to all clients for %s.\nWaiting for response from all clients...\n", file_name);
		
		gather_data(file_path, 0, TESTING_RECORD_TIME);
		//checkCSV(argv[3], activity_names[completed_activities], record_time[completed_activities], 1);

		read_from_clients(client_socket_fd, num_clients);

	}

	free(file_path);
	free(file_name);
	free(buffer);
	free(cmd);

	printf("Testing completed Exiting.\n");

	return 1;
}

void create_all_server_directories(char *username){

	char *directory = (char*) malloc(sizeof(char)*BUFF_SIZE);
  	
  	memset(directory, 0, sizeof(char)*BUFF_SIZE);
  	sprintf(directory,"%s_%s", server_training_directory, username);
	mkdir(directory, 0777);

	memset(directory, 0, sizeof(char)*BUFF_SIZE);
  	sprintf(directory,"%s_%s", server_testing_directory, username);
	mkdir(directory, 0777);

	// Create the directory to hold are all recordings from client
	memset(directory, 0, sizeof(char)*BUFF_SIZE);
  	sprintf(directory,"%s_%s", client_training_directory, username);
	mkdir(directory, 0777);

	memset(directory, 0, sizeof(char)*BUFF_SIZE);
  	sprintf(directory,"%s_%s", client_testing_directory, username);
	mkdir(directory, 0777);

	free(directory);
}

int check_record_again(int *client_socket_fd, int num_clients, char* username){

	printf("Informing clients to initiate re-recording process...\n");
	server_send_write("y", client_socket_fd, num_clients);

	//int num_clients, char **activities, int num_activities, char *username, int *client_socket_fd
	server_training(num_clients, activity_names, ACTIVITIES, username, client_socket_fd);

	return 1;
}

int main(int argc, char *argv[])
{
	// error check command line arguments
	if (argc != 4) {
		fprintf(stderr,"Usage: ./server <PORT_NUMBER> <NUM_CLIENTS> <USERNAME>\n");
		exit(1);
	}

	int i = 0, j = 0, key_pairing = -1;
	int num_missing_files = 0;
	int server_training_result;
	
	char *buffer = (char*) malloc(sizeof(char)*256);
	char input;
	
	char *server_training_dir = (char*) malloc(sizeof(char)*BUFF_SIZE);
	memset(server_training_dir, 0, sizeof(char)*BUFF_SIZE);
	sprintf(server_training_dir, "%s_%s", server_training_directory, argv[3]);

	// Connect the server, start by trying to latch to port 1000
	int server_socket_fd = server_connect(1000);

	// Connect server to clients
	int *client_socket_fd = server_client_connect(server_socket_fd, atoi(argv[2]));

	// Create the directory to hold all our recordings from server
	create_all_server_directories(argv[3]);

	// Initiate public/private key with client
	//while (key_pairing == -1)
	//	key_pairing = setup_scp_without_auth_server();

	printf("Checking for missing files...\n");
	char **missing_files = check_missing_training_files(server_training_dir, argv[3], activity_names, ACTIVITIES, &num_missing_files);

	// Obtain all missing training files, if they exist; must be completed before proceeding to testing
	if (num_missing_files == 0){

		// No missing files exist, inform clients
		server_send_write("done", client_socket_fd, atoi(argv[2]));
		printf("Successfully sent no-missing-files to all clients.\n");

	} else {
		
		while (num_missing_files > 0){
			printf("Total missing training files: %d\n", num_missing_files);
			printf("The following training files are missing:\n");
				for (i = 0; i < num_missing_files; i++)
					printf("\t%s\n", missing_files[i]);

        	printf("Initiating training sequence for missing training files...\n");
			server_training_result = server_training(atoi(argv[2]), missing_files, num_missing_files, argv[3], client_socket_fd);
			missing_files = check_missing_training_files(server_training_dir, argv[3], activity_names, ACTIVITIES, &num_missing_files);
		}

		// done collecting data for missing files; send signal to client
		//char *write_message, int *client_socket_fd, int num_clients
		server_send_write("done", client_socket_fd, atoi(argv[2]));
	}

	printf("Training sequence completed. Would you like to re-train motions? [y/n]\n");
	input = obtain_yes_or_no_input();

	while (input == 'y'){
		server_training_result = check_record_again(client_socket_fd, atoi(argv[2]), argv[3]);
		printf("Training sequence completed. Would you like to re-train motions? [y/n]\n");
		input = obtain_yes_or_no_input();
	}

	// Let client know that we've completed re-training sequence
	server_send_write("n", client_socket_fd, atoi(argv[2]));

	printf("Training sequence completed. Would you like to enter testing mode? [y/n]\n");
	input = obtain_yes_or_no_input();

	if (input == 'n'){
		printf("Press 'y' when ready to enter testing mode.\n");
		while (input != 'y')
			input = obtain_yes_or_no_input();
	}

  	printf("Entering testing mode...");
	int server_testing_result = server_testing(atoi(argv[2]), argv[3], client_socket_fd);

	printf("Closing sockets.");
	for (i = 0; i < atoi(argv[2]); i++){
		close(client_socket_fd[i]);
	}

	free(buffer);
	close(server_socket_fd);
	return 0; 
}
