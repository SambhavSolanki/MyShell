#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <limits.h>
#include<fcntl.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

struct JobList{
  int pid;
  char name[100];
  int stat;
  int st;
};

struct JobList Jobs[100];
int no=0;
char ipath[1000];
char * hpath;
int globalpid;
int globalpgid;
int currpid;
static int fin;
static int fout;
static int ferr;
static fpos_t inpos;
static fpos_t outpos;
static fpos_t errpos;

int status = 1;
void clearScreen()
{
  printf("\e[1;1H\e[2J");
}

void ChangeFromTilda(char path[])
{
  if(!strncmp(path,"~",1))
  {
    char * ans = (char *)malloc(sizeof(char)*1000);
    for(int i = 0;i<strlen(hpath);i++)
      ans[i] = hpath[i];
    if(strlen(path) > 2)
    {
      strcat(ans,"/");
      strcat(ans,path+2);
    }
    else
    {
      ans[strlen(hpath)] = '\0';
    }
    strncpy(path,ans,strlen(ans));
    free(ans);
    return;
  }
  else
  return;
}
#include"pwd.h"
#include"cd.h"
#include"ls.h"
#include"pinfo.h"
#include"fgbg.h"
#include"echo.h"
#include"jobs.h"
#include <netdb.h>


void ExecuteFunction(char command[])
{
  if(strlen(command) <= 1)
    return;

  //Finding all arguments of the command
  int jflag = 0;
  if(!strncmp(command+strlen(command)-2,"&",1))
  {
      strncpy(command+strlen(command)-2,"",1);
    // strncpy(command+strlen(command)-3,"\n",1);
    jflag = 1;
  }
  else if(!strncmp(command+strlen(command)-1,"&",1))
  {
    strncpy(command+strlen(command)-1,"",1);
    jflag = 1;
  }
  int j = 0;
  char * tmp = (char *)malloc(sizeof(char)*strlen(command));
  strncpy(tmp,command,strlen(command));
  long unsigned int size = 1024;
  char * rest = NULL;
  char *token;
  char *args[100];
  for (token = strtok_r(command, " \t \n", &rest);
         token != NULL;
         token = strtok_r(NULL, " \t \n", &rest)) {
           if(token != "")
           {
             args[j] = token;
             j++;
           }
  }
  int argno = j;
  if(jflag == 1)
  {
    argno--;
    // strcpy(args[argno],"\0");
  }

  //Executing the command
  if(!strncmp(args[0],"quit",strlen(args[0])) || !strncmp(args[0],"quit\n",strlen(args[0])))
  status = 0;
  else if(!strncmp(args[0],"cd",strlen(args[0])) || !strncmp(args[0],"cd\n",strlen(args[0])))
  ChangeDir(args[1]);
  else if(!strncmp(args[0],"ls",strlen(args[0])) || !strncmp(args[0],"ls\n ",strlen(args[0])))
  ListFiles(args,argno);
  else if(!strncmp(args[0],"pwd",strlen(args[0])) || !strncmp(args[0],"pwd\n ",strlen(args[0])))
  PrintPwd();
  else if(!strncmp(args[0],"echo",strlen(args[0])) || !strncmp(args[0],"echo\n ",strlen(args[0])))
  EchoStuff(args);
  else if(!strncmp(args[0],"pinfo",strlen(args[0])) || !strncmp(args[0],"pinfo\n ",strlen(args[0])))
  pinfoFunc(args);
  else if(!strncmp(args[0],"jobs",strlen(args[0])) || !strncmp(args[0],"jobs\n ",strlen(args[0])))
  JobsFunc();
  else if(!strncmp(args[0],"overkill",strlen(args[0])) || !strncmp(args[0],"overkill\n ",strlen(args[0])))
  overkill();
  else if(!strncmp(args[0],"bg",strlen(args[0])) || !strncmp(args[0],"bg\n ",strlen(args[0])))
  bg(args);
  else if(!strncmp(args[0],"fg",strlen(args[0])) || !strncmp(args[0],"fg\n ",strlen(args[0])))
  fg(args);
  else if(!strncmp(args[0],"kjob",strlen(args[0])) || !strncmp(args[0],"kjob\n ",strlen(args[0])))
  kjob(args);
  else
  {
    int stat;
    int pid = fork();
    if(jflag == 1)
    {
      if(pid == 0)
      {
          setpgid(0,0);
          if(execvp(args[0],args) < 0)
          perror("Error");
          exit(0);
      }
      else
      {
        AddJob(pid,args[0],1);
        char * yy = NULL;
        j = 0;
        while(j<argno && args[j] != NULL)
        {
          memset(args[j], 0 , strlen(args[j]));
          args[j] = yy;
          j++;
        }
      }
    }
    else
    {
      if(pid == 0)
      {
        setpgid(0,0);
        if(execvp(args[0],args) < 0)
        perror("Error");
        exit(0);
      }
      else
      {
        tcsetpgrp(0,pid);
        currpid = pid;
        AddJob(pid,args[0],0);
        signal(SIGTTOU, SIG_IGN);
        RemoveJob(pid);
        tcsetpgrp(0,getpid());
        signal(SIGTTOU, SIG_DFL);
      }
      char * yy = NULL;
      j = 0;
      while(j<argno && args[j] != NULL)
      {
        memset(args[j], 0 , strlen(args[j]));
        args[j] = yy;
        j++;
      }
    }
  }
  char * yy = NULL;
  j = 0;
  while(j<argno && args[j] != NULL)
  {
    memset(args[j], 0 , strlen(args[j]));
    args[j] = yy;
    j++;
  }
  return;

}

void ExecuteRedirection(char command[])
{
  int p1 = -1,p2 = -1,p3=-1;
  int k = strlen(command);
  while(k--)
  {
      if(!strncmp(command+k-1,">>",2))
      {
        if(p1==-1 && p2==-1 && p3==-1)
        p2 = k-1;
        else
        printf("Invalid redirection\n");
        k--;
      }
      else if(!strncmp(command+k,">",1))
      {
        if(p1==-1 && p2==-1 && p3==-1)
        p1 = k;
        else
        printf("Invalid redirection\n");
      }
      else if(!strncmp(command+k,"<",1))
      {
        if(p3==-1)
        p3 = k;
        else
        printf("Invalid redirection\n");
      }
  }
  if(p1 != -1 && p3 != -1)
  {
    char *args[100];
    int top = 0;
    char *save;
    char *token = strtok_r(command,"<",&save);
    args[0] = token;
    token = strtok_r(NULL,"> \t \n",&save);
    args[1] = token;
    token = strtok_r(NULL,"> \t \n",&save);
    args[2] = token;
    int res = dup(0);
    int l = 0;
    int fd = open(args[1],O_RDONLY,644);
    int fg = open(args[2],O_CREAT | O_WRONLY | O_TRUNC,644);
    if(fd == -1)
    {
      perror("Failed to open file 1");
      close(fd);
      return;
    }
    if(fg == -1)
    {
      perror("Failed to open file 2");
      close(fg);
      return;
    }
    dup2(fg,1);
    dup2(fd,0);
    ExecuteRedirection(args[0]);
    close(fd);
    close(fg);

  }
  else if(p2 != -1 && p3 != -1)
  {
    char *args[100];
    int top = 0;
    char *save;
    char *token = strtok_r(command,"<",&save);
    args[0] = token;
    token = strtok_r(NULL,">> \t \n",&save);
    args[1] = token;
    token = strtok_r(NULL,"> \t \n",&save);
    args[2] = token;
    int res = dup(0);
    int l = 0;
    int fd = open(args[1],O_RDONLY,644);
    int fg = open(args[2],O_CREAT | O_WRONLY | O_APPEND,644);
    if(fd == -1)
    {
      perror("Failed to open file 1");
      close(fd);
      return;
    }
    if(fg == -1)
    {
      perror("Failed to open file 2");
      close(fg);
      return;
    }
    dup2(fg,1);
    dup2(fd,0);
    ExecuteRedirection(args[0]);
    close(fd);
    close(fg);

  }

  else if(p1 != -1)
  {
    char *args[100];
    int top = 0;
    char *save;
    char *token = strtok_r(command,">",&save);
    args[0] = token;
    token = strtok_r(NULL," \t \n",&save);
    args[1] = token;
    int fd = open(args[1],O_WRONLY | O_TRUNC,644);
    if(fd == -1)
    {
      perror("Failed to open file");
      close(fd);
      return;
    }
    dup2(fd, 1);
    ExecuteRedirection(args[0]);
    close(fd);
  }
  else if(p2 != -1)
  {
  char *args[100];
  int top = 0;
  char *save;
  char *token = strtok_r(command,">>",&save);
  args[0] = token;
  token = strtok_r(NULL,"> \t \n",&save);
  args[1] = token;
  int fd = open(args[1],O_WRONLY | O_APPEND,644);
  lseek(fd,0,SEEK_END);
  if(fd == -1)
  {
    perror("Failed to open file");
    close(fd);
    return;
  }
  dup2(fd, 1);
  ExecuteRedirection(args[0]);
  close(fd);
  }
  else if(p3 != -1)
  {
    char *args[100];
    int top = 0;
    char *save;
    char *token = strtok_r(command,"<",&save);
    args[0] = token;
    token = strtok_r(NULL,"< \n",&save);
    args[1] = token;
    int res = dup(0);
    int l = 0;
    while(!strncmp(args[1]+l," ",1))l++;
    int fd = open(args[1]+l,O_RDONLY,644);
    if(fd == -1)
    {
      perror("Failed to open file");
      close(fd);
      return;
    }
    dup2(fd,0);
    ExecuteRedirection(args[0]);
    dup2(res,fd);
    // close(fd);
  }
  else
  {
    ExecuteFunction(command);
  }
}

void ExecutePiping(char command[])
{
  int fin ;
  fin = dup(0);
  int fout;
  fout = dup(1);
  int cin = dup(fin);
  int cout;
  char * rest = NULL;
  char *token;
  char *args[100];
  int j = 0;
  for (token = strtok_r(command, "|", &rest);
         token != NULL;
         token = strtok_r(NULL, "|", &rest)) {
           if(token != "")
           {
             args[j] = token;
             j++;
           }
  }
  int k = 0;
  while(args[k] != NULL)
  {
    // int pipelines[2];
    fflush(0);
    // fflush(cin);
    dup2(cin,0);
    close(cin);
    if(k+1 == j){
      cout = dup(fout);
    }
    else{
      int pdata[2];
      pipe(pdata);
      cout = pdata[1];
      cin = pdata[0];
    }
    dup2(cout,1);
    close(cout);
    char tmd[1000];
    for(int i = 0;i <strlen(args[k]);i++)
    tmd[i] = args[k][i];
    ExecuteRedirection(tmd);
    for(int i = 0;i <strlen(args[k]);i++)
    tmd[i] = 0;
    k++;
  }
  j = 0;
  while(j<k)
  {
    memset(args[j], 0 , strlen(args[j]));
    j++;
  }
  fflush(0);
  dup2(fout,1);
  dup2(fin,0);
}

void chandler(int sig);

void loop_shell()
{
  char* cmd;
  while(status){
    //Printing Display Requirement
    signal(SIGCHLD,chandler);
    char * printpath = malloc(sizeof(char) * 1000);
    getRelative(hpath,printpath);
    char * hostname = malloc(sizeof(char) * 1024);
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    printf(""ANSI_COLOR_CYAN "<" ANSI_COLOR_RED"%s"ANSI_COLOR_CYAN"@"ANSI_COLOR_BLUE"%s:%s"ANSI_COLOR_CYAN ">:"ANSI_COLOR_RESET,getenv("USER"),hostname,printpath);

    //Breaking up all commands seperated by ";"
    char * cms = malloc(sizeof(char) * 1024);
    long unsigned int lsize = 1024;
    fgets(cms,lsize,stdin);
    char *cmd = strtok(cms,";");
    while(cmd != NULL)
    {
      if(strlen(cmd) == 0)
        continue;
      char tmd[1000];
      for(int i = 0;i < 1000;i++)
        tmd[i] = cmd[i];
      //Executing each command
      ExecutePiping(tmd);
      cmd = strtok(NULL,";");
    }
  }
}

void Zhandler(int sig)
{
  return;
}

void Chandler(int sig)
{
  return;
}

void chandler(int sig)
{
    pid_t pid;
    int   status;
    pid = waitpid(-1, &status, WNOHANG);
    if(pid > 0)
    {
      char * printpath = malloc(sizeof(char) * 1000);
      getRelative(hpath,printpath);
      char * hostname = malloc(sizeof(char) * 1024);
      gethostname(hostname, 1023);
      hostname[1023] = '\0';
      for(int i = 0;i<no;i++)
      {
        if(Jobs[i].stat == pid || Jobs[i].pid == pid)
        {
          printf("\n[%d]\t",pid);
          printf("%s\texited\n",Jobs[i].name);
          fflush(stdout);
          printf(""ANSI_COLOR_CYAN "<" ANSI_COLOR_RED"%s"ANSI_COLOR_CYAN"@"ANSI_COLOR_BLUE"%s:%s"ANSI_COLOR_CYAN ">:"ANSI_COLOR_RESET"\n",getenv("USER"),hostname,printpath);
        }
      }
      RemoveJob(pid);

    }
}

int main(int argc, char **argv)
{
  char buf[PATH_MAX];
  hpath = realpath(argv[0], buf);
  for(int i = strlen(buf);i>=0;i--)
    if(strncmp(buf+i,"/",1))
    {
      buf[i] = '\0';
    }
    else
    {
      buf[i] = '\0';
      break;
    }
  printf("%s\n",hpath);
  StoreTilda(hpath);
  clearScreen();
  globalpid = getpid();
  currpid = globalpid;
  signal(SIGINT,Chandler);
  signal(SIGTSTP,Zhandler);
  loop_shell();
  return 0;
}
