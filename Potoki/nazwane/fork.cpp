// 3.5

#include "includes.h"

int main(int argc, char *argv[])
{
  if (argc != 3)
  {
    cout << "(Fork) Niepoprawna liczba argumentów (!=2)" << endl;
    exit(-1);
  }

  int liczba_prod = atoi(argv[1]);
  int liczba_kons = atoi(argv[2]);

  system("./limit.sh");
  char ulimit[40];
  FILE *plik;
  plik = fopen("./limit", "r");

  if (!plik)
  {
    cout << "Nie udało się otworzyć pliku z limitem procesów" << endl;
    exit(1);
  }
  else
    while (!feof(plik))
      fgets(ulimit, 64, plik);

  fclose(plik);

  int limit = (int) strtol(ulimit, NULL, 10);

  cout << "Limit procesów potomnych: " << limit << endl;

  if (liczba_prod < 0)
  {
    cout << "Liczba producentów musi być większa od 0" << endl;
    exit(4);
  }

  if (liczba_kons < 0)
  {
    cout << "Liczba konsumentów musi być większa od 0" << endl;
    exit(4);
  }

  if (liczba_prod + liczba_kons > limit)
  {
    cout << "Przekroczono limit procesów" << endl;
    exit(4);
  }

  char bufor[5];
  char nazwa_pliku[32];

  for (int i = 0; i < liczba_prod; i++)
  {
    sprintf(bufor, "%d", i+1);

    switch(fork())
    {
      case -1:
        cout << "Błąd tworzenia producenta: " << errno << "(" << strerror(errno) << ")" << endl;
        exit(-4);
      case 0:
        if (execl("./producent", "producent", bufor, NULL) == -1)
        {
          cout << "Błąd exec (producent): " << errno << "(" << strerror(errno) << ")" << endl;
          exit(-4);
        }
    }
  }

  for (int i = 0; i < liczba_kons; i++)
  {
    sprintf(bufor, "%d", i+1);

    switch(fork())
    {
      case -1:
        cout << "Błąd tworzenia konsumenta: " << errno << "(" << strerror(errno) << ")" << endl;
        exit(-4);
      case 0:
        if (execl("./konsument", "konsument", bufor, NULL) == -1)
        {
          cout << "Błąd exec (konsument): " << errno << "(" << strerror(errno) << ")" << endl;
          exit(-4);
        }
    }
  }

  for (int i = 0; i < liczba_prod + liczba_kons; i++)
    wait(NULL);

  return 0;
}
