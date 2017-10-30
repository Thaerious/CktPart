#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "BucketList.h"
#include "Definitions.h"

// Bucket :
// [PREV][MAX][value]...[value]

BucketList* newBucketList(int modCount, int level) {
    BucketList* this = calloc(1, sizeof (BucketList));
    BucketList_setup(this, BUCKET_WIDTH, (ULONG) level, modCount);
    return this;
}

void BucketList_setup(BucketList* this, ULONG degree, ULONG level, ULONG modCount) {
    degree = 10;
    this->degree = degree;
    this->level = level;
    this->modCount = modCount;
    this->bktSize = degree * 2 + 3;
    this->maxModule = -1;
    this->bktCount = 0;
    this->bktCount = 0;
    this->sz = 0;

    for (ULONG l = 0; l < this->level; l++) {
        if ((ULONG) pow(this->degree * 2 + 1, l) >= modCount) this->bktCount += this->modCount;
        else this->bktCount += (ULONG) pow((double) this->degree * 2 + 1, (double) l);
    }

    this->next = (ULONG*) calloc(this->modCount, sizeof (ULONG));
    this->prev = (ULONG*) calloc(this->modCount, sizeof (ULONG));
    this->buckets = (ULONG*) calloc(this->bktCount * this->bktSize, sizeof (ULONG));
    this->vectors = (int*) calloc(this->modCount * (this->level), sizeof (int));
    this->root = &this->buckets[0];

    BucketList_reset(this);
}

int BucketList_getMax(BucketList* this) {
    return this->maxModule;
}

int* BucketList_getVector(BucketList* this, int m) {
    return &this->vectors[m * this->level];
}

int BucketList_size(BucketList* this) {
    return this->sz;
}

void deleteBucketList(BucketList** thisp) {
    BucketList* this = *thisp;
    if (this->next) free(this->next);
    if (this->prev) free(this->prev);
    if (this->buckets) free(this->buckets);
    if (this->vectors) free(this->vectors);
    free(thisp);
}

void BucketList_reset(BucketList* this) {
    this->maxModule = -1;
    this->sz = 0;

    memset(this->next, NaN, this->modCount * sizeof (void*));
    memset(this->prev, NaN, this->modCount * sizeof (void*));
    memset(this->buckets, NaN, this->bktCount * this->bktSize * sizeof (void*));
    memset(this->vectors, 0, this->modCount * this->level * sizeof (int));

    this->emptyBuckets = &this->buckets[this->bktSize];
    for (ULONG i = 1; i < this->bktCount; i++) {
        this->buckets[i * this->bktSize] = (ULONG) & this->buckets[(i + 1) * this->bktSize];
    }
}

int BucketList_compare(BucketList* this, int a, int b) {
    if (a == NaN) return -1;
    if (b == NaN) return 1;

    int* v1 = &this->vectors[a * this->level];
    int* v2 = &this->vectors[b * this->level];

    for (ULONG i = 0; i < this->level; i++)
        if (v1[i] != v2[i]) return v1[i] - v2[i];

    return 0;
}

inline void BucketList_insert(BucketList* this, ULONG* location, ULONG mod) {
    /* if the location is empty */
    if (*location == NaN) {
        *location = mod;
        this->prev[mod] = (ULONG) location;
    } else {
        ULONG current = *location;

        if (BucketList_compare(this, current, mod) <= 0 || current == NaN) {
            this->next[mod] = current;
            this->prev[mod] = (ULONG) location;
            if (current != NaN) this->prev[current] = mod;
            *location = mod;
        } else {
            ULONG last = current;
            current = this->next[current];

            while (BucketList_compare(this, current, mod) > 0) {
                last = current;
                current = this->next[current];
            }

            this->next[last] = mod;
            if (current != NaN) this->prev[current] = mod;
            this->next[mod] = current;
            this->prev[mod] = last;
        }
    }
}

void BucketList_set(BucketList* this, int m) {
    int offset = 0;

    /* if the bucket is already Inserted, remove it */
    if (this->prev[m] != NaN) BucketList_remove(this, m);

    /* check if the module is now a maximum module */
    if (BucketList_compare(this, this->maxModule, m) <= 0) this->maxModule = m;

    this->sz++;

    int* vector = &this->vectors[m * this->level];
    ULONG* current = this->root;

    for (ULONG lvl = 0; lvl < this->level; lvl++) {
        offset = vector[lvl] + this->degree + 2;

        if (offset <= 2) {
            if (current[MAX] == NaN) current[MAX] = 2;
            BucketList_insert(this, &current[2], m);
            break;
        } else if (offset >= this->bktSize - 1) {
            current[MAX] = this->bktSize - 1;
            BucketList_insert(this, &current[this->bktSize - 1], m);
            break;
        } else if (lvl == this->level - 1) {
            if (current[MAX] == NaN) current[MAX] = offset;
            else if (current[MAX] < offset) current[MAX] = offset;

            this->next[m] = current[offset];
            if (this->next[m] != NaN) this->prev[this->next[m]] = m;
            current[offset] = m;
            this->prev[m] = (ULONG) & current[offset];
        } else if (current[offset] == NaN) {
            if (current[MAX] == NaN) current[MAX] = offset;
            else if (current[MAX] < offset) current[MAX] = offset;

            ULONG* last = current;
            current[offset] = (ULONG) this->emptyBuckets;
            current = (ULONG*) this->emptyBuckets;
            this->emptyBuckets = (ULONG*) this->emptyBuckets[PREV];
            current[PREV] = (ULONG) (&last[offset]);
        } else {
            current = (ULONG*) current[offset];
        }
    }
}

void BucketList_remove(BucketList* this, int m) {
    printf("BucketList_remove %d\n", m);
    if (this->prev[m] == NaN) return;
    this->sz--;


    if (this->prev[m] < (ULONG) this->root) {
        /* the this->prev value is a module not a bucket */
        this->next[this->prev[m]] = this->next[m];
        if (this->next[m] != NaN) this->prev[this->next[m]] = this->prev[m];
    } else {
        /* the this->prev value is a bucket */
        if (this->next[m] != NaN) {
            /* there is a this->next module */
            *((ULONG*) this->prev[m]) = this->next[m];
            this->prev[this->next[m]] = this->prev[m];
            if (m == this->maxModule) this->maxModule = this->next[m];
        } else {
            /* there is no this->next module */
            int offset = ((this->prev[m] - (ULONG) (this->root)) / (sizeof (ULONG))) % this->bktSize;
            ULONG* parent = (ULONG*) (this->prev[m] - (offset * sizeof (ULONG)));

            int flag = 1;
            while (flag) {
                parent[offset] = NaN;
                if (parent[parent[MAX]] != NaN) break;

                for (int i = parent[MAX]; i > 1; i--) {
                    if (parent[i] != NaN) {
                        parent[MAX] = i;
                        if (m == this->maxModule) BucketList_updateMaxModule(this);
                        flag = 0;
                        break;
                    }
                } /* end for */

                if (flag == 1) {
                    if (parent != this->root) {
                        ULONG* recover = parent;
                        parent[MAX] = NaN;
                        offset = ((parent[PREV] - (ULONG) this->root) / sizeof (ULONG)) % this->bktSize;
                        parent = (ULONG*) (parent[PREV] - (offset * sizeof (ULONG)));
                        recover[PREV] = (ULONG) this->emptyBuckets;
                        this->emptyBuckets = recover;
                    } else {
                        parent[MAX] = NaN;
                        this->maxModule = -1;
                        flag = 0;
                    }
                }
            }/* end while */
        }
    }

    this->prev[m] = NaN;
    this->next[m] = NaN;
}

void BucketList_updateMaxModule(BucketList* this) {
    if (this->sz == 0) {
        this->maxModule = -1;
        return;
    }

    ULONG* current = this->root;
    while (current > (ULONG*) this->modCount) {
        current = (ULONG*) current[current[MAX]];
    }
    this->maxModule = (ULONG) current;
}
