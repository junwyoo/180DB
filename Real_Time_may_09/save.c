//
// Created by Amaael Antonini on 5/7/17.
//






void collect_training_data(char * folder, char *user)
{

    sig_atomic_t volatile interrupt_flag = 1;
    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            interrupt_flag = 0;
    }

    // determine which files are to be created and a directory for the files
    // assume training data is needed
    // create folder?? assume folder has been created?? chech if folder exist and then create it

    int hold;       // how long to wait before start recording
    int record;     // how long will the recording be
    int done_training;       // training is complete
    int current;    // index of the current file in use
    int *indexes;   // list of files indexes
    int repeat_all_session;       // restart training if requested
    int i;
    indexes = (int *) malloc(sizeof(int) * FILES);

    char** file_names;
    char *dir;
    dir = (char *) malloc(BUFF_SIZE * sizeof(char));
    memset(dir, 0, BUFF_SIZE);
    snprintf(dir, BUFF_SIZE, "%s_%s", train_file, user);
    file_names = (char**)malloc(FILES*sizeof(char*));

    double time_start, time_end, time_wait, time_ref;
    struct timeval time_before, time_after;
    float a_res, g_res, m_res;
    FILE *fp;


    for(i = 0; i < FILES; i++) {
        file_names[i] = (char *) malloc(BUFF_SIZE * sizeof(char));
        memset(file_names[i], 0, BUFF_SIZE);
        snprintf(file_names[i], BUFF_SIZE, "%s_%s/%s_%s.csv", train_file, user, names[i], user);
        indexes[i] = i;
    }




//    mraa_i2c_context accel, gyro, mag;
//    accel_scale_t a_scale;
//    gyro_scale_t g_scale;
//    mag_scale_t m_scale;
//    data_t ad, gd, md;
//
//    a_scale = A_SCALE_16G;
//    g_scale = G_SCALE_500DPS;
//    m_scale = M_SCALE_8GS;
//
//    accel = accel_init();
//    set_accel_ODR(accel, A_ODR_100);
//    set_accel_scale(accel, a_scale);
//    a_res = calc_accel_res(a_scale);
//
//    gyro = gyro_init();
//    set_gyro_ODR(gyro, G_ODR_190_BW_70);
//    set_gyro_scale(gyro, g_scale);
//    g_res = calc_gyro_res(g_scale);

//    mag = mag_init();
//    set_mag_ODR(mag, M_ODR_125);
//    set_mag_scale(mag, m_scale);
//    m_res = calc_mag_res(m_scale);

    time_wait = 0;
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    // Train_Data_Set_JY/walk_speed_1_JY.csv

    repeat_all_session = 1;
    while(repeat_all_session)
    {
        repeat_all_session = 0;
        snprintf(file_names[i], BUFF_SIZE, "%s_%s/%s_%s.csv", train_file, user, names[i], user);

        mkdir(dir, 0777);
        for(i = 0; i < FILES; i++)
        {
            if(access(file_names[i], F_OK) == -1 )
            {
                indexes[i] = -1;
            }
            else
            {
                printf("done\n");
            }
        }
        // determine if folder exist and create it otherwise
        // if folder created break
        // if folder existed determine which files are missing
        // set indexes[existing] to -1;
        // request if any overwrites
        // request which to overwrite if yes
        // make a list of the indexes to overwrite
        // set default hold and record???

        i = 0;
        done_training = 0;
        hold = 10; // set hold to 10????
        current = -1;

        while(interrupt_flag && done_training != 1) {

            // dialog to determine which file to train, how long to hold, how long to record
            for(i = 0; i < FILES; i++)
            {
                if(indexes[i] == i)
                {
                    printf("%d\t%s\n", i+1, names[i]);
                    // print the names of the files missing
                    // assume that indexes of not missing files are set to -1
                }
            }
            // prompt to enter the index of the file to record
            // prompt to enter hold and record time or default to standard
            // set current to the index to train: e.g i-1;

            if(current == 1)
            {
                printf("invalid entry, choose again\t");
                continue;
            }


            time_wait = 0;
            gettimeofday(&time_before, NULL);
            time_ref = time_before.tv_sec;
            int safe_guard;
            safe_guard = 0;
            printf("next %s\tin:\t%d\n", names[i], (int) (WAIT-time_wait));
            while (time_wait <= hold && safe_guard < 10000)
            {

                printf("%d\t%s\n", (int) (WAIT - time_wait), names[i]);
                usleep(1000000);
                safe_guard++;
                gettimeofday(&time_before, NULL);
                time_end = time_before.tv_sec;
                time_wait = time_end - time_ref;
            }
            usleep(1000000);
            printf("Attempting to write to file \'%s\'.\n", file_names[current]);
            fp = fopen(file_names[current], "w");
            if (fp == NULL) {
                fprintf(stderr,
                        "Failed to write to file \'%s\'.\n",
                        file_names[current]
                );
                exit(EXIT_FAILURE);
            }
            fprintf(fp, "time_stamp before, Acceleration x, Acceleration y,"
                    " Acceleration z, Gyro x, Gyro y, Gyro z\n");
            time_wait = 0;
            gettimeofday(&time_before, NULL);
            time_ref = time_before.tv_sec;
            while(time_wait < RUN & interrupt_flag)
            {
                gettimeofday(&time_before, NULL);


//            ad = read_accel(accel, a_res);
//            gd = read_gyro(gyro, g_res);

                time_wait = time_before.tv_sec - time_ref;

                time_start = time_before.tv_sec + time_before.tv_usec / NORM;
//            fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
//                    time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

                usleep(100);
            }

            fclose(fp);
            // dialog to determine if recording is successful
            // determine if section is done_training
        }
        // dialog to restart data collecting
        // if need to restart, repeat_all_session = 1
    }
}


void gather_data(char *file_name, int hold_time, int record_time)
{
    sig_atomic_t volatile interrupt_flag = 1;
    void do_when_interrupted(int sig)
    {
        if (sig == SIGINT)
            interrupt_flag = 0;
    }
    int i = 0;
    FILE *fp;

    double time_start, time_wait, time_ref;
    struct timeval time_before;
    float a_res, g_res, m_res;

//    mraa_i2c_context accel, gyro, mag;
//    accel_scale_t a_scale;
//    gyro_scale_t g_scale;
//    mag_scale_t m_scale;
//    data_t ad, gd, md;
//
//
//    a_scale = A_SCALE_16G;
//    g_scale = G_SCALE_500DPS;
//    m_scale = M_SCALE_8GS;
//
//    accel = accel_init();
//    set_accel_ODR(accel, A_ODR_100);
//    set_accel_scale(accel, a_scale);
//    a_res = calc_accel_res(a_scale);
//
//    gyro = gyro_init();
//    set_gyro_ODR(gyro, G_ODR_190_BW_70);
//    set_gyro_scale(gyro, g_scale);
//    g_res = calc_gyro_res(g_scale);
//    mag = mag_init();
//    set_mag_ODR(mag, M_ODR_125);
//    set_mag_scale(mag, m_scale);
//    m_res = calc_mag_res(m_scale);

    time_wait = 0;
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
    gettimeofday(&time_before, NULL);
    time_ref = time_before.tv_sec;
    while(time_wait < TEST_SIZE && interrupt_flag) {

//        ad = read_accel(accel, a_res);
//        gd = read_gyro(gyro, g_res);

        gettimeofday(&time_before, NULL);
        time_start = time_before.tv_sec + time_before.tv_usec / NORM;
        time_wait += (int) (time_start - time_ref);
//        fprintf(fp, "%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf,%20.10lf\n",
//                time_start, ad.x, ad.y, ad.z, gd.x, gd.y, gd.z);

        usleep(100);
    }

    fclose(fp);
}



#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/time.h>
#include "classifiers.h"
#include "constants.h"


//const int activities[] =        {0, 0, 0, 0, 1, 1, 1, 2, 3, 4, 4, 4};
//const int speeds_outputs[] =    {0, 1, 2, 3, 0, 1, 2, 0, 0, 0, 1, 2};
//const int speeds_start[]  =     {0, 4, 7, 8, 9};
//const int speeds_end[] =        {4, 7, 8, 9, 12};
int main(int argc, char **argv) {

    char *fake_test;
    fake_test = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(fake_test, 0, sizeof(char) * BUFF_SIZE);
    sprintf(fake_test, "%s%s%s_%s.csv", main_path, paths[0], names[0], user_names[0]);

    //char * test_file_name;
//    test_file_name = (char*)malloc(sizeof(char)*BUFF_SIZE);
//    memset(test_file_name, 0, BUFF_SIZE);

    float ***classifiers = NULL;
    // pointer to 3 d array with first D1 is activities,
    // second D2 is rows per activity,
    // third D3 is number of classifies (27 for now)
    int **counts = NULL;
    // pointer to 2 d array with first D1 is holds number of activities (12 total for now)
    // second D number of rows
    // each element is number of classifiers in each row, (constant of 27, not necessary)
    int *dimen = NULL;
    // dime is a pointer to 1 d array with first D1 number of activities (12 for now)
    // second D2 is rows per activity
    int n;
    // n is integer with number of activities (12 for now)
    int input;
    input = FEATURES;
    // when training, there will be sum o n rows if dim[i] for i from 0 to n-1 rows
    // dim[i] is the truth value or element 1 in row of -1 -1 -1 1(ith) -1

    // example follows
    int i, j, k;
    int speeds;
    float **test_array;
    int num_of_test_classifers;
    int truth_value;
    char *training_name;
    training_name = (char *) malloc(sizeof(char) * BUFF_SIZE);
//    if(access("training.txt", F_OK) == -1 || (argv[1] == "y" || argv[1] == "Y")){
    printf("if loop\n");


    int dot_net_files = 0;
    char *dot_net_name;
    dot_net_name = (char *) malloc(sizeof(char) * BUFF_SIZE);
    memset(dot_net_name, 0, sizeof(char) * BUFF_SIZE);
    sprintf(dot_net_name, "%s_%s/%s_%s.net", main_path, argv[1], train_file, activities_file, argv[1]);
    if (access(dot_net_name, F_OK) == -1)
        dot_net_files = -1;
    for (i = 0; i < ACTIVITIES; i++) {
        memset(dot_net_name, 0, sizeof(char) * BUFF_SIZE);
        sprintf(dot_net_name, "%s_%s/%s_%s.net", main_path, argv[1], train_speeds[i], argv[1]);
        if (access(dot_net_name, F_OK) == -1)
            dot_net_files = -1;
    }
    if (dot_net_files == -1) {
        printf("Training is required, proceeding to trainging\n");
        // function call
    }

    if (dot_net_files == -1) {
        printf("after training\n");
        // add name to train parameters function
        train_parameters(&classifiers, &counts, &dimen, &n);
        memset(training_name, 0, sizeof(char) * BUFF_SIZE);
        sprintf(training_name, "%s%s_%s", main_path, activities_file, train_name);
        printf("before text file\n");
        training_file(classifiers, training_name, dimen, activities, input, 0, n, ACTIVITIES);
        for (i = 0; i < ACTIVITIES; i++) {
            memset(training_name, 0, sizeof(char) * BUFF_SIZE);
            sprintf(training_name, "%s%s_%s", main_path, train_speeds[i], train_name);
            speeds = speeds_end[i] - speeds_start[i];
            training_file(classifiers, training_name, dimen, speeds_outputs, input, speeds_start[i], speeds_end[i],
                          speeds);
        }
        //train neural_network
        train_network(train_name);
        for (i = 0; i < n; i++) {
            for (j = 0; j < dimen[i]; j++) {
                free(classifiers[i][j]);
            }
            free(classifiers[i]);
            free(counts[i]);
        }
        free(classifiers);
        free(dimen);
    }
//}
//void consumer()
//{

//    test_data(test_name2);
//     Check if file exists; -1 means file does not exists

    // consumer code
    int loops;
    int check;
    check = 0;
    loops = 0;
    int l = 0;
    const char *test_file_name;
    test_file_name = (char*)malloc(sizeof(char)*BUFF_SIZE);
    //memset(test_file_name, 0, sizeof(char)*BUFF_SIZE);

//    while (loops < CYCLES && run_flag){
    while (loops < FILES ) {
        memset(fake_test, 0, sizeof(char)*BUFF_SIZE);
        sprintf(fake_test, "%s%s%s_%s.csv", main_path, paths[0], names[l++], user_names[0]);


        sprintf(test_file_name, "%s%s_%d.csv", main_path, test_name, loops);
        // delete next line
        test_data(main_path, test_name);

        if(access(fake_test, F_OK) == -1){
            // File does not exist
            sleep(1);
            printf("%s, sleeping\n", fake_test);
        }

        else {
            // File exists, so populate our data structure

            truth_value = test_classifiers(&test_array, fake_test, &num_of_test_classifers);
            printf("%s\t", test_file_name);
            check = remove(test_file_name);
            printf("%d\n", check);
            test_neural_network(test_array, num_of_test_classifers);
            for(i = 0; i < num_of_test_classifers; i++)
                free(test_array[i]);
            free(test_array);
        }
        loops++;
    }


}