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

  if (!(wyjscie = fopen(nazwa_pliku, "w")))
  {
    perror("(Konsument) Błąd otwarcia pliku wyjściowego\n");
    exit(1);
  }

//  sleep(2);

  const char *potok = "/home/inf-16/paszynskip/SO_Projekty/projekt7/nazwane/potok";

  if (access(potok, F_OK) == -1)
  {
    cout << "(Konsument) Brak dostępu do potoku" << endl;
    exit(2);
  }

  int otwarty_potok;
  otwarty_potok = open(potok, O_RDONLY | O_NONBLOCK);

  if (otwarty_potok == -1)
  {
    perror("(Konsument): Błąd otwarcia potoku\n");
    exit(1);
  }

  char bufor;
  int uchwyt_pot;

  while(1)
  {
    ssize_t retval = read(otwarty_potok, &bufor, sizeof(bufor));

    if (retval <= 0)
    {
      if (retval == -1 && errno != 11)
      {
        perror("(Konsument) Błąd odczytu\n");
        exit(6);
      }

      if (retval == 0)
        break;
    }
    else
    {
      printf("(Konsument) Odczytane dane: %c - kons. nr %d\n", bufor, atoi(argv[1]));
      fputc(bufor, wyjscie);
      fflush(wyjscie);
    }
  }

  if (fclose(wyjscie) == EOF)
  {
    printf("(Konsument) Błąd zamykania pliku wyjściowego\n");
    exit(-1);
  }

  close(otwarty_potok);
  unlink(potok);
  return 0;
}
