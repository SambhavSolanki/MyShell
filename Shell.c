#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/wait.h>
#include <limits.h>
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_COLOR_RESET   "\x1b[0m"

char ipath[1000];
char * hpath;
int globalpid;
int currpid;

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

#include"echo.h"
#include <netdb.h>
void Zhandler(int sig);


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
  EchoStuff(tmp);
  else if(!strncmp(args[0],"pinfo",strlen(args[0])) || !strncmp(args[0],"pinfo\n ",strlen(args[0])))
  pinfoFunc(args);
  else
  {
    int stat;
    int pid = fork();
    if(jflag == 1)
    {
      if(pid == 0)
      {
        int pidid = fork();
        if(pidid == 0)
        {
          setpgid(0,0);
          if(execvp(args[0],args) < 0)
          perror("Error");
          exit(0);
        }
        else
        {
          wait(0);
          printf("\n%s with pid %d exited normally\n",args[0],pidid);
        }

        // setpgid(0,globalpid);
        printf(ANSI_COLOR_CYAN">"ANSI_COLOR_RESET);
        // setpid(0,globalpid);
        // loop_shell();
        exit(0);
      }
      else
      {
        char * yy = NULL;
        j = 0;
        while(j<100)
        args[j++] = yy;
        return;
      }
      // return;

    }
    else
    {
      if(pid == 0)
      {
        printf("%d\n",getpid());
        if(execvp(args[0],args) < 0)
        perror("Error");
        // kill(getpid(), SIGKILL);
        exit(0);
      }
      else
      {
        currpid = pid;
        waitpid(pid, &stat, WUNTRACED);
      }
      char * yy = NULL;
      j = 0;
      while(j<100)
      args[j++] = yy;
      return;

    }
  }
  char * yy = NULL;
  j = 0;
  while(j<100)
  args[j++] = yy;
  return;

}

void loop_shell()
{
  char* cmd;
  while(status){
    //Printing Display Requirement
    char * printpath = malloc(sizeof(char) * 1000);
    getRelative(hpath,printpath);
    char * hostname = malloc(sizeof(char) * 1024);
    hostname[1023] = '\0';
    gethostname(hostname, 1023);
    printf(ANSI_COLOR_CYAN "<" ANSI_COLOR_RED"%s"ANSI_COLOR_CYAN"@"ANSI_COLOR_BLUE"%s:%s"ANSI_COLOR_CYAN ">:"ANSI_COLOR_RESET,getenv("USER"),hostname,printpath);
    // printf( "<%s@%s:%s>:" ,getenv("USER"),hostname,printpath);

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
      ExecuteFunction(tmd);
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
