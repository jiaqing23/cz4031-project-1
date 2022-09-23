#include "memory.h"
#include <iostream>

using uchar = unsigned char;

using namespace std;

Memory::Memory(size_t memSize, size_t blkSize) {
    if(memSize % blkSize != 0) memSize = size_t(memSize / blkSize) * blkSize;
    
    this->memSize = memSize;
    this->blkSize = blkSize;

    this->memPtr = new uchar[memSize];
    memset(memPtr, 0, memSize);

    this->numAllocBlks = 0;

    this->deallocatedBlk.clear();
}

pair<void *, size_t> Memory::allocBlk() {
    if (numAllocBlks < memSize / blkSize) {
        pair<void *, size_t> memBlk {memPtr + (numAllocBlks * blkSize), blkSize};
        numAllocBlks += 1;
        return memBlk;
    }

    return {nullptr, 0};
}

void Memory::deallocBlk(uchar *blkAddr){
    if(!isValidBlkAddr(blkAddr)) throw "Block haven't be allocated or invalid block address";
    memset(blkAddr, 0, blkSize);
    deallocatedBlk.emplace_back(blkAddr);
    Dn("Deallocate blkAddr = " << (void *)blkAddr);
}

bool Memory::isAllocated(uchar * blkAddr){ 
    return blkAddr >= memPtr && blkAddr < memPtr + numAllocBlks * blkSize;
}

bool Memory::isValidBlkAddr(uchar * blkAddr){
    if(!isAllocated(blkAddr)) throw "Block haven't be allocated";
    return (blkAddr-memPtr) % blkSize == 0;
}

void Memory::write(uchar * blkAddr, size_t blkOffset, const void * record, size_t recordSize){
    if(recordSize > blkSize) throw "Record size greater than block size";
    if(!isValidBlkAddr(blkAddr)) throw "Block haven't be allocated or invalid block address";
    if(blkOffset + recordSize > blkSize) throw "Block not enough space"; 
    
    memcpy(blkAddr + blkOffset, record, recordSize);
}

void Memory::read(uchar * blkAddr, char* returnBlk){
    if(!isValidBlkAddr(blkAddr)) throw "Block haven't be allocated or invalid block address";
    
    memcpy(returnBlk, blkAddr, blkSize);
}

pair<uchar *, size_t> Memory::getBlkAddrAndOffset(uchar *addr){
    if(addr < memPtr || addr >= memPtr + memSize) throw "Out of bound"; 
    size_t offset = (addr - memPtr) % blkSize;
    uchar *  blkAddr = addr - offset;
    return {blkAddr, offset};
}

/**
 * Destructor
 */
Memory::~Memory() {
    delete memPtr;
    memPtr = nullptr;
}