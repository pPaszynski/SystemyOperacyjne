/*
 ============================================================================
 Name        : SOproj7_potoki.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */

// ./producent.o fd[0] fd[1] liczbaProduktow
#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <limits.h>
#include <string.h>



//////// ZMIENNE GLOBALNE ////////
int fd[2];
int liczbaProduktow;
char bufor;
char strSciezkaDoPliku[64] = "./producenci/";
char strPid[32];
enum { READ = 0, WRITE = 1 };

FILE *file;

int rozmiarBuforaPotoku;


//////////////////////////////////// MAIN ///////////////////////////////////
int main(int argc, char* argv[]) {

	//# Porządkowanie argumnetow:
	fd[0]=atoi(argv[1]);
	fd[1]=atoi(argv[2]);
	liczbaProduktow=atoi(argv[3]);

	//# Przygotowanie scieżkid do zapisu produkowanych danych:
	sprintf(strPid,"%d",getpid());
	strcat(strSciezkaDoPliku,strPid);

	//# Przygotowanie pliku:
	file=fopen((const char*)strSciezkaDoPliku,"w");
	if(file==NULL){
		printf("ERROR: Błąd otwierania pliku!");
		exit(-1);
	}

	//# ZAMKNIECIE CZYTANIA POTOKU I PRODUKUJE DANE:
	srand(getpid());
	close(fd[READ]);
	for(int i=0; i<liczbaProduktow; i++){
		bufor=rand()%25 + 65;					 // A-Z
		//sleep(1);
		write(fd[WRITE],&bufor,sizeof(char));
		fprintf(file,"%c",bufor);
	}
	close(fd[WRITE]);



	if(fclose(file) == -1){
		perror("ERROR: Błąd zamykania pliku!");
		exit(-1);
	}


	return EXIT_SUCCESS;
}
