/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/*
 * File:   ComponentList.h
 * Author: edward
 *
 * Created on October 10, 2017, 9:36 AM
 */
#pragma once
#ifndef CIRCUIT_H
#define CIRCUIT_H

#include "Definitions.h"

#ifdef __cplusplus
extern "C" {
#endif

    typedef int* NET;
    typedef int* MOD;

    typedef struct {
        int** nets;
        int** mods;
        int* modSize;
        int* netSize;
        int netCount;
        int modCount;
    } Circuit;

    Circuit* newCircuit(char* filename);
    void deleteCircuit(Circuit*);
    int cutSize(Circuit*, SOLUTION solution);

    #define modCount(ckt) (ckt->modCount)
    #define netCount(ckt) (ckt->netCount)
    #define netSize(ckt, n)  (ckt->netSize[n])
    #define modSize(ckt, m)  (ckt->modSize[m])
    #define getNet(ckt, n)  (ckt->nets[n])
    #define getMod(ckt, m)  (ckt->mods[m])

#ifdef __cplusplus
}
#endif

#endif /* CIRCUIT_H */

