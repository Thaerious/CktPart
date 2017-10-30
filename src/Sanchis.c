#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "Sanchis.h"

#define setPassCap(i) (Sanchis_setPassCap(this, i)
#define repair(s) (Sanchis_repair(this, s))
#define passCount() (Sanchis_passCount(this))
#define updateGains(s, i) (Sanchis_updateGains(this, s, i))
#define pass(s) (Sanchis_pass(this, s))
#define move(s) (Sanchis_move(this, s))
#define updateSolution(s) (Sanchis_updateSolution(this, s))
#define getBestMove() (Sanchis_getBestMove(this))
#define moveMod(s, i) (Sanchis_moveMod(this, s, i))
#define setupVariables(s) (Sanchis_setupVariables(this, s))
#define setupBeta(s) (Sanchis_setupBeta(this, s))
#define setupGains(s) (Sanchis_setupGains(this, s))

#define updateMaxModule(bl) (BucketList_updateMaxModule(bl))
#define insert(bl, a, b) (BucketList_insert(bl, a, b))
#define compare(bl, aa, bb) (BucketList_compare(bl, aa, bb))
#define setup(bl, degree, level, modcount) (BucketList_setup(bl, degree, level, modcount))
#define recalculateMax(bl) (BucketList_recalculateMax(bl))
#define set(bl, m) (BucketList_set(bl, m))
#define remove(bl, m) (BucketList_remove(bl, m))
#define reset(bl) (BucketList_reset(bl))
#define removeMax(bl) (BucketList_removeMax(bl))
#define getMax(bl) (BucketList_getMax(bl))
#define getVector(bl, m) (BucketList_getVector(bl, m))
#define size(bl) (BucketList_size(bl))

Sanchis* newSanchis(Circuit* ckt, int ibal, int level) {
    Sanchis* this = calloc(1, sizeof(Sanchis));

    this->ckt = ckt;
    float fbal = (float) ibal / 50.0;

    int sumWeight = 0;
    for (int i = 0; i < modCount(this->ckt); i++) sumWeight += 1;
    this->balance = 0;

    if (ibal != 0) {
        this->maxBalance = (int) (fbal * (float) sumWeight);
        this->minBalance = (int) (fbal * -1 * (float) sumWeight);
    } else {
        this->minBalance = -1;
    }

    this->level = level;
    this->gains[0] = newBucketList(modCount(this->ckt), level);
    this->gains[1] = newBucketList(modCount(this->ckt), level);
    this->lockedNets = 0;
    this->passCap = 0;
    this->repairCount = 0;
    this->passCnt = 0;

    this->cellMoveArray = (int*) calloc(modCount(this->ckt), sizeof (int));
    this->blockMoveArray = (int*) calloc(modCount(this->ckt), sizeof (int));
    this->locked = (int*) calloc(modCount(this->ckt), sizeof (int));
    this->beta[0] = (int*) calloc(netCount(this->ckt), sizeof (int));
    this->beta[1] = (int*) calloc(netCount(this->ckt), sizeof (int));

    return this;
}

void deleteSanchis(Sanchis* this) {
//    free(cellMoveArray);
//    free(blockMoveArray);
//    free(locked);
//    free(beta[0]);
//    free(beta[1]);
//    delete(this->gains[0]);
//    delete(this->gains[1]);
}

void Sanchis_setPassCap(Sanchis* this,  int i) {
    this->passCap = i;
}

int Sanchis_passCount(Sanchis* this) {
    return this->passCnt;
}

void Sanchis_repair(Sanchis* this,  SOLUTION solution) {
    int curCutSize = cutSize(this->ckt, solution);
    int before = curCutSize;
    int pCount = 0;

    do {
        this->lastCutSize = curCutSize;
        pass(solution);
        pCount++;
        this->passCnt++;
        curCutSize = cutSize(this->ckt, solution);
        if (pCount >= this->passCap && this->passCap != 0) break;
    } while (curCutSize < this->lastCutSize);
    this->repairCount++;
}

void Sanchis_pass(Sanchis* this,  SOLUTION solution) {
    setupVariables(solution);
    while (move(solution) >= 0);
    updateSolution(solution);
    reset(this->gains[0]);
    reset(this->gains[1]);
}

void Sanchis_updateSolution(Sanchis* this,  SOLUTION solution) {
    for (int i = 0; i <= this->bestMove; i++) {
        solution[this->cellMoveArray[i]] = this->blockMoveArray[i];
    }
}

void Sanchis_setupVariables(Sanchis* this,  SOLUTION solution) {
    memset(this->locked, 0, sizeof (int) * modCount(this->ckt)); /* clear this->locked */

    setupBeta(solution);
    setupGains(solution);
    this->balance = 0;

    for (int i = 0; i < modCount(this->ckt); i++) {
        if (solution[i] == 0) this->balance -= 1;
        else this->balance += 1;
    }

    this->lockedCount = 0;
    this->bestGain = netCount(this->ckt) * -1;
    this->bestMove = -1;
    this->currentGain = 0;
    this->currentMove = 0;
    this->lockedNets = 0;
}

void Sanchis_setupBeta(Sanchis* this,  SOLUTION solution) {
    memset(this->beta[0], 0, sizeof (int) * netCount(this->ckt));
    memset(this->beta[1], 0, sizeof (int) * netCount(this->ckt));

    for (int n = 0; n < netCount(this->ckt); n++) {
        int* net = getNet(this->ckt, n);
        for (int m = 0; m < netSize(this->ckt, n); m++) {
            if (this->locked[m]) this->beta[solution[net[m]]][n] = INT_MAX;
            else if (this->beta[solution[net[m]]][n] != INT_MAX) this->beta[solution[net[m]]][n]++;
        }
    }
}

void Sanchis_setupGains(Sanchis* this,  SOLUTION solution) {
    for (int m = 0; m < modCount(this->ckt); m++) {
        for (int i = 0; i < this->level; i++) {
            getVector(this->gains[0], m)[i] = 0;
            getVector(this->gains[1], m)[i] = 0;
        }
        int block = solution[m];
        int* mod = getMod(this->ckt, m);

        for (int n = 0; n < modSize(this->ckt, m); n++) {
            int bFrom = this->beta[block][mod[n]];
            int bTo = this->beta[block ? 0 : 1][mod[n]];

            if (bFrom <= this->level && bTo > 0) getVector(this->gains[block], m)[bFrom - 1]++;
            if (bFrom > 0 && bTo < this->level) getVector(this->gains[block], m)[bTo]--;
        }

        if (!this->locked[m]) set(this->gains[block], m);
    }
}

int Sanchis_move(Sanchis* this,  SOLUTION solution) {
    if (this->lockedCount == modCount(this->ckt)) return -1;
    int cell = getBestMove();
    if (cell < 0) return -1;

    if (this->lockedNets >= this->lastCutSize - this->bestGain) return -1;

    moveMod(solution, cell);
    return cell;
}

int Sanchis_moveMod(Sanchis* this,  SOLUTION solution, int mod) {
    int BF = solution[mod];
    int BT = BF ? 0 : 1;

    this->currentGain += getVector(this->gains[solution[mod]], mod)[0];

    this->cellMoveArray[this->currentMove] = mod;
    this->blockMoveArray[this->currentMove] = BT;

    if (this->currentGain > this->bestGain && this->balance >= this->minBalance && this->balance <= this->maxBalance) {
        this->bestGain = this->currentGain;
        this->bestMove = this->currentMove;
    }
    this->currentMove++;

    remove(this->gains[solution[mod]], mod);

    if (BF == 1) this->balance -= 2;
    else this->balance += 2;

    this->locked[mod] = 1;
    this->lockedCount++;
    updateGains(solution, mod);

    int* modC = getMod(this->ckt, mod);

    for (int n = 0; n < modSize(this->ckt, mod); n++) {
        this->beta[BT][modC[n]] = INT_MAX;
        if (this->beta[BF][modC[n]] != INT_MAX) this->beta[BF][modC[n]]--;
    }

    return (getMod(this->ckt, mod)[0]);
}

void Sanchis_updateGains(Sanchis* this,  SOLUTION solution, int c) {
    int CF = solution[c];
    int CT = CF ? 0 : 1;
    int betaFrom = 0;
    int betaTo = 0;
    int* mod = getMod(this->ckt, c);
    int* net = NULL;

    for (int n = 0; n < modSize(this->ckt, c); n++) {
        net = getNet(this->ckt, mod[n]);
        betaFrom = this->beta[CF][mod[n]];
        betaTo = this->beta[CT][mod[n]];

        if (this->beta[CT][mod[n]] != INT_MAX && this->beta[CF][mod[n]] == INT_MAX) this->lockedNets++;
        if (betaTo > this->level && betaFrom - 1 > this->level) continue;

        for (int d = 0; d < netSize(this->ckt, mod[n]); d++) {
            if (this->locked[net[d]] == 0) {
                int* D = getVector(this->gains[solution[net[d]]], net[d]);

                if (solution[net[d]] == CF) {
                    if (betaTo < this->level && betaTo >= 0) {
                        D[betaTo]++;
                    }

                    if (betaFrom - 1 <= this->level && betaFrom - 2 >= 0) {
                        D[betaFrom - 2]++;

                        if (betaFrom <= this->level && betaTo > 0) {
                            D[betaFrom - 1]--;
                        }
                    }
                    set(this->gains[CF], net[d]);
                } else {
                    if (betaTo <= this->level && betaTo - 1 >= 0) {
                        D[betaTo - 1]--;
                    }

                    if (betaFrom <= this->level && betaFrom - 1 >= 0) {
                        D[betaFrom - 1]--;

                        if (betaTo > 0 && betaFrom < this->level) {
                            D[betaFrom]++;
                        }
                    }
                    set(this->gains[CT], net[d]);
                }
            }
        }
    }
}

int Sanchis_getBestMove(Sanchis* this) {
    int c0 = getMax(this->gains[0]);
    int c1 = getMax(this->gains[1]);

    int g0 = -1 * INT_MAX;
    int g1 = -1 * INT_MAX;

    if (c0 != -1) g0 = getVector(this->gains[0], c0)[0];
    if (c1 != -1) g1 = getVector(this->gains[1], c1)[0];

    if (this->balance >= this->minBalance && this->balance <= this->maxBalance) {
        if (size(this->gains[0]) > 0 && size(this->gains[1]) > 0) {
            if (g0 > g1) return c0;
            else if (g0 < g1) return c1;
            else if (this->balance > 0) return c1;
            else if (this->balance < 0) return c0;
            else return c0;
        } else if (size(this->gains[0]) > 0) return c0;
        else if (size(this->gains[1]) > 0) return c1;
        else return -2;
    } else if (this->balance < this->minBalance && size(this->gains[0]) > 0) {
        return c0;
    } else if (this->balance > this->maxBalance && size(this->gains[1]) > 0) {
        return c1;
    } else return -1;
}