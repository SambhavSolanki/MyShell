#include<stdio.h>
#include<unistd.h>
#include <stdlib.h>
#include <limits.h>

void pinfoFunc(char *args[])
{
  char * pid = (char *)malloc(sizeof(char)*100) ;
  if(args[1] == NULL || strlen(args[1]) < 1)
  {
    int * p = (int *)malloc(sizeof(int));
    *p = getpid();
    sprintf(pid,"%d",*p);
    free(p);
  }
  else
  strncpy(pid,args[1],strlen(args[1]));
  printf("pid -- %s\n",pid);
  char fpath[100] = "/proc/";
  strcat(fpath,pid);

  char statm[100];
  char status[100];
  char exe[100];
  strncpy(statm,fpath,100);
  strncpy(status,fpath,100);
  strncpy(exe,fpath,100);

  strcat(statm,"/statm");
  strcat(status,"/stat");
  strcat(exe,"/exe");
  FILE * pFile;
  pFile = fopen (status,"r");
  if (pFile!=NULL)
  {
    printf("Process status ");
    long unsigned int lsize = 1000;
    char * cuf;
    getline(&cuf,&lsize,pFile);
    char buf[1000];
    strcpy(buf,cuf);
    for(int i = 0;i<strlen(buf);i++)
    {
      if(!strncmp(buf+i,")",1))
      {
        printf("%c\n",buf[i+2]);
      }
    }

    fclose(pFile);
  }
  else
  perror("ERROR");

  pFile = fopen (statm,"r");
  if (pFile!=NULL)
  {
    printf("memory ");
    char * buf = (char *)malloc(sizeof(char)*1000) ;
    fscanf(pFile,"%s",buf);
    printf("%s\n",buf);
    fclose(pFile);
    free(buf);
  }
  else
  perror("ERROR");
  char duf[PATH_MAX];
  char *res = realpath(exe, duf);
  if (res) {
    printf("Executable Path ");
     printf("%s\n", duf);
  }
  else {
     perror("Executable Path ");
  }

  free(pid);

  // printf("%s\n",status);

}
