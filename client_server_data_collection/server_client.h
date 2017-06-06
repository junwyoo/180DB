#ifndef SERVER_CLIENT_H_ 
#define SERVER_CLIENT_H_

#define ACTIVITIES 4
#define HOLD_TIME 10
#define BUFF_SIZE 1024
#define FILES 38
#define NORM 1000000.0f
#define TESTING_RECORD_TIME 10

// file transfer automation; note that this key pairing is meant for two Edisons
// Need to automate, but process listed here: http://alvinalexander.com/linux-unix/how-use-scp-without-password-backups-copy
int setup_scp_without_auth_client(char* SERVER_IP_ADDRESS);
int setup_scp_without_auth_server();

// error outputting
void error(const char *msg);

// helper methods
char obtain_yes_or_no_input();
int obtain_record_time(char *activity, char **activities, int num_activities, int *recording_times);
char **check_missing_training_files(char *directory, char *username, char **activities, int num_activities, int *num_missing);

// data gathering process
void gather_data(char *file_name, int hold_time, int record_time);

// holds all activities to train/test for, with their corresponding recording times
extern char *activity_names[];
extern int record_time[];

// directories holding .csv files corresponding to each activity in *activity_names 
extern const char *server_training_directory;
extern const char *server_testing_directory;
extern const char *client_training_directory;
extern const char *client_testing_directory;


#endif
