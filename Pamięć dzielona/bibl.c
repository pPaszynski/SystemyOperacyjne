#include <errno.h>
#include <stdio.h>
#include "bibl.h"
extern int shmid;
key_t klucz;

void shmcreate()
{
    klucz=ftok("/home",'K');
    if(klucz==-1){
	printf("Can't create key.(ftok)\n");
	exit(1);
	}
	shmid=shmget(klucz,2,0660|IPC_CREAT);
	if (shmid==-1) {
	printf("Problemy z utworzeniem pamieci dzielonej.\n");
	exit(EXIT_FAILURE);
	}
}




void shmdelete()
{
    if (shmctl(shmid,IPC_RMID,0) == -1)
      {
        printf("Problem z usuwaniem pamieci dzielonej.\n");
        exit(EXIT_FAILURE);
      }
	else printf("Usunieto pamiec wspoldzielona\n");
}



