#include<stdio.h>

void EchoStuff(char * cmd[],int argno)
{
  int k = 1;
  while(k < argno)
  {
    if(!strncmp(cmd[k],"\"",1) && !strncmp(cmd[k] + strlen(cmd[k])-1 ,"\"",1))
    {
      cmd[k][strlen(cmd[k])-1] = '\0';
      printf("%s\n",cmd[k]+1);
    }
    else
    printf("%s\n",cmd[k]);
    k++;
  }

  // char * rest = NULL;
  // char *token;
  // token = strtok_r(cmd, " \t \n", &rest);
  // token = strtok_r(NULL,"\n",&rest);
  // if(token == NULL)
  // return;

}
