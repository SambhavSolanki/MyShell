#include<stdio.h>

void EchoStuff(char * cmd)
{

  char * rest = NULL;
  char *token;
  token = strtok_r(cmd, " \t \n", &rest);
  token = strtok_r(NULL,"\n",&rest);
  if(token == NULL)
  return;
  if(!strncmp(token,"\"",1) && !strncmp(token + strlen(token)-1 ,"\"",1))
  {
        token[strlen(token)-1] = '\0';
        printf("%s\n",token+1);
  }
  else
  printf("%s\n",token);
}
