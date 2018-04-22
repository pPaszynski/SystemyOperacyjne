#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/wait.h>

// ./konsument [ilosc konsumentow]

//===========================ZMIENNE GLOBALNE=============================
int i, j, pid, potok, readstatus, status;

FILE *fp;

char sciezka[50] = "./konsumenci/";
char charpid[10];
char znak;
char buf = 'a';
const int ILOSC_K = 10000;

//===========================PROGRAM GLOWNY=============================
int main(int argc, char *argv[]) {

    if (argc != 2) {
        printf("ERROR KONSUMENT: Nieprawidlowa liczba argumentow\n");
        exit(1);
    }

    int iloscKonsumentow = atoi(argv[1]);

//========================TWORZENIE KONSUMENTOW=========================
    for (j = 1; j < iloscKonsumentow; ++j) {

        if ((pid = fork()) < 0) {
            perror("ERROR KONSUMENT: fork ");
            exit(1);
        } else if (pid != 0) {
            break;
        }
    }

    sprintf(charpid, "%d", getpid());
    strcat(sciezka, charpid);

    fp = fopen(sciezka, "w");

    if (fp == NULL) {
        perror("ERROR KONSUMENT: fopen ");
        exit(1);
    }

    puts("TUTAJ\n");
    potok = open("./tmp/TST", O_RDONLY | O_NONBLOCK); //tylko do odczytu;
    //If O_NONBLOCK is set, an open() for reading-only shall return without delay.
    // An open() for writing-only shall return an error if no process currently has the file open for reading.

    puts("HERE\n");
    if (potok == -1) {
        perror("ERROR KONSUMENT: open ");
        exit(1);
    }

    sleep(10);
    //========================POBIERANIE=========================
    while (1){
        char buffer;
        ssize_t retval = read(potok, &buffer, sizeof(buffer));

        //Koniec danych
        if (retval <= 0){
            if (retval == -1) //EBADF -> Ostatni odczyt. Koniec danych, po zamknięciu.
            {
                perror("Read error handlable");
                //exit(6);

            }
            else {
                exit(0);
            }
        }
        else{

        }

    	printf("%c",buffer);
            	fprintf(fp,"%c",buffer);
    }


//    //if(readstatus==0)
//    printf("KONSUMENT: nie ma juz produktow do wziecia! \n");
//    sleep(1);
//    if (readstatus == -1)
//        exit(-1);
//    //    perror("ERROR KONSUMENT: read ");

    close(potok);
    fclose(fp);

//    unlink("./fifo");

    wait(&status);

    return 0;
}
