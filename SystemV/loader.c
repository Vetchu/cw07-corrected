//
// Created by vetch on 11.05.19.
//
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "../utils/utils.h"
#include "common.h"
#include <signal.h>
#include <sys/wait.h>

int globalflag = 0;

unsigned long workerCount;
int *children;
int maxPkgMass = 0;
struct belt *myBelt;

struct box generatePackage() {
    return (struct box) {getpid(), rand() % (maxPkgMass + 1 - 1) + 1, getCurrentTime()};
}

int insertBox(struct belt *myBelt, struct box currentBox) {

    if (currentBox.mass > calculateFreeMass(myBelt)) {
        return MASS_EXCEEDED_ERROR;
    }

    if (myBelt->beltArray[BELT_SIZE - 1].mass != 0) {
        return SPACE_EXCEEDED_ERROR;
    }

    myBelt->currentMass += currentBox.mass;
    currentBox.time = getCurrentTime();
    myBelt->beltArray[BELT_SIZE - 1] = currentBox;
    return 0;
}


int insertPackage(struct belt *currentBelt) {
    struct box currentBox = generatePackage();
    int returnCode = -1;
    long time = getCurrentTime();
    while (returnCode && myBelt->flag) {
        moveBelt(currentBelt);
        returnCode = insertBox(currentBelt, currentBox);

        if (returnCode) {
            if (returnCode == MASS_EXCEEDED_ERROR) {
                printf("%d cannot insert pkg, mass %d, mass\n",
                       currentBox.workerPID, currentBox.mass);
            } else if (returnCode == SPACE_EXCEEDED_ERROR) {
                printf("%d cannot insert pkg, mass: %d, space\n",
                       currentBox.workerPID, currentBox.mass);
            }
            unlockSemaphore(truckerSemaphore);
            lockSemaphore(tryInsertSemaphore);
//            printf("Worker %d waits\n",
//                   currentBox.workerPID);
        } else {
            printf("%d insert %d time %ld after %ld \n",
                   currentBox.workerPID, currentBox.mass,
                   myBelt->beltArray[BELT_SIZE - 1].time,
                   myBelt->beltArray[BELT_SIZE - 1].time - time);
        }
        moveBelt(currentBelt);
    }

    unlockSemaphore(tryInsertSemaphore);
    return 0;
}

void isKill() {
    //if(globalflag==0)
    //	kill(getppid(), SIGKILL);
    printf("wyłanczam siem\n");
    if (myBelt != NULL) {
        detachSharedMem(myBelt);
        //myBelt->currentUsers--;
    }
    exit(-1);
}

void workerLoop(struct belt *currentBelt) {
    currentBelt->currentUsers++;
    while (currentBelt->flag) {
//        printf("worker %d is awaiting a chance to put new package\n",getpid());
        printf("Worker %d at time %ld Waits\n", getpid(), getCurrentTime());

        while (semvalue(tryInsertSemaphore)) {
            lockSemaphore(workerSemaphore);
            if(semvalue(tryInsertSemaphore)) {
                lockSemaphore(tryInsertSemaphore);
            }
//            if (!semvalue(tryInsertSemaphore)) {
//                unlockSemaphore(workerSemaphore);
//            }
        }

        insertPackage(currentBelt);
        unlockSemaphore(truckerSemaphore);

        if (!currentBelt->flag) {
            unlockSemaphore(truckerSemaphore);
            unlockSemaphore(workerSemaphore);
            exit(-1);
        }
//        printf("worker %d inserted a new package\n",getpid());
//        printf("passing away semaphore\n");
    }
//    unlockSemaphore(workerSemaphore);

    exit(EXIT_SUCCESS);
}

void initWorker() {
    key = initkey();
    truckerSemaphore = joinSemaphores(key);
    workerSemaphore = joinSemaphores(key + 1);
    tryInsertSemaphore = joinSemaphores(key + 3);
}

void sigInt() {
    exit(EXIT_SUCCESS);
}

int main(int args, char **argv) {
    atexit(isKill);
    initWorker();
    struct sigaction sa;
    sa.sa_handler = sigInt;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);

    srand(time(NULL));
    if (args == 3) {

        workerCount = _convert_to_unsigned_long(argv[1]);
        maxPkgMass = _convert_to_unsigned_long(argv[2]);

        children = calloc(workerCount, sizeof(int));

        myBelt = (struct belt *) joinSharedMem(key + 2);
//    workerLoop(myBelt);
        for (int i = 0; i < workerCount; i++) {
            globalflag = fork();
            if (globalflag == 0) {
                workerLoop(myBelt);
            }
        }

        globalflag = 1;

        for (int i = 0; i < workerCount; i++) {
            printf("%d ", children[i]);
        }

        printf("\n");
        for (int i = 0; i < workerCount; i++) {
            wait(NULL);
        }
    } else {
        globalflag = 1;
        printf("INCORRECT ARGS\n");
    }
    return 0;
}
