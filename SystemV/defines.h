//
// Created by vetch on 13.05.19.
//

#ifndef CW07_DEFINES_H
#define CW07_DEFINES_H

#include <sys/ipc.h>
#include <semaphore.h>

#define BELT_SIZE 2
#define MASS_EXCEEDED_ERROR -1
#define SPACE_EXCEEDED_ERROR -2

#ifdef POSIX
typedef sem_t* sem_id_t;
#else
typedef int sem_id_t;
#endif

key_t key;

int sharedMemId;
sem_id_t truckerSemaphore;
sem_id_t workerSemaphore;
sem_id_t tryInsertSemaphore;

struct box {
    int workerPID;
    int mass;
    long time;
};

struct belt{
    struct box beltArray[BELT_SIZE];
    int maxMass;
    int currentMass;

    int flag;
    int currentUsers;
};

#endif //CW07_DEFINES_H
