//
// Created by rafal on 10.12.17.
//

#include "zapis.h"
#include "includes.h"

void information_tasks(int program, int task){

	FILE *file_ptr;// pointer do pliku

	file_ptr = fopen("../plik_wynikowy.txt", "a"); // otwarcie w trybie do dopisu
	if (!file_ptr){
		printf("[PROCES NR=%d] OPEN FILE error.\n", program);
		exit(4);
	}

	printf("Sekcja t%d%d procesu o PID = %d\n", program, task, getpid());//stdout
	fprintf(file_ptr, "Sekcja t%d%d procesu o PID = %d\n", program, task, getpid());//fileout
	sleep(2);

	fclose(file_ptr);


}
void clear_file(){
	FILE *file_ptr;// pointer do pliku
	file_ptr = fopen("../plik_wynikowy.txt", "w");
	if (!file_ptr){
		exit(4);
	}
	fclose(file_ptr);
}
