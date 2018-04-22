//
// Created by rafal on 10.12.17.
//
#include "includes.h"
#include "semafory.h"
#include "zapis.h"

int main(int argc, char **argv){
	if (argc != 2){
		printf("[PROCESS 3] Error with argc\n");
		exit(1);
	}
	else
		semafor = (int) strtol(argv[1], NULL, 10);

	semafor_wait(0);
	information_tasks(3, 1);
	information_tasks(3, 2);
	semafor_signal(1);
	semafor_wait(4);
	information_tasks(3, 3);

	return 0;
}
