#include <stdio.h>
#include <stdlib.h>


int main()
{
  printf("UID = %d\nGID = %d\nPID = %d\nPPID = %d\n\n", getuid(), getgid(), getpid(), getppid());

  return 0;
}
