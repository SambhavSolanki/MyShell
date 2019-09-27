void fg(char *args[])
{
  int status;
  if(args[1] == NULL || args[2] != NULL)
  {
    printf("Invalid arguments for fg\n");
    return;
  }
  int p = 0;
  int len = strlen(args[1]);
  for(int i=0; i<len; i++){
    if(args[1][i] < 47 || args[1][i] > 57)
      {
        printf("Invalid Args\n");
        return;
      }
    p = p * 10 + ( args[1][i] - '0' );
  }
  p=p-1;
  kill(Jobs[p].pid,SIGCONT);
  tcsetpgrp(0,Jobs[p].pid);
  waitpid(Jobs[p].pid,&status,WUNTRACED);
  signal(SIGTTOU, SIG_IGN);
  tcsetpgrp(0,getpid());
  signal(SIGTTOU, SIG_DFL);
  // setpgid(Jobs[p].pid,);
  // setpgid(Jobs[p].pid,getpgid(globalpid));
}

void bg(char *args[])
{
  if(args[1] == NULL || args[2] != NULL)
  {
    printf("Invalid arguments for bg\n");
    return;
  }
  int p = 0;
  int len = strlen(args[1]);
  for(int i=0; i<len; i++){
    if(args[1][i] < 47 || args[1][i] > 57)
      {
        printf("Invalid Args\n");
        return;
      }
    p = p * 10 + ( args[1][i] - '0' );
  }
  p=p-1;
    char fpath[100] = "/proc/";
    char cpid[50];
    sprintf(cpid,"%d",Jobs[p].pid);
    strcat(fpath,cpid);
    strcat(fpath,"/stat");
    FILE * pFile;
    pFile = fopen (fpath,"r");
    if (pFile!=NULL)
    {
      long unsigned int lsize = 1000;
      // printf("%d\n",p);
      char buf[1000];
      fgets(buf,lsize,pFile);
      char c[1];
      for(int i = 0;i<strlen(buf);i++)
      {
        if(!strncmp(buf+i,")",1))
        {
          c[0] = buf[i+2];
        }
      }
      if(strncmp(c+0,"Z",1))
      {
        if(!strncmp(c+0,"T",1))
        {
          // printf("Hello\n");
          kill(Jobs[p].pid,SIGCONT);
        }
      }

    }

}
