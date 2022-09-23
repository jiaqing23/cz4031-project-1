#include "b-plus-tree.h"
#include <iostream>

using namespace std;

using keypair = pair<int, string>; 

Node::Node() : Node(false, 0, {}, nullptr, nullptr) {}

Node::Node(bool isLeaf, int size): Node(isLeaf, size, {}, nullptr, nullptr) {}

Node::Node(bool isLeaf, int size, keypair firstKey, void *firstPtr, void *secondPtr): isLeaf{isLeaf},
                                                                                        size{size}
{
    key = new keypair[MAX_KEY];
    ptr = new void *[MAX_KEY + 1];
    key[0] = firstKey;
    ptr[0] = firstPtr;
    ptr[1] = secondPtr;
}

Node::~Node(){
    delete[] key;
    delete[] ptr;
}

void Node::display(){
    cout << "Address = " << (void*)this << "\n";
    for(int i = 0; i < size; i++) cout << "(" << ptr[i] << ") " << key[i].first << "," << key[i].second << " ";
    cout << "(" << ptr[size] << ")\n";  
}

BPTree::BPTree() : root{nullptr} {}

vector<keypair> BPTree::search(int x, int y, int displayCount = 0){
    vector<keypair> ret;
    if(!root) return ret;

    keypair target{x, ""};
    Node *cursor = root;
    while(!cursor->isLeaf){
        int i = 0;
        while (target >= cursor->key[i] && i < cursor->size) i++;
        cursor = (Node *)cursor->ptr[i];
    }

    // Case 1: The first (>=x, "...") in this current leaf
    int i;
    for(i = 0; i < cursor->size; i++) if(cursor->key[i].first >= x) break;

    // Case 2: The first (>=x, "...") is the first key of next leaf
    if(i == cursor->size){
        cursor = (Node *)cursor->ptr[cursor->size];
        if(cursor->key[0].first >= x) i = 0;
        else return ret;
    }
   
    while(cursor->key[i].first <= y){
        ret.emplace_back(cursor->key[i]);
        i++;
        if(i == cursor->size){
            if(cursor->ptr[i]){
                cursor = (Node *)cursor->ptr[cursor->size];
                i = 0;
            }
            else break;
        }
    }
    
    return ret;
}

void BPTree::insert(keypair x, void *p)
{
    if (!root)
    {
        root = new Node(true, 1, x, p, nullptr);
        return;
    }

    Node *cursor = root;
    Node *parent;

    // Go to the leaf node that contains target value
    while (!cursor->isLeaf)
    {
        int i = 0;
        while (x >= cursor->key[i] && i < cursor->size) i++;
        parent = cursor;
        cursor = (Node *)cursor->ptr[i];
    }

    // Node is not full yet
    if (cursor->size < MAX_KEY)
    {
        int i = 0;
        while (x >= cursor->key[i] && i < cursor->size) i++;

        cursor->ptr[cursor->size + 1] = cursor->ptr[cursor->size];
        for (int j = cursor->size; j > i; j--){
            cursor->key[j] = cursor->key[j - 1];
            cursor->ptr[j] = cursor->ptr[j - 1];
        }
        cursor->key[i] = x;
        cursor->ptr[i] = p;
        cursor->size++;
        return;
    }

    // Node is full, need to split
    keypair virtualKey[MAX_KEY + 1];
    int i = 0, j;
    void *virtualPtr[MAX_KEY + 1];
    while (x >= cursor->key[i] && i < MAX_KEY) i++;
    for (j = 0; j < i; j++)
    {
        virtualKey[j] = cursor->key[j];
        virtualPtr[j] = cursor->ptr[j];
    }
    virtualKey[i] = x;
    virtualPtr[i] = p;
    for (j = i + 1; j < MAX_KEY + 1; j++)
    {
        virtualKey[j] = cursor->key[j - 1];
        virtualPtr[j] = cursor->ptr[j - 1];
    }

    Node *newLeaf = new Node(true, (MAX_KEY + 1) / 2);
    cursor->size = MAX_KEY + 1 - newLeaf->size;
    cursor->ptr[cursor->size] = newLeaf;
    newLeaf->ptr[newLeaf->size] = cursor->ptr[MAX_KEY];
    
    for (i = 0; i < cursor->size; i++)
    {
        cursor->key[i] = virtualKey[i];
        cursor->ptr[i] = virtualPtr[i];
    }
    for (j = cursor->size, i = 0; i < newLeaf->size; i++, j++)
    {
        newLeaf->key[i] = virtualKey[j];
        newLeaf->ptr[i] = virtualPtr[j];
    }

    if (cursor == root) root = new Node(false, 1, newLeaf->key[0], cursor, newLeaf);
    else insertInternal(newLeaf->key[0], parent, newLeaf);
}

void BPTree::insertInternal(keypair x, Node *cursor, Node *child)
{
    // Node is not full yet
    if (cursor->size < MAX_KEY)
    {
        int i = 0;
        while (x >= cursor->key[i] && i < cursor->size) i++;
        for (int j = cursor->size; j > i; j--)
        {
            cursor->key[j] = cursor->key[j - 1];
        }
        for (int j = cursor->size + 1; j > i + 1; j--)
        {
            cursor->ptr[j] = cursor->ptr[j - 1];
        }
        cursor->key[i] = x;
        cursor->size++;
        cursor->ptr[i + 1] = child;
    }
    else
    {
        keypair virtualKey[MAX_KEY + 1];
        Node *virtualPtr[MAX_KEY + 2];

        int i = 0, j;
        while (x >= cursor->key[i] && i < MAX_KEY) i++;

        for (j = 0; j < i; j++)
        {
            virtualKey[j] = cursor->key[j];
        }
        virtualKey[i] = x;
        for (j = i + 1; j < MAX_KEY + 1; j++)
        {
            virtualKey[j] = cursor->key[j - 1];
        }

        for (j = 0; j < i + 1; j++)
        {
            virtualPtr[j] = (Node *)cursor->ptr[j];
        }
        virtualPtr[i + 1] = child;
        for (j = i + 2; j < MAX_KEY + 2; j++)
        {
            virtualPtr[j] = (Node *)cursor->ptr[j - 1];
        }

        Node *newInternal = new Node(false, MAX_KEY / 2);
        cursor->size = MAX_KEY - newInternal->size;
        for (i = 0; i < cursor->size; i++)
        {
            cursor->key[i] = virtualKey[i];
            cursor->ptr[i] = virtualPtr[i];
        }
        cursor->ptr[cursor->size] = virtualPtr[cursor->size];

        for (i = 0, j = cursor->size + 1; i < newInternal->size; i++, j++)
        {
            newInternal->key[i] = virtualKey[j];
            newInternal->ptr[i] = virtualPtr[j];
        }
        newInternal->ptr[newInternal->size] = virtualPtr[MAX_KEY + 1];

        if (cursor == root) root = new Node(false, 1, cursor->key[cursor->size], cursor, newInternal);
        else insertInternal(cursor->key[cursor->size], findParent(root, cursor), newInternal);
    }
}

Node *BPTree::findParent(Node *cursor, Node *child)
{
    Node *parent;
    if (cursor->isLeaf || ((Node *)cursor->ptr[0])->isLeaf)
    {
        return NULL;
    }
    for (int i = 0; i < cursor->size + 1; i++)
    {
        if (cursor->ptr[i] == child)
        {
            parent = cursor;
            return parent;
        }
        else
        {
            parent = findParent((Node *)cursor->ptr[i], child);
            if (parent != NULL)
                return parent;
        }
    }
    return parent;
}

void BPTree::remove(keypair x)
{ 
    if (root == NULL)
    {
        cout << "Tree empty\n";
        return;
    }
    
    Node *cursor = root;
    Node *parent;
    int leftSibling, rightSibling;
    while (!cursor->isLeaf)
    {
        int i = 0;
        while (x >= cursor->key[i] && i < cursor->size) i++;
        parent = cursor;
        cursor = (Node *)cursor->ptr[i];
        leftSibling = i - 1;
        rightSibling = i + 1;
    }
 
    
    bool found = false;
    int pos = 0;
    while(x > cursor->key[pos] && pos < cursor->size) pos++;
    if (pos == cursor->size || cursor->key[pos] != x)
    {
        cout << "Not found\n";
        return;
    }
 
    cursor->size--;
    for (int i = pos; i < cursor->size; i++)
    {
        cursor->key[i] = cursor->key[i + 1];
        cursor->ptr[i] = cursor->ptr[i + 1];
    }
    cursor->ptr[cursor->size] = cursor->ptr[cursor->size + 1];
    cursor->ptr[cursor->size + 1] = nullptr;
    
    if (cursor == root)
    { 
        if (cursor->size == 0)
        {
            cout << "Empty Tree\n";
            root = nullptr;
        }
        return;
    }
    if (cursor->size >= (MAX_KEY + 1) / 2) return;

    if (leftSibling >= 0)
    {
        Node *leftNode = (Node *)parent->ptr[leftSibling];
        if (leftNode->size >= (MAX_KEY + 1) / 2 + 1)
        {
            cursor->ptr[cursor->size + 1] = cursor->ptr[cursor->size];
            for (int i = cursor->size; i > 0; i--)
            {
                cursor->key[i] = cursor->key[i - 1];
                cursor->ptr[i] = cursor->ptr[i - 1];
            }
            cursor->size++;
            cursor->key[0] = leftNode->key[leftNode->size - 1];
            cursor->ptr[0] = leftNode->ptr[leftNode->size - 1];
            leftNode->size--;
            leftNode->ptr[leftNode->size] = cursor;
            leftNode->ptr[leftNode->size + 1] = nullptr;
            parent->key[leftSibling] = cursor->key[0];
            return;
        }
    }
    if (rightSibling <= parent->size)
    {
        Node *rightNode = (Node *)parent->ptr[rightSibling];
        if (rightNode->size >= (MAX_KEY + 1) / 2 + 1)
        {
            cursor->size++;
            cursor->ptr[cursor->size] = cursor->ptr[cursor->size - 1];
            cursor->ptr[cursor->size - 1] = rightNode->ptr[0];
            cursor->key[cursor->size - 1] = rightNode->key[0];
            rightNode->size--;
            for (int i = 0; i < rightNode->size; i++)
            {
                rightNode->key[i] = rightNode->key[i + 1];
                rightNode->ptr[i] = rightNode->ptr[i + 1];
            }
            rightNode->ptr[rightNode->size] = rightNode->ptr[rightNode->size + 1];
            rightNode->ptr[rightNode->size + 1] = nullptr;
            parent->key[rightSibling - 1] = rightNode->key[0];
            return;
        }
    }

    if (leftSibling >= 0)
    {
        Node *leftNode = (Node *)parent->ptr[leftSibling];
        for (int i = leftNode->size, j = 0; j < cursor->size; i++, j++)
        {
            leftNode->key[i] = cursor->key[j];
            leftNode->ptr[i] = cursor->ptr[j];
        }
        leftNode->size += cursor->size;
        leftNode->ptr[leftNode->size] = cursor->ptr[cursor->size];
        removeInternal(parent->key[leftSibling], parent, cursor);
    }
    else if (rightSibling <= parent->size)
    {
        
        Node *rightNode = (Node *)parent->ptr[rightSibling];
        for (int i = cursor->size, j = 0; j < rightNode->size; i++, j++)
        {
            cursor->key[i] = rightNode->key[j];
            cursor->ptr[i] = rightNode->ptr[j];
        }
        cursor->size += rightNode->size;
        cursor->ptr[cursor->size] = rightNode->ptr[rightNode->size]; 
        removeInternal(parent->key[rightSibling - 1], parent, rightNode);
    }
}

void BPTree::removeInternal(keypair x, Node *cursor, Node *child)
{ 
    if (cursor == root)
    {
        if (cursor->size == 1)
        {
            cout << "Root changed, level decrease\n";
            if (cursor->ptr[1] == child)
            {
                root = (Node *)cursor->ptr[0];
                return;
            }
            else if (cursor->ptr[0] == child)
            {
                root = (Node *)cursor->ptr[1];
                return;
            }
        }
    }
    
    int pos = 0; 
    while (cursor->key[pos] != x && pos < cursor->size) pos++; // must exist because of valid call
    for (int i = pos; i < cursor->size; i++)
    {
        cursor->key[i] = cursor->key[i + 1];
    }

    while (cursor->ptr[pos] != child && pos < cursor->size + 1) pos++; // must exist because of valid call
    for (int i = pos; i < cursor->size + 1; i++)
    {
        cursor->ptr[i] = cursor->ptr[i + 1];
    }
    cursor->size--;
    
    if (cursor->size >= MAX_KEY / 2) return;
    if (cursor == root) return;

    Node *parent = findParent(root, cursor);
    int leftSibling, rightSibling;
    for (pos = 0; pos < parent->size + 1; pos++)
    {
        if (parent->ptr[pos] == cursor)
        {
            leftSibling = pos - 1;
            rightSibling = pos + 1;
            break;
        }
    }
    if (leftSibling >= 0)
    {
        Node *leftNode = (Node *)parent->ptr[leftSibling];
        if (leftNode->size >= MAX_KEY / 2 + 1)
        {
            for (int i = cursor->size; i > 0; i--)
            {
                cursor->key[i] = cursor->key[i - 1];
            }
            cursor->key[0] = parent->key[leftSibling];
            parent->key[leftSibling] = leftNode->key[leftNode->size - 1];
            for (int i = cursor->size + 1; i > 0; i--)
            {
                cursor->ptr[i] = cursor->ptr[i - 1];
            }
            cursor->ptr[0] = leftNode->ptr[leftNode->size];
            cursor->size++;
            leftNode->size--;
            return;
        }
    }
    if (rightSibling <= parent->size)
    {
        Node *rightNode = (Node *)parent->ptr[rightSibling];
        if (rightNode->size >= MAX_KEY / 2 + 1)
        {
            cursor->key[cursor->size] = parent->key[pos];
            parent->key[pos] = rightNode->key[0];
            for (int i = 0; i < rightNode->size - 1; i++)
            {
                rightNode->key[i] = rightNode->key[i + 1];
            }
            cursor->ptr[cursor->size + 1] = rightNode->ptr[0];
            for (int i = 0; i < rightNode->size; ++i)
            {
                rightNode->ptr[i] = rightNode->ptr[i + 1];
            }
            cursor->size++;
            rightNode->size--;
            return;
        }
    }

    if (leftSibling >= 0)
    {
        Node *leftNode = (Node *)parent->ptr[leftSibling];
        leftNode->key[leftNode->size] = parent->key[leftSibling];
        for (int i = leftNode->size + 1, j = 0; j < cursor->size; i++, j++)
        {
            leftNode->key[i] = cursor->key[j];
        }
        for (int i = leftNode->size + 1, j = 0; j < cursor->size + 1; i++, j++)
        {
            leftNode->ptr[i] = cursor->ptr[j];
            cursor->ptr[j] = nullptr;
        }
        leftNode->size += cursor->size + 1;
        cursor->size = 0;
        removeInternal(parent->key[leftSibling], parent, cursor);
    }
    else if (rightSibling <= parent->size)
    {
        Node *rightNode = (Node *)parent->ptr[rightSibling];
        cursor->key[cursor->size] = parent->key[pos];
        for (int i = cursor->size + 1, j = 0; j < rightNode->size; i++, j++)
        {
            cursor->key[i] = rightNode->key[j];
        }
        for (int i = cursor->size + 1, j = 0; j < rightNode->size + 1; i++, j++)
        {
            cursor->ptr[i] = rightNode->ptr[j];
            rightNode->ptr[j] = nullptr;
        }
        cursor->size += rightNode->size + 1;
        rightNode->size = 0;

        removeInternal(parent->key[pos], parent, rightNode);
    } 
}

int BPTree::getBlockNumInternal(Node *cursor){
    int res = 1;
    if(cursor && !cursor->isLeaf){
        for(int i = 0; i < cursor->size + 1; i++) res += getBlockNumInternal((Node*) cursor->ptr[i]);
    }
    return res;
}

int BPTree::getBlockNum()
{
    return getBlockNumInternal(root);
}

int BPTree::getLevel()
{
    int res = root ? 1 : 0;
    Node *cursor = root;
    while(cursor && !cursor->isLeaf){
        cursor = (Node *)cursor->ptr[0];
        res++;
    }

    return res;
}

void BPTree::displayInternal(Node *cursor, string prefix)
{
    string newPrefix;
    if (prefix.size() == 0)
        newPrefix = "|-";
    else
        newPrefix = " " + prefix;

    if (cursor)
    {
        for (int i = 0; i < cursor->size; i++)
        { 
            if (cursor->isLeaf) 
                cout << "(" << cursor->key[i].first  << "|" << cursor->key[i].second 
                << ", " << cursor->ptr[i] << ") ";
            else 
                cout << cursor->key[i].first << " ";
        }
        cout << "\n";
        if (!cursor->isLeaf)
        {
            for (int i = 0; i < cursor->size + 1; i++)
            {
                cout << newPrefix;
                displayInternal((Node *)cursor->ptr[i], newPrefix);
            }
        }
    }
}

void BPTree::display() { displayInternal(root, ""); }

BPTree::~BPTree(){
    if(root) delete root;
}