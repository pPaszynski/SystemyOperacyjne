#include "includes.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    cout << "(Konsument) Niepoprawna liczba argumentów (!=1)" << endl;
    exit(1);
  }

  int arg1 = atoi(argv[1]);

  if (arg1 <= 0)
  {
    cout << "(Konsument) Argument 1 musi być dodatni" << endl;
    exit(2);
  }

  char nazwa_pliku[32];
  sprintf(nazwa_pliku, "./wyj%d", arg1);

  FILE *wyjscie;

  if ((wyjscie = fopen(nazwa_pliku, "w")) == NULL)
  {
    perror("(Konsument) Błąd otwarcia pliku wyjściowego\n");
    exit(1);
  }

  sleep(2);

  char bufor;
  int uchwyt_pot;

  while (uchwyt_pot = read(0, &bufor, sizeof(bufor)) > 0)
  {
    printf("(Konsument) Odczytano dane: %c - kons. nr %d\n", bufor, arg1);
    
    if (fputc(bufor, wyjscie) == EOF)
    {
      perror("(Konsument) Błąd zapisu do pliku\n");
      exit(-10);
    }

    if (uchwyt_pot == -1)
    {
      perror("Błąd odczytu z potoku\n");
      exit(EXIT_FAILURE);
    }
  }

  if (fclose(wyjscie) == EOF)
  {
    printf("(Konsument) Błąd zamykania pliku wyjściowego\n");
    exit(-1);
  }

  return 0;
}
