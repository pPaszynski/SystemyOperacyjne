#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/types.h>
#include <sys/wait.h>



void inf()
{
  printf("UID = %d\tGID = %d\tPID = %d\tPPID = %d\n", getuid(), getgid(), getpid(), getppid());
}

int main()
{
  int i, e, w, x, pid = getpid();
  char drzewo[50];
  printf("Proces aktualny:\n");
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
        e = execl("/bin/cat","cat","../Projekt1/program1.c", NULL);
        if(e == -1)
        {
          perror("Nie udało się wykonać programu");
          exit(2);
    exit(2);
        }
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

  printf("Kończenie procesow: \n");

  for (i = 0; i < 3; i++)
  {
    w = wait(&x);

    if(w == -1)
    {
      perror("Nie udało się wykonać funkcje wait");
      exit(3);
    }
    printf("Zakończono proces o ID = %d. Kod powrotu: %d\n", w, x);
  }

  if (getpid() == pid)
  {
    printf("Drzewo po zakonczeniu operacji:\n");
    sprintf(drzewo, "pstree -ph %d", pid);
    system(drzewo);
  }

  return 0;
}
