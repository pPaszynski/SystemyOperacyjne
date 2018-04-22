#include "includes.h"

int main(int argc, char *argv[])
{
  if (argc != 2)
  {
    cout << "(Producent) Niepoprawna liczba argumentów (!=1)" << endl;
    exit(1);
  }

  int arg1 = atoi(argv[1]);

  if (arg1 <= 0)
  {
    cout << "(Producent) Argument 1 musi być dodatni" << endl;
    exit(2);
  }

  char nazwa_pliku[32];
  sprintf(nazwa_pliku, "./wej%d", arg1);

  FILE *wejscie;

  srand(time(NULL));

  if (!(wejscie = fopen(nazwa_pliku, "r")))
  {
    cout << "(Producent) Nie znaleziono pliku wejściowego, tworzę nowy" << endl;
    if((wejscie = fopen(nazwa_pliku, "w")))
    {
      char litera = 0;

      for (int i = 0; i < 10; i++)
      {
        litera = static_cast<char>('A' + (rand() % 26));
        fputc(litera, wejscie);
      }

      fclose(wejscie);
    }
  }

  if (!(wejscie = fopen(nazwa_pliku, "r")))
  {
    perror(nazwa_pliku);
    exit(1);
  }

  if (sizeof(nazwa_pliku) > (PIPE_BUF * 8))
  {
    perror("Plik jest za duży\n");
    exit(-2);
  }

  char znak;

  while (znak != EOF)
  {
    znak = fgetc(wejscie);
    if (znak != EOF)
    {
      int uchwyt_mac;
      uchwyt_mac = write(1, &znak, sizeof(char));

      if (uchwyt_mac == -1)
      {
        perror("(Producent) Błąd zapisu do pliku\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  if (fclose(wejscie) == EOF)
  {
    perror("(Producent) Błąd zamykania pliku wejściowego\n");
    exit(-1);
  }

  return 0;
}
