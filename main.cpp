#include <iostream>
#include <vector>
#include <memory>

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
#include "heap/BinaryHeap.h"
#include "heap/BinomialHeap.h"
#include "heap/FibonacciHeap.h"
#include "heap/LeftistHeap.h"
#include "heap/SkewHeap.h"
#include "heap/PairingHeap.h"
using namespace test;

#include "MySort.h"
using namespace sort_algorithms;

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

template<> int BinomialHeap<int>::Node::used_count = 0;
template<> int FibonacciHeap<int>::Node::used_count = 0;
template<> int LeftistHeap<int>::Node::used_count = 0;
template<> int SkewHeap<int>::Node::used_count = 0;
template<> int PairingHeap<int>::Node::used_count = 0;

template<> int HuffmanTree<int>::Node::used_count = 0;
typedef HuffmanTree<int>::Node HuffmanNode;
template<> int BinomialHeap<HuffmanNode*>::Node::used_count = 0;


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

  if (1) {
      std::cout << "===============================\n test BubbleSortV1\n";
      int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
      size_t length = sizeof(arr) / sizeof(arr[0]);
      BubbleSortV1(arr, length);
      PrintArray(arr, length);
  }
  if (1) {
      std::cout << "===============================\n test BubbleSortV2\n";
      int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
      size_t length = sizeof(arr) / sizeof(arr[0]);
      BubbleSortV2(arr, length);
      PrintArray(arr, length);
  }
  if (1) {
      std::cout << "===============================\n test BubbleSortV3\n";
      int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
      size_t length = sizeof(arr) / sizeof(arr[0]);
      BubbleSortV3(arr, length);
      PrintArray(arr, length);
  }
  if (1) {
      std::cout << "===============================\n test SelectionSortV1\n";
      int arr[] = {23,7,3,17,24,18,52,38,30,26,46,39,41,35,30};
      size_t length = sizeof(arr) / sizeof(arr[0]);
      SelectionSortV1(arr, length);
      PrintArray(arr, length);
  }
  if (1) {
      std::cout << "===============================\n test SelectionSort\n";
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

  return 0;
}
