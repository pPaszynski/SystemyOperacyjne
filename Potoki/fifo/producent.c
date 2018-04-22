

//WYKONANIE PROGRAMU: ./producent.o [ilosc_producentow] [ilosc_produktow_na_producentach]
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <fcntl.h>
#include <sys/types.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/wait.h>

/////////////// ZMIENNE GLOBALNE /////////////////
 int liczbaProducentow;
 int liczbaProduktow;

unsigned int limitProcesow;
char limitProcesowUzytkownika[10];
char uruchomioneProcesyUzytkownika[10];
FILE *limitFile;

char sciezkaPliku[32] = "./producenci/";
char nazwaPliku[32];
FILE *file;

int potok;
char bufor;

/////////////////////////// MAIN ///////////////////////////////
int main(int argc, char* argv[]) {

	//# PORZADKOWANIE ARGUMENTOW:
	if (argc != 3) {
		printf("ERROR: Błędna liczba argumentów\n");
		exit(-1);
	}
	if (atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0) {
		printf(
				"ERROR: Liczba producentow/produktow powinna byc liczba dodatnią!\n");
		exit(-1);
	}

	liczbaProducentow = atoi(argv[1]);
	liczbaProduktow =  atoi(argv[2]);

	//# LIMIT PROCESÓW UŻYTKOWNIKA:
	if (system("./limity.sh") == -1) {
		printf("ERROR: Błąd zbierania informacji o limitach!\n");
		exit(-1);
	}

	limitFile = fopen("limity.txt", "r");
	if (limitFile == NULL) {
		printf("ERROR: Błąd otwierania pliku!\n");
		exit(-1);
	}

	fgets(limitProcesowUzytkownika, 10, limitFile);	//# printf("!%d!",atoi(limitProcesowUzytkownika));
	fgets(uruchomioneProcesyUzytkownika, 10, limitFile);//# printf("!%d!",atoi(uruchomioneProcesyUzytkownika));
	limitProcesow = atoi(limitProcesowUzytkownika)- atoi(uruchomioneProcesyUzytkownika);

	if (fclose(limitFile) == -1) {
		printf("ERROR: Zamykanie pliku!");
		exit(-1);
	}

	if (limitProcesow < liczbaProducentow) {
		printf("ERROR: Nie mozna utworzyć tylu procesow!\n");
		exit(-1);
	}

	//# PRODUKOWANIE DANYCH:
	system("rm konsumenci/*");
	system("rm producenci/*");

	//# TWORZENIE FIFO:
	printf("TWORZENIE FIFO\n");
	if (access("./tmp/TST", F_OK) == -1) {
		if (mkfifo("./tmp/TST", 0777) == -1) {
			perror("ERROR: Błąd tworzenia kolejki fifo!");
			exit(-1);
		}
	}



	printf("TUTAj\n");


	potok = open("./tmp/TST", O_WRONLY); //otworz tylko do pisania

	printf("HERE\n");
	if (potok == -1) {
		perror("ERROR: Błąd otwierania potoku!\n");
		exit(-1);
	}

	//# TWORZENIE PRODUCENTOW:
	printf("TWORZENIE PRODUCENTOW\n");
	int pid;
	for (int i = 0; i < liczbaProducentow; i++) {
		puts("FORK\n");
		pid = fork();


		if (pid == -1) {
			perror("ERROR: Błąd funkcji fork(): Tworzenie producentów!");
			exit(-1);
		}
		if(pid == 0){
			puts("PROCES POTOMNY\n");
			sprintf(nazwaPliku, "%d", getpid());
			strcat(sciezkaPliku, nazwaPliku);

			file = fopen(sciezkaPliku, "w");
				if (file == NULL) {
					perror("ERROR: Błąd otwierania pliku!");
					exit(1);
				}

				//# TWORZENIE PRODUKTOW:
				//printf("TWORZENIE PRODUKTOW\n");

				srand(getpid());
				for (int i = 0; i < liczbaProduktow; i++) {

					bufor = (char) (rand() % 25 + 65);	//A-Z
					if (write(potok, (void *)&bufor, sizeof(char)) == EOF) {
						perror("ERROR: Błąd produkowania danych ");
					}
					fprintf(file, "%c", bufor);
					printf("Produkuję daną nr %d: %c\n", i+1, bufor);
				}


				if (fclose(file) == -1) {
					printf("ERROR: Zamykanie pliku!");
					exit(-1);
				}

				break;
		}

		int waitStatus;
	    if( i != liczbaProducentow){
	    	wait(&waitStatus);
	    }

	}


	//# ZAMYKANIE POTOKU I PLIKU KONCZENIE PROCESÓW POTOMNYCH:
	if( close(potok) == -1){
		perror("ERROR: Błąd zamykania potoku!");
		exit(-1);
	}



	int waitStatus;
	wait(&waitStatus);


return EXIT_SUCCESS;
}
