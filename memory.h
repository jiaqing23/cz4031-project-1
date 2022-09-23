#pragma once

#include <cstring>
#include <iostream>
#include <vector>

#ifdef DEBUG 
#define D(x) cerr << x
#define Dn(x) cerr << x << '\n'
#else 
#define D(x)
#define Dn(x)
#endif

using uchar = unsigned char;

using namespace std;

class Memory
{
private:
    uchar *memPtr;

    size_t memSize;
    size_t blkSize;
    size_t numAllocBlks;

    vector<uchar*> deallocatedBlk;

public:
    Memory(size_t memPoolSize, size_t blkSize);

    ~Memory();

    pair<void *, size_t> allocBlk();

    void deallocBlk(uchar * blkAddr);

    bool isAllocated(uchar * blkAddr);

    bool isValidBlkAddr(uchar * blkAddr);

    void write(uchar * blkAddr, size_t blkOffset, const void * record, size_t recordSize);

    pair<void *, size_t> writeRecord(size_t recordSize);

    void read(uchar * blkAddr, char* returnBlk);

    pair<uchar *, size_t> getBlkAddrAndOffset(uchar *addr);

    size_t getMemorySize() {
        return memSize;
    }

    size_t getBlkSize() {
        return blkSize;
    }

    size_t getNumAllocBlks() {
        return numAllocBlks;
    }
};
