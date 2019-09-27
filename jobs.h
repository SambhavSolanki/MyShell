


void AddJob(int pid,char* name,int st)
{
  int stat;
  Jobs[no].pid = pid;
  strncpy(Jobs[no].name,name,strlen(name));
  if(st == 0)
  {
    Jobs[no].st = 0;
    Jobs[no].stat = waitpid(pid, &stat, WUNTRACED);
  }
  if(st == 1)
  {
    Jobs[no].st = 1;
    Jobs[no].stat = waitpid(pid, &stat, WNOHANG);
  }

  no++;
}

void RemoveJob(int pid)
{
  for(int i = 0;i<no;i++)
  {
    if(Jobs[i].pid == pid)
    {
      Jobs[i].stat = 0;
      break;
    }
  }
}

void JobsFunc()
{
  for(int i = 0;i<no;i++)
  {
      if(Jobs[i].pid != 0)
      {
        char fpath[100] = "/proc/";
        char cpid[50];
        sprintf(cpid,"%d",Jobs[i].pid);
        strcat(fpath,cpid);
        strcat(fpath,"/stat");
        FILE * pFile;
        pFile = fopen (fpath,"r");
        if (pFile!=NULL)
        {
          long unsigned int lsize = 1000;
          // char * cuf;
          char buf[1000];
          fgets(buf,lsize,pFile);
          char c[1];
          // strcpy(buf,cuf);
          for(int i = 0;i<strlen(buf);i++)
          {
            if(!strncmp(buf+i,")",1))
            {
              c[0] = buf[i+2];
            }
          }
          if(strncmp(c+0,"Z",1))
          {

          printf("[%d]\t",i+1);
          printf("%d\t",Jobs[i].pid);
          if(!strncmp(c+0,"T",1))
            printf("Suspended ");
          else
            printf("Running   ");
          printf("%s\n",Jobs[i].name);
        }
      }
    }
  }
}

void overkill()
{
  for(int i = 0;i<no;i++)
  {
    if(Jobs[i].pid != 0)
    {
      signal(SIGCHLD,SIG_DFL);
      kill(Jobs[i].pid,SIGKILL);
      Jobs[i].pid = 0;
      for(int j = 0;j<100;j++)
      Jobs[i].name[j] = 0;

    }
  }
  no = 0;

}

void kjob(char *args[])
{
  if(args[1] == NULL || args[2] == NULL || args[3] !=NULL)
  {
    printf("Invalid arguments for bg\n");
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
  int sig;
  len = strlen(args[2]);
  for(int i=0; i<len; i++){
    if(args[2][i] < 47 || args[2][i] > 57)
      {
        printf("Invalid Args\n");
        return;
      }
    sig = sig * 10 + ( args[2][i] - '0' );
  }
  kill(Jobs[p].pid,sig);
}
