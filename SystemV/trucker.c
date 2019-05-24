#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include "../utils/utils.h"
#include "common.h"

struct belt *myBelt;
struct truck {
    int currentMass;
    int maxMass;
};


struct box getBox(struct belt *myBelt) {
    if (myBelt->beltArray[0].mass == 0) {
        return (struct box) {0};
    }

    struct box currentBox = myBelt->beltArray[0];
    myBelt->currentMass -= currentBox.mass;
    myBelt->beltArray[0] = (struct box) {0};
    return currentBox;
}

int getPackage(struct belt *myBelt) {
    struct box currentBox = getBox(myBelt);

    if (currentBox.workerPID == 0) {
        return -1;
    }

    printf("Mass: %d,\t From: %d,\t Timediff: %ld,\t Timestamp: %ld\n",
           currentBox.mass, currentBox.workerPID, getCurrentTime() - currentBox.time, currentBox.time);

    return currentBox.mass;
}


void initTruck() {
    key = initkey();

    truckerSemaphore = createSemaphoreSet(key);
    workerSemaphore = createSemaphoreSet(key + 1);
    tryInsertSemaphore = createSemaphoreSet(key + 3);

    unlockSemaphore(tryInsertSemaphore);
    unlockSemaphore(workerSemaphore);

    sharedMemId = createSharedMem(key + 2);
}

int globalflag = 1;




void truckLoop(struct belt *myBelt, int maxWeight, int cycles) {
    for (int i = 0; i < BELT_SIZE; i++) {
        myBelt->beltArray[i].workerPID = 0;
        myBelt->beltArray[i].mass = 0;
        myBelt->beltArray[i].time = 0;
    }
    struct truck Truck = {0, maxWeight};

    int lastWeight = 0;
    printf("================\n");
    printf("==Truck enters==\n");
    printf("================\n");

    while (globalflag) {

        lockSemaphore(truckerSemaphore);

        moveBelt(myBelt);
        int space = calculateFreeSpace(myBelt);
        int freemass = calculateFreeMass(myBelt);
        printf("Free space %d\nFree mass %d\n", space, freemass);
        if (lastWeight == 0) {
            lastWeight = getPackage(myBelt);
        }
        if (lastWeight == -1) {
            printf("Trucker Waits\n");
            lastWeight = 0;
        }

        if (semvalue(tryInsertSemaphore)) {
            unlockSemaphore(workerSemaphore);
        } else {
            unlockSemaphore(tryInsertSemaphore);
        }

        if (Truck.currentMass + lastWeight <= Truck.maxMass) {
            Truck.currentMass += lastWeight;
            if (Truck.maxMass == Truck.currentMass) {
                printf("Truck leaves with mass %d/%d\n",
                       Truck.currentMass, Truck.maxMass);
                printf("================\n");
                printf("==Truck enters==\n");
                printf("================\n");

                Truck.currentMass = 0;
            }
            lastWeight = 0;
        } else {
            printf("Truck leaves with mass %d/%d\n",
                   Truck.currentMass, Truck.maxMass);
            printf("================\n");
            printf("==Truck enters==\n");
            printf("================\n");

            Truck.currentMass = 0;
        }

    }
}


void isKill() {
    myBelt->flag = 0;

    globalflag = 0;
    unlockSemaphore(tryInsertSemaphore);
    unlockSemaphore(tryInsertSemaphore);
    unlockSemaphore(tryInsertSemaphore);
    unlockSemaphore(workerSemaphore);
    unlockSemaphore(workerSemaphore);
    unlockSemaphore(workerSemaphore);




    while (calculateFreeSpace(myBelt) != BELT_SIZE) {
        moveBelt(myBelt);
        getPackage(myBelt);
    }

    printf("wyłanczam siem\n");
    removeSemaphores(workerSemaphore, key);
    removeSemaphores(truckerSemaphore, key + 1);
    removeSemaphores(tryInsertSemaphore, key + 3);

    detachSharedMem(myBelt);
    removeSharedMemory(sharedMemId, key + 2);
}

void sigInt() {
    exit(EXIT_SUCCESS);
}

int main(int args, char **argv) {
    struct sigaction sa;
    sa.sa_handler = sigInt;

    sigaction(SIGINT, &sa, NULL);
    sigaction(SIGTERM, &sa, NULL);
    if (args == 3) {
        atexit(isKill);

        initTruck();
        unsigned long maxTruckMass = _convert_to_unsigned_long(argv[1]);
        if(maxTruckMass==ARGUMENT_ERROR){
            exit(-1);
        }
        unsigned long maxBeltWeight = _convert_to_unsigned_long(argv[2]);
        if(maxBeltWeight==ARGUMENT_ERROR){
            exit(-1);
        }
        myBelt = (struct belt *) joinSharedMem(key + 2);
        myBelt->maxMass = maxBeltWeight;
        myBelt->currentMass = 0;
        myBelt->flag = 1;
        myBelt->currentUsers = 0;

        truckLoop(myBelt, maxTruckMass, 2147483647);
    } else {
        printf("INCORRECT ARGS\n");
        exit(-1);
    }
    return 0;
}
