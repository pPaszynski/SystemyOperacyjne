#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cctype>

#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <csignal>

#include <cerrno>
#include <unistd.h>
#define MAX_MESSAGE_SIZE_IN_BYTES 1024

struct Message{
        long type;
        long sender;
        char data[MAX_MESSAGE_SIZE_IN_BYTES];
};

key_t key(){
	key_t key = ftok("/home", 340);

        if (key == -1){
                perror("FTOK error:");
                exit(-1);
        }
	else
            	return key;
}

int kolejka_id = 0;
int this_proccess_pid = 0;

void alternate_signals();
void *odbieranie(void *);
void *wysylanie(void *);
std::string temp;

int main()
{
        this_proccess_pid = getpid();
        key_t klucz = key();

        if (klucz == -1)
        {
                perror("FTOK error");
                exit(-1);
        }

	kolejka_id = msgget(klucz, 0600 | IPC_CREAT);
        if (kolejka_id == -1)
        {
                perror("QUEUE error");
                exit(-2);
        }
	else{std::cout<<kolejka_id<<std::endl;}
	std::cout << "PID Klienta - " << this_proccess_pid << " ID Kolejki - " << kolejka_id << ""<< std::endl;

        signal(SIGINT, reinterpret_cast<__sighandler_t>(alternate_signals));
		pthread_t odbieranieID, wysylanieID;
if (pthread_create(&odbieranieID, 0, odbieranie, 0) == -1)
{
		perror("Klient: blad tworzenia watku(odbieranie)");
		exit(-9);
}
if (pthread_create(&wysylanieID, 0, wysylanie, 0) == -1)
{
		perror("Klient: blad tworzenia watku(wysylanie)");
		exit(-9);
}
if (pthread_join(odbieranieID, 0) == -1)
{
		perror("Klient: blad dolaczania watku(odbieranie)");
		exit(-9);
}
if (pthread_join(wysylanieID, 0) == -1)
{
		perror("Klient: blad dolaczania watku(wysylanie)");
		exit(-9);
}
}


void alternate_signals()
{
if (msgctl(kolejka_id, IPC_RMID, 0) == -1)
{
		perror("Klient: Blad usuwania kolejki");
		exit(-1);
}
else
{
	std::cout<< "\nUsunieto kolejke"<< std::endl;
}
std::cout << "\nOdebrano sygnal, zakonczono klienta" << std::endl;
exit(0);
}

void *odbieranie(void *)
{
Message p{};
int retval = 0;

while (true)
{
		while (true)
		{
				memset(p.data,NULL,MAX_MESSAGE_SIZE_IN_BYTES);
				retval = static_cast<int>(msgrcv(kolejka_id, &p, sizeof(Message) - sizeof(long), this_proccess_pid, 0));
				if (retval == 0 || errno != 4)
						break;
		}

		if (retval == -1 && errno != 4)
		{
				if (errno == EIDRM)
				{
						std::cout << "Serwer odlaczony" << std::endl;
						exit(10);
				}
				else
				{
						perror("Klient blad odbioru");
						exit(4);
				}
		}
		else
		{
				std::string dane((char *) p.data, strlen(p.data));
				std::cout << "\nKlient o PID - " << this_proccess_pid << " Odebrano od PID - " 
					  <<p.sender << "\nTresc wiadomosci: "<<dane<<std::endl
					  <<"Wpisz wiadomosc: ";
		}
}

pthread_exit((void *) 0);
}

void *wysylanie(void *)
{
msqid_ds buf{};
msgctl(kolejka_id, IPC_STAT, &buf);
msglen_t max_ilosc_wiadomosci = (buf.msg_qbytes / (sizeof(Message) - sizeof(long))) -1;

while (true)
{
		std::string data,data1;
		std::cout << "\nWpisz wiadomosc: ";
		std::getline(std::cin, data);

		if (data.length() > 4096)
		{
				std::cout << "Wiadomosc zbyt długa spróbuj < 4096" << std::endl;
				continue;
		}

		if (data.length() == 0)
		{
				std::cout << "Wiadomosc pusta" << std::endl;
		}

		Message p{};
		p.type = 1;
		p.sender = this_proccess_pid;

		while (data.length() > 0)
		{
				memset(p.data,NULL,MAX_MESSAGE_SIZE_IN_BYTES);
				if (data.length() > MAX_MESSAGE_SIZE_IN_BYTES)
				{
						data1=data;
						data[MAX_MESSAGE_SIZE_IN_BYTES-1]='\0';
						memcpy(p.data, data.data(), MAX_MESSAGE_SIZE_IN_BYTES);
						data = data1.substr(MAX_MESSAGE_SIZE_IN_BYTES-1);
				}
				else
				{
						memcpy(p.data, data.data(), data.length());
						data = "";
				}

				msgctl(kolejka_id, IPC_STAT, &buf);

				while (buf.msg_qnum >= max_ilosc_wiadomosci)
				{
						std::cout << "Kolejka pelna czekam na serwer ..." << std::endl;
						sleep(1);
						msgctl(kolejka_id, IPC_STAT, &buf);
				}

				int retval = 0;
				while (true)
				{
						temp = p.data;
						retval = msgsnd(kolejka_id, &p, strlen(p.data) + sizeof(long),IPC_NOWAIT);
						if (retval == 0 || errno != 4 || errno != EAGAIN)
								break;
				}

				if ((retval == -1 && errno != 4) || (retval == -1 && errno != EAGAIN))
				{
						perror("Blad wysylania wiadomosci");
						pthread_exit((void *) 0);
				}
				else
				{
						std::cout << "\nKlient o PID - " << this_proccess_pid << "\tWyslano wiadomosc: "
						<< p.data;
						//sleep(1);
				}
		}
}

pthread_exit((void *) 0);
}
