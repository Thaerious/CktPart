#pragma once
#include <cstdio>

class NetList;

class ModList{
    private:
        bool ModContains(int mIdx, int v);

    protected:
        int** array;
		int*  weights;
		int*  sizes;
		int   mcount;
		int   ncount;

    public:
        ModList(FILE*);
        ModList(NetList&);
		virtual ~ModList();
        int* getMod(int i)		{return array[i];}
		int  modSize(int i)   	{return sizes[i];}
		int  modWeight(int i)	{return weights[i];}
		int  modCount()         {return mcount;}
		int  netCount()         {return ncount;}
};
