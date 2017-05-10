
#include "functions.h"

int main()
{
  if(checkCSV()==-1)
    collectData();

  trainData();
  
  //  [test Data]
  system("./consumer yoo");
  system("./producer yoo");
}
