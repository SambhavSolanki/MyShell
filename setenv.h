void set(char *args[])
{
  if(args[1] == NULL || args[3] !=NULL)
  {
    printf("Invalid Arguments\n");
    return;
  }
  if(args[2] != NULL){
    if(setenv(args[1],args[2],1))
      perror("Errorwq");
  }
  else{
      if(setenv(args[1],"",1))
      perror("Errorwq");
  }
}

void unset(char *args[])
{
  if(args[1] == NULL || args[2] !=NULL)
  {
    printf("Invalid Arguments\n");
    return;
  }
  if(unsetenv(args[1]))
    perror("Errorwq");

}
