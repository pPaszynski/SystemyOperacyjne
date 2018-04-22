#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <linux/sched.h>
//#include <linux/tasks.h>
//#include <linux/kernel/fork.c>

void delsem(int semid)
{
     if(semctl(semid,0,IPC_RMID)==-1)
     {
      perror("Can't delete semaphore");
      exit(EXIT_FAILURE);
     }
}


void sem_p(int semid,int sem_num)
  {
    int change;
    struct sembuf bufor_sem;
    bufor_sem.sem_num=sem_num;
    bufor_sem.sem_op=-1;
    bufor_sem.sem_flg=SEM_UNDO;

    while (1)
    {
		change=semop(semid,&bufor_sem,1);
		if (change == 0 || errno != 4)
			break;
	}
	if (change == -1 && errno != 4){
		perror("Can't close semaphore.");
        delsem(semid);
        exit(EXIT_FAILURE);
	}
	//state?


}
 void sem_v(int semid,int sem_num)
  {
    int change;
    struct sembuf bufor_sem;
    bufor_sem.sem_num=sem_num;
    bufor_sem.sem_op=1;
    bufor_sem.sem_flg=SEM_UNDO;
    while (1)
    {
		change=semop(semid,&bufor_sem,1);
		if (change == 0 || errno != 4)
			break;
	}
	if (change == -1 && errno != 4){
		perror("Can't open semaphore.");
        delsem(semid);
        exit(EXIT_FAILURE);
	}
	//state?
}



int main(int argc, char *argv[])
{
  if(argc!=2)
  {
    printf("Inncorrect number of argumeents.\n");
    exit(14);
  }

  key_t key= ftok("/home",'b');
    if(key==-1)
     {
      printf("Can't create key.(ftok)");
      exit(1);
     }



    int semid = semget(key,1,IPC_CREAT|0666); //0600
    if(semid == -1)
     {
      perror("Can't use semaphore.\n");
      exit(EXIT_FAILURE);
     }
  int i;
  int arg1=atoi(argv[1]);
  //printf("POTOMNY SEMID:%d | arg1:%d \n",semid,arg1);
  for(i=0;i<arg1;i++)
  {
    printf("Proces o PID:%d Przed SK.(%d).\n",getpid(),i+1);
    sem_p(semid,0);
    printf("Proces o PID:%d W trakcie SK.(%d).\n",getpid(),i+1);
    sleep(1);
    sem_v(semid,0);
    printf("Proces o PID:%d Po SK.(%d).\n",getpid(),i+1);
  }
    return 0;
}
