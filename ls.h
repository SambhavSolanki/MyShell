#include <dirent.h>
#include <stdio.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>
#include<strings.h>
// #include "pwd.h"
int aflag = 0;
int lflag = 0;
int kflag = 0;
int n = 0;

void printl(char name[])
{
  struct stat fileStat;
    if(stat(name,&fileStat) < 0)
        return;
  struct passwd *pw = getpwuid(fileStat.st_uid);
  struct group  *gr = getgrgid(fileStat.st_gid);
  char * date = (char *)malloc(sizeof(char)*20);
  strftime(date, 20, "%b %m %H:%M", localtime(&(fileStat.st_ctime)));
  printf( (S_ISDIR(fileStat.st_mode)) ? "d" : "-");
  printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-");
  printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-");
  printf( (fileStat.st_mode & S_IROTH) ? "r" : "-");
  printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-");
  printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
  printf(" %d",fileStat.st_nlink);
  printf(" %s",pw->pw_name);
  printf(" %s",gr->gr_name);
  printf("%8d",fileStat.st_size);
  printf(" %s",date);
  printf(" %s\n",name);
  free(date);
}

void PrintLS(char dir[][100])
{
  if(n > 0)
  {
    for(int i = 0;i<n;i++)
    {
      char * curr = (char *)malloc(sizeof(char)*1000);
      getcwd(curr,1000);
      ChangeDir(dir[i]);
      struct dirent *de;
      DIR *dr = opendir(".");
      if (dr == NULL)
      {
          printf("Could not open current directory %s",dir[i]);
          return ;
      }

      while ((de = readdir(dr)) != NULL)
      if(strncmp(de->d_name,".",1) || aflag == 1)
      {
        if(lflag == 0)
        printf("%s\n", de->d_name);
        else
        printl(de->d_name);
      }
      printf("\n");
      closedir(dr);

      ChangeDir(curr);
      free(curr);
    }
    return ;
  }
  else
  {
    struct dirent *de;
    DIR *dr = opendir(".");

    if (dr == NULL)   {
        printf("Could not open current directory" );
        return ;
    }
    while ((de = readdir(dr)) != NULL)
    if(strncmp(de->d_name,".",1) || aflag == 1)
    {
      if(lflag == 0)
      printf("%s\n", de->d_name);
      else
      printl(de->d_name);
    }
    closedir(dr);
    return ;
  }
}

void ListFiles(char *args[], int argno)
{
  struct dirent *de;
  int j = 0;
  aflag = 0;
  lflag = 0;
  kflag = 0;
  n=0;
  char dir[100][100];
  // for(int i = 0;i<100;i++)
    // dir[i] = (char *)malloc(sizeof(char)*1000);
  while(args[++j] != NULL && j < argno )
  {
    if(!strncmp(args[j],"-",1))
    {
    if(strlen(args[j]) >= 3 && (!strncmp(args[j],"-la",3) || !strncmp(args[j],"-al",3)))
    {
      lflag = 1;
      aflag = 1;
    }
    else if(strlen(args[j]) >= 2 && !strncmp(args[j],"-l",strlen(args[j])))
      lflag = 1;
    else if(strlen(args[j]) >= 2 && !strncmp(args[j],"-a",strlen(args[j])))
      aflag = 1;
    else
    {
      printf("Error: %s is not a valid argument for ls\n",args[j]);
      return;
    }
    }
    else
    strcpy(dir[n++],args[j]);
    // dir[n++] = args[j];
  }
  PrintLS(dir);
}
