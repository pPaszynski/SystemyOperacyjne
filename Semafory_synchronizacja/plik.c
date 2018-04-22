//
// Created by rafal on 10.12.17.
//

// Main of project


#include "includes.h"
#include "semafory.h"


int main(){
//	Kolejnosc wykonywania zadan
//	t21
//	t31
//	t32
//	t22
//	t11
//	t12
//	t23
	int w, x, id;
	char nazwa[64];
	char argument1[64];
	char argument2[64];
	printf("[PROCES GŁÓWNY] ... ... ...\n");
	printf("[PROCES GŁÓWNY semafor_create()...\n");
	semafor_create(); //tworzenie semaforow
	printf("[PROCES GŁÓWNY semafor_set()...\n");
	semafor_set(); //ustawianie semaforow na 0

	//semafor ustawiony w create zapisuje sie do extern in semafor skad mozemy go pobrac
	//w wielu miejscach tego projektu tam gdzie jest dolaczany
	sprintf(argument2, "%d", semafor); //argument 2 dla wywolania gdzie przekazujemy id semafora

	//tworzenie wątków
	for (int i = 1; i <= 3; i++){
		switch (id = fork()){
			case -1:
				perror("Fork error");//blad fork
				exit(1);
			case 0:
				//programy potomne
				//przy przejsciu na kolejne wywolanie zmiana na kolejny program od 1 co 3
				sprintf(nazwa, "./p%d.out", i);//sciezka programu
				sprintf(argument1, "p%d.out", i);//nazwa programu
				if (execl(nazwa, argument1, argument2, NULL) == -1){ //zmiana na programy
					perror("EXECL error");
					exit(-1);
				}
				break;
			default:
				//program macierzysty
				break;
		}
	}

	//zakańczanie wątków
	for (int i = 0; i < 3; i++){
		w = wait(&x);
		if (w == -1){
			perror("WAIT error");//blad zakanczania procesow
			exit(-1);
		}
		else{
			printf("Zakonczenie procesu PID=%d, kod powrotu=%d\n", w, x);
		}
	}

	printf("[PROCES GŁÓWNY] Usuwanie semaforów\n");
	semafor_delete();//usuniecie semaforow

	return 0;
}
