#include <iostream>

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
    BPTree * bpTree = database.getBPTree();

    string temp;
    cin >> temp >> temp >> temp; // Ignoring the first line (column names)
    Record record;
    int recordNum = 0;
    while (cin >> record.tconst >> record.averageRating >> record.numVotes){
        pair<void *, size_t> dataRecordAddr = database.writeRecord(record);

        if(++recordNum % 100 == 0) cout << recordNum << "\r"; 
    }
    cout << recordNum << "\n\n";

    // For debug on small dataset
    if(recordNum <= 20) bpTree->display(); cout << "\n";

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
    auto res = bpTree->search(500, 500, 5);
    cout << "Total ";

    return 0;
}
