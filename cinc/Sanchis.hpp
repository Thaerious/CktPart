#pragma once
#include "Circuit.hpp"
#include "BucketList.hpp"
#include "Definitions.hpp"

class Sanchis{
    private:
        BucketList* gains[2];

        int* cellMoveArray;
        int* blockMoveArray;
        int* locked;
        int* beta[2];
        int  bestGain;
        int  bestMove;
        int  currentGain;
        int  currentMove;

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

    protected:
        void updateGains(SOLUTION solution, int c);
        void pass(SOLUTION);
        int  move(SOLUTION);
        void updateSolution(SOLUTION);
        int  getBestMove();
        int  move(SOLUTION, int);
        void setupVariables(SOLUTION);
        void setupBeta(SOLUTION);
        void setupGains(SOLUTION);

    public:
        Sanchis(Circuit* ckt, int balance, int level);
        virtual ~Sanchis();
        void setPassCap(int i) {passCap = i;}
        void repair(SOLUTION);
        int  passCount() {return passCnt;}
};