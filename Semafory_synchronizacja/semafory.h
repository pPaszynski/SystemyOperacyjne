//
// Created by rafal on 10.12.17.
//

#ifndef PROJEKT3_SEMAFORY_H
#define PROJEKT3_SEMAFORY_H

int semafor;

void semafor_create(void);
void semafor_set(void);
void semafor_wait(unsigned short sem_number);
void semafor_signal(unsigned short sem_number);
void semafor_delete(void);


#endif //PROJEKT3_SEMAFORY_H
