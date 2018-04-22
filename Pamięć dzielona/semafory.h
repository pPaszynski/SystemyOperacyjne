#ifndef PROJEKT3_SEMAFORY_H
#define PROJEKT3_SEMAFORY_H
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
int semafor;

void semafor_create(void);
void semafor_set(void);
void semafor_wait(unsigned short sem_number);
void semafor_signal(unsigned short sem_number);
void semafor_delete(void);


#endif //PROJEKT3_SEMAFORY_H
