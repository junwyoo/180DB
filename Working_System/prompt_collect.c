#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <string.h>
#include <sys/stat.h>

//#include "neural_network.h"
#include "classifiers.h"
//#include "prompt_collect.h"

#define BUFF_SIZE 1024
#define FILES 28


  const char * activity_names[] = { // changed from "names"
    "walk_speed_1_50sec_32m",
    "walk_speed_2_35sec_32m",
    "walk_speed_3_25sec_32m",
    "walk_speed_4_15sec_32m",
    "slow_run",
    "medium_slow_run",
    "medium_fast_run",
    "fast_run",
    "slow_stairs_up",
    "medium_slow_stairs_up",
    "medium_fast_stairs_up",
    "fast_stairs_up",
    "slow_stairs_down",
    "medium_slow_stairs_down",
    "medium_fast_stairs_down",
    "fast_stairs_down",
    "low_jump",
    "medium_low_jump",
    "medium_high_jump",
    "high_jump",
    "turning_right_speed_1",
    "turning_right_speed_2",
    "turning_right_speed_3",
    "turning_right_speed_4",
    "turning_left_speed_1",
    "turning_left_speed_2",
    "turning_left_speed_3",
    "turning_left_speed_4" //28 csv files
  };

const int hold_time[FILES] = {
  10,//"walk_speed_1_50sec_32m",
  10,//"walk_speed_2_35sec_32m",
  10,//"walk_speed_3_25sec_32m",
  10,//"walk_speed_4_15sec_32m",
  10,//"slow_run",
  10,//"medium_slow_run",
  10,//"medium_fast_run",
  10,//"fast_run",
  10,//"slow_stairs_up",
  10,//"medium_slow_stairs_up",
  10,//"medium_fast_stairs_up",
  10,//"fast_stairs_up",
  10,//"slow_stairs_down",
  10,//"medium_slow_stairs_down",
  10,//"medium_fast_stairs_down",
  10,//"fast_stairs_down",
  10,//"low_jump",
  10,//"medium_low_jump",
  10,//"medium_high_jump",
  10,//"high_jump",
  10,//"turning_right_speed_1",
  10,//"turning_right_speed_2",
  10,//"turning_right_speed_3",
  10,//"turning_right_speed_4",
  10,//"turning_left_speed_1",
  10,//"turning_left_speed_2",
  10,//"turning_left_speed_3",
  10//"turning_left_speed_4"
};
const int record_time[FILES] = {
  15,//"walk_speed_1_50sec_32m",
  15,//"walk_speed_2_35sec_32m",
  10,//"walk_speed_3_25sec_32m",
  10,//"walk_speed_4_15sec_32m",

  15,//"slow_run",
  15,//"medium_slow_run",
  10,//"medium_fast_run",
  10,//"fast_run",
  
  15,//"slow_stairs_up",
  15,//"medium_slow_stairs_up",
  10,//"medium_fast_stairs_up",
  10,//"fast_stairs_up",
  15,//"slow_stairs_down",
  15,//"medium_slow_stairs_down",
  10,//"medium_fast_stairs_down",
  10,//"fast_stairs_down",

  15,//"low_jump",
  15,//"medium_low_jump",
  15,//"medium_high_jump",
  15,//"high_jump",

  15,//"turning_right_speed_1",
  15,//"turning_right_speed_2",
  10,//"turning_right_speed_3",
  10,//"turning_right_speed_4",

  15,//"turning_left_speed_1",
  15,//"turning_left_speed_2",
  10,//"turning_left_speed_3",
  10//"turning_left_speed_4"
};

int checkCSV(char *username)
{
  int redo_prompt = 1;
  int i;
  int n_missing = 0;
  char type; // input char
   
  char *target_string = (char*) malloc(sizeof(char)*BUFF_SIZE);
  char **file_names = (char **) malloc(sizeof(char*)*FILES);
  char **missing_file_names = (char **) malloc(sizeof(char*)*FILES);

  printf("Your name is: %s\n",username);
  
  //  CSV file format:
  //  Train_Data_Set_<name>/<activity name>_<name>.csv 

  
  //make a directory
  sprintf(target_string,"Train_Data_Set_%s",username);
  mkdir(target_string, 0777);

  while(redo_prompt)
    {
      redo_prompt = 0;
      
      //check for csv files
      for(i = 0; i < FILES; i++) {
	file_names[i] = (char *) malloc(BUFF_SIZE * sizeof(char));
	memset(file_names[i], 0, BUFF_SIZE);
	snprintf(file_names[i], BUFF_SIZE, "Train_Data_Set_%s/%s_%s.csv", username, activity_names[i], username);
	if(access(file_names[i], F_OK) == -1)
	  {
	    printf("Error: %s file not found\n",file_names[i]);    
	    
	    //prompt for training
	    printf("Do you wish to train data for: %s? [y/n]\n",username);
	    type = getchar();
	    if(type == '\n') type = getchar();
	    while(type != 'n' && type != 'y')
	      {
		printf("Wrong input. [y/n] \n");
		type = getchar();
		if(type == '\n') type = getchar();
	      }
	    printf("You answered: %c\n",type);
	    if(type == 'n')
	      {
		//saving the missing ones
		missing_file_names[n_missing] = file_names[i];
		n_missing ++;
		printf("Okay.. continue checking..\n");
	      }
	    else
	      {
		gather_data(file_names[i], hold_time[i], record_time[i]);
		printf("Training for %s!\n",file_names[i]);
	      }
	  } //end of checking/collecting
      } //end of for loop

      if(n_missing != 0)
	{
	  printf("You have %d missing CSV files.\nDo you want to collect those? [y/n]\n");
	  
	  type = getchar();
	  if(type == '\n') type = getchar();
	  while(type != 'n' && type != 'y')
	    {
	      printf("Wrong input. [y/n] \n");
	      type = getchar();
	      if(type == '\n') type = getchar();
	    }
	  printf("You answered: %c\n",type);
	  if(type == 'n')
	    {
	      printf("Error: cannot train with incomplete CSV files.\n");
	      return -1;
	    }
	  else
	    {
	      printf("Running through again...\n");
	      redo_prompt = 1;
	    }
	}
      else // no missing file
      return 1;
    } //end of while loop

  printf("Critical Error: Should not output this message!\n");
  return -127;

}

// [main function commented out below] //

/*
int main(int argc, char** argv)
{
  int c, i;
  char type; // input char
  char *filename = (char*)malloc(sizeof(char)*BUFF_SIZE);//for fopen
  char *command = (char*)malloc(sizeof(char)*BUFF_SIZE);//for shell cmd
  char* username = (char*)malloc(sizeof(char) * BUFF_SIZE);//user name
  int test_flag = 0; //set to 1 if "test" option
  FILE *t_fp, *w_fp, *r_fp, *j_fp, *sa_fp, *sd_fp; //fps for .net files
  
  if (argc == 1)
  {
    fprintf(stderr,"Error: Need Arguments.\nex)./main yoo\n");
    exit(1);
  }

  
<<<<<<< HEAD
<<<<<<< HEAD
   
=======
  * 
>>>>>>> 7f5b522a382a4aa6663da45f7628f2b81f257614
=======
>>>>>>> parent of 7f5b522... fixed makefile
     [INPUT EXAMPLE]
     ./main --train walking_3.csv   => train with name "yoo" & test
     ./main --test yoo    => test with name "yoo"

     ./main yoo => check for .net file for "yoo", if not prompt to user, 
     train, and test 
  */

  
  /* Option Handling Starts */
/*  while(1) 
    {
      static struct option long_options[]=
	{
	  {"train",required_argument,0,'n'},
	  {"test",required_argument,0,'t'},
	  {0,0,0,0}
	};
      c = getopt_long(argc, argv, "", long_options, 0);

      if(c == -1)
	break;
      switch(c)
	{
	case'n':
	  test_flag = 0;
	  break;
	case't':
	  test_flag = 1;
	  break;
	case':':
	case'?':
	default:
	  fprintf(stderr,"ex) \"--train yoo\" or \"--test yoo\"\n");
	  exit(1);
	}
      if(optarg[0] == '-' && optarg[1] == '-')
	{
	  fprintf(stderr,"Error: Requires an Argument\n");
	  exit(1);
	}
      else if(argv[optind] != '\0' && argv[optind][0] != '-' && argv[optind][1]
	      != '-')
	{
	  fprintf(stderr, "Error: More than One Argument\n");
	  exit(1);
	}
      username = optarg;

    }
  if(argc == 2)
    username = argv[1];
  *Option handling ends*/

/*
  printf("Your name is: %s\n",username);
  
//  CSV file format:
//  Train_Data_Set_<name>/<activity name>_<name>.csv 

  char *target_string = (char*) malloc(sizeof(char)*BUFF_SIZE);
  char **file_names = (char **) malloc(sizeof(char*)*FILES);

  //make a directory
  sprintf(target_string,"Train_Data_Set_%s",username);
  mkdir(target_string, 0777);

  //check for csv files
  for(i = 0; i < FILES; i++) {
    file_names[i] = (char *) malloc(BUFF_SIZE * sizeof(char));
    memset(file_names[i], 0, BUFF_SIZE);
    snprintf(file_names[i], BUFF_SIZE, "Train_Data_Set_%s/%s_%s.csv", username, activity_names[i], username);
    if(access(file_names[i], F_OK) == -1)
      {
	printf("Error: %s file not found\n",file_names[i]);
	  //prompt for training
	  printf("Do you wish to train data for: %s? [y/n]\n",username);
	  type = getchar();
	  if(type == '\n') type = getchar();
	  while(type != 'n' && type != 'y')
	    {
	      printf("Wrong input. [y/n] \n");
	      type = getchar();
	      if(type == '\n') type = getchar();
	    }
	  printf("You answered: %c\n",type);
	  if(type == 'n')
	    {
	      printf("Okay.. continue checking..\n");
	    }
	  else
	    {
	      //[GATHER DATA]
	      //gather_data(filename_i[i],index[i]);//
	      gather_data(file_names[i],10); //change 10 seconds to time_i[i]
	      printf("Training for %s!\n",file_names[i]);
	    }
      
      }
  }
  
  exit(0);
}
*/
