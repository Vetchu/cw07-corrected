//
// Created by vetch on 11.05.19.
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>
#include "defines.h"
#include <fcntl.h>

struct sembuf* lockSem;
struct sembuf* unlockSem;

void initOperations(){
    lockSem =calloc(1, sizeof(struct sembuf));
    unlockSem = calloc(1, sizeof(struct sembuf));

    lockSem->sem_num=unlockSem->sem_num=0;
    lockSem->sem_flg=unlockSem->sem_flg=0;
    lockSem->sem_op=-1;
    unlockSem->sem_op=1;
}

int createSemaphoreSet(key_t key){
    int  semid;

    if((semid=semget(key, 1, 0644 | IPC_CREAT))==-1){
        perror("couldnt create semaphore set\n");
        exit(EXIT_FAILURE);
    }

    union semun {
        int              val;    /* Value for SETVAL */
        struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
        unsigned short  *array;  /* Array for GETALL, SETALL */
        struct seminfo  *__buf;  /* Buffer for IPC_INFO(Linux-specific) */
    } semopts;

    semopts.val=0;

    if((semctl(semid, 0, SETVAL, semopts))==-1){
        perror("couldnt  set value to semaphores");
        exit(EXIT_FAILURE);
    }

    initOperations();
    return semid;
}

int joinSemaphores(key_t key){
    int semid;
    if((semid=semget(key, 1, 0))==-1){
        perror("couldnt  join semaphore\n");
        exit(EXIT_FAILURE);
    }
    initOperations();

    return semid;
}



int lockSemaphore(int id){
    if(semop(id,lockSem,1)==-1){
        //perror("couldnt lock semaphore");
        return -1;
    }
    return 0;
}
int unlockSemaphore(int id){
    if(semop(id,unlockSem,1)==-1){
        //perror("couldnt unlock semaphore");
        return -1;
    }
    return 0;
}
int semvalue(int id){
    return semctl(id,0,GETVAL,0);
}

int removeSemaphores(int id,int key){
    if(semctl(id, 0, IPC_RMID,0)==-1){
        perror("could not remove semaphore");
        return -1;
    }
    return 0;
}



int createSharedMem(sem_id_t id){
    int identifier=0;
    if((identifier=shmget(key, sizeof(struct belt),IPC_CREAT| 0777))==-1){
        perror("couldnt  create shared memory\n");
    } else{
        (void) fprintf(stderr, "shmget: shmget returned %d\n", identifier);
    }
    return identifier;
}

struct belt* joinSharedMem(sem_id_t id){
    int identifier=0;
    if((identifier=shmget(key, 0, 0777))==-1){
        perror("couldnt  get id of shared memory\n");
    } else{
        (void) fprintf(stderr, "shmget: shmget returned %d\n", identifier);
    }
    struct belt *myBelt = (struct belt*) shmat(identifier, (void*) 0,0);


    return myBelt;
}

int  detachSharedMem(struct belt* myBelt){
    if(shmdt(myBelt)==-1){
        perror("cannot detach shared mem");
        return -1;
    }
    return 0;
}


int removeSharedMemory(int id){
    if(shmctl(id, IPC_RMID, NULL)==-1){
        perror("Could   not  remove memory \n");
        return -1;
    }
    return 0;
}
