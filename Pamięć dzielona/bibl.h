#ifndef BIBL_H
#define BIBL_H
#include <stdlib.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <sys/sysinfo.h>
#include <linux/sched.h>
#include <sys/shm.h> 
#include <unistd.h>
#include <errno.h>
#include <stdio.h>
int semid;
void semcreate();
void semv(unsigned short);
void semp(unsigned short);
void semdelete(int semid);
void shmcreate();
void shmdelete();
void shmjoin();
void semset();


#endif //BIBL_H

