#pragma once
#include <cstdio>

class ModList;

class NetList{
    private:
		bool NetContains(int nIdx, int v);

    protected:
        int** array;
		int*  sizes;
		int   mcount;
		int   ncount;

    public:
        NetList(FILE*);
		NetList(NetList&);
		virtual ~NetList();
        int* getNet(int i)		{return array[i];}
		int  netSize(int i)   	{return sizes[i];}
		int  modCount()         {return mcount;}
		int  netCount()         {return ncount;}
};
