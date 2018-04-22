#include "bibl.h"
#include "semafory.h"
#include <time.h>

FILE* ptr;
extern int semid;int shmid;
int EO;
char* adres;

void linkn(){
	if((ptr = fopen("source.txt","r")) == NULL){
	perror("Nie mozna otworzyc pliku do odczytu!");
	exit(-10);	
	}
}
void connect()
{
    	adres = (char*)shmat(shmid,0,0);
    	if (adres== (char*) -1) 
     	 {
      	  printf("Problem z przydzieleniem adresu.\n");
      	  exit(EXIT_FAILURE);
     	 }    
   	 else printf("Przestrzen adresowa zostala przyznana\n");
}

void disconnect()
{
    if (shmdt(adres) == -1)
      {
        printf("Problemy z odlaczeniem pamieci dzielonej.\n");
        exit(EXIT_FAILURE);
      }
    else printf("Pamiec dzielona zostala odlaczona.\n");
}


int main()
{
	//srand( time( NULL ) );
	//int i;
	char a[2];
	linkn();
	char product;
	semafor_create();
	semafor_set();
	shmcreate();
	connect();
	while(1)
	{
		semafor_wait(1);
		if(fscanf(ptr, "%c", &a[0]) == EOF) 
		{
			*adres = -2;
			break;
		}
		//i = rand() % 3;
		//sleep(i);
		*adres = a[0];

		adres++;
		if(fscanf(ptr, "%c", &a[1]) == EOF)
		{
			*adres = -2;
			break;
		}
		//i = rand() % 3;
		//sleep(i);
		*adres = a[1];
		
		adres--;
		semafor_signal(0);
	}
//	shmdelete();
	semafor_delete();	

}
