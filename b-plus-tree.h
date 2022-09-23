#pragma once

#include <iostream>
#include <vector>

#ifdef DEBUG
#define D(x) cerr << x
#define Dn(x) cerr << x << '\n'
#else
#define D(x)
#define Dn(x)
#endif

using namespace std;

using uchar = unsigned char;
using uint = unsigned int;
using keypair = pair<int, string>;

class BPTree;

struct Node
{
public:
    bool isLeaf;
    int size;
    keypair *key; // <key, id>
    void **ptr;

    Node();

    ~Node();

    Node(bool isLeaf, int size);

    Node(bool isLeaf, int size, keypair firstKey, void *firstPtr, void *secondPtr);

    void display();
};

class BPTree
{
private:
    Node *root;

    void insertInternal(keypair x, Node *cursor, Node *child);

    void removeInternal(keypair x, Node *cursor, Node *child);

    void displayInternal(Node *cursor, string prefix);

    int getBlockNumInternal(Node *cursor);

    Node *findParent(Node *cursor, Node *child);

public:
    static int MAX_KEY;

    BPTree(size_t blkSize);

    ~BPTree();

    vector<pair<keypair, void *>> search(int x, int y, bool exp3and4);

    void insert(keypair x, void *p);

    void remove(keypair x);

    void display();

    int getBlockNum();

    int getLevel();

    Node *getRoot()
    {
        return root;
    };
};