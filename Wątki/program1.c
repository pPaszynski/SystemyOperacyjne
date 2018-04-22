#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define REENTRANT //definiuje wielobieznosc informacja dla preprocesora.

// uwaga zmienne lokalne są zapisywane na stosie a globalne na stercie.
// deklaracja funkcji nie wiadomo gdzie sie znajduje, ale jak powiem na stercie
// 

#define ROW 2
#define COL 10
int tab[ROW][COL];
//te nizej pozmieniac
int sum1 = 0;
int sum2 = 0;
int create_thread;
int join_thread;
int detach_thread;

void randomize()
{
  printf("[WĄTEK GŁÓWNY] TABLICA: \n");
  int i,j;
  for(i=0; i<ROW; i++)
  {
    for(j=0; j<COL; j++)
    {
      tab[i][j]=rand()%10;
      printf("%d ",tab[i][j]);
    }
    printf("\n");
  }
}

void *first_row() //  void *first_row(void *aram) nie musi byc void *aram
{
		//1 watek programu
		// sum = 0;
    unsigned long int id = 0;
    id = pthread_self();
    printf("[WĄTEK PIERWSZY] Identyfikator watku pierwszego = %lu\n",id);
		for(int i = 0; i<COL; i++) sum1=sum1 + tab[0][i];
		printf("[WĄTEK PIERWSZY] first_row: ");
		printf("[WĄTEK PIERWSZY] suma=(%d)\n\n", sum1);
		pthread_exit((void *)1); // wraca void pointer na jedynke. Bo funkcja zwraca void pointer
}

void *second_row()
{
		//2 watek programu
		// sum = 0;
    unsigned long int id = 0;
    id = pthread_self();
	  printf("[WĄTEK DRUGI] Identyfikator watku drugiego = %lu\n",id);
    for (int i = 0; i<COL; i++)	sum2+=tab[1][i];
		printf("[WĄTEK DRUGI] second_row: ");
		printf("[WĄTEK DRUGI] suma=(%d)\n\n", sum2);
		pthread_exit((void*)2);
}

int main()
{
    int all_sum = 0;
    int i = 0;
    srand(time(NULL));
    randomize();
    pthread_t id_thread1, id_thread2; // stworzenie zmiennej ktora bedzie przechowywac tid watku
    create_thread =pthread_create(&id_thread1,NULL,first_row,NULL); // argumenty dla watku, 1 NULL ATRYBUTY WATKU, funckja obslugujaca dany watek, null ostatni to argumenty
    // dla tej funckji
    if (create_thread == -1) exit(-1); // dodac jakis komunikat
    else if (create_thread == 0) printf("[WĄTEK GŁÓWNY] SUCCES CREATE P1\n");

	 create_thread = pthread_create(&id_thread2,NULL,second_row,NULL);
	 if (create_thread == -1) exit(-1); //dodac komunikat
   else if (create_thread == 0) printf("[WĄTEK GŁÓWNY] SUCCES CREATE P2\n");

   long int join_thread_succ;
   join_thread = pthread_join(id_thread1,(void**)&join_thread_succ); //join_thread -> status ktory zwara
   if (join_thread == -1) exit(-1); //dodac jakis komunikat
   printf("[WĄTEK GŁÓWNY] Wartość zwrócona przez wątek P1= %lu\n",join_thread_succ);

   long int join_thread_succ2;
   join_thread = pthread_join(id_thread2,(void**)&join_thread_succ2);
   if (join_thread == -1) exit(-1); //dodac jakis komunikat
   printf("[WĄTEK GŁÓWNY] Wartość zwrócona przez wątek P2= %lu\n\n",join_thread_succ2);

   all_sum+=sum2;
   all_sum+=sum1;

   //UWAGA WĄTEK ODŁĄCZA SIĘ DOMYŚLNIE PO JOINIE.

   detach_thread = pthread_detach(id_thread1);
   if (detach_thread == -1) exit(-1); //dodac jakis komunikat
   else if(detach_thread == 3) printf("[WĄTEK GŁÓWNY] CAN'T FIND THREAD P1 (MAYBE DETACHED AFTER JOIN)\n");
   /*else printf("[WĄTEK GŁÓWNY] SUCCES DETACH P1 code:%d\n",detach_thread);*/
   // 3 zwraca ze tego watku nie znaleziono
   // 0 ZE POMYSLNIE A ZERA KURWA NIE BEDZIE BO JUZ TEGO WATKU NIE MA XD
   // 22 jak sa bledne argumenty

   detach_thread = pthread_detach(id_thread2);
   if (detach_thread == -1) exit(-1); //dodac jakis komunikat
   else if(detach_thread == 3) printf("[WĄTEK GŁÓWNY] CAN'T FIND THREAD P2 (MAYBE DETACHED AFTER JOIN)\n");
   /*else printf("[WĄTEK GŁÓWNY] SUCCES DETACH P2 code:%d\n\n",detach_thread);*/

   printf("[WĄTEK GŁÓWNY] SUM from ROWS=%d\n",all_sum);
   // tid jeszcze glownego to selfem.
    return 0;
}
