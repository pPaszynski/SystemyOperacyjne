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
    cout << "(Producent) Tworzenie pliku wejściowego" << endl;

    if (wejscie = fopen(nazwa_pliku, "w"))
    {
      char litera = 0;

      for (int i = 0; i < 1000; i++)
      {
        litera = static_cast<char>('A' + (rand() % 26));
        fputc(litera, wejscie);
      }
      fclose(wejscie);
    }

    if (!(wejscie = fopen(nazwa_pliku, "r")))
    {
      perror(nazwa_pliku);
      exit(1);
    }
  }

  const char *potok = "/home/inf-16/paszynskip/SO_Projekty/projekt7/nazwane/potok";

  if (access(potok, F_OK) == -1)
  {
    if (mkfifo(potok, 0660) == -1)
    {
      if (errno != 17)
      {
        perror("Błąd mkfifo\n");
        exit(3);
      }
    }
  }

  int otwarty_potok;
  otwarty_potok = open(potok, O_WRONLY);

  if (otwarty_potok == -1)
  {
    cout << "Otwieranie potoku do zapisu: " << errno << strerror(errno) << endl;
    exit(1);
  }

  char znak;

  while (znak != EOF)
  {
    znak = fgetc(wejscie);

    if (znak != EOF)
    {
      int uchwyt_mac;
      uchwyt_mac = write(otwarty_potok, &znak, sizeof(char));

      if (uchwyt_mac == -1)
      {
        perror("(Producent) Błąd zapisu\n");
        exit(EXIT_FAILURE);
      }
    }
  }

  if (fclose(wejscie) == EOF)
  {
    perror("(Producent) Błąd zamknięcia pliku wejściowego\n");
    exit(-1);
  }

  close(otwarty_potok);
}
