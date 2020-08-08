TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

SOURCES += \
        main.cpp

HEADERS += \
    include/MyImportantAlgorithms.h \
    include/MySort.h \
    include/MyUnionFind.h \
    include/heap/BinaryHeap.h \
    include/heap/BinomialHeap.h \
    include/heap/FibonacciHeap.h \
    include/heap/LeftistHeap.h \
    include/heap/PairingHeap.h \
    include/heap/SkewHeap.h \
    include/list/ArrayList.h \
    include/MyCommon.h \
    include/list/ArrayListObject.h \
    include/list/LinkedList.h \
    include/queue/BinomialQueue.h \
    include/queue/CircleDeque.h \
    include/queue/CircleQueue.h \
    include/queue/Deque.h \
    include/queue/Queue.h \
    include/sort/BubbleSort.h \
    include/stack/Stack.h \
    include/tree/AATree.h \
    include/tree/AVLTree.h \
    include/tree/BPlusTree.h \
    include/tree/BTree.h \
    include/tree/BinarySearchTree.h \
    include/tree/CartesianTree.h \
    include/tree/HuffmanTree.h \
    include/tree/IntervalTree.h \
    include/tree/KDTree.h \
    include/tree/RBTree.h \
    include/tree/RadixTree.h \
    include/tree/RangeTree.h \
    include/tree/SegmentTree.h \
    include/tree/SkipList.h \
    include/tree/SkipListV2.h \
    include/tree/SplayTree.h \
    include/tree/SplayTreeV2.h \
    include/tree/SuffixTree.h \
    include/tree/SuffixTreeV2.h \
    include/tree/SuffixTreeV3.h \
    include/tree/Treap.h \
    include/tree/Trie.h

INCLUDEPATH += include/

DISTFILES += \
    README.md
