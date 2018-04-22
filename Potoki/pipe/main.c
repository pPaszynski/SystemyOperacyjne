/*

 Uruchamianie:	./start.o [liczba_producentow] [liczba_konsumentow] [liczba_produktow]
 ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/errno.h>
#include <limits.h>
#include <fcntl.h>


/////// ZMIENNE GLOBALNE //////////////
unsigned int liczbaProducentow;
unsigned int liczbaKonsumentow;
unsigned int liczbaProduktow;

unsigned int limitProcesow;
char limitProcesowUzytkownika[10];
char uruchomioneProcesyUzytkownika[10];

FILE *limitFile;

int fd[2];

/////////////////////////////////// MAIN //////////////////////////////////
int main(int argc, char* argv[]) {

	if(argc != 4){
		printf("ERROR: Nieprawidłowa liczba argumentów!\n");
		exit(-1);
	}

	//# Zabezpieczenia ilości argumentów:
	if(atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 ){
		printf("ERROR: Liczba producentow/konsumentow/produktow mniejsza lub równa zero!\n");
		exit(-1);
	}

	liczbaProducentow=atoi(argv[1]);
	liczbaKonsumentow=atoi(argv[2]);
	liczbaProduktow=atoi(argv[3]);

	//# LIMIT PROCESÓW UŻYTKOWNIKA:
	if(system("./limity.sh") == -1){
		printf("ERROR: Błąd zbierania informacji o limitach!\n");
		exit(-1);
	}

	limitFile=fopen("limity.txt", "r");
	if(limitFile == NULL){
		printf("ERROR: Błąd otwierania pliku!\n");
		exit(-1);
	}

	fgets( limitProcesowUzytkownika, 10, limitFile );		//# printf("!%d!",atoi(limitProcesowUzytkownika));
	fgets( uruchomioneProcesyUzytkownika, 10, limitFile );	//# printf("!%d!",atoi(uruchomioneProcesyUzytkownika));
	limitProcesow=atoi(limitProcesowUzytkownika)-atoi(uruchomioneProcesyUzytkownika);

	if(fclose(limitFile) == -1){
		printf("ERROR: Zamykanie pliku!");
		exit(-1);
	}

	if(limitProcesow < liczbaProducentow+liczbaKonsumentow){
		printf("ERROR: Nie mozna utworzyć tylu procesow!\n");
		exit(-1);
	}

	//# TWORZENIE PIPE`A
	if(pipe(fd) == -1){
		perror("ERROR: Błąd tworzenia potoku");
		exit(-1);
	}
	char arg1[32];
	sprintf(arg1,"%d",fd[0]);
	char arg2[32];
	sprintf(arg2,"%d",fd[1]);
	char arg3[32];
	sprintf(arg3,"%d",liczbaProduktow);

	system("rm producenci/*");
	system("rm konsumenci/*");

//	int flags;
//    flags = fcntl(fd[0], F_GETFL);
//    flags |= O_NONBLOCK;
//    if (fcntl(fd[0], F_SETFL, flags))
//        perror("fcntl");

	//# TWORZENIE PRODUCENTÓW:
	pid_t pid;
	for(int i=0; i < liczbaProducentow;i++){

		pid=fork();
		switch(pid){
		case -1:
			perror("ERROR: Błąd funkcji fork()!");
			exit(-1);
			break;

		case 0:
			if(execl("./producent", "./producent", arg1, arg2, arg3, NULL) == -1){
				perror("ERROR: Błąd funkcji execl()!");
				exit(-1);
			}
			break;

		default:
			break;

			}
	}



	//# TWORZENIE KONSUMENTOW:
	for(int i=0; i < liczbaKonsumentow;i++){

	pid=fork();
	switch(pid){
	case -1:
		perror("ERROR: Błąd funkcji fork()!");
		exit(-1);
		break;

	case 0:
		if(execl("./konsument", "./konsument", arg1, arg2, NULL) == -1){
			perror("ERROR: Błąd funkcji execl()!");
			exit(-1);
		}
		break;

	default:
		break;

		}
	}

	//# OCZEKIWANIE NA PROCESY POTOMNE
	int wait_status;
	for(int i=0; i<liczbaProducentow; i++)
		wait(&wait_status);

	close(fd[0]);
	close(fd[1]);

	for(int i=0; i<liczbaKonsumentow; i++)
		wait(&wait_status);



	return EXIT_SUCCESS;

}
