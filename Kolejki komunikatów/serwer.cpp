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

int kolejka_id;
void alternate_signals();

int main()
{
    int this_proccess_pid = getpid();
    key_t klucz = key();

    if (klucz == -1)
    {
        perror("FTOK error");
        exit(-1);
    }

	kolejka_id = msgget(klucz, IPC_CREAT | 0600);
    if (kolejka_id == -1)
    {
        perror("QUEUE error");
        exit(-2);
    }
	else
	{
		std::cout << "Połączono. \nCzekam na wiadomości: " << std::endl;
    }

	signal(SIGTERM, reinterpret_cast<__sighandler_t>(alternate_signals));

    Message p{};
	int retval = 0;
	while (true)
	{
		memset(p.data,NULL,MAX_MESSAGE_SIZE_IN_BYTES);
		while (true)
		{
				retval = static_cast<int>(msgrcv(kolejka_id, &p, sizeof(Message) - sizeof(long), 1, 0));
				if (retval == 0 || errno != 4) break;
		}

		if (retval == -1 && errno != 4)
		{
				perror("Blad odbioru komunikatu");
				exit(-3);
		}
		else
		{
			std::cout <<  "\nOtrzymalem "<<strlen(p.data)<<"B od klienta o PID - "
			<< p.sender<< "\nTresc wiadomosci: " << p.data << std::endl;
		}

		p.type = p.sender;
		p.sender = this_proccess_pid;

		for (unsigned int i = 0; i < strlen(p.data); i++)
		{
				p.data[i] = static_cast<char>(toupper(p.data[i]));
		}

		while (true)
		{
				retval = msgsnd(kolejka_id, &p, strlen(p.data)+sizeof(long),IPC_NOWAIT);
				if (retval == 0 || errno != 4)
						break;
		}

		if (retval == -1 && errno != 4)
		{
				perror("ERROR send message");
				exit(-4);
		}
		else
		{
			std::cout<< "\nWysłałem: "<<strlen(p.data)<<"B do klienta o PID - "
                << p.type << "\nTresc wiadomosci: " << p.data << std::endl;
		}

}

}

void alternate_signals()
{
	if (msgctl(kolejka_id, IPC_RMID, 0) == -1)
	{
			perror("Serwer: blad usuwania kolejki");
			exit(-1);
	}
	else
	{
		std::cout<< "\nUsunieto kolejke" << std::endl;
	}
	std::cout << "\nOdebrano sygnal, serwer zostanie wylaczony" << std::endl;
        exit(0);
}
