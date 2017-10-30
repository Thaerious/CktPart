#pragma once

#pragma once
#ifndef SANCHIS_H
#define SANCHIS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "Circuit.h"
#include "BucketList.h"
#include "Definitions.h"

    typedef struct {
        BucketList* gains[2];
        int* cellMoveArray;
        int* blockMoveArray;
        int* locked;
        int* beta[2];
        int bestGain;
        int bestMove;
        int currentGain;
        int currentMove;

        Circuit* ckt;

        int level;
        int balance;
        int maxBalance;
        int minBalance;
        int lockedCount;
        int lockedNets;
        int lastCutSize;

        int passCap;
        int passCnt;
        int repairCount;
    } Sanchis;

    Sanchis* newSanchis(Circuit* ckt, int balance, int level);
    void deleteSanchis(Sanchis*);
    void Sanchis_setPassCap(Sanchis*, int i);
    void Sanchis_repair(Sanchis*, SOLUTION);
    int Sanchis_passCount(Sanchis*);
    void Sanchis_updateGains(Sanchis*, SOLUTION solution, int c);
    void Sanchis_pass(Sanchis*, SOLUTION);
    int Sanchis_move(Sanchis*, SOLUTION);
    void Sanchis_updateSolution(Sanchis*, SOLUTION);
    int Sanchis_getBestMove(Sanchis*);
    int Sanchis_moveMod(Sanchis*, SOLUTION, int);
    void Sanchis_setupVariables(Sanchis*, SOLUTION);
    void Sanchis_setupBeta(Sanchis*, SOLUTION);
    void Sanchis_setupGains(Sanchis*, SOLUTION);

#ifdef __cplusplus
}
#endif

#endif /* SANCHIS_H */