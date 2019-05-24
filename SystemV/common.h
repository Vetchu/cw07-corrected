//
// Created by vetch on 11.05.19.
//

#ifndef CW07_COMMON_H
#define CW07_COMMON_H


//SHARED MEM
#include <sys/shm.h>
#include "defines.h"
key_t initkey();

sem_id_t createSemaphoreSet();
sem_id_t joinSemaphores();
int lockSemaphore(sem_id_t id);
int unlockSemaphore(sem_id_t id);
int semvalue(sem_id_t id);
int removeSemaphores(sem_id_t id,int key);

int createSharedMem();
struct belt* joinSharedMem(int id);
int  detachSharedMem(struct belt* myBelt);
int removeSharedMemory(int id,int key);

void moveBelt(struct belt *myBelt);
int calculateFreeMass(struct belt* myBelt);
int calculateFreeSpace(struct belt* myBelt);
#endif //CW07_COMMON_H
