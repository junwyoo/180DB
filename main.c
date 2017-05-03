#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#define BUFF_SIZE 1024

int main(int argc, char** argv)
{
  int c;
  int test_flag = 0; //set to 1 if "test" option
  char* name = (char*)malloc(sizeof(char) * BUFF_SIZE); 

  /* input example
     ./main --train yoo   => train with name "yoo"
     ./main --test yoo    => test with name "yoo"
     
     TO BE IMPLEMENTED
     ./main --test yoo --time 10  => test with name "yoo" & time int. 10
  */
  
  while(1) //This while loop handles options
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
      name = optarg;
      fprintf(stdout,"Your name is: %s\n",name);
    }
  /*Option handling ends*/

  
  if(!test_flag)
    printf("Training!\n");
  else
    printf("Testing!\n");

  
  exit(0);
}
