#include "b-plus-tree.h"

using keypair = pair<int, string>; 

int main()
{
    BPTree node; 
    node.insert(make_pair(1,"20"), (void*)1);
    node.insert(make_pair(2,"20"), (void*)2);
    node.insert(make_pair(3,"20"), (void*)3);
    node.insert(make_pair(4,"20"), (void*)4);
    node.insert(make_pair(5,"20"), (void*)5);
    node.insert(make_pair(6,"20"), (void*)6);
    node.insert(make_pair(7,"20"), (void*)7);
    node.insert(make_pair(8,"20"), (void*)8);
    node.insert(make_pair(9,"20"), (void*)9);
    node.insert(make_pair(11,"20"), (void*)10);
    node.insert(make_pair(12,"20"), (void*)11);
    node.insert(make_pair(13,"20"), (void*)12);
    
    node.display( );

    // cout << "__________________\n";
    // BPTree node2;
    // node2.insert(12, (void*)12);
    // node2.insert(11, (void*)11);
    // node2.insert(10, (void*)10);
    // node2.insert(9, (void*)9);
    // node2.insert(8, (void*)8);
    // node2.insert(7, (void*)7);
    // node2.insert(6, (void*)6);
    // node2.insert(5, (void*)5);
    // node2.insert(4, (void*)4);
    // node2.insert(3, (void*)3);
    // node2.insert(2, (void*)2);
    // node2.insert(1, (void*)1);

    //  node2.display( );

    // cout << "__________________\n";
    // BPTree node3; 
    
    // node3.insert(3, (void*)3);
    // node3.insert(2, (void*)2);
    // node3.insert(1, (void*)1);
    // node3.insert(3, (void*)3);
    // node3.insert(2, (void*)2);
    // node3.insert(1, (void*)1);
    // node3.insert(3, (void*)3);
    // node3.insert(2, (void*)2);
    // node3.insert(1, (void*)1);
    // node3.insert(3, (void*)3);
    // node3.insert(2, (void*)2);
    // node3.insert(1, (void*)1);

    // node3.display( );

    cout << "__________________\n";
    node.remove(make_pair(5, "20"));

    node.display( );
    cout << "__________________\n";
    node.remove(make_pair(6, "20"));
    node.remove(make_pair(3, "20"));
    node.remove(make_pair(4, "20")); 
    // cout << "__________________\n";

    // cout  << "Start removing 1 ... \n";
    node.remove(make_pair(1, "20"));

    // cout << "__________________\n";
    node.display( );
}