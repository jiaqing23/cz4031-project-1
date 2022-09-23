#pragma once

#include "memory.h"
#include "b-plus-tree.h"
#include <iostream>

#ifdef DEBUG 
#define D(x) cerr << x
#define Dn(x) cerr << x << '\n'
#else 
#define D(x)
#define Dn(x)
#endif

using uchar = unsigned char;
using uint = unsigned int;

using namespace std;

struct Record
{
    float averageRating;
    uint numVotes;
    char tconst[10];

    void display();
};

class Database
{
private:
    Memory& memory;

    size_t memSize;
    size_t blkSize;

    uchar *freeBlkPtr;
    size_t freeBlkOffset;

    BPTree *bpTree;

public:
    Database(Memory& memory);

    ~Database();

    pair<void *, size_t> writeRecord(Record& record);

    void readRecord(void * addr, Record &record);

    void deleteRecord(void * addr);

    void displayBlock(void * addr);

    BPTree *getBPTree() {
        return bpTree;
    }
};
