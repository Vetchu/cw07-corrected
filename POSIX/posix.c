//
// Created by vetch on 13.05.19.
//

#include "posix.h"

//
// Created by vetch on 11.05.19.
//
#include <stdlib.h>
#include <stdio.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/mman.h>

#include <unistd.h>
#include "../SystemV/defines.h"
#include <fcntl.h>
#include <semaphore.h>
#include <sys/stat.h>


#define _GNU_SOURCE
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <sys/mman.h>
#include <unistd.h>
#include "error.h"

int createSharedMem(int key) {
    char path[32];
    sprintf(path, "/%d", key);

    int id = shm_open(path, O_RDWR | O_CREAT , 0644);
    if (id == -1) {
        perror("unable to create shared memory");
        exit(-1);
    }

    int result = ftruncate(id, sizeof(struct belt));
    if (result == -1) {
        perror("unable to resize shared memory to %dB");
    }

    return id;
}

void* joinSharedMem(int key) {
    char path[32];
    sprintf(path, "/%d", key);

    int id = shm_open(path, O_RDWR, 0);
    if (id == -1) {
        perror("unable to open shared memory");
    }
    void* ptr = mmap(NULL, sizeof(struct belt), PROT_READ | PROT_WRITE, MAP_SHARED, id, 0);
    if (ptr == (void*)-1) {
        perror("unable to map shared memory");
    }

    return ptr;
}

void detachSharedMem(void* ptr) {
    if (munmap(ptr, sizeof(struct belt)) == -1) {
        perror("unable to unmap shared memory");
    }
}

void removeSharedMemory(int id,int key) {
    char path[32];
    sprintf(path, "/%d", key);

    if (shm_unlink(path) == -1) {
        perror("unable to remove shared memory");
    }
}

int unlockSemaphore(sem_id_t id) {
    if (sem_post(id) == -1) {
        perror("unable to unlock semaphore");
        return -1;
    }
    return 0;
}
int semvalue(sem_id_t id){
    int val=0;
    sem_getvalue(id,&val);
    return val;
}
sem_id_t joinSemaphores(int key) {
    char path[32];
    sprintf(path, "/%d", key);

    sem_id_t id = sem_open(path, O_RDWR, 0644);

    if (id == SEM_FAILED) {
        perror("unable to open semaphore");
        exit(-1);
    }
    unlockSemaphore(id);

    return id;
}

int lockSemaphore(sem_id_t id) {
    if (sem_wait(id) == -1) {
        perror("unable to lock semaphore");
        return -1;
    }
    return 0;
}



sem_id_t createSemaphoreSet(int key) {
    char path[32];
    sprintf(path, "/%d", key);

    sem_id_t id = sem_open(path, O_RDWR | O_CREAT , 0644,0);

    if (id == NULL) {
        perror("unable to create semaphore");
        exit(-1);
    }

    return id;
}

void close_semaphore(sem_id_t id) {
    if (sem_close(id) == -1) {
        perror("unable to close semaphore");
    }
}

void removeSemaphores(sem_id_t id,int key) {
    close_semaphore(id);

    char path[32];
    sprintf(path, "/%d", key);

    if (sem_unlink(path) == -1) {
        printf("%ld",id->__align);
        perror("unable to remove semaphore");
    }
}
