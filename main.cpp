#include <iostream>
#include <set>

#include "memory.h"
#include "database.h"

#define MB *1'000'000

using namespace std;

using uchar = unsigned char;

int main()
{

    cout << "Reading data file...\n";
    freopen("./data/data_short.tsv", "r", stdin);
    // freopen("./data/data.tsv", "r", stdin);

    Memory memory{100 MB, 100};
    Database database{memory};
    BPTree *bpTree = database.getBPTree();

    string temp;
    cin >> temp >> temp >> temp; // Ignoring the first line (column names)
    Record record;
    int recordNum = 0;
    while (cin >> record.tconst >> record.averageRating >> record.numVotes)
    {
        pair<void *, size_t> dataRecordAddr = database.writeRecord(record);

        if (++recordNum % 100 == 0)
            cout << recordNum << "\r";
    }
    cout << recordNum << "\n\n";

    // For debug on small dataset
    if (recordNum <= 20)
        bpTree->display();
    cout << "\n";

    cout << "\n>>>>> Experiment 1 <<<<<\n";
    cout << "Number of blocks: " << memory.getNumAllocBlks() << "\n";
    cout << "Total size: " << memory.getNumAllocBlks() * memory.getBlkSize() * 1.0 / (1 MB) << " MB\n";

    cout << "\n>>>>> Experiment 2 <<<<<\n";
    cout << "Parameter n: " << MAX_KEY << "\n";
    cout << "Number of nodes: " << bpTree->getBlockNum() << "\n";
    cout << "Number of levels: " << bpTree->getLevel() << "\n";
    cout << "----Root Node----\n";
    bpTree->getRoot()->display();
    cout << "----First Child Node----\n";
    ((Node *)bpTree->getRoot()->ptr[0])->display();

    cout << "\n>>>>> Experiment 3 <<<<<\n";
    auto res = bpTree->search(500, 500, true);
    set<void *> blkAddrs;
    double total_rating = 0;
    int displayed_count = 0;
    for (auto [key, ptr] : res)
    {
        auto [blkAddr, _] = memory.getBlkAddrAndOffset((uchar *)ptr);
        if (blkAddrs.count(blkAddr) == 0)
        {
            blkAddrs.insert(blkAddr);
            if (displayed_count < 5)
                cout << "Data block " << ++displayed_count << "\n";
            database.displayBlock(blkAddr);
        }
        Record record;
        database.readRecord(ptr, record);

        total_rating += record.averageRating;
    }
    cout << "Total number of data blocks accessed = " << blkAddrs.size() << "\n";
    cout << "Average rating = " << total_rating / (double)res.size() << endl;

    cout << "\n>>>>> Experiment 4 <<<<<\n";
    res.clear();
    res = bpTree->search(30000, 40000, true);
    blkAddrs.clear();
    total_rating = 0;
    displayed_count = 0;
    for (auto [key, ptr] : res)
    {
        auto [blkAddr, _] = memory.getBlkAddrAndOffset((uchar *)ptr);
        if (blkAddrs.count(blkAddr) == 0)
        {
            blkAddrs.insert(blkAddr);
            if (displayed_count < 5)
                cout << "Data block " << ++displayed_count << "\n";
            database.displayBlock(blkAddr);
        }
        Record record;
        database.readRecord(ptr, record);

        total_rating += record.averageRating;
    }
    cout << "Total number of data blocks accessed = " << blkAddrs.size() << "\n";
    cout << "Average rating = " << total_rating / (double)res.size() << endl;

    cout << "\n>>>>> Experiment 4 <<<<<\n";
    // auto res = bpTree->search(30000, 40000);

    cout << "\n>>>>> Experiment 5 <<<<<\n";
    auto res1 = bpTree->search(1000, 1000, false);
    int initialCnt = bpTree->getBlockNum();
    for (auto i : res1)
    {
        bpTree->remove(i.first);
    }
    int postCnt = bpTree->getBlockNum();
    int nodesDel = initialCnt - postCnt;
    cout << "Number of nodes deleted: " << nodesDel << "\n";
    cout << "Number of nodes in updated B+ tree: " << postCnt << "\n";
    cout << "Height of updated B+ tree: " << bpTree->getLevel() << "\n";
    cout << "Contents of the root node: "
         << "\n";
    bpTree->getRoot()->display();
    cout << "Contents of 1st Child Node: "
         << "\n";
    ((Node *)bpTree->getRoot()->ptr[0])->display();
    return 0;
}
