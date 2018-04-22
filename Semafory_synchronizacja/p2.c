//
// Created by rafal on 10.12.17.
//

#include "includes.h"
#include "semafory.h"
#include "zapis.h"


//info co sie robi w p1.c
int main(int argc, char **argv){
	if (argc != 2){
		printf("[PROCESS 2] Error with argc\n");
		exit(1);
	}
	else
		semafor = (int) strtol(argv[1], NULL, 10);

	clear_file();//wymazanie pliku jako iz to sie zawsze pierwsze wykona
	information_tasks(2, 1);
	semafor_signal(0);
	semafor_wait(1);
	information_tasks(2, 2);
	semafor_signal(2);
	semafor_wait(3);
	information_tasks(2, 3);
	semafor_signal(4);

	return 0;
}
