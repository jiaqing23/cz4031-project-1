#include "database.h"
#include <iostream>

using uchar = unsigned char;

using namespace std;

void Record::display()
{
    cout << tconst << " " << averageRating << " " << numVotes << endl;
}

Database::Database(Memory &memory) : memory{memory},
                                     memSize{memory.getMemorySize()},
                                     blkSize{memory.getBlkSize()},
                                     freeBlkOffset{0},
                                     freeBlkPtr{nullptr}
{
    this->bpTree = new BPTree;
}

pair<void *, size_t> Database::writeRecord(Record &record)
{
    size_t recordSize = sizeof(record);
    if (!freeBlkPtr || freeBlkOffset + recordSize > blkSize)
    {
        auto [blkAddr, _] = memory.allocBlk();

        if (!blkAddr)
            return {nullptr, 0};

        this->freeBlkPtr = (uchar *)blkAddr;
        this->freeBlkOffset = 0;
    }

    memory.write((uchar *)freeBlkPtr, freeBlkOffset, &record, recordSize);
    pair<void *, size_t> recordAddr{freeBlkPtr, freeBlkOffset};

    bpTree->insert({record.numVotes, record.tconst}, freeBlkPtr + freeBlkOffset);

    this->freeBlkOffset += recordSize;

    return recordAddr;
}

void Database::readRecord(void *addr, Record &record)
{
    auto [blkAddr, offset] = memory.getBlkAddrAndOffset((uchar *)addr);
    char block[blkSize];
    memory.read(blkAddr, block);
    memcpy(&record, block + offset, sizeof(record));
}

void Database::deleteRecord(void *addr)
{
    auto [blkAddr, offset] = memory.getBlkAddrAndOffset((uchar *)addr);
    if (offset % sizeof(Record) != 0)
        throw "Invalid record address";

    char block[blkSize];
    memory.read(blkAddr, block);

    char emptyRecord[sizeof(Record)];
    memset(emptyRecord, 0, sizeof(Record));
    memory.write(blkAddr, offset, emptyRecord, sizeof(Record));

    // Check if a block is fully empty
    char emptyBlock[blkSize];
    memset(emptyBlock, 0, blkSize);
    if (!memcmp(emptyBlock, blkAddr, blkSize))
        memory.deallocBlk((uchar *)blkAddr);
}

void Database::displayBlock(void *addr)
{
    auto [blkAddr, _] = memory.getBlkAddrAndOffset((uchar *)addr);
    char block[blkSize];
    memory.read(blkAddr, block);

    char emptyRecord[sizeof(Record)];
    memset(emptyRecord, 0, sizeof(Record));

    int offset = 0;
    while (offset < blkSize)
    {
        // Check if a record is deleted (empty)
        if (!memcmp(emptyRecord, block + offset, sizeof(Record)))
        {
            offset += sizeof(Record);
            continue;
        }

        Record record;
        memcpy(&record, block + offset, sizeof(record));
        cout << record.tconst << " ";

        offset += sizeof(Record);
    }
    cout << "\n";
}

/**
 * Destructor
 */
Database::~Database()
{
    delete bpTree;
    bpTree = nullptr;
}