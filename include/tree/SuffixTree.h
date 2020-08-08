#ifndef SUFFIXTREE_H
#define SUFFIXTREE_H

#include <iostream>
#include <map>
#include <string>
#include "MyCommon.h"

using std::map;
using std::string;


/*
 * 后缀树，线性时间。典型应用是子串问题。给定长度为m的文本T，进行O(m)时间的预处理后，
 * 给定一个长度为n的字符串S，需要在O(n)时间内对T进行搜索。
 * 后缀树T，从长度为m的字符串S建立，有m个叶子，编号为1-->m。每个内部节点，除了根节点，至少有
 * 2个孩子，每条边用一个S的非空子串标记。从一个节点出发的任意两条边，其第一个字符都不相同。
 * 对于任意叶子节点i，从根节点到叶子结点的边标记连接组成的字符串都是S的从位置i开始的一个后缀S[i..m]。
 * 为了避免出现问题，在字符串末尾添加一个终止符$(不在S中出现的)。因此在构建S的后缀树时，除非特别说明，
 * 都隐含的包含了这个终止符。
 * 一个path-label表示从根节点开始导当前节点结束，路径上的标记连接组成的字符串。
 * 位于任意节点v，其string-depth表示v的label的字符个数。就是path-label包含多少个字符吧。
 * 两个节点组成的一条边(u,v)，如果在某点分裂了，路径标记定义为从u的标记连接那些(u,v)上标记组成的字符串。
 *
 * 对于一个长度为m的字符串S，S[1..m]$进行建树，先处理整个字符串S[1..m]$，然后再处理S[j..m]$，j=2->m。
 * N_i为所有从1到i的所有后缀的中间树。
 * N_1：一个根节点，一个叶子结点，叶子结点为1，边标记为S$。
 * N_i：从N_i的根节点出发，找到一个最长路径，能够匹配S[i+1..m]$子串。不断匹配，一直到不能匹配上。
 * 匹配路径是唯一的，因为在一个节点上不会有两个相同的起始字符。
 * 匹配完后，要么在一个节点w结束，要么在一条边(u,v)的中间结束。如果是在一条边(u,v)的中间结束,
 * 会把这个边分裂，插入一个新节点w。会形成两条边，(u,w), (w,v)，(w,i+1)，i+1是一个新的叶子节点。
 * 这样就添加了一个新的后缀S[i+1..m]$了。
 *
 * 隐式后缀树。S[1..i] I_i，i=1-->m。
 * 当至少有一个后缀是另一个后缀的前缀时，隐式后缀树有更少的叶子节点。
 * 当添加了终止符后，隐式后缀树的每个后缀都是一个叶子，因此会成为一个真后缀树。
 * 对于S的每个前缀S[1..i]，构造隐式后缀树I_i，从I_1开始。
 * 首先介绍一个O(m^3)算法，然后再优化。
 * 长度为m,有m步，在第i+1步中，在I_i基础上构造I_(i+1)。
 * 每个步骤i+1又分为i+1个扩展，对于S[1..i+1]的i+1个后缀，都有一个扩展。
 * 在第j个扩展中，对于子串S[j..i]，首先找到其路径的结尾，然后添加S(i+1)到结尾(除非S(i+1)已经存在了)。
 * S[1..i+1]首先放到树里面，然后S[2..i+1],S[3..i+1],...放到树里面。
 * 最后一个扩展，对应S[1..i]的空后缀，表示将S(i+1)的单个字符放到树里面(除非它已经存在)。
 * 树I_1只有一个边，其自复位S(1)。。
 *
 * beta=S[j..i]为S[1..i]的一个后缀，在extension j中，当算法找到beta的结束时，将beta进行扩展到
 * beta*S(i+1)。有以下3个规则：
 * 规则1：当beta是一个叶子节点时，也就是说这个后缀已经存在了，那么只需要把新的字符S(i+1)添加到
 * 叶子边的末尾即可。
 * 规则2：查找beta结束后，后面的字符不是S(i+1),并且后面还有字符，那么这时需要新建一条新的边，这个边
 * 的标记为S(i+1)，而这条边连接的叶子结点的编号为j，因为它是对应后缀S[j..i+1]。
 * 还要新建一个新的内部节点，这个内部节点分为两个孩子，一个是原来的剩余字符，一个是这个新字符。
 * 规则3：在beta结束时，路径的后面还有字符，且后面第一个字符正好是S(i+1)，这种情况表示
 * beta*S(i+1)已经存在于树中，不需要做任何事。
 *
 * 关键问题在于对于S[1..i]的所有i+1个后缀，如何定位他们的结尾。一个方法是暴力查找，从根出发，依次比较。
 *
 * 因此引入后缀链接suffix links的概念
 * x*alpha是任意字符串，x是单个字符，alpha是可能为空的一个子串。对于内部节点v，其path-label为x*alpha，
 * 如果存在另一个节点s(v)，其path-label为alpha，那么从v到s(v)就叫做一个suffix link.
 * (v, s(v))
 * 作为一个特例，当alpha为空时，从x到根节点组成一条suffix link。根节点bu能作为内部节点，因此没有从它开始
 * 的suffix link。
 * 如果在phase i+1的extension j里面，添加了一个新的internal node v，其路径字符串为x*alpha，那么
 * 标记为alpha的路径已经在一个内部节点结束了，或者以alpha末尾结束的内部节点将在extension j+1被创建。
 * 因此，任意新创建的内部节点将会产生一个suffix link，从它到下一个extension的末尾。
 * 所有内部节点都会有suffix links，除了最近新添加的内部节点，它将会在下一次extension结束时收到它的
 * suffix link。
 *
 * 在phase i+1时，算法在extension j中定位S[1..i]的后缀S[j..i]，j=1..i+1。可以从根开始查找，
 * 但是使用suffix link，可以简化这个查找。
 * 对于j=1和j=2，后缀为S[1..i]和S[2..i]，是树的最长子串，很容易找到这个后缀的末尾。
 * 另S[1..i]为x*alpha，(v,1)为进入叶子1的那条边，接下来算法必须找到S[2..i]=alpha结束的那条边，
 * 关键是节点v要么为根节点，要么是一个内部节点。如果是根节点，只需要沿着根向下查找就行。
 * 如果是内部节点，v有一个suffix link到s(v)，因为s(v)的字符串是alpha的一个前缀，alpha的结尾
 * 必须是在s(v)的子树上进行结尾。因此，在搜索alpha的结尾时，算法不必遍历从根开始的所有路径，
 * 只需要从节点s(v)开始遍历就行了。
 * 令gamma为边(v,1)的字符串，为了找到alpha的末尾，从叶子1向上走到节点v，然后沿着从v的
 * suffix link走到s(v)，然后从s(v)沿着路径走，那个路径的末尾就是alpha的末尾。在alpha的末尾，
 * 树沿着extension rules进行更新。
 * 对于j>2的后缀,S[j..i]到S[j..i+1]，重复同样的过程，从S[j-1..i]的末尾开始，向上走一步最多一个节点
 * 到根节点或者一个节点v,v有一个suffix link指向它。让gamma为那条边的字符串，假设v不是根节点，
 * 从v走到s(v)，然后从s(v)开始，沿着一个标记为gamma的路径到S[j..i]的末尾，最后，根据extension
 * rules扩展后缀到S[j..i+1].
 *
 *
 */
namespace test {

template <typename V>
class SuffixTree {
public:
  class Node {
    int _start;
    int _end;
    int _link;
    map<char, int> _next;

    Node(int start, int end) : _start(start), _end(end) {
      std::cout << "Node::Node()\n";
    }

    ~Node() {
      std::cout << "Node::~Node()\n";
    }

    int GetEdgeLength(int position) {
      return std::min(_end, position + 1) - _start;
    }
  };

public:
  Node* _nodes;
  char* _text;
  int _root;
  int _position;
  int _currentNode;
  int _needSuffixLink;
  int _remainder;
  int _active_node;
  int _active_length;
  int _active_edge;

  SuffixTree(int length) {
    _nodes = new Node[2 * length + 2];//最多这么多个节点
    _text = new char[length];

    _root = _active_node = NewNode(-1, -1);
  }

  ~SuffixTree() {
    if (_nodes != nullptr) {
      delete[] _nodes; _nodes = nullptr;
    }

    if (_text != nullptr) {
      delete[] _text; _text = nullptr;
    }
  }

  int NewNode(int start, int end) {
    _nodes[++_currentNode] = new Node(start, end);
    return _currentNode;
  }

  void AddSuffixLink(int node) {
    if (_needSuffixLink > 0) {
      _nodes[_needSuffixLink]->_link = node;
    }
    _needSuffixLink = node;
  }

  char ActiveEdge() {
    return _text[_active_edge];
  }

  bool WalkDown(int next) {
    if (_active_length >= _nodes[next]->GetEdgeLength(_position)) {
      _active_edge += _nodes[next]->GetEdgeLength(_position);
      _active_length -= _nodes[next]->GetEdgeLength(_position);
      _active_node = next;
    }
  }

  void Add(char c) {

    _text[++_position] = c;
    _needSuffixLink = -1;
    _remainder++;

    while (_remainder > 0) {
      if (_active_length == 0) _active_edge = _position;
      if (_nodes[_active_node]->_next.find(ActiveEdge()) ==
          _nodes[_active_node]->_next.end()) {
        int leaf = NewNode(_position, 1e10);
        _nodes[_active_node]->_next[ActiveEdge()] = leaf;
        AddSuffixLink(_active_node);
      } else {
        int next = _nodes[_active_node]->_next[ActiveEdge()];
        if (WalkDown(next)) continue;
        if (_text[_nodes[next]->_start + _active_length] == c) {
          _active_length++;
          AddSuffixLink(_active_node);
          break;
        }

        int split = NewNode(_nodes[next]->_start, _nodes[next]->_start + _active_length);
        _nodes[_active_node]->_next[ActiveEdge()] = split;
        int leaf = NewNode(_position, 1e10);
        _nodes[split]->_next[c] = leaf;
        _nodes[next]->_start += _active_length;
        _nodes[split]->_next[_text[_nodes[next]->_start]] = next;
        AddSuffixLink(split);
      }
      _remainder--;

      if (_active_node == _root && _active_length > 0) {
        _active_length--;
        _active_edge = _position - _remainder + 1;
      } else {
        _active_node = _nodes[_active_node]->_link > 0 ? _nodes[_active_node]->_link : _root;
      }
    }

  }


};

}

#endif // SUFFIXTREE_H
