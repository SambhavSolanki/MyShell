#include<unistd.h>
#include<strings.h>

void ChangeDir(char args[])
{
  if(args != NULL && strlen(args) > 0)
  {
    ChangeFromTilda(args);
    if(chdir(args))
    perror("Error");
  }
  return;
}
