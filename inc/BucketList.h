/*
 * File:   BucketList.h
 * Author: edward
 *
 * Created on October 10, 2017, 8:56 AM
 */

#pragma once
#ifndef BUCKETLIST_H
#define BUCKETLIST_H

#ifdef __cplusplus
extern "C" {
#endif


#include "Definitions.h"
#define BUCKET_WIDTH 10
#define MAX 1
#define PREV 0
#define NaN (~0)

typedef struct {
        ULONG degree;
        ULONG level;
        ULONG modCount;
        ULONG bktSize;
        ULONG bktCount;
        int sz;
        int maxModule;

        ULONG* next;
        ULONG* prev;
        int* vectors; /* one vector for each module */

        ULONG* root;
        ULONG* emptyBuckets;
        ULONG* buckets;
    } BucketList;

    BucketList* newBucketList(int modCount, int level);
    void deleteBucketList(BucketList** this);
    void BucketList_updateMaxModule(BucketList* this);
    void BucketList_insert(BucketList* this, ULONG*, ULONG);
    int BucketList_compare(BucketList* this, int, int);
    void BucketList_setup(BucketList* this, ULONG degree, ULONG level, ULONG modcount);
    int BucketList_recalculateMax(BucketList* this);
    void BucketList_set(BucketList* this, int m);
    void BucketList_remove(BucketList* this, int m);
    void BucketList_reset(BucketList* this);
    int BucketList_removeMax(BucketList* this);
    int BucketList_getMax(BucketList* this);
    int* BucketList_getVector(BucketList* this, int m);
    int BucketList_size(BucketList* this);

#ifdef __cplusplus
}
#endif
#endif /* BUCKETLIST_H */

