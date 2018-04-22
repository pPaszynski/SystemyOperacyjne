#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>
#define REENTRANT //definiuje wielobieznosc informacja dla preprocesora.
#define wiersz 2
#define kolumna 10

int tab[wiersz][kolumna];
int suma1 = 0, suma2 = 0;
int create_thread, q, detach_thread;

void losowanie_liczb()
{
  printf("\n\nWylosowane liczby to: \n");
  int i,j;
  for(i=0; i<wiersz; i++)
  {
    for(j=0; j<kolumna; j++)
    {
      tab[i][j]=rand()%10;
      printf("%d ",tab[i][j]);
    }
    printf("\n\n");
  }
}

void *pierwszy_wiersz()
{
    unsigned long int id_watka = 0;
    id_watka = pthread_self();
    printf("ID watku pierwszego = %lu\n",id_watka);
        for(int i = 0; i<kolumna; i++) suma1+=tab[0][i];
        printf("Suma pierwszego wiersza = %d\n", suma1);
        pthread_exit((void *)1);
}

void *drugi_wiersz()
{
    unsigned long int id_watka = 0;
    id_watka = pthread_self();
      printf("ID dla watku drugiego = %lu\n",id_watka);
    for (int i = 0; i<kolumna; i++)    suma2+=tab[1][i];
        printf("Suma drugiego wiersza = %d\n", suma2);
        pthread_exit((void*)2);
}

int main()
{
    int suma = 0;
    int i = 0;
    srand(time(NULL));
    losowanie_liczb();
    pthread_t id_w1, id_w2;
    create_thread =pthread_create(&id_w1,NULL,pierwszy_wiersz,NULL); // argumenty dla watku, 1 NULL ATRYBUTY WATKU, funckja obslugujaca dany watek, null ostatni to argumenty
    // dla tej funckji
    if (create_thread == -1) {exit(-1); printf("blad create_thread");}

     create_thread = pthread_create(&id_w2,NULL,drugi_wiersz,NULL);
     if (create_thread == -1) {exit(-1); printf("blad create_thread");}

   long int status_w1;
   q = pthread_join(id_w1,(void**)&status_w1);
   if (q == -1) {exit(-1); printf("blad join_thread");}
   //printf("Zwrocone z joina watku pierwszego = %lu\n",status_w1);

   long int status_w2;
   q = pthread_join(id_w2,(void**)&status_w2);
   if (q == -1) {exit(-1); printf("blad join_thread");}
   //printf("Zwrocone z joina watku drugiego = %lu\n\n",status_w2);

   suma = suma1 + suma2;

   detach_thread = pthread_detach(id_w1);
   if (detach_thread == -1) {exit(-1); printf("blad detach_thread");}

   detach_thread = pthread_detach(id_w2);
   if (detach_thread == -1) {exit(-1); printf("blad detach_thread");}

   printf("Suma z obu procesow(czyli obu wierszy) = %d\n\n",suma);
    return 0;
}

