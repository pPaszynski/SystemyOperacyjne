//
// Created by rafal on 10.12.17.
//

#include "includes.h" //wszystkie biblioteki
#include "semafory.h" //zbior fukcji semaforowych
#include "zapis.h" //zapis na stdout i do pliku

//pobranie zmiennej semafor z semafory.h

int main(int argc, char **argv){ //pobranie id semafora
	char * pEnd = NULL; //zedfiniowanie wskaznika do "konca"
	if (argc != 2){
		printf("[PROCESS 1] Error with argc\n"); //error nie ma 2 argumentow przekazania
		exit(1);
	}
	else
//		long int strtol (const char* str, char** endptr, int base);
		semafor = (int) strtol(argv[1], NULL, 10); //zamiast atoi zgodnie z kompilatorem
//		semafor = atoi(argv[1]); //przypisanie przekazanego argv do semafor

	semafor_wait(2);//czekanie na semafor numer 2
	information_tasks(1, 1);//wykonanie zadania proces 1 task 1
	information_tasks(1, 2);//wykonanie zadania proces 1 task 2
	semafor_signal(3);//zasygnalizowanie semaforowi 3 ze moze wykonac nastepny task

	return 0;
}
