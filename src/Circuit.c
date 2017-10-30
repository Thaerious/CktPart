#include <stdio.h>
#include <stdlib.h>
#include "Circuit.h"
#include "Definitions.h"

int* NetList_construct(FILE* fp, int* size){
    if (fscanf(fp, "%d", size) != 1) return NULL;
    int* this = calloc(*size, sizeof(int));

    for (int i = 0; i < *size; i++){
        if (fscanf(fp, "%d", &this[i]) != 1) return NULL;
    }

    return this;
}

Circuit* newCircuit(char* filename){
    Circuit* this = calloc(1, sizeof(Circuit));

	FILE* fp = fopen(filename, "r");

    if (fp == NULL){
        fprintf(stderr, "file not found: %s\n", filename);
        exit(1);
    }

    /* potential memory leak on error */
    if (fscanf(fp, "modcount %d\n", &this->modCount) != 1) return NULL;
	if (fscanf(fp, "netcount %d\n", &this->netCount) != 1) return NULL;

    this->netSize = calloc(this->netCount, sizeof(int));
    this->modSize = calloc(this->modCount, sizeof(int));
    this->nets = calloc(this->netCount, sizeof(int*));
    this->mods = calloc(this->modCount, sizeof(int*));

    for (int n = 0; n < this->netCount; n++){
        int* net = NetList_construct(fp, &this->netSize[n]);
        if (net == NULL) return NULL;
        for (int m = 0; m < this->netSize[n]; m++){
            int modIdx = net[m];
            this->modSize[modIdx]++;
        }
        this->nets[n] = net;
    }

    for (int m = 0; m < this->modCount; m++){
        this->mods[m] = calloc(this->modSize[m], sizeof(int));
        this->modSize[m] = 0;
    }

    for (int n = 0; n < this->netCount; n++){
        int* net = this->nets[n];
        for (int m = 0; m < this->netSize[n]; m++){
            int* mod = this->mods[net[m]];
            mod[this->modSize[net[m]]++] = n;
        }
    }

	fclose(fp);
    return this;
}

void deleteCircuit(Circuit* this){
    for (int m = 0; m < this->modCount; m++) free(this->mods[m]);
    for (int n = 0; n < this->netCount; n++) free(this->nets[n]);
    free(this->mods);
    free(this->nets);
    free(this->modSize);
    free(this->netSize);
    free(this);
}

int cutSize(Circuit* this, SOLUTION solution) {
    int rvalue = 0;

    for (int n = 0; n < this->netCount; n++){
        int* net = this->nets[n];
        for (int m = 1; m < this->netSize[n]; m++){
            if (solution[(net[m])] != solution[(net[0])]){
                rvalue++;
                break;
            }
        }
    }

    return rvalue;
}
