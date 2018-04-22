#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <limits.h>
#include <string.h>


//////// ZMIENNE GLOBALNE ////////
int fd[2];
char bufor;
char strSciezkaDoPliku[64] = "./konsumenci/";
char strPid[32];
enum { READ = 0, WRITE = 1 };

FILE *file;

//////////////////////////////////// MAIN ///////////////////////////////////
int main(int argc, char* argv[]) {

	//# Porządkowanie argumnetow:
	fd[0]=atoi(argv[1]);
	fd[1]=atoi(argv[2]);

	//# Przygotowanie scieżkid do zapisu produkowanych danych:
	sprintf(strPid,"%d",getpid());
	strcat(strSciezkaDoPliku,strPid);

	//# Przygotowanie pliku:
	file=fopen((const char*)strSciezkaDoPliku,"w");
	if(file==NULL){
		printf("ERROR: Błąd otwierania pliku!");
		exit(-1);
	}

	//# KONSUMOWANIE DANYCH
	close(fd[WRITE]);

	while(read(fd[READ],&bufor,sizeof(char))> 0){
		fprintf(file,"%c",bufor);
	}

	close(fd[READ]);

	if(fclose(file) == -1){
			perror("ERROR: Błąd zamykania pliku!");
			exit(-1);
		}


	return EXIT_SUCCESS;
}
