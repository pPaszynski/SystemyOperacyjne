#include <errno.h>
#include <stdio.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/sem.h>

int main()
{
system("gcc program_glowny.c -o P1");
printf("\n --------------------- \n");
system("gcc program_potomny.c -o P2");
printf("\n --------------------- \n");

return 0;
}
