//
// Created by rafal on 10.12.17.
//
#include <errno.h>
#include "semafory.h"
#include "includes.h"

extern int seamfor;

void semafor_create(void){
	key_t klucz = ftok(".", 62606); //stworzenie klucza (sciezka,kod)
	if (klucz == -1){
		perror("ftok error\n");
		exit(-1);
	}
	else
		semafor = semget(klucz, 5, 0600 | IPC_CREAT); //klucz, liczba semaforow, prawa|polecenie
	if (semafor == -1){
		printf("SEMAFOR_CREATE error\n");
		exit(EXIT_FAILURE);
	}
	else{
		printf("Semafor zostal utworzony : %d\n", semafor);
	}
}

void semafor_set(void){
	int ustaw_sem;
	unsigned short array[5] = {0, 0, 0, 0, 0};//tablica zmiennych
	ustaw_sem = semctl(semafor, 0, SETALL,
					   array);//modyfkacja semaforow: id numer, polecenie(wszystkie), argumenty polecenia
//	SETALL
//	Set semval for all semaphores of the set using arg.array,
//			updating also the sem_ctime member of the semid_ds struc‐
//	ture associated with the set.  Undo entries (see semop(2))
//	are cleared for altered semaphores in all processes.  If
//	the changes to semaphore values would permit blocked
//	semop(2) calls in other processes to proceed, then those
//	processes are woken up.  The argument semnum is ignored.
//			The calling process must have alter (write) permission on
//	the semaphore set.
	if (ustaw_sem == -1){
		printf("SEMAFOR_SET error\n");
		exit(EXIT_FAILURE);
	}
	else{
		printf("Semafory zostaly ustawione.\n");
	}
}


void semafor_wait(unsigned short sem_number){//semafor_p
	int zmien_sem;
	struct sembuf bufor_sem; // stworzenie structa do edycji
	bufor_sem.sem_num = sem_number; // numer semafora do edycji
	bufor_sem.sem_op = -1; // zamkniecie semafora
	bufor_sem.sem_flg = SEM_UNDO; // niezapamietanie
	zmien_sem = semop(semafor, &bufor_sem, 1); // operacje na semaforze: id, *struktura, liczba elementow tablicy na
	//ktorej ma zostac wykonana operacja
	printf("zmiensem=%d,errno=%d\n",zmien_sem,errno);

	if (errno == 4){
		while (errno == 4 && zmien_sem == -1){
			printf("zmiensem=%d,errno=%d\n", zmien_sem, errno);
			zmien_sem = semop(semafor, &bufor_sem, 1);
		}
		return;
	}
	if (zmien_sem == -1){
		printf("SEMAFOR_CLOSE error\n");
		exit(EXIT_FAILURE);
	}
}

void semafor_signal(unsigned short sem_number){//semafor_v
	//to samo dla wait
	int zmien_sem;
	struct sembuf bufor_sem;
	bufor_sem.sem_num = sem_number;
	bufor_sem.sem_op = 1;
	bufor_sem.sem_flg = SEM_UNDO;
	zmien_sem = semop(semafor, &bufor_sem, 1);


	if (errno == 4){
		while (errno == 4 && zmien_sem == -1){
			printf("zmiensem=%d,errno=%d\n", zmien_sem, errno);
			zmien_sem = semop(semafor, &bufor_sem, 1);
		}
		return;
	}
	if (zmien_sem == -1){
		printf("SEMAFOR_OPEN error\n");
		exit(EXIT_FAILURE);
	}
}

void semafor_delete(void){
	int sem;
	sem = semctl(semafor, 0, IPC_RMID); //id, numer semafora, komenda
	if (sem == -1){
		printf("SEMAFOR_DELETE error\n");
		exit(EXIT_FAILURE);
	}
	else{
		printf("Semafory zostały usuniete kod:%d\n", sem);
	}
}



