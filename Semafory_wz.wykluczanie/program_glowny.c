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
//strtol()??
void delsem(int semid)
{
     if(semctl(semid,0,IPC_RMID)==-1)
     {
      perror("Can't delete semaphore");
      exit(EXIT_FAILURE);
     }
}


int main(int argc, char *argv[])
{
    if(argc!=4)
    {
     printf("Inncorrect number of argumeents.\n");
     exit(10);
    }
  //  struct rlimit {
  //             rlim_t rlim_cur;  /* Soft limit */
  //             rlim_t rlim_max;  /* Hard limit (ceiling for rlim_cur) */
  //         };*/
    struct rlimit softlimit;
    if(getrlimit(RLIMIT_NPROC,&softlimit)==-1)
    {
      perror("Can't get limit");
      exit(11);
    }

    int max_processes=softlimit.rlim_cur;
/*
    struct sysinfo info;
    if(sysinfo(&info)==-1)
    {
      perror("Can't get number of current processes");
      exit(12);
    }
*/
    if(getenv("CUR")==0)
    {
     printf("Environment variable does not exist.\n"); 
     exit(20);
    }
    int cur_processes=atoi(getenv("CUR"));


    key_t key= ftok("/home",'b');
    if(key==-1)
     {
      printf("Can't create key.(ftok)");
      exit(1);
     }



    int semid = semget(key,1,IPC_CREAT|0666); //0600
    if(semid == -1)
     {
      perror("Can't create semaphore.\n");
      exit(EXIT_FAILURE);
     }
    unsigned long int setsem;

    setsem=semctl(semid,0,SETVAL,(int)1);  //czu tu musi być &???
    if(setsem==-1)
     {
      perror("Can't set semaphore");
      delsem(semid);
      exit(EXIT_FAILURE);
     }

    int arg2=atoi(argv[2]);

    int i;
    if(max_processes-cur_processes-arg2<0)
    {
        printf("Can't open %d processes.",arg2);
        delsem(semid);
	exit(13);
    }

    //printf("GLOWNY SEMID:%d | arg2:%d |arg3:%s \n",semid,arg2,argv[3]);
    if(arg2<=0||atoi(argv[3])<=0)
    {
        printf("Invalid arguments\n.");
        exit(14);
    }

    for(i=0;i<arg2;i++)
    {
        switch(fork())
        {
         case -1:
         perror("Can't fork()");
         break;

         case 0:
         if( execl(argv[1],argv[1],argv[3],NULL)==-1)
         {
          perror("Can't execl()");
         }
         break;

         default:
         break;
        }
    }

    int pid,returncode;
    for(i=0;i<arg2;i++)
    {
      pid=wait(&returncode);
      if(pid==-1)
      {
        printf("Wait() error. \n");
      }
      else
      printf("Pid:%d Return code:%d \n",pid,returncode);
    }
    printf("Deleting semaphore.\n");
    delsem(semid);
    return 0;
}
