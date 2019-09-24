#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include <unistd.h>

char * Tilda;

void StoreTilda(char * hpath)
{
  Tilda = hpath;
}

void PrintPwd()
{
    char * curr = (char *)malloc(sizeof(char)*1000);
    getcwd(curr,1000);
    printf("%s\n",curr);
    free(curr);
}


void getRelative(char * hpath,char * printpath)
{
  char currpath[1000];
  getcwd(currpath,sizeof(currpath));
  int flag = 1;

  for(int i = 0; i < strlen(hpath) ; i++)
    if(hpath[i] != currpath[i])
      flag = 0;

  if(flag == 0)
  {
    for(int i = 0;i<=strlen(currpath);i++)
    {
      printpath[i] = currpath[i];
    }
  }

  else
  {
    char add[] = "~";
    strcat(add,currpath + strlen(hpath));
    for(int i = 0;i<=strlen(add);i++)
    {
      printpath[i] = add[i];
    }
  }
}
