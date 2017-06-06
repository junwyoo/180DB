#include <stdio.h>
#include <signal.h>
#include <math.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <dirent.h>

//following is for mraa
#include <mraa/pwm.h>
#include <mraa/aio.h>
#include <mraa/i2c.h>

#include "LSM9DS0.h"
#include "server_client.h"

char * activity_names[] = {
/* 1 */     "walk_speed_1_50sec_32m",
/* 2 */     "walk_speed_2_35sec_32m",
/* 3 */     "walk_speed_3_25sec_32m",
/* 4 */     "walk_speed_4_15sec_32m"
};



int record_time[] = {

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

// Each of these directories will hold the corresponding csv files
const char *server_training_directory = "./Server_Train_Data_Set";
const char *server_testing_directory = "./Server_Testing_Data_Set";
const char *client_training_directory = "./Client_Train_Data_Set";
const char *client_testing_directory = "./Client_Testing_Data_Set";

int setup_scp_without_auth_client(char* SERVER_IP_ADDRESS){
    
    if (access("/home/root/.ssh/id_rsa.pub", F_OK ) != -1)
        return 1;

    char *cmd = (char*) malloc(sizeof(char)*256);
    memset(cmd, 0, sizeof(char)*256);

    // setup public and private key pair
    system("ssh-keygen -t rsa");

    // copy id_rsa.pub file to remote server
    sprintf(cmd, "scp /home/root/.ssh/id_rsa.pub root@%s:./", SERVER_IP_ADDRESS);
    system(cmd);

    return 0;
}

int setup_scp_without_auth_server(){
    
    if (access("/home/root/id_rsa.pub", F_OK ) == -1 )
        return -1;  // public key does not exist

    // Check if directory exists
    DIR* dir = opendir("/home/root/.ssh");
    if (dir)
        closedir(dir);
    else {
        printf("Creating directory: /home/root/.ssh\n");
        mkdir("/home/root/.ssh", 0777);
    }

    system("cp /home/root/id_rsa.pub .ssh/authorized_keys");

    return 0;
}

// error outputting
void error(const char *msg)
{
    perror(msg);
    exit(0);
}

// Returns a "y" for yes or "n" for no from the user
char obtain_yes_or_no_input(){
    char input;
    
    input = getchar();
    while (input != 'y' && input != 'n'){
        printf("Input 'y' or 'n'.\n");
        input = getchar();
    }

    return input;
}

// Returns the recording time associated with the given activity
int obtain_record_time(char *activity, char **activities, int num_activities, int *recording_times){
    int index = 0;
    
    while (index < num_activities){
        if (strcmp(activities[index], activity) != 0){
            index++;
            continue;
        } else
            break;
    }

    return recording_times[index];
}

// Returns an array of missing files. Returns null if directory does not exist.
char **check_missing_training_files(char *directory, char *username, char **activities, int num_activities, int *num_missing){

    // Check if directory exists
    DIR* dir = opendir(directory);
    if (dir)
        closedir(dir);
    else {
        printf("Directory %s does not exist.\n", directory);
        return;
    }

    int i = 0, missing_files_index = 0, num_missing_files = 0;

    char **missed_files = (char**) malloc(sizeof(char*) * num_activities);
    char *activity = (char*) malloc(sizeof(char)*256);
    char *filename_path = (char*) malloc(sizeof(char)*BUFF_SIZE);

    for (i = 0; i < num_activities; i++){
        
        missed_files[i] = (char*) malloc(sizeof(char) * 256);
        memset(missed_files[i], '\0', sizeof(char) * 256);

        memset(filename_path, '\0', sizeof(char)*BUFF_SIZE);
        sprintf(filename_path, "%s/%s_%s.csv", directory, activities[i], username);
        //printf("filename_path = %s\n", filename_path);

        // Check if file exists
        if (access(filename_path, F_OK) == -1) {
            
            memset(activity, '\0', sizeof(char)*256);
            sprintf(activity, "%s", activities[i], username);

            num_missing_files++;
            sprintf(missed_files[missing_files_index], "%s", activity);
            //printf("missed_files[%d] = %s\n", missing_files_index, missed_files[missing_files_index]);
            missing_files_index++;
        }
    }

    free(filename_path);
    free(activity);
    
    (*num_missing) = num_missing_files;
    return missed_files;
}


void gather_data(char *file_name, int hold_time, int record_time)
{

    sig_atomic_t volatile interrupt_flag = 1;


    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            interrupt_flag = 0;
    }

    int i;

    double time_start, time_end, time_wait, time_ref;
    struct timeval time_before, time_after;
    float a_res, g_res, m_res;
    FILE *fp;

    //mraa thingy starts
    mraa_i2c_context accel, gyro, mag;
    accel_scale_t a_scale;
    gyro_scale_t g_scale;
    mag_scale_t m_scale;
    data_t ad, gd, md;

    a_scale = A_SCALE_16G;
    g_scale = G_SCALE_500DPS;
    m_scale = M_SCALE_8GS;

    accel = accel_init();
    set_accel_ODR(accel, A_ODR_100);
    set_accel_scale(accel, a_scale);
    a_res = calc_accel_res(a_scale);

    gyro = gyro_init();
    set_gyro_ODR(gyro, G_ODR_190_BW_70);
    set_gyro_scale(gyro, g_scale);
    g_res = calc_gyro_res(g_scale);

    mag = mag_init();
    set_mag_ODR(mag, M_ODR_125);
    set_mag_scale(mag, m_scale);
    m_res = calc_mag_res(m_scale);


    time_wait = 0;
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    int safe_guard;
    safe_guard = 0;
    printf("next %s\tin:\t%d\n", file_name, (int) (hold_time-time_wait));
    while (time_wait <= hold_time && safe_guard < 10000)
    {

        printf("%d\t%s\n", (int) (hold_time - time_wait), file_name);
        usleep(1000000);
        safe_guard++;
        gettimeofday(&time_before, NULL);
        time_end = time_before.tv_sec;
        time_wait = time_end - time_ref;
    }
    printf("Attempting to write to file \'%s\'.\n", file_name);
    fp = fopen(file_name, "w");
    if (fp == NULL) {
        fprintf(stderr,
                "Failed to write to file \'%s\'.\n",
                file_name
        );
        exit(EXIT_FAILURE);
    }
    fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
            " Acceleration z, Gyro x, Gyro y, Gyro z\n");
    time_wait = 0;
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    while(time_wait < record_time /*&& interrupt_flag*/)
    {
        gettimeofday(&time_before, NULL);


            ad = read_accel(accel, a_res);
            gd = read_gyro(gyro, g_res);

        time_wait = time_before.tv_sec - time_ref;

        time_start = time_before.tv_sec + time_before.tv_usec / NORM;
            fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

        usleep(100);
    }
    fclose(fp);
}
