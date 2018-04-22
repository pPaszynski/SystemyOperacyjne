#include <stdio.h>
#include <stdlib.h>


void inf()
{
  printf("UID = %d\tGID = %d\tPID = %d\tPPID = %d\n", getuid(), getgid(), getpid(), getppid());
}

int main()
{
  int i, pid = getpid();
  char drzewo[50];
  printf("Procesy:\n");
  inf();

  for (i = 0; i < 3; i++)
  {
    switch(fork())
    {
      case -1:                      
        printf("Fork nie powstal\n");
        exit(1);
        break;
      case 0:
        inf();
        break;
      default:
        break;
    }
  }

  sleep(3);

  sprintf(drzewo, "pstree -ph %d", pid);
  if (pid == getpid())
    system(drzewo);
sleep(1);

  return 0;
}

