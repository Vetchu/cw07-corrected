//
// Created by vetch on 13.05.19.
//

#include "common.h"
void moveBelt(struct belt *myBelt){
    for(int i=0;i<BELT_SIZE-1;i++){
        if(myBelt->beltArray[i].mass==0){
            myBelt->beltArray[i]=myBelt->beltArray[i+1];
            myBelt->beltArray[i+1]=(struct box){0};
        }
    }
}
int calculateFreeSpace(struct belt* myBelt) {
    int space = 0;
    for (int i = 0; i < BELT_SIZE; i++) {
        if (myBelt->beltArray[i].mass == 0)
            space++;
    }
    return space;
}

int calculateFreeMass(struct belt* myBelt) {
    int freemass = myBelt->maxMass;
    for (int i = 0; i < BELT_SIZE; i++) {
        freemass -= myBelt->beltArray[i].mass;
    }
    return freemass;
}
int updateIndex(int index){
    return (index+1)%BELT_SIZE;
}
key_t initkey(){
    return ftok(".",'a');
}