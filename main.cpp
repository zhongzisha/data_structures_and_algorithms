#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <unordered_set>
#include <unordered_map>
#include <optional>

#include "list/ArrayList.h"
#include "list/ArrayListObject.h"
#include "list/LinkedList.h"
#include "stack/Stack.h"
#include "queue/Queue.h"
#include "queue/Deque.h"
#include "queue/CircleQueue.h"
#include "queue/CircleDeque.h"
#include "tree/BinarySearchTree.h"
#include "tree/AVLTree.h"
#include "tree/RBTree.h"
#include "tree/BTree.h"
#include "tree/BPlusTree.h"
#include "tree/SplayTree.h"
#include "tree/SplayTreeV2.h"
#include "tree/Trie.h"
#include "tree/RadixTree.h"
#include "tree/Treap.h"
#include "tree/KDTree.h"
#include "tree/HuffmanTree.h"
#include "tree/SkipList.h"
#include "tree/SkipListV2.h"
#include "tree/IntervalTreeV2.h"
#include "tree/SegmentTree.h"
#include "tree/BinaryIndexedTree.h"
#include "tree/PrioritySearchTree.h"
#include "heap/BinaryHeap.h"
#include "heap/BinomialHeap.h"
#include "heap/FibonacciHeap.h"
#include "heap/LeftistHeap.h"
#include "heap/SkewHeap.h"
#include "heap/PairingHeap.h"
#include "MyBloomFilter.h"
#include "MyUnionFind.h"
using namespace test;

#include "MySort.h"
using namespace sort_algorithms;

#include "graph/ListGraph.h"
using namespace graph;

#include "MyProblems.h"

#include "MyString.h"

#include "problems/LowestCommonAncestor.h"
#include "problems/MaxFlowMinCut.h"
using namespace problems;

template<> int BinarySearchTree<int>::Node::use_count = 0;  //如果要使用类的静态成员，需要在外部进行初始化
template<> int BTree<int>::Node::use_count = 0;  //如果要使用类的静态成员，需要在外部进行初始化
//template<> std::vector<BTree<int>::Node*> BTree<int>::Node::node_pointers = {};
template<> int BPlusTree<int>::Node::use_count = 0;  //如果要使用类的静态成员，需要在外部进行初始化
template<> int SplayTree<int>::Node::used_count = 0;  //如果要使用类的静态成员，需要在外部进行初始化
template<> int Trie<Person>::Node::used_count = 0;  //如果要使用类的静态成员，需要在外部进行初始化
int Person::used_count = 0;
template<> int RadixTree<Person>::Node::used_count = 0;  //如果要使用类的静态成员，需要在外部进行初始化
template<> int Treap<int>::Node::used_count = 0;
template<> int KDTree<int>::Node::used_count = 0;
template<> int KDTree<int, 3>::Node::used_count = 0;
template<> int SkipList<int, int>::Node::used_count = 0;
template<> int SkipList<int, int>::modulus_table[32] = {
    1, 1, 3, 7, 15, 31, 63, 127, 255, 511, 1023, 2047, 4095, 8191, 16383, 32767,
    65535, 131071, 262143, 524287, 1048575, 2097151, 4194303, 8388607,
    16777215, 33554431, 67108863, 134217727, 268435455, 536870911, 1073741823,
    2147483647
};
template<> int SkipListV2<int, int>::Node::used_count = 0;
template<> int IntervalTreeV2<int, int>::Node::used_count = 0;
int SegmentTree::Node::used_count = 0;
int SegmentTreeV7::Element::used_count = 0;
template<> int Point2D<int>::used_count = 0;
template<> int Point2D<float>::used_count = 0;
template<> int Point2D<double>::used_count = 0;

template<> int BinomialHeap<int>::Node::used_count = 0;
template<> int FibonacciHeap<int>::Node::used_count = 0;
template<> int LeftistHeap<int>::Node::used_count = 0;
template<> int SkewHeap<int>::Node::used_count = 0;
template<> int PairingHeap<int>::Node::used_count = 0;

template<> int HuffmanTree<int>::Node::used_count = 0;
typedef HuffmanTree<int>::Node HuffmanNode;
template<> int BinomialHeap<HuffmanNode*>::Node::used_count = 0;

template<> int ListGraph<std::string, int>::Vertex::used_count = 0;
template<> int ListGraph<std::string, int>::Edge::used_count = 0;

template<> int GenericUnionFind<ListGraph<std::string, int>::Vertex*>::Node::used_count = 0;

int main()
{

    if(0)
    {
        std::cout << "===============================\n 1. test ArrayList(int)\n";
        ArrayList<int> *arr = new ArrayList<int>();
        arr->Add(1);
        arr->Add(2);
        std::cout << "size = " << arr->Size() << ", empty = " << arr->IsEmpty() << "\n";
        std::cout << *arr << "\n";
        delete arr;
    }

    if(0)
    {
        std::cout << "===============================\n 2. test ArrayList(std::string)\n";
        ArrayList<std::string> arr;
        for (int i = 0; i < 15; i++) {
            arr.Add(std::to_string(i));
        }
        std::cout << "size = " << arr.Size() << ", empty = " << arr.IsEmpty() << "\n";
        std::cout << arr << "\n";

        std::string item0 = arr.Remove(0);
        std::string item1 = arr.Remove(1);
        std::cout << "Removed " << item0 << ", " << item1 << "\n";
        std::cout << "size = " << arr.Size() << ", empty = " << arr.IsEmpty() << "\n";
        std::cout << arr << "\n";
    }

    if(0)
    {
        std::cout << "===============================\n 3. test ArrayList(Person)\n";
        ArrayList<Person> arr;
        arr.Add(Person("Jack", 10));
        arr.Add(Person("Rose", 15));
        arr.Add(Person("Lily", 20));
        std::cout << "size = " << arr.Size() << ", empty = " << arr.IsEmpty() << "\n";
        std::cout << arr << "\n";
    }

    if(0)
    {
        std::cout << "===============================\n 4. test ArrayListObject(Person*)\n";
        ArrayListObject<Person*> arr;
        arr.Add(new Person("Jack", 10));
        arr.Add(new Person("Rose", 15));
        arr.Add(new Person("Lily", 20));
        arr.Add(new Person("", 30));
        arr.Add(new Person("", 50));
        std::cout << "size = " << arr.Size() << ", empty = " << arr.IsEmpty() << "\n";
        std::cout << arr << "\n";

        Person* item0 = arr.Remove(0);
        Person* item1 = arr.Remove(1);
        std::cout << "Removed " << item0 << ", " << item1 << "\n";
        std::cout << "size = " << arr.Size() << ", empty = " << arr.IsEmpty() << "\n";
        std::cout << arr << "\n";

        delete item0;
        delete item1;
    }

    if(0)
    {
        std::cout << "===============================\n 5. test LinkedList\n";
        LinkedList<int> l;
        l.Add(1);
        l.Add(2);
        l.Add(3);

        std::cout << "size = " << l.Size() << ", empty = " << l.IsEmpty() << "\n";
        std::cout << l << "\n";

        int item0 = l.Remove(0);
        std::cout << "Removed " << item0 << "\n";
        std::cout << "size = " << l.Size() << ", empty = " << l.IsEmpty() << "\n";
        std::cout << l << "\n";

        item0 = l.Remove(0);
        std::cout << "Removed " << item0 << "\n";
        std::cout << "size = " << l.Size() << ", empty = " << l.IsEmpty() << "\n";
        std::cout << l << "\n";

        item0 = l.Remove(0);
        std::cout << "Removed " << item0 << "\n";
        std::cout << "size = " << l.Size() << ", empty = " << l.IsEmpty() << "\n";
        std::cout << l << "\n";

        //    item0 = l.Remove(0);
        //    std::cout << "Removed " << item0 << "\n";
        //    std::cout << "size = " << l.Size() << ", empty = " << l.IsEmpty() << "\n";
        //    std::cout << l << "\n";
    }

    if(0)
    {
        std::cout << "===============================\n 6. test Stack\n";
        Stack<int> s;
        s.Push(1);
        s.Push(2);
        s.Push(3);
        s.Push(4);
        std::cout << "size = " << s.Size() << ", empty = " << s.IsEmpty() << "\n";
        std::cout << s << "\n";

        std::cout << "Top = " << s.Top() << "\n";
        while (!s.IsEmpty()) {
            int elem = s.Pop();
            std::cout << elem << "\n";
        }
    }

    if(0)
    {
        std::cout << "===============================\n 7. test Queue\n";
        Queue<int> q;
        q.EnQueue(1);
        q.EnQueue(2);
        q.EnQueue(3);
        q.EnQueue(4);
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";

        std::cout << "Front = " << q.Front() << "\n";
        while (!q.IsEmpty()) {
            int elem = q.DeQueue();
            std::cout << elem << "\n";
        }
    }

    if(0)
    {
        std::cout << "===============================\n 8. test Deque\n";
        Deque<int> q;
        q.EnQueueFront(1);
        q.EnQueueFront(2);
        q.EnQueueFront(3);
        q.EnQueueFront(4);
        q.EnQueueRear(1);
        q.EnQueueRear(2);
        q.EnQueueRear(3);
        q.EnQueueRear(4);
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";

        std::cout << "Front = " << q.Front() << "\n";
        while (!q.IsEmpty()) {
            int elem = q.DeQueueFront();
            std::cout << elem << "\n";
        }

        q.EnQueueRear(5);
        q.EnQueueRear(6);
        q.EnQueueRear(7);
        q.EnQueueRear(8);
        q.EnQueueFront(1);
        q.EnQueueFront(2);
        q.EnQueueFront(3);
        q.EnQueueFront(4);
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";

        std::cout << "Front = " << q.Front() << "\n";
        while (!q.IsEmpty()) {
            int elem = q.DeQueueRear();
            std::cout << elem << "\n";
        }
    }

    if(0)
    {
        std::cout << "===============================\n 9. test CircleQueue\n";
        CircleQueue<int> q;
        q.EnQueue(1);
        q.EnQueue(2);
        q.EnQueue(3);
        q.EnQueue(4);
        q.EnQueue(5);

        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << "\n";
        //    while (!q.IsEmpty()) {
        //      int elem = q.DeQueue();
        //      std::cout << elem << "\n";
        //    }

        int i = q.DeQueue(); std::cerr << i << "\n";
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << "\n";

        i = q.DeQueue();
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << "\n";

        q.EnQueue(10);
        q.EnQueue(11);
        q.EnQueue(21);
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << "\n";

        i = q.DeQueue();
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << "\n";
        i = q.DeQueue();
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << "\n";
        i = q.DeQueue();
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << "\n";
        i = q.DeQueue();
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << "\n";
        //    i = q.DeQueue();
        //    std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        //    std::cout << q << "\n";
        //    std::cout << "Front Elem = " << q.Front() << "\n";
        //    i = q.DeQueue();
        //    std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        //    std::cout << q << "\n";
        //    std::cout << "Front Elem = " << q.Front() << "\n";
        //    i = q.DeQueue();
        //    std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        //    std::cout << q << "\n";
        //    std::cout << "Front Elem = " << q.Front() << "\n";
    }

    if(0)
    {
        std::cout << "===============================\n 10. test CircleDeque\n";
        CircleDeque<int> q;
        q.EnQueueFront(1);
        q.EnQueueFront(2);
        q.EnQueueFront(3);
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << ", Rear Elem = " << q.Rear() << "\n";

        q.EnQueueRear(11);
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << ", Rear Elem = " << q.Rear() << "\n";

        q.EnQueueFront(22);
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << ", Rear Elem = " << q.Rear() << "\n";

        q.EnQueueRear(33);
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << ", Rear Elem = " << q.Rear() << "\n";

        int i = q.DeQueueRear();
        std::cout << "Remove " << i << "\n";
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << ", Rear Elem = " << q.Rear() << "\n";

        i = q.DeQueueFront();
        std::cout << "Remove " << i << "\n";
        std::cout << "size = " << q.Size() << ", empty = " << q.IsEmpty() << "\n";
        std::cout << q << "\n";
        std::cout << "Front Elem = " << q.Front() << ", Rear Elem = " << q.Rear() << "\n";
    }

    if(0)
    {
        std::cout << "===============================\n 11. test BinarySearchTree\n";
        BinarySearchTree<int> t;
        int arr[] = {7, 2, 5, 6, 1, 9};
        for (int& i : arr) {
            t.Add(i);
        }
        // t.LevelOrder();
        std::cout << t;

        std::cout << "test predecessor and successor\n";
        for (int& i : arr) {
            std::cout << i << " ====================================\n";
            BinarySearchTree<int>::Node* node = t.GetNode(i);
            std::cout << "Node: " << node << "\n";
            BinarySearchTree<int>::Node* node_predecessor = t.Predecessor(node);
            BinarySearchTree<int>::Node* node_successor = t.Successor(node);
            std::cout << "Predecessor Node: " <<node_predecessor << "\n";
            std::cout << "Successor Node: " <<node_successor << "\n";
        }

        std::cout << "test Remove 7 +++++++++++++++++++++++++++++++++++\n";
        t.Remove(7);
        std::cout << t;
        for (int& i : arr) {
            std::cout << i << " ====================================\n";
            BinarySearchTree<int>::Node* node = t.GetNode(i);
            std::cout << "Node: " << node << "\n";
            BinarySearchTree<int>::Node* node_predecessor = t.Predecessor(node);
            BinarySearchTree<int>::Node* node_successor = t.Successor(node);
            std::cout << "Predecessor Node: " <<node_predecessor << "\n";
            std::cout << "Successor Node: " <<node_successor << "\n";
        }

        std::cout << "test Remove 2 +++++++++++++++++++++++++++++++++++\n";
        t.Remove(2);
        std::cout << t;
        for (int& i : arr) {
            std::cout << i << " ====================================\n";
            BinarySearchTree<int>::Node* node = t.GetNode(i);
            std::cout << "Node: " << node << "\n";
            BinarySearchTree<int>::Node* node_predecessor = t.Predecessor(node);
            BinarySearchTree<int>::Node* node_successor = t.Successor(node);
            std::cout << "Predecessor Node: " <<node_predecessor << "\n";
            std::cout << "Successor Node: " <<node_successor << "\n";
        }

        //    std::cout << "test Remove 1 +++++++++++++++++++++++++++++++++++\n";
        //    t.Remove(1);
        //    std::cout << t;
        //    for (int& i : arr) {
        //      std::cout << i << " ====================================\n";
        //      BinarySearchTree<int>::Node* node = t.GetNode(i);
        //      std::cout << "Node: " << node << "\n";
        //      BinarySearchTree<int>::Node* node_predecessor = t.Predecessor(node);
        //      BinarySearchTree<int>::Node* node_successor = t.Successor(node);
        //      std::cout << "Predecessor Node: " <<node_predecessor << "\n";
        //      std::cout << "Successor Node: " <<node_successor << "\n";
        //    }
        //    std::cout << "test Remove 5 +++++++++++++++++++++++++++++++++++\n";
        //    t.Remove(5);
        //    std::cout << t;
        //    for (int& i : arr) {
        //      std::cout << i << " ====================================\n";
        //      BinarySearchTree<int>::Node* node = t.GetNode(i);
        //      std::cout << "Node: " << node << "\n";
        //      BinarySearchTree<int>::Node* node_predecessor = t.Predecessor(node);
        //      BinarySearchTree<int>::Node* node_successor = t.Successor(node);
        //      std::cout << "Predecessor Node: " <<node_predecessor << "\n";
        //      std::cout << "Successor Node: " <<node_successor << "\n";
        //    }
        //    std::cout << "test Remove 9 +++++++++++++++++++++++++++++++++++\n";
        //    t.Remove(9);
        //    std::cout << t;
        //    for (int& i : arr) {
        //      std::cout << i << " ====================================\n";
        //      BinarySearchTree<int>::Node* node = t.GetNode(i);
        //      std::cout << "Node: " << node << "\n";
        //      BinarySearchTree<int>::Node* node_predecessor = t.Predecessor(node);
        //      BinarySearchTree<int>::Node* node_successor = t.Successor(node);
        //      std::cout << "Predecessor Node: " <<node_predecessor << "\n";
        //      std::cout << "Successor Node: " <<node_successor << "\n";
        //    }

        //    std::cout << "test Remove 6 +++++++++++++++++++++++++++++++++++\n";
        //    t.Remove(6);
        //    std::cout << t;
        //    for (int& i : arr) {
        //      std::cout << i << " ====================================\n";
        //      BinarySearchTree<int>::Node* node = t.GetNode(i);
        //      std::cout << "Node: " << node << "\n";
        //      BinarySearchTree<int>::Node* node_predecessor = t.Predecessor(node);
        //      BinarySearchTree<int>::Node* node_successor = t.Successor(node);
        //      std::cout << "Predecessor Node: " <<node_predecessor << "\n";
        //      std::cout << "Successor Node: " <<node_successor << "\n";
        //    }
    }

    if (0)
    {
        std::cout << "===============================\n 12. test AVLTree\n";
        AVLTree<int> t;
        int arr[] = {
            7, 4, 9, 2, 5, 8, 11, 3, 12, 1
            //1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            std::cout << "Add " << i << " ====================================\n";
            t.Add(i);
            std::cout << t << "\n";
            std::cout << "IsAVLTree: " << t.IsAVLTree() << "\n";
        }
        // t.LevelOrder();

        for (int& i : arr) {
            std::cout << "Remove " << i << " ====================================\n";
            t.Remove(i);
            std::cout << t << "\n";
            std::cout << "IsAVLTree: " << t.IsAVLTree() << "\n";
        }
    }

    if(0)
    {
        std::cout << "===============================\n 13. test RBTree\n";
        RBTree<int> t;
        int arr[] = {
            7, 4, 9, 2, 5, 8, 11, 3, 12, 1
            // 55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50
            //1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            std::cout << "Add " << i << " ====================================\n";
            t.Add(i);
            std::cout << t << "\n";
            std::cout << "IsBST: " << t.IsBST() << "\n";
            std::cout << "IsRBTree: " << t.IsRBTree() << "\n";
        }
        // t.LevelOrder();

        for (int& i : arr) {
            std::cout << "Remove " << i << " ====================================\n";
            t.Remove(i);
            std::cout << t << "\n";
            std::cout << "IsBST: " << t.IsBST() << "\n";
            std::cout << "IsRBTree: " << t.IsRBTree() << "\n";
        }
    }

    if (0)
    {
        std::cout << "===============================\n 13. test BTree\n";
        BTree<int> t;
        int arr[] = {
            7, 4, 9, 2, 5, 8, 11, 3, 12, 1
            //55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50
            //1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            std::cout << "Add " << i << " ====================================\n";
            t.Add(i);
            std::cout << t << "\n";
        }

        for (int& i : arr) {
            std::cout << "Remove " << i << " ====================================\n";
            t.Remove(i);
            std::cout << t << "\n";
        }
    }

    if (0)
    {
        std::cout << "===============================\n 13. test BTree\n";
        BTree<int> *t = new BTree<int>();
        int arr[] = {
            7, 4, 9, 2, 5, 8, 11, 3, 12, 1,
            //55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50,
            //1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            std::cout << "Add " << i << " ====================================\n";
            t->Add(i);
            std::cout << *t << "\n";
        }

        for (int& i : arr) {
            std::cout << "Remove " << i << " ====================================\n";
            t->Remove(i);
            std::cout << *t << "\n";
        }

        delete t;
    }

    if (0)
    {
        std::cout << "===============================\n 14. test BPlusTree\n";
        BPlusTree<int> *t = new BPlusTree<int>();
        int arr[] = {
            7, 4, 1, 2, 5, 12, 11, 3, 8, 9,
            55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50,
            1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            std::cout << "Add " << i << " ====================================\n";
            t->Add(i);
            std::cout << *t << "\n";
        }

        int i = 1;
        std::cout << "Remove " << i << " ====================================\n";
        t->Remove(i);
        std::cout << *t << "\n";

        i = 12;
        std::cout << "Remove " << i << " ====================================\n";
        t->Remove(i);
        std::cout << *t << "\n";

        for (int& i : arr) {
            std::cout << "Remove " << i << " ====================================\n";
            t->Remove(i);
            std::cout << *t << "\n";
        }

        delete t;
        std::cout << "\n";
    }

    if (0)
    {
        std::cout << "===============================\n 15. test SplayTree\n";
        SplayTree<int> *t = new SplayTree<int>();
        int arr[] = {
            //6,5,4,3,1,2
            //10, 50, 40, 30, 20, 60
            //7, 4, 9, 2, 5, 8, 11, 3, 12, 1,
            55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50,
            //1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";
            t->Add(i);
            std::cout << *t;
            std::cout << "IsBST: " << t->IsBST() << "\n";
        }

        //    int search_arr[] = {
        //      1, 4, 5, 2, 4, 4, 6, 2, 2, 4, 4, 2, 4
        //    };
        //    for (int& i : search_arr) {
        //      std::cout << "\nFind " << i << " ====================================\n";
        //      bool found = t->Contains(i);
        //      std::cout << i << (found ? " Found " : " Not Found") << "\n";
        //      std::cout << *t;
        //      std::cout << "IsBST: " << t->IsBST() << "\n";
        //    }

        SplayTree<int> *t2;
        {
            t2 = new SplayTree<int>();
            for (const int& i : {6,5,4,3,1,2}) {
                std::cout << "\nAdd " << i << " ====================================\n";
                t2->Add(i);
                std::cout << *t2;
                std::cout << "IsBST: " << t2->IsBST() << "\n";
            }

            t = t2->MergeAnotherTree(t);
        }

        for (int& i : arr) {
            std::cout << "\nRemove " << i << " ====================================\n";
            t->Remove(i);
            std::cout << *t;
            std::cout << "IsBST: " << t->IsBST() << "\n";
        }

        //根据给定的元素分裂成两棵树，左边<=element，右边>element
        // now the nodes in t is managed by splited_trees
        std::vector<SplayTree<int>*> splited_trees = t->SplitToTwoTrees(4);
        std::cout << splited_trees[0];
        std::cout << "IsBST: " << splited_trees[0]->IsBST() << "\n";
        std::cout << splited_trees[1];
        std::cout << "IsBST: " << splited_trees[1]->IsBST() << "\n";
        for (auto& p : splited_trees) {  // this is real delete
            delete p; p = nullptr;
        }
        //splited_trees.clear();                              // no real delete
        //splited_trees.shrink_to_fit();                      // no real delete
        //std::vector<SplayTree<int>*>().swap(splited_trees); // no real delete

        //delete t;  // no need
    }


    if (0)
    {
        std::cout << "===============================\n 16. test SplayTree\n";
        SplayTree<int> *t = new SplayTree<int>();
        int arr[] = {
            //6,5,4,3,1,2
            //10, 50, 40, 30, 20, 60
            //7, 4, 9, 2, 5, 8, 11, 3, 12, 1,
            55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50,
            //1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";
            t->Add(i);
            std::cout << *t;
            std::cout << "IsBST: " << t->IsBST() << "\n";
        }

        //    int search_arr[] = {
        //      1, 4, 5, 2, 4, 4, 6, 2, 2, 4, 4, 2, 4
        //    };
        //    for (int& i : search_arr) {
        //      std::cout << "\nFind " << i << " ====================================\n";
        //      bool found = t->Contains(i);
        //      std::cout << i << (found ? " Found " : " Not Found") << "\n";
        //      std::cout << *t;
        //      std::cout << "IsBST: " << t->IsBST() << "\n";
        //    }

        t->RangeFind(0, 1000);

        std::cout << *t << "\n";

        delete t;
    }

    if (0)
    {
        std::cout << "===============================\n 16. test SplayTree\n";
        SplayTree<int> *t = new SplayTree<int>();
        std::vector<int> arr = {
            //6,5,4,3,1,2
            //10, 50, 40, 30, 20, 60
            //7, 4, 9, 2, 5, 8, 11, 3, 12, 1,
            55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50,
            50, 22, 90, 22
            //1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";
            t->Add(i);
            std::cout << *t;
            std::cout << "IsBST: " << t->IsBST() << "\n";
        }


        for (int& i : arr) {
            // std::cout << "Rank of " << i << " is " << t->GetRank_V1(i) << "\n";
            std::pair<int, int> result = t->GetRank(i);
            std::cout << "Rank of " << i << " is " << result.first << "-->" << result.second << "\n";
        }

        std::cout << *t;
        size_t rank = 1;
        do {
            std::cout << "The element of rank " << rank << " is " << t->GetKth(rank) << "\n";
        } while (rank++ < arr.size());

        t->InOrder();

        //    for (int& i : arr) {
        //      std::cout << "\nRemove " << i << " ====================================\n";
        //      t->Remove(i);
        //      std::cout << *t;
        //      std::cout << "IsBST: " << t->IsBST() << "\n";
        //    }

        std::cout << *t;

        delete t;
    }

    if (0) {
        std::cout << "===============================\n 17. test SplayTree\n";
        // https://www.keithschwarz.com/interesting/code/?dir=splay-tree
        other::SplayTree<int, int> t;
        int arr[] = {
            //6,5,4,3,1,2
            //10, 50, 40, 30, 20, 60
            //7, 4, 9, 2, 5, 8, 11, 3, 12, 1,
            55, 87, 56, 74, 96, 22, 62, 20, 70, 68, 90, 50,
            //1, 3, 14, 21, 57, 85, 99, 19, 7, 69, 2, 11, 44, 95, 4, 56, 58, 70, 93
        };
        for (int& i : arr) {
            // std::cout << "\nAdd " << i << " ====================================\n";
            t.insert(i, i);
        }
        std::cout << t;

        for (int& i : arr) {
            std::cout << "\nRemove " << i << " ====================================\n";
            t.erase(i);
            std::cout << t;
        }
    }

    if (0)
    {
        std::cout << "===============================\n 18. test Trie\n";
        Trie<Person> *t = new Trie<Person>();
        t->Add("abc", new Person("abc", 10));
        t->Add("abcd", new Person("abcd", 20));
        t->Add("def", new Person("def", 30));
        t->Add("apple", new Person("apple", 21));
        t->Add("applefree", new Person("applefree", 31));

        std::cout << t;

        std::cout << "Remove abcd ===============================\n";
        Person* value = nullptr;
        value = t->Remove("abcd");
        std::cout << value << "\n";
        std::cout << t << "\n";
        delete value;

        std::cout << "Remove abc ===============================\n";
        value = t->Remove("abc");
        std::cout << value << "\n";
        std::cout << t << "\n";
        delete value;

        std::cout << "Remove def ===============================\n";
        value = t->Remove("def");
        std::cout << value << "\n";
        std::cout << t << "\n";
        delete value;


        delete t;
    }


    if (0)
    {
        std::cout << "===============================\n 19. test RadixTree\n";
        RadixTree<Person> *t = new RadixTree<Person>();
        t->Add("abc", new Person("abc", 10));
        t->Add("abcd", new Person("abcd", 20));
        t->Add("def", new Person("def", 30));
        t->Add("apple", new Person("apple", 21));
        t->Add("applefree", new Person("applefree", 31));

        std::cout << t;

        bool f1;
        f1 = t->Contains("abc");
        f1 = t->Contains("bc");
        f1 = t->Contains("a");
        f1 = t->Contains("appl");
        f1 = t->Contains("applefree");
        f1 = t->Contains("de");
        f1 = t->Contains("def");

        std::cout << t << "\n";

        std::cout << "Remove abcd ===============================\n";
        t->Remove("abcd");
        std::cout << t << "\n";

        std::cout << "Remove apple ===============================\n";
        t->Remove("apple");
        std::cout << t << "\n";

        std::cout << "Remove abc ===============================\n";
        t->Remove("abc");
        std::cout << t << "\n";

        std::cout << "Remove def ===============================\n";
        t->Remove("def");
        std::cout << t << "\n";


        delete t;
    }


    if (0)
    {
        //将一个字符串的所有后缀添加到一颗压缩trie里面，就是后缀树？
        std::cout << "===============================\n 19. test RadixTree\n";
        RadixTree<Person> *t = new RadixTree<Person>();
        t->Add("banana", new Person("banana", 10));
        t->Add("anana", new Person("anana", 20));
        t->Add("nana", new Person("nana", 30));
        t->Add("ana", new Person("ana", 21));
        t->Add("na", new Person("na", 31));
        t->Add("a", new Person("a", 31));

        std::cout << t;

        delete t;
    }

    if (0) {
        // 测试shared_ptr的用法，自动释放内存，不需要手动delete
        std::vector<std::shared_ptr<Person>> a;
        std::shared_ptr<Person> p1 = std::make_shared<Person>("a", 10);
        std::shared_ptr<Person> p2 = std::make_shared<Person>("b", 20);
        a.push_back(p1);
        a.push_back(p2);


        std::shared_ptr<Person> p3 = std::make_shared<Person>("c", 20);
        a[0] = p3;

        std::cout << "yes1\n";

        a.clear();

        std::cout << "yes2\n";

    }

    if (0) {
        std::cout << "===============================\n 20. test Treap\n";
        int arr[] = {
            10, 9, 18, 20, 7, 140, 2, 1, 9,7
        };

        test::Treap<int> *t = new test::Treap<int>();

        for(int& i : arr) {
            t->Add(i);
        }

        std::cout << t;

        for(int& i : arr) {
            t->Remove(i);
        }
        std::cout << t;

        delete t;
    }


    if (0) {
        std::cout << "===============================\n 21. test KDTree\n";
        int arr[][2] = {
            {30, 40}, {5, 25}, {70, 70}, {10, 12}, {50, 30}, {35, 45}
        };

        typedef test::KDTree<int, 2>::Point Point;
        test::KDTree<int, 2> *t = new test::KDTree<int, 2>();

        int size = sizeof(arr) / sizeof(arr[0]);
        for (int i = 0; i< size; i++) {
            std::cout << "\nAdd " << arr[i] << " ====================================\n";
            t->Add(Point(arr[i]));
        }

        std::cout << t;

        for (int i = 0; i< size; i++) {
            std::cout << "\nRemove " << Point(arr[i]) << " ====================================\n";
            t->Remove(Point(arr[i]));
            std::cout << t;
        }

        delete t;
    }

    if (0) {
        std::cout << "===============================\n 21. test KDTree 3\n";
        int arr[][3] = {
            {30, 40, 1}, {5, 25, 1}, {70, 70, 1}, {10, 12, 1}, {50, 30, 1}, {35, 45, 1}
        };

        typedef test::KDTree<int, 3>::Point Point;
        test::KDTree<int, 3> *t = new test::KDTree<int, 3>();

        int size = sizeof(arr) / sizeof(arr[0]);
        for (int i = 0; i< size; i++) {
            std::cout << "\nAdd " << arr[i] << " ====================================\n";
            t->Add(Point(arr[i]));
        }

        std::cout << t;

        for (int i = 0; i< size; i++) {
            std::cout << "\nRemove " << Point(arr[i]) << " ====================================\n";
            t->Remove(Point(arr[i]));
            std::cout << t;
        }

        delete t;
    }

    if (0) {
        std::cout << "===============================\n 22. test BinaryHeap\n";

        int arr[] = {
            68, 72, 43, 50, 38, 10, 90, 65
        };
        int length = sizeof(arr) / sizeof(arr[0]);

        //    test::BinaryHeap<int> *t = new BinaryHeap<int>();

        //    for (int& i : arr) {
        //      std::cout << "\nAdd " << i << " ====================================\n";
        //      t->Add(i);
        //      std::cout << t;
        //    }

        //    t->Print();

        //    for (int i = 0; i < length; i++) {
        //      std::cout << "\nRemove " << " ====================================\n";
        //      t->Remove();
        //      std::cout << t;
        //    }

        //    delete t;

        test::BinaryHeap<int> *t2 = new BinaryHeap<int>(arr, length);
        std::cout << t2;
        delete t2;
    }

    if (0) {
        std::cout << "===============================\n 22. test BinomialHeap\n";
        typedef test::BinomialHeap<int>::Node Node;
        test::BinomialHeap<int> *t = new test::BinomialHeap<int>();

        if (0){ // 测试链表合并
            int arr[] = {
                1,3,5,7
            };
            int length = sizeof(arr) / sizeof(arr[0]);

            int arr2[] = {
                2,3,6,8,9,10
            };
            int length2 = sizeof(arr2) / sizeof(arr2[0]);

            Node* nodeList1 = t->GenerateNodeList(arr, length);
            t->PrintNodeList(nodeList1);
            Node* nodeList2 = t->GenerateNodeList(arr2, length2);
            t->PrintNodeList(nodeList2);

            Node* mergedList = t->MergeNode(nodeList1, nodeList2);
            t->PrintNodeList(mergedList);

            Node* reversedList = t->ReverseNodeList(mergedList);
            t->PrintNodeList(reversedList);

            //    t->DeleteNodeList(nodeList1);
            //    t->DeleteNodeList(nodeList2);

            t->DeleteNodeList(reversedList);
        }

        int arr[] = {
            37,10,1,41,28,13,6,16,12,25,77,8,14,29,26,23,18,
            11,17,38,42,27
        };
        // int length = sizeof(arr) / sizeof(arr[0]);
        for (int i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";

            t->Add(i);

            std::cout << t;

            if (!t->Check()) {
                throw "Invalid BinomialHeap.\n";
            }
        }

        std::cout << t;

        Node* minNode = t->ExtractMin();
        std::cout << "minNode: " << minNode << "\n";
        delete minNode;

        std::cout << t;

        Node* node12 = t->FindNode(t->_root, 12);
        std::cout << "FOUND 12: " << node12 << "\n";

        node12->_key = 100;
        bool isValidHeap = t->Check();
        std::cout << "IsValidHeap: " << isValidHeap << "\n";
        node12->_key = 12;

        std::cout << t;

        std::cout << "\nremove the key 37 \n";
        t->Remove(37, INT_MIN);
        std::cout << t;

        std::cout << "\nremove the key 12 \n";
        t->Remove(12, INT_MIN);
        std::cout << t;

        std::cout << "\nremove the key 17 \n";
        t->Remove(17, INT_MIN);
        std::cout << t;

        t->Visit();

        std::cout << "\nremove the key 14 \n";
        t->Remove(14, INT_MIN);
        std::cout << t;

        for (int i : arr) {
            std::cout << "\nRemove " << i << " ====================================\n";

            t->Remove(i, INT_MIN);

            std::cout << t;

            if (!t->Check()) {
                throw "Invalid BinomialHeap.\n";
            }
        }

        std::cout << t;

        delete t;
    }

    if (0) {
        std::cout << "===============================\n 23. test FibonacciHeap\n";
        typedef test::FibonacciHeap<int>::Node Node;
        test::FibonacciHeap<int> *t = new test::FibonacciHeap<int>();

        //    int arr[] = {
        //      23,7,3,17,24,18,52,38,30,26,46,39,41,35
        //    };

        size_t length = 500000;
        std::vector<int> arr(length);
        for (size_t i = 0; i < length; i++) {
            arr[i] = rand();
        }

        // int length = sizeof(arr) / sizeof(arr[0]);
        for (int& i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";

            t->Add(i);

            //      std::cout << t;

            //      if (!t->Check()) {
            //        throw "Invalid BinomialHeap.\n";
            //      }
        }

        //std::cout << t;
        std::cout << "Size = " << t->_size << "\n";

        Node* minNode = t->ExtractMin();
        std::cout << "Fout minNode = " << minNode << "\n";
        delete minNode;

        //std::cout << t;

        //int i = 23;
        //std::cout << "\nRemove " << i << " ====================================\n";

        //t->Remove(i, INT_MIN);

        // std::cout << t;


        for (int& i : arr) {
            std::cout << "\nRemove " << i << " ====================================\n";

            //      if (i == 18) {
            //        std::cout << i << "\n";
            //      }

            //t->Remove(i, INT_MIN);
            std::cout << "Size = " << t->_size << "\n";

            Node* minNode = t->ExtractMin();
            std::cout << "Fout minNode = " << minNode << "\n";
            delete minNode;
            // std::cout << t;

            //      if (!t->Check()) {
            //        throw "Invalid BinomialHeap.\n";
            //      }
        }

        std::cout << t;

        delete t;
    }

    if (0) {
        std::cout << "===============================\n 23. test LeftistHeap\n";
        //typedef test::LeftistHeap<int>::Node Node;
        test::LeftistHeap<int> *t = new test::LeftistHeap<int>();

        const size_t length = 100000;
        int arr[length] = {0};
        for (size_t i = 0; i < length; i++) {
            arr[i] = rand();
        }

        //    int arr[] = {
        //      23,7,3,17,24,18,52,38,30,26,46,39,41,35
        //    };

        for (int& i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";
            t->Push(i);
        }

        std::cout << t;

        while (!t->IsEmpty()) {
            t->Pop();
        }

        std::cout << t;

        delete t;
    }

    if (0) {
        std::cout << "===============================\n 23. test SkewHeap\n";
        //typedef test::SkewHeap<int>::Node Node;
        test::SkewHeap<int> *t = new test::SkewHeap<int>();

        const size_t length = 100000;
        int arr[length] = {0};
        for (size_t i = 0; i < length; i++) {
            arr[i] = rand();
        }

        //    int arr[] = {
        //      23,7,3,17,24,18,52,38,30,26,46,39,41,35
        //    };

        for (int& i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";
            t->Push(i);
        }

        std::cout << t;

        while (!t->IsEmpty()) {
            t->Pop();
        }

        std::cout << t;

        delete t;
    }

    if (0) {
        std::cout << "===============================\n 23. test PairingHeap\n";
        //typedef test::PairingHeap<int>::Node Node;
        test::PairingHeap<int> *t = new test::PairingHeap<int>();

        const size_t length = 100000;
        int arr[length] = {0};
        for (size_t i = 0; i < length; i++) {
            arr[i] = rand();
        }

        //        int arr[] = {
        //          23,7,3,17,24,18,52,38,30,26,46,39,41,35
        //        };

        for (int& i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";
            t->Push(i);
        }

        std::cout << t;

        while (!t->IsEmpty()) {
            // std::cout << t;
            int minElement = t->Pop();
            std::cout << "Removed " << minElement << " ====================================\n";
        }

        std::cout << t;

        delete t;
    }

    if (0) {
        std::cout << "===============================\n 23. test HuffmanTree\n";
        //typedef test::HuffmanTree<int>::Node Node;
        test::HuffmanTree<int> *t = new test::HuffmanTree<int>();

        //    const size_t length = 100000;
        //    int arr[length] = {0};
        //    for (size_t i = 0; i < length; i++) {
        //      arr[i] = rand();
        //    }

        int arr[] = {
            5,6,8,7,15
        };
        size_t length = sizeof(arr) / sizeof(arr[0]);

        t->Create(arr, length);

        std::cout << t;

        delete t;

    }

    if (0) {
        std::cout << "===============================\n 23. test SkipList\n";
        //typedef test::SkipList<int, int>::Node Node;
        test::SkipList<int, int> *t = new test::SkipList<int, int>();

        //    int arr[] = {
        //        23,7,3,17,24,18,52,38,30,26,46,39,41,35
        //    };

        const size_t length = 100;
        int arr[length] = {0};
        for (size_t i = 0; i < length; i++) {
            arr[i] = rand();
        }

        for (int& i : arr) {
            std::cout << "\nAdd " << i << " ====================================\n";
            t->Push(i, i);
        }

        std::cout << t;


        delete t;
    }

    if (0) {
        std::cout << "===============================\n 23. test SkipListV2\n";
        //typedef test::SkipList<int, int>::Node Node;
        test::SkipListV2<int, int> *t = new test::SkipListV2<int, int>();

        //    int arr[] = {
        //        23,7,3,17,24,18,52,38,30,26,46,39,41,35
        //    };

        const size_t length = 200;
        int arr[length] = {0};
        for (size_t i = 0; i < length; i++) {
            arr[i] = rand() % 100;
        }

        {
            for (int& i : arr) {
                std::cout << "\nAdd " << i << " ====================================\n";
                t->Put(i, i + rand() % 10);
            }
            std::cout << t;
        }
        {
            std::pair<bool, int> result = t->Get(arr[length>>1]);
            if (result.first) {
                std::cout << arr[length>>1] << ":" << result.second << "\n";
            } else {
                std::cout << "no Key " << arr[length>>1] << "\n";
            }
        }
        {
            int key = 101;
            std::pair<bool, int> result = t->Get(key);
            if (result.first) {
                std::cout << key << ":" << result.second << "\n";
            } else {
                std::cout << "no Key " << key << "\n";
            }
        }

        if (0) {
            for (int& i : arr) {
                std::cout << "\nRemove " << i << " ====================================\n";
                std::pair<bool, int> result = t->Remove(i);
                if (result.first) {
                    std::cout << i << ":" << result.second << "\n";
                } else {
                    std::cout << "no Key " << i << "\n";
                }
                std::cout << t;
            }

            std::cout << t;
        }

        delete t;
    }

    if (0) {
        std::cout << "===============================\n test BubbleSortV1\n";
        int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
        size_t length = sizeof(arr) / sizeof(arr[0]);
        BubbleSortV1(arr, length);
        PrintArray(arr, length);
    }
    if (0) {
        std::cout << "===============================\n test BubbleSortV2\n";
        int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
        size_t length = sizeof(arr) / sizeof(arr[0]);
        BubbleSortV2(arr, length);
        PrintArray(arr, length);
    }
    if (0) {
        std::cout << "===============================\n test BubbleSortV3\n";
        int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
        size_t length = sizeof(arr) / sizeof(arr[0]);
        BubbleSortV3(arr, length);
        PrintArray(arr, length);
    }
    if (0) {
        std::cout << "===============================\n test SelectionSortV1\n";
        int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
        size_t length = sizeof(arr) / sizeof(arr[0]);
        SelectionSortV1(arr, length);
        PrintArray(arr, length);
    }
    if (0) {
        std::cout << "===============================\n test Sorting \n";
        int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30,23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
        size_t length = sizeof(arr) / sizeof(arr[0]);
        PrintArray(arr, length);

        SelectionSort<int> selectionSort(arr, length);
        BubbleSort<int> bubbleSort(arr, length);
        HeapSort<int> heapSort(arr, length);
        InsertionSortV1<int> insertionSort(arr, length);
        InsertionSortV2<int> insertionSortV2(arr, length);
        InsertionSortV3<int> insertionSortV3(arr, length);
        MergeSort<int> mergeSort(arr, length);
        QuickSort<int> quickSort(arr, length);
        ShellSort<int> shellSort(arr, length);
        CountingSort<int> countingSort(arr, length);
        RadixSort<int> radixSort(arr, length);
    }
    if (0) {
        double arr[] = {0.9,0.8,0.7,0.6,0.5,0.4,0.3,0.2,0.1,0.0};
        size_t length = sizeof(arr) / sizeof(arr[0]);
        PrintArray(arr, length);

        BucketSort<double> bucetSort(arr, length);
    }

    if (0) {
        //有向图
        ListGraph<std::string, int> *g = new ListGraph<std::string, int>();

        //      g->AddEdge("V0", "V1");
        //      g->AddEdge("V1", "V0");

        //      g->AddEdge("V0", "V2");
        //      g->AddEdge("V2", "V0");

        //      g->AddEdge("V0", "V3");
        //      g->AddEdge("V3", "V0");

        //      g->AddEdge("V1", "V2");
        //      g->AddEdge("V2", "V1");

        //      g->AddEdge("V2", "V3");
        //      g->AddEdge("V3", "V2");
        //      g->AddEdge("V2", "V3");
        //      g->AddEdge("V3", "V2");

        std::cout << g;

        g->AddEdge("V1", "V0", 9);


        std::cout << g;


        g->AddEdge("V1", "V2", 3);
        std::cout << g;

        g->AddEdge("V2", "V0", 2);
        std::cout << g;

        g->AddEdge("V2", "V3", 5);
        std::cout << g;

        g->AddEdge("V3", "V4", 1);
        std::cout << g;

        g->AddEdge("V0", "V4", 6);

        std::cout << g;

        g->AddEdge("V3", "V4", 6);

        std::cout << g;

        //      g->RemoveVertex("V0");
        //      g->RemoveVertex("V1");
        //      std::cout << g;
        //      g->RemoveEdge("V3", "V0");
        //      g->RemoveEdge("V3", "V2");


        g->BFS("V1");

        std::cout << g;

        g->DFS("V1");
        g->DFS_Iteratively("V1");

        g->TopologicalSort();  //有向无环图


        delete g;
    }
    if (0) {
        //无向图
        ListGraph<std::string, int> *g = new ListGraph<std::string, int>();

        // graph1
        if (0) {
            //有向图
            g->AddEdge("V0", "V1", 1);

            g->AddEdge("V0", "V2", 2);

            g->AddEdge("V0", "V3", 3);

            g->AddEdge("V1", "V2", 4);

            g->AddEdge("V2", "V3", 5);

            g->AddVertex("V");
        }

        if (0) {
            //无向图
            g->AddEdge("A", "B", 17);
            g->AddEdge("B", "A", 17);

            g->AddEdge("A", "F", 1);
            g->AddEdge("F", "A", 1);

            g->AddEdge("A", "E", 16);
            g->AddEdge("E", "A", 16);

            g->AddEdge("F", "B", 11);
            g->AddEdge("B", "F", 11);

            g->AddEdge("E", "F", 33);
            g->AddEdge("F", "E", 33);

            g->AddEdge("E", "D", 4);
            g->AddEdge("D", "E", 4);
            g->AddEdge("F", "D", 14);
            g->AddEdge("D", "F", 14);
            g->AddEdge("B", "D", 5);
            g->AddEdge("D", "B", 5);
            g->AddEdge("B", "C", 6);
            g->AddEdge("C", "B", 6);
            g->AddEdge("D", "C", 10);
            g->AddEdge("C", "D", 10);

            g->AddVertex("V"); //添加一个孤立顶点，变成非连通图
        }

        if (1) {
            // 带负权值的边（Dijkstra也能检测出最短路径，可能跟实现有关）
            g->AddEdge("A", "B", -1);
            g->AddEdge("B", "C", 3);
            g->AddEdge("D", "B", 2); //！！！！等于1会存在0权环,打印最短路径会不正确
            g->AddEdge("E", "D", -3);
            g->AddEdge("A", "C", 4);
            g->AddEdge("B", "D", 2);
            g->AddEdge("D", "C", 5);
            g->AddEdge("B", "E", 2);
            g->AddVertex("V");
            if (0) {
                //打开这里设置成无向图，BellmanFord会检测出负权环，不存在最短路径
                //关闭这里是一个有向图，不存在负权环(有一个0权环)，存在最短路径
                g->AddEdge("B", "A", -1);
                g->AddEdge("C", "B", 3);
                g->AddEdge("B", "D", 2); //！！！！等于1会存在0权环,打印最短路径会不正确
                g->AddEdge("D", "E", -3);
                g->AddEdge("C", "A", 4);
                g->AddEdge("D", "B", 2);
                g->AddEdge("C", "D", 5);
                g->AddEdge("E", "B", 2);

            }
        }

        if (0) {
            //负权环
            g->AddEdge("A", "B", 1);
            g->AddEdge("B", "C", 7);
            g->AddEdge("B", "A", -2);
        }

        std::cout << g;

        g->BFS("V1");

        std::cout << g;

        g->DFS("V1");
        g->DFS_Iteratively("V1");

        //MST必须用在联通图上
        //      g->MST_Prim();
        //      g->MST_Prim_UsingMinHeap();
        //      g->MST_Prim_UsingMinHeap2();
        //      g->MST_Kruskal();

        g->ShortestPath_Dijkstra("V1");
        g->ShortestPath_Dijkstra_WithPath("V1");
        g->ShortestPath_Dijkstra_WithPath2("V1");
        g->ShortestPath_BellmanFord("V1");
        g->ShortestPath_Dijkstra("V0");
        g->ShortestPath_Dijkstra_WithPath("V0");
        g->ShortestPath_Dijkstra_WithPath2("V0");
        g->ShortestPath_BellmanFord("V0");
        g->ShortestPath_Dijkstra("V3");
        g->ShortestPath_Dijkstra_WithPath("V3");
        g->ShortestPath_Dijkstra_WithPath2("V3");
        g->ShortestPath_BellmanFord("V3");

        g->ShortestPath_Dijkstra("A");
        g->ShortestPath_Dijkstra_WithPath("A");
        g->ShortestPath_Dijkstra_WithPath2("A");
        g->ShortestPath_BellmanFord("A");
        g->ShortestPath_Floyd();
        g->ShortestPath_Floyd_WithPath();

        g->ShortestPath_Dijkstra("E");
        g->ShortestPath_Dijkstra_WithPath("E");
        g->ShortestPath_Dijkstra_WithPath2("E");
        g->ShortestPath_BellmanFord("E");
        delete g;
    }

    if (0) {
        using namespace my_problems;

        // Fibonacci
        if (0) {
            int result;
            int n = 20;

            {
                result = Fibonacci::Run_v1(n);
                std::cout << "result = " << result << "\n";
            }

            {
                result = Fibonacci::Run_v2(n);
                std::cout << "result = " << result << "\n";
            }
            {
                result = Fibonacci::Run_v3(n);
                std::cout << "result = " << result << "\n";
            }
            {
                result = Fibonacci::Run_v4(n);
                std::cout << "result = " << result << "\n";
            }
            {
                result = Fibonacci::Run_v5(n);
                std::cout << "result = " << result << "\n";
            }
            {
                result = Fibonacci::Run_v6(n);
                std::cout << "result = " << result << "\n";
            }
            {
                result = Fibonacci::Run_v7(n);
                std::cout << "result = " << result << "\n";
            }

            // ClimpStairs
            {
                result = ClimpStairs::Run(n);
                std::cout << "result = " << result << "\n";
            }
        }

        // Hanoi
        if (0) {
            Hanoi::Run(1, "a", "b", "c");
            Hanoi::Run(2, "a", "b", "c");
            Hanoi::Run(3, "a", "b", "c");
        }

        // Queens
        if (0) {
            Queens *queen = new Queens(4);
            int result = queen->GetWays();
            std::cout << "result = " << result << "\n";
            delete queen;
        }

        // Pirate
        if (0) {
            int capacity = 30;
            int weights[] = {3, 4, 5, 10, 1, 2, 5, 7};
            size_t size = sizeof(weights) / sizeof(weights[0]);
            int result;
            result = Pirate::Run(capacity, weights, size);
            std::cout << "result = " << result << "\n";
        }

        // CoinChange
        if (0) {
            int money = 41;
            // int faces[] = {1, 10, 5, 25};
            int faces[] = {1, 20, 5, 25};  // 不是最优解
            size_t size = sizeof(faces) / sizeof(faces[0]);
            int result;
            result = CoinChange::Run(money, faces, size);
            std::cout << "result = " << result << "\n";
        }

        // MaxSubSeqSum
        if (0) {
            int nums[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
            size_t size = sizeof(nums) / sizeof(nums[0]);
            int result;
            {
                result = MaxSubSeqSum::Run_V1(nums, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = MaxSubSeqSum::Run_V2(nums, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = MaxSubSeqSum::Run_V3(nums, size);
                std::cout << "result = " << result << "\n";
            }
        }

        // 找零钱,DP
        if (0) {
            int money = 41;
            // int faces[] = {1, 10, 5, 25};
            int faces[] = {1, 20, 5, 25};  // 不是最优解
            size_t size = sizeof(faces) / sizeof(faces[0]);
            int result;
            {
                result = CoinChange_DP::Run(money, faces, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = CoinChange_DP::Run_V2(money, faces, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = CoinChange_DP::Run_V3(money, faces, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = CoinChange_DP::Run_V4(money, faces, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = CoinChange_DP::Run_V5(money, faces, size);
                std::cout << "result = " << result << "\n";
            }
            {
                int faces1[] = {20, 4, 25};  // 测试凑不够的情况
                size_t size1 = sizeof(faces1) / sizeof(faces1[0]);
                result = CoinChange_DP::Run_V6(money, faces1, size1);
                std::cout << "result = " << result << "\n";
            }
        }

        // 最大连续子序列和,DP
        if (0) {
            int nums[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
            int length = sizeof(nums) / sizeof(nums[0]);
            int result;
            {
                result = MaxSubSeqSum_DP::Run(nums, length);
                std::cout << "result = " << result << "\n";
            }
            {
                result = MaxSubSeqSum_DP::Run_V2(nums, length);
                std::cout << "result = " << result << "\n";
            }
        }

        // 最长上升子序列长度,DP
        if (0) {
            int nums[] = {10, 2, 2, 5, 1, 7, 101, 18};
            int length = sizeof(nums) / sizeof(nums[0]);
            int result;
            {
                result = LongestIncreasiveSubSeq::Run(nums, length);
                std::cout << "result = " << result << "\n";
            }
            {
                result = LongestIncreasiveSubSeq::Run_V2(nums, length);
                std::cout << "result = " << result << "\n";
            }
        }

        // 最长公共子序列,DP
        if (0) {
            int nums1[] = {1, 3, 9, 5, 4, 8, 7, 5, 100, 10};
            // int nums2[] = {1, 9, 3, 7, 10, 11};
            int nums2[] = {2, 8, 4, 7, 10};
            int len1 = sizeof(nums1) / sizeof(nums1[0]);
            int len2 = sizeof(nums2) / sizeof(nums2[0]);
            {
                LongestCommonSubSeq::Run_V1(nums1, len1, nums2, len2);
            }
            {
                LongestCommonSubSeq::Run_V2(nums1, len1, nums2, len2);
            }
            {
                LongestCommonSubSeq::Run_V3(nums1, len1, nums2, len2);
            }
            {
                LongestCommonSubSeq::Run_V4(nums1, len1, nums2, len2);
            }
            {
                LongestCommonSubSeq::Run_V5(nums1, len1, nums2, len2);
            }
            {
                LongestCommonSubSeq::Run_V6(nums1, len1, nums2, len2);
            }
            {
                LongestCommonSubSeq::Run_V7(nums1, len1, nums2, len2);
            }
            {
                LongestCommonSubSeq::Run_V8(nums1, len1, nums2, len2);
            }
        }

        // 最长公共子串,DP
        if (0) {
            //string str1 = "ABADBasdf";
            //string str2 = "EBABDasdd";
            string str1 = "beceeasdb";
            string str2 = "ecedasd";
            {
                LongestCommonSubStr::Run(str1, str2);
            }
            {
                LongestCommonSubStr::Run_V2(str1, str2);
            }
        }

        // 01背包,DP
        if (0) {
            int values[] = {6, 3, 5, 4, 6};
            int weights[] = {2, 2, 6, 5, 4};
            int length = sizeof(values) / sizeof(values[0]);
            int capacity = 10;
            {
                Knapsack_DP::Run(values, weights, length, capacity);
            }
            {
                Knapsack_DP::Run_V2(values, weights, length, capacity);
            }
            {
                Knapsack_DP::Run_V3(values, weights, length, capacity);
            }
            {
                Knapsack_DP::Run_V4(values, weights, length, capacity);
            }
        }
    }

    if (0) {
        BloomFilter<int> *bloomfilter = new BloomFilter<int>(1e8, 0.01);

        for (int i = 1; i < 10000; i++) {
            bloomfilter->Put(i);
        }

        int count = 0;
        for (int i = 10000 + 1; i < 20000; i++) {
            if (bloomfilter->Contains(i)) {
                count++;
            }
        }
        std::cout << count << "\n";

        delete bloomfilter;
    }

    if (0) {
        using my_string::MyString;
        MyString *str_ops = new MyString();
        string text = "hoawaa aoawxoawre yoawoawoawoawaau";
        string pattern = "oawxoaw";
        {
            int pos;
            pos = MyString::BruteForceV1(text, pattern);
            std::cout << pos << "\n";
        }

        {
            std::vector<int> results;
            MyString::BruteForceV2(text, pattern, results);
            for (int& pos : results) {
                std::cout << pos << ", ";
            }
            std::cout << "\n";
        }

        {
            int pos;
            pos = MyString::BruteForceV3(text, pattern);
            std::cout << pos << "\n";
        }

        {
            int pos;
            pos = MyString::BruteForceV4(text, pattern);
            std::cout << pos << "\n";
        }

        {
            int pos;
            pos = MyString::KMP(text, pattern);
            std::cout << pos << "\n";
        }
        delete str_ops;
    }

    if (0) {
        typedef typename IntervalTreeV2<int, int>::Interval Interval;
        IntervalTreeV2<int, int> *t = new IntervalTreeV2<int, int>();

        t->Add(Interval(4, 8), 4);
        t->Add(Interval(5, 8), 5);
        t->Add(Interval(15, 18), 15);
        t->Add(Interval(7, 10), 7);
        t->Add(Interval(5, 8), 5);
        t->Add(Interval(17, 19), 17);
        t->Add(Interval(21, 24), 21);
        t->Add(Interval(16, 22), 16);
        t->Add(Interval(25, 26), 25);
        t->Add(Interval(20, 21), 20);
        std::cout << t;

        std::cout << "1. testing query one overlapping intervals: \n";
        std::optional<Interval> r = t->GetOverlapInterval(Interval(20, 21));
        if (r) {
            std::cout << "Found overlapping interval: " << r.value() << "\n";
        } else {
            std::cout << "no overlapping interval.\n";
        }

        std::cout << "2. testing query one overlapping intervals: \n";
        std::optional<Interval> r1 = t->GetMininumOverlapInterval(Interval(20, 21));
        if (r1) {
            std::cout << "Found mininum overlapping interval: " << r1.value() << "\n";
        } else {
            std::cout << "no overlapping interval.\n";
        }

        std::cout << "3. testing query all overlapping intervals: \n";
        std::optional<std::vector<Interval>> r2 = t->GetAllOverlappingInterval(Interval(20, 21));
        if (r2) {
            for (Interval& interval : r2.value()) {
                std::cout << "Found mininum overlapping interval: " << interval << "\n";
            }
        } else {
            std::cout << "no overlapping intervals.\n";
        }

        std::cout << "4. testing query all overlapping intervals: \n";
        std::optional<std::vector<Interval>> r3 = t->GetAllOverlappingInterval(Interval(1, 27));
        if (r3) {
            for (Interval& interval : r3.value()) {
                std::cout << "Found mininum overlapping interval: " << interval << "\n";
            }
        } else {
            std::cout << "no overlapping intervals.\n";
        }

        std::cout << "5. testing query all overlapping intervals: \n";
        std::optional<std::vector<Interval>> r4 = t->GetAllOverlappingInterval(Interval(5, 6));
        if (r4) {
            for (Interval& interval : r4.value()) {
                std::cout << "Found mininum overlapping interval: " << interval << "\n";
            }
        } else {
            std::cout << "no overlapping intervals.\n";
        }

        t->Remove(Interval(5, 8));
        t->Remove(Interval(25, 26));
        t->Remove(Interval(15, 18));
        t->Remove(Interval(17, 19));
        t->Remove(Interval(4, 8));
        t->Remove(Interval(21, 24));
        std::cout << t;

        delete t;
    }


    if (0) {
        int arr[] = {1, 3, -2, 8, -7};
        int size = sizeof(arr) / sizeof(arr[0]);

        SegmentTree *t = new SegmentTree(arr, size);

        std::cout << t;

        int start = 3;
        int end = 4;
        int sum = t->RangeSumQuery(start, end);
        int min = t->RangeMinQuery(start, end);
        std::cout << "sum([" << start <<"," << end << "]) = " << sum << "\n"
                  << "min([" << start <<"," << end << "]) = " << min << "\n";

        t->Update(0, 0, 3);
        std::cout << t;

        delete t;
    }

    if (0) {
        // 统计区间内的最大值及其出现的次数
        int arr[] = {1, 8, 3, 10, -2, 8, 8, -7};
        int size = sizeof(arr) / sizeof(arr[0]);

        SegmentTreeV2 *t = new SegmentTreeV2(arr, size);
        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 0, size-1);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 0, 3);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 4, 4);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 4, size-1);
            std::cout << r.first << ", " << r.second << "\n";
        }

        std::cout << "---------------------------\n";
        t->Update(1, 10);
        // int arr[] = {1, 10, 3, 10, -2, 8, 8, -7};

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 0, size-1);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 0, 3);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 4, 4);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 4, size-1);
            std::cout << r.first << ", " << r.second << "\n";
        }

        std::cout << "---------------------------\n";
        t->Update(1, 3, 4);
        // int arr[] = {1, 4, 4, 4, -2, 8, 8, -7};

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 0, size-1);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 0, 3);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 4, 4);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 4, size-1);
            std::cout << r.first << ", " << r.second << "\n";
        }

        std::cout << "---------------------------\n";
        t->Update(1, 1, 5);
        // int arr[] = {1, 5, 4, 4, -2, 8, 8, -7};

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 0, size-1);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 0, 3);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 4, 4);
            std::cout << r.first << ", " << r.second << "\n";
        }

        {
            std::pair<int, int> r = t->GetMax(1, 0, size-1, 4, size-1);
            std::cout << r.first << ", " << r.second << "\n";
        }

        delete t;
    }

    if (0) {
        // 统计区间内的最大公约数，最小公倍数类似
        int arr[] = {1, 8, 3, 10, 2, 8, 4, 6};
        int size = sizeof(arr) / sizeof(arr[0]);

        SegmentTreeV3 *t = new SegmentTreeV3(arr, size);
        {
            std::optional<int> gcd = t->GetGCD(0, size-1);
            if (gcd) {
                std::cout << gcd.value() << "\n";
            } else {
                std::cout << "no gcd.\n";
            }
        }
        {
            std::optional<int> gcd = t->GetGCD(3, size-1);
            if (gcd) {
                std::cout << gcd.value() << "\n";
            } else {
                std::cout << "no gcd.\n";
            }
        }

        {
            std::optional<int> gcd = t->GetGCD(5, 6);
            if (gcd) {
                std::cout << gcd.value() << "\n";
            } else {
                std::cout << "no gcd.\n";
            }
        }

        {
            std::optional<int> gcd = t->GetGCD(2, 4);
            if (gcd) {
                std::cout << gcd.value() << "\n";
            } else {
                std::cout << "no gcd.\n";
            }
        }

        // 更新区间元素，看看GCD是否正确
        std::cout << "---------------------------\n";
        t->Update(0, 0, 4);
        t->Update(2, 2, 4);
        // int arr[] = {2, 8, 4, 10, 2, 8, 4, 6};
        {
            std::optional<int> gcd = t->GetGCD(0, size-1);
            if (gcd) {
                std::cout << gcd.value() << "\n";
            } else {
                std::cout << "no gcd.\n";
            }
        }
        {
            std::optional<int> gcd = t->GetGCD(3, size-1);
            if (gcd) {
                std::cout << gcd.value() << "\n";
            } else {
                std::cout << "no gcd.\n";
            }
        }
        {
            std::optional<int> gcd = t->GetGCD(0, 2);
            if (gcd) {
                std::cout << gcd.value() << "\n";
            } else {
                std::cout << "no gcd.\n";
            }
        }

        delete t;
    }

    if (0) {
        // 统计区间内的0的个数
        int arr[] = {1, 0, 3, 0, 2, 0, 4, 0};
        int size = sizeof(arr) / sizeof(arr[0]);

        SegmentTreeV4 *t = new SegmentTreeV4(arr, size);
        {
            int count = t->CountZeros(0, size-1);
            std::cout << count << "\n";
        }

        {
            int count = t->CountZeros(2, size-1);
            std::cout << count << "\n";
        }
        {
            int count = t->CountZeros(0, 0);
            std::cout << count << "\n";
        }
        {
            int count = t->CountZeros(1, size-2);
            std::cout << count << "\n";
        }

        // 更新区间元素，是否正确
        std::cout << "---------------------------\n";
        t->Update(0, 0, 0);
        t->Update(2, 4, 4);
        // int arr[] = {0, 0, 4, 4, 4, 0, 4, 0};
        {
            int count = t->CountZeros(0, size-1);
            std::cout << count << "\n";
        }

        {
            int count = t->CountZeros(2, size-1);
            std::cout << count << "\n";
        }
        {
            int count = t->CountZeros(0, 0);
            std::cout << count << "\n";
        }
        {
            int count = t->CountZeros(1, size-2);
            std::cout << count << "\n";
        }

        std::cout << "---------------------------\n";
        // 查找区间内的第k个0的索引
        // int arr[] = {0, 0, 4, 4, 4, 0, 4, 0};
        {
            int index = t->FindKthZero(1);
            std::cout << index << "\n";
        }
        {
            int index = t->FindKthZero(2);
            std::cout << index << "\n";
        }
        {
            int index = t->FindKthZero(3);
            std::cout << index << "\n";
        }
        {
            int index = t->FindKthZero(4);
            std::cout << index << "\n";
        }
        {
            int index = t->FindKthZero(1, 1, size-1);
            std::cout << index << "\n";
        }
        {
            int index = t->FindKthZero(1, 2, size-1);
            std::cout << index << "\n";
        }

        delete t;
    }

    if (0) {
        // 数组前缀和，找出最小的i，使得数组前i个元素之和大于等于x
        int arr[] = {1, 0, 3, 1, 2, 0, 4, 0};
        int size = sizeof(arr) / sizeof(arr[0]);

        SegmentTreeV5 *t = new SegmentTreeV5(arr, size);
        {
            int x = t->GetFirstSum(0, size-1, 2);
            std::cout << x << "\n";
        }
        {
            int x = t->GetFirstSum(0, size-1, 1);
            std::cout << x << "\n";
        }
        {
            int x = t->GetFirstSum(0, size-1, 4);
            std::cout << x << "\n";
        }
        {
            int x = t->GetFirstSum(0, size-1, 5);
            std::cout << x << "\n";
        }
        {
            int x = t->GetFirstSum(2, size-1, 1); //这里返回的是全局索引
            std::cout << x << "\n";
        }
        {
            int x = t->GetFirstSum(3, size-1, 1);
            std::cout << x << "\n";
        }

        delete t;
    }

    if (0) {
        // 搜索某区间内第一个大于x的元素索引, O(logn)，没有等于
        int arr[] = {1, 0, 3, 0, 2, 0, 4, 0};
        int size = sizeof(arr) / sizeof(arr[0]);

        SegmentTreeV6 *t = new SegmentTreeV6(arr, size);
        {
            int index = t->GetFirstGreater(0, size-1, 2);
            std::cout << index << "\n";
        }
        {
            int index = t->GetFirstGreater(0, size-1, 3);
            std::cout << index << "\n";
        }
        {
            int index = t->GetFirstGreater(0, size-1, 5);
            std::cout << index << "\n";
        }
        {
            int index = t->GetFirstGreater(1, size-1, 2);  // 起始不为0的话，返回的是全局索引
            std::cout << index << "\n";
        }
        {
            int index = t->GetFirstGreater(2, size-1, 2);
            std::cout << index << "\n";
        }
        {
            int index = t->GetFirstGreater(3, size-1, 2);
            std::cout << index << "\n";
        }

        delete t;
    }

    if (0) {
        // 搜索区间内最大连续子序列和
        int arr[] = {1, 0, 3, -1, 2, 1, -4, 5};
        //int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        int size = sizeof(arr) / sizeof(arr[0]);

        typedef SegmentTreeV7::Element Element;
        SegmentTreeV7 *t = new SegmentTreeV7(arr, size);

        // t->TestElement();

        Element e;
        {
            e = t->Query(0, size-1);
            std::cout << e._subarray_sum << "\n";
        }

        int result;
        {
            result = my_problems::MaxSubSeqSum_DP::Run(arr, size);
            std::cout << "result = " << result << "\n";
        }
        {
            result = my_problems::MaxSubSeqSum_DP::Run_V2(arr, size);
            std::cout << "result = " << result << "\n";
        }

        std::cout << "----------------------------\n";
        t->Update(1, -5); // 点更新
        // int arr[] = {1, -5, 3, -1, 2, 1, -4, 5};
        {
            e = t->Query(0, size-1);
            std::cout << e._subarray_sum << "\n";
        }
        {
            int arr[] = {1, -5, 3, -1, 2, 1, -4, 5};
            int size = sizeof(arr) / sizeof(arr[0]);
            {
                result = my_problems::MaxSubSeqSum_DP::Run(arr, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = my_problems::MaxSubSeqSum_DP::Run_V2(arr, size);
                std::cout << "result = " << result << "\n";
            }
        }

        std::cout << "----------------------------\n";
        t->Update(1, 2, -5);  // 区间更新
        // int arr[] = {1, -5, -5, -1, 2, 1, -4, 5};
        {
            e = t->Query(0, size-1);
            std::cout << e._subarray_sum << "\n";
        }
        {
            int arr[] = {1, -5, -5, -1, 2, 1, -4, 5};
            int size = sizeof(arr) / sizeof(arr[0]);
            {
                result = my_problems::MaxSubSeqSum_DP::Run(arr, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = my_problems::MaxSubSeqSum_DP::Run_V2(arr, size);
                std::cout << "result = " << result << "\n";
            }
        }


        delete t;
    }

    if (0) {
        // 搜索区间内第一个大于等于x的元素
        int arr[] = {1, 0, 3, -1, 2, 1, -4, 5};
        //int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        int size = sizeof(arr) / sizeof(arr[0]);

        SegmentTreeV8 *t = new SegmentTreeV8(arr, size);

        int r;
        {
            r = t->Query(0, size-1, 0);
            std::cout << r << "\n";
        }
        {
            r = t->Query(0, size-1, 1);
            std::cout << r << "\n";
        }
        {
            r = t->Query(0, size-1, 4);
            std::cout << r << "\n";
        }
        {
            r = t->Query(1, size-1, 0);
            std::cout << r << "\n";
        }
        {
            r = t->Query(2, size-1, 1);
            std::cout << r << "\n";
        }
        {
            r = t->Query(1, 4, 4);
            std::cout << r << "\n";
        }

        delete t;

    }

    if (0) {
        // 搜索区间内第一个大于等于x的元素
        // 使用multiset，在V8基础上加入update操作
        int arr[] = {1, 0, 3, -1, 2, 1, -4, 5};
        //int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        int size = sizeof(arr) / sizeof(arr[0]);

        SegmentTreeV9 *t = new SegmentTreeV9(arr, size);

        int r;
        {
            r = t->Query(0, size-1, 0);
            std::cout << r << "\n";
        }
        {
            r = t->Query(0, size-1, 1);
            std::cout << r << "\n";
        }
        {
            r = t->Query(0, size-1, 4);
            std::cout << r << "\n";
        }
        {
            r = t->Query(1, size-1, 0);
            std::cout << r << "\n";
        }
        {
            r = t->Query(2, size-1, 1);
            std::cout << r << "\n";
        }
        {
            r = t->Query(1, 4, 4);
            std::cout << r << "\n";
        }

        // 单点更新
        t->Update(3, 5);
        // int arr[] = {1, 0, 3, 5, 2, 1, -4, 5};
        {
            r = t->Query(0, size-1, 0);
            std::cout << r << "\n";
        }
        {
            r = t->Query(0, size-1, 1);
            std::cout << r << "\n";
        }
        {
            r = t->Query(0, size-1, 4);
            std::cout << r << "\n";
        }
        {
            r = t->Query(1, size-1, 0);
            std::cout << r << "\n";
        }
        {
            r = t->Query(2, size-1, 1);
            std::cout << r << "\n";
        }
        {
            r = t->Query(1, 4, 4);
            std::cout << r << "\n";
        }

        delete t;
    }


    if (0) {
        int arr[] = {1, 0, 3, -1, 2, 1, -4, 5};
        //int arr[] = {-2, 1, -3, 4, -1, 2, 1, -5, 4};
        int size = sizeof(arr) / sizeof(arr[0]);

        { // 求区间[l,r]和, 树状数组, 0-based indexing
            BinaryIndexedTreeSum *t = new BinaryIndexedTreeSum(arr, size);
            int result;
            {
                result = t->Sum(0, size-1);
                std::cout << "result = " << result << "\n";
            }
            {
                result = t->Sum(1, 5);
                std::cout << "result = " << result << "\n";
            }
            delete t;
        }

        { // 求[0,r]的最小值, 树状数组, 0-based indexing
            BinaryIndexedTreeMin *t = new BinaryIndexedTreeMin(arr, size);
            int result;
            {
                result = t->GetMin(size-1);
                std::cout << "result = " << result << "\n";
            }
            {
                result = t->GetMin(5);
                std::cout << "result = " << result << "\n";
            }
            delete t;
        }

        { // 求区间[l,r]和, 树状数组, 1-based indexing
            BITSumOneBasedIndexing *t = new BITSumOneBasedIndexing(arr, size);
            int result;
            {
                result = t->Sum(0, size-1);
                std::cout << "result = " << result << "\n";
            }
            {
                result = t->Sum(1, 5);
                std::cout << "result = " << result << "\n";
            }
            delete t;
        }

        { // 区间更新和区间和, 树状数组, 1-based indexing
            // int arr[] = {1, 0, 3, -1, 2, 1, -4, 5};
            BIT *t = new BIT(arr, size);

            int result;
            {
                result = t->RangeSum(1, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = t->RangeSum(2, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = t->RangeSum(2, 5);
                std::cout << "result = " << result << "\n";
            }

            // 区间更新
            {
                // before: {1, 0, 3, -1, 2, 1, -4, 5};
                t->RangeAdd(2, 5, 1);
                // after: {1, 1, 4, 0, 3, 1, -4, 5};
            }
            {
                result = t->RangeSum(1, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = t->RangeSum(2, size);
                std::cout << "result = " << result << "\n";
            }
            {
                result = t->RangeSum(2, 5);
                std::cout << "result = " << result << "\n";
            }

            delete t;
        }
    }

    // PrioritySearchTree
    if (0) {
        std::string names[] = {"A", "B", "C", "D", "E",
                               "F", "G", "H", "I", "J",
                               "K", "M", "N"};
        float arr[][2] = {{15.f, 7.f},
                          {16.f, 2.f},
                          {12.f, 1.f},
                          {14.f, -1.f},
                          {10.f, -2.f},
                          {-1.f, 9.f},
                          {6.f, 4.f},
                          {7.f, 6.f},
                          {-2.f, 5.f},
                          {2.f, 3.f},
                          {4.f, 0.f},
                          {9.f, -3.f},
                          {1.f, 8.f}};
        int size = sizeof(arr) / sizeof(arr[0]);

        if (0) {
            PrioritySearchTree<float> *t = new PrioritySearchTree<float>(names, arr, size);

            // test sort
            {
                t->Print("before");
                t->DoSort();
                t->Print("after");
            }

            delete t;
        }

        if (1) {
            PSTMultiset<float> *t = new PSTMultiset<float>(names, arr, size);

            // test sort
            {
                t->Print("before");
                //              t->DoSort();
                //              t->Print("after");
            }

            // build PST
            {
                t->Build();  // OK
            }

            // query
            {
                t->Query(0, 11, 4.5f);  // OK
            }

            delete t;
        }
    }

    // max-flow-min-cut
    if (1) {
        int n = 6;
        int s = 0, t = 5;

        MaxFlowMinCut *algo = new MaxFlowMinCut();

        {
            vector<vector<int>> capacity =
            {{0,7,0,0,4,0},
             {7,0,5,3,3,0},
             {0,5,0,3,0,8},
             {0,3,3,0,2,5},
             {4,3,0,2,0,0},
             {0,0,8,5,0,0}};
            vector<vector<int>> adj =
            {{1,4},
             {0,2,3,4},
             {1,3,5},
             {1,2,4,5},
             {0,1,3},
             {2,3}};

            int flow = algo->MaxFlow_EdmondsKarp(capacity, adj, s, t, n);
            std::cout << flow << "\n";
        }
        {
            vector<vector<int>> capacity =
            {{0,7,0,0,4,0},
             {7,0,5,3,3,0},
             {0,5,0,3,0,8},
             {0,3,3,0,2,5},
             {4,3,0,2,0,0},
             {0,0,8,5,0,0}};
            vector<vector<int>> adj =
            {{1,4},
             {0,2,3,4},
             {1,3,5},
             {1,2,4,5},
             {0,1,3},
             {2,3}};

            int flow = algo->MaxFlow_PushRelabel(capacity, adj, s, t, n);
            std::cout << flow << "\n";
        }
        {
            vector<vector<int>> capacity =
            {{0,7,0,0,4,0},
             {7,0,5,3,3,0},
             {0,5,0,3,0,8},
             {0,3,3,0,2,5},
             {4,3,0,2,0,0},
             {0,0,8,5,0,0}};
            vector<vector<int>> adj =
            {{1,4},
             {0,2,3,4},
             {1,3,5},
             {1,2,4,5},
             {0,1,3},
             {2,3}};

            int flow = algo->MaxFlow_PushRelabelFaster(capacity, adj, s, t, n);
            std::cout << flow << "\n";
        }


        {
            vector<vector<int>> capacity =
            {{0,7,0,0,4,0},
             {7,0,5,3,3,0},
             {0,5,0,3,0,8},
             {0,3,3,0,2,5},
             {4,3,0,2,0,0},
             {0,0,8,5,0,0}};
            vector<vector<int>> adj =
            {{1,4},
             {0,2,3,4},
             {1,3,5},
             {1,2,4,5},
             {0,1,3},
             {2,3}};

            MaxFlowMinCut::Dinic *dinic = new MaxFlowMinCut::Dinic(n, s, t);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (capacity[i][j] != 0) {
                        dinic->AddEdge(i, j, capacity[i][j]);
                    }
                }
            }

            MaxFlowMinCut::Long flow = dinic->MaxFlow_Dinic();
            std::cout << flow << "\n";

            delete dinic;
        }

        {
            vector<vector<int>> capacity =
            {{0,3,2,0,0,0},
             {3,0,0,2,5,0},
             {2,0,0,0,4,0},
             {0,2,0,0,1,1},
             {0,5,4,1,0,4},
             {0,0,0,1,4,0}};
            vector<vector<int>> adj =
            {{1,2},
             {0,3,4},
             {0,4},
             {1,4,5},
             {1,2,3,5},
             {3,4}};


            MaxFlowMinCut::Dinic *dinic = new MaxFlowMinCut::Dinic(n, s, t);
            for (int i = 0; i < n; i++) {
                for (int j = 0; j < n; j++) {
                    if (capacity[i][j] != 0) {
                        dinic->AddEdge(i, j, capacity[i][j]);
                    }
                }
            }

            MaxFlowMinCut::Long flow = dinic->MaxFlow_Dinic();
            std::cout << flow << "\n";

            delete dinic;
        }

        // 最小代价最大流 example1
        {
            // https://blog.csdn.net/lym940928/article/details/90209172
            vector<vector<int>> cost =
            {{0,  10 ,8   ,0  ,0   ,0},
             {-10,0  ,0   ,7  ,2   ,0},
             {-8, 0  ,0   ,0  ,10  ,0},
             {0,  -7 ,0   ,0  ,-4  ,6},
             {0,  -2 ,-10 ,4  ,0   ,7},
             {0,  0  ,0   ,-6 ,-7  ,0}};
            vector<vector<int>> capacity =
            {{0,3,2,0,0,0},
             {0,0,0,2,5,0},
             {0,0,0,0,4,0},
             {0,0,0,0,0,1},
             {0,0,0,1,0,4},
             {0,0,0,0,0,0}};
            vector<vector<int>> adj =
            {{1,2},
             {0,3,4},
             {0,4},
             {1,4,5},
             {1,2,3,5},
             {3,4}};

            MaxFlowMinCut::MinCostFlow *mincost =
                    new MaxFlowMinCut::MinCostFlow(n,
                                                      std::move(adj),
                                                      std::move(cost),
                                                      std::move(capacity));

            int K = 1e9;
            std::cout << mincost->Run(K, 0, 5) << "\n";

            delete mincost;
        }

        // 最小代价最大流 example2
        {
            // https://www.cnblogs.com/keye/p/11059148.html
            vector<vector<int>> cost =
            {{0,  4 ,1   ,0  ,0 },
             {-4, 0  ,-2   ,6  ,1 },
             {-1, 2  ,0   ,3  ,0 },
             {0,  -6 ,-3   ,0  ,2 },
             {0,  -1 ,0 ,-2  ,0  }};
            vector<vector<int>> capacity =
            {{0,10,8,0,0},
             {0,0,0,2,7},
             {0,5,0,10,0},
             {0,0,0,0,4},
             {0,0,0,0,0}};
            vector<vector<int>> adj =
            {{1,2},
             {0,2,3,4},
             {0,1,3},
             {1,2,4},
             {1,3}};

            MaxFlowMinCut::MinCostFlow *mincost =
                    new MaxFlowMinCut::MinCostFlow(n,
                                                      std::move(adj),
                                                      std::move(cost),
                                                      std::move(capacity));

            int K = 1e9;
            std::cout << mincost->Run(K, 0, 4) << "\n";

            delete mincost;
        }


        // 最小代价最大流 example3
        {
            // https://www.geeksforgeeks.org/minimum-cost-maximum-flow-from-a-graph-using-bellman-ford-algorithm/
            vector<vector<int>> cost =
            {{0, 1, 0, 0, 0},
             {-1, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0},
             {0, 0, 0, 0, 0}};
            vector<vector<int>> capacity =
            {{0, 3, 4, 5, 0},
             {0, 0, 2, 0, 0},
             {0, 0, 0, 4, 1},
             {0, 0, 0, 0, 10},
             {0, 0, 0, 0, 0}};
            vector<vector<int>> adj =
            {{1,2,3},
             {0,2},
             {0,1,3,4},
             {0,2,4},
             {2,3}};

            MaxFlowMinCut::MinCostFlow *mincost =
                    new MaxFlowMinCut::MinCostFlow(n,
                                                      std::move(adj),
                                                      std::move(cost),
                                                      std::move(capacity));

            int K = 1e9;
            std::cout << mincost->Run(K, 0, 4) << "\n";

            delete mincost;
        }



        // 最小代价最大流 example4
        {
            // https://www.geeksforgeeks.org/minimum-cost-maximum-flow-from-a-graph-using-bellman-ford-algorithm/
            vector<vector<int>> cost =
            { { 0, 1, 0, 0, 2 },
              { -1, 0, 0, 3, 0 },
              { 0, 0, 0, 0, 0 },
              { 0, -3, 0, 0, 1 },
              { -2, 0, 0, -1, 0 } };
            vector<vector<int>> capacity =
            { { 0, 3, 1, 0, 3 },
              { 0, 0, 2, 0, 0 },
              { 0, 0, 0, 1, 6 },
              { 0, 0, 0, 0, 2 },
              { 0, 0, 0, 0, 0 } };
            vector<vector<int>> adj =
            {{1,2,4},
             {0,2},
             {0,1,3,4},
             {2,4},
             {0,2,3}};

            MaxFlowMinCut::MinCostFlow *mincost =
                    new MaxFlowMinCut::MinCostFlow(n,
                                                      std::move(adj),
                                                      std::move(cost),
                                                      std::move(capacity));

            int K = 1e9;
            std::cout << mincost->Run(K, 0, 4) << "\n";

            delete mincost;
        }

        delete algo;
    }

    return 0;
}
