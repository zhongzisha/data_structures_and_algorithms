#ifndef SUFFIXTREEV3_H
#define SUFFIXTREEV3_H


#include <functional>
#include <algorithm>
#include <stdexcept>
#include <iterator>
#include <iostream>
#include <vector>
#include <string>
#include <stack>
#include <queue>

#include <assert.h>
#include <stdio.h>

using std::vector;
using std::basic_string;

#ifdef DEBUG_STREE
FILE* __streeLogFile=stderr;
#define dbg(...)                              \
    do{                                       \
        fprintf(__streeLogFile, __VA_ARGS__); \
        fflush(__streeLogFile);               \
    } while(0)
#define dbg_puts(str, len)                    \
    do{                                       \
        for(int i=0;i<len;++i)                \
            fputc(str[i], __streeLogFile);    \
        fflush(__streeLogFile);               \
    } while(0)
#else
#define dbg(...)       /*nothing at all*/
#define dbg_puts(s, l) /*nothing at all*/
#endif

template <class CharT, class Additional=void*>
struct Node
{
  // Important members:
  CharT const*  str;      // since edge-node has one-to-one correspondence
  size_t           len;      // there is no need to separate them.
  size_t           startAt;  // for the use of presenting an implicit node.
      // it will then be the position of first different
      // charactor.

  Node       *  link;     // suffix link.
  Node       *  parent;
  vector<Node*> branches;

  size_t           belongTo; // which string does this leaf belongs to.
      // *only recorded on leaves*

  // Misc members:
  int           leafNum;  // leaf number ( nth suffix ).
  int           depth;    // no need to explain.
  Additional    info;     // some more information you need, depending on
      // your application.

  Node():
           str(0),
           len(0),
           startAt(0),
           link(0),
           parent(0),
           branches(),
           belongTo(static_cast<size_t>(-1)),
           leafNum(0),
           depth(0),
           info()
  { }
  Node(Node const& other):
                            str(other.str),
                            len(other.len),
                            startAt(other.startAt),
                            link(other.link),
                            parent(other.parent),
                            branches(other.branches),
                            belongTo(other.belongTo),
                            leafNum(other.leafNum),
                            depth(other.depth),
                            info(other.info)
  { }
  ~Node() { }

  /*
    static inline int matchFirst(Node const& node, CharT const val){
        return node.str[0]==val? node.len : 0;
    }
    static inline bool ptrMatchFirst(Node const* node, CharT const val){
        return node->str[0]==val? node->len : 0;
    }
     */

  struct MatchFirst {
    CharT val;
    MatchFirst(CharT v)
        : val(v) { }
    inline bool operator()(Node const* node) const {
      return node->str[0]==val;
    }
  };
};

template <class CharT=char, class T=void*>
class SuffixTree
{
protected:
  typedef Node<CharT,T>   NodeT;
  typedef typename vector<NodeT*>::iterator NodeIterator;
  NodeT*                  root_;  // 根节点
  vector<NodeT*>          leaves_;  //叶子节点
  vector<NodeT*>          allLeaves_;  //所有叶子节点
  int                     nLeaves_;  //叶子节点个数
  vector<basic_string<CharT> > strings_;  //
  NodeT*                  latestInternalNode_; //最新的内部节点
  NodeT*                  currentPos_;  //当前的位置
  int                     phase_;
  int                     extension_;

protected:
  //分配一个新节点
  NodeT* allocNode() {
    return new NodeT;
  }
  //释放一个节点
  void freeNode(NodeT *& node) {
    delete node;
    node = nullptr;
  }

  /**
     * following str[0:len], and then add str[len] to current tree.
     *
     * @return true if undone, false if str[0..len] already exists - which means
     *         extension process is done.
     */
  bool   extend  (CharT const* str, int pos);

  // follow path "str" from node down.
  NodeT* jumpDown(CharT const* str, int len, NodeT* node, bool fromFront=0);

  /**
     * split one edge from its startAt position.
     *
     * @param   edge: a node with startAt!=0
     * @param   leafLabel: label of the newly created leaf, with length==1
     * @return  newly created leaf.
     */
  NodeT* splitEdge(NodeT* edge, CharT const* leafLabel);

  void putEnd() {
    int p = phase_+1;
    //size_t const nth = strings_.size()-1;
    for(NodeIterator itr=leaves_.begin(),end=leaves_.end(); itr!=end; ++itr)
    {
      (*itr)->len = --p - (*itr)->depth;
      allLeaves_.push_back(*itr);
    }
  }

  inline void destroy(NodeT* node) {
    for(NodeIterator itr=node->branches.begin(), end=node->branches.end();
         itr!=end; ++itr)
      destroy(*itr);
    freeNode(node);
  }

private:
  //禁用拷贝构造函数
  SuffixTree(SuffixTree<CharT,T> const&); // complicated while useless,
      // i've got no interest.

public:
  inline SuffixTree();
  inline SuffixTree(basic_string<CharT> const&);

  ~SuffixTree() { destroy(root_); }

  void addString(CharT const * str, int len);
  void addString(basic_string<CharT> const& str) {
    addString(str.c_str(), str.length()); // to maintain a copy
  }

  NodeT const* root() {
    return root_;
  }
  /*
    void deleteString(CharT const * str, int len);
    void deleteString(basic_string<CharT> const& str) {
        deleteString(str.c_str(), str.length());
    }
*/

  // will call f(node) on each node of this tree, including root.
  template <class Function>
  void dfs(Function f) {
    std::stack<NodeT*> todo;
    todo.push(root_);
    while(!todo.empty()) {
      NodeT* node = todo.top();
      todo.pop();
      for(NodeIterator itr=node->branches.begin(),
                        end=node->branches.end();
           itr!=end;
           ++itr) {
        todo.push(*itr);
      }
      f(*node);
    }
  }

  // will call f(node) on each node of this tree, including root.
  template <class Function>
  void bfs(Function f) {
    std::queue<NodeT*> todo;
    todo.push(root_);
    while(!todo.empty()) {
      NodeT* node = todo.front();
      todo.pop();
      for(NodeIterator itr=node->branches.begin(),
                        end=node->branches.end();
           itr!=end;
           ++itr) {
        todo.push(*itr);
      }
      f(*node);
    }
  }

  template <class Function>
  void eachLeaf(Function f) {
    for(NodeIterator itr=allLeaves_.begin(), end=allLeaves_.end();
         itr!=end; ++itr) {
      f(**itr);
    }
  }

  // human-readable json-like format output
  void print(std::basic_ostream<CharT> * stream);

  // machine-readable dot format output, can be used to
  // generate graph using Graphviz
  void dot(std::basic_ostream<CharT> * stream);
};

template <class CharT, class T>
SuffixTree<CharT,T>::SuffixTree():
                                     root_(0),
                                     leaves_(),
                                     nLeaves_(0),
                                     strings_(),
                                     latestInternalNode_(0),
                                     currentPos_(0),
                                     phase_(0),
                                     extension_(0)
{
  root_          = allocNode();
  root_->parent  = root_; // for convenience
  root_->link    = root_;
  root_->len     = 0;
  root_->str     = 0;
  root_->startAt = 0;
  currentPos_    = root_;
}

template <class CharT, class T>
SuffixTree<CharT,T>::SuffixTree(basic_string<CharT> const& str):
                                                                   root_(0),
                                                                   leaves_(),
                                                                   nLeaves_(0),
                                                                   strings_(),
                                                                   latestInternalNode_(0),
                                                                   currentPos_(0),
                                                                   phase_(0),
                                                                   extension_(0)
{
  root_          = allocNode();
  root_->parent  = root_; // for convenience
  root_->link    = root_;
  root_->len     = 0;
  root_->str     = 0;
  root_->startAt = 0;
  currentPos_    = root_;

  this->addString(str);
}

template <class CharT, class T>
void SuffixTree<CharT,T>::addString(CharT const* str, int len)
{
  strings_.push_back(basic_string<CharT>(str, str+len));
  // int whichString = strings_.size()-1;
  CharT  const* cstr = strings_.back().c_str();
  dbg("adding string \"%s\" to suffix tree ... \n", cstr);
  root_->len = 0;
  root_->depth = 0;
  leaves_ = vector<NodeT*>();
  nLeaves_=0;
  currentPos_=root_;
  for(phase_=0; phase_<=len; ++phase_) { // there's a '\0' behind
    bool undone = true;
    latestInternalNode_ = 0;
    for(int j=nLeaves_; j <= phase_ && undone; ++j) {
      extension_ = j;
      undone = extend( cstr+extension_, phase_-extension_ );
    }
  }
  putEnd();
}

template <class CharT, class T>
typename SuffixTree<CharT,T>::NodeT*
SuffixTree<CharT,T>::
    jumpDown(CharT const* str, int len, NodeT* node, bool fromFront)
{
  dbg("#   jumping down path len=%d str=\"", len);
  dbg_puts(str,len);
  dbg("\" from node %08x ... \n", node);
  int i=0;
  if(fromFront) {
    if (node!=root_ && str[0]!=node->str[0]) {
      dbg("!!! cannot find such path !!!\n");
      throw(std::runtime_error("path not found"));
    }
    i = node->len != -1?
                        node->len:
                        phase_ - node->depth;
  }
  for(; i<len; ) {
    NodeIterator itr =
        std::find_if(   node->branches.begin(),
                     node->branches.end(),
                     typename NodeT::MatchFirst(str[i])  );
    if(itr == node->branches.end()) {
      dbg("!!! cannot find such path !!!\n");
      throw(std::runtime_error("path not found"));
    } else {
      dbg(">   going into node %08x which begins with \'%c\', len=%d,"
          " depth=%d\n",
          *itr, (*itr)->str[0], (*itr)->len, (*itr)->depth );
      node = *itr;
      if( node->len != -1 ) // internal node
        i += node->len;
      else                  // leaf
        i += phase_ - node->depth;
    }
  }
  if( i != len ) { // implicit node
    if(node->len != -1)
      node->startAt = node->len - (i-len);
    else
      node->startAt = phase_ - node->depth - (i-len);
  }
  dbg("<   done.\n");
  return node;
}

template <class CharT, class T>
typename SuffixTree<CharT,T>::NodeT*
SuffixTree<CharT,T>::splitEdge(NodeT* node, CharT const* leafLabel /*len==-1*/)
{
  /*
     *
     *        \
     *         #  <-- startAt
     *          \
     *          node
     *
     *  => => => => => => =>
     *
     *        \
     *         \
     *    newInternal
     *     /       \
     *    /       newLeaf (return value)
     *  node
     *
     */
  dbg("X   spliting edge %08x \n", node);
  NodeT* newLeaf        = allocNode();
  NodeT* newInternal    = allocNode();

  // newInternal->belongTo = node->belongTo;
  newInternal->startAt  = 0;
  newInternal->len      = node->startAt;
  newInternal->str      = node->str;
  newInternal->parent   = node->parent;
  newInternal->link     = root_;
  newInternal->depth    = node->depth;
  newInternal->branches.push_back(node);
  newInternal->branches.push_back(newLeaf);

  *(std::find(newInternal->parent->branches.begin(),
              newInternal->parent->branches.end(),
              node)) = newInternal;

  node->depth      += node->startAt;
  node->str        += node->startAt;
  node->parent      = newInternal;
  if(node->len != -1) {
    node->len    -= node->startAt;
  }
  node->startAt     = 0;

  newLeaf->str      = leafLabel;
  newLeaf->depth    = node->depth;
  newLeaf->link     = root_;
  newLeaf->len      = -1;
  newLeaf->parent   = newInternal;

  dbg("    new internal node = %08x ; \t new leaf = %08x\n",
      newInternal, newLeaf);

  return newLeaf;
}

template <class CharT, class T>
bool
SuffixTree<CharT,T>::extend(CharT const* str, int pos)
{
  dbg("{{{ in phase %d, extension %d;\n", phase_, extension_);
  dbg("    adding \'%c\' to suffix tree ... \n", str[pos]);
  int const whichString = strings_.size()-1;
  int  skiped = currentPos_->depth;
  NodeT *node = jumpDown(str+skiped, pos-skiped, currentPos_, true);
  if (node->startAt != 0) { // implicit node
    dbg("    got implicit node %08x starts at %d\n", node, node->startAt);
    if(node->str[node->startAt] != str[pos]) {
      NodeT *newLeaf = splitEdge( node, str+pos );
      newLeaf->leafNum = extension_;
      newLeaf->belongTo=whichString;
      node = newLeaf->parent;
      if(latestInternalNode_) {
        latestInternalNode_->link = node;
        dbg("->  creating suffix link from %08x to %08x\n",
            latestInternalNode_,
            node);
      }
      latestInternalNode_ = node;
      currentPos_ = node->parent;
      leaves_.push_back(newLeaf);
      ++nLeaves_;
    } else {
      dbg("    suffix already exists.\n}}} done.\n");
      node->startAt = 0;
      return false;      // suffix already exists, all done.
    }
  } else {                   // explicit node
    dbg("    got explicit node\n");
    if(latestInternalNode_) {
      latestInternalNode_->link = node;
      dbg("->  creating suffix link from %08x to %08x\n",
          latestInternalNode_,
          node);
    }
    latestInternalNode_ = node;
    if( std::find_if(   node->branches.begin(),
                     node->branches.end(),
                     typename NodeT::MatchFirst(str[pos]) )
        != node->branches.end())  {
      dbg("    suffix already exists.\n}}} done.\n");
      currentPos_ = node;
      return false;      // all done.
    } else if(node->branches.empty() && node!=root_) {
      // a leaf created in the past, now it should be exdended.
      node->belongTo=whichString;
      node->str = str+pos-node->len;
      node->len = -1;
      currentPos_ = node->parent;
      node->leafNum = extension_;
      leaves_.push_back(node);
      ++nLeaves_;
    } else {
      // add a leaf to existing explicit node.
      NodeT *newLeaf = allocNode();
      dbg("    adding new leaf %08x to node %08x.\n", newLeaf, node);

      newLeaf->parent   = node;
      newLeaf->link     = root_;
      newLeaf->str      = str+pos;
      newLeaf->len      = -1;      // leaf
      newLeaf->belongTo=whichString;
      newLeaf->depth    = node->depth + node->len;
      node->branches.push_back(newLeaf);
      currentPos_       = node;
      newLeaf->leafNum  = extension_;
      leaves_.push_back(newLeaf);
      ++nLeaves_;
    }
  }

  dbg("->  following suffix link from %08x to %08x ...\n",
      currentPos_,
      currentPos_->link );
  currentPos_ = currentPos_->link;
  dbg("}}} done.\n");
  return true;
}

static inline char* x08 (char* buf, void const * ptr) {
  sprintf(buf, "%08x", ptr);
  return buf;
}

template <class CharT, class T>
static void __printSuffixTreeNode(Node<CharT,T>* node,
                                  size_t margin_left,
                                  size_t padding_left,
                                  std::basic_ostream<CharT> * stream)
{
  size_t i,I;
  char addr[16];
  if( node->branches.empty()) {
    for(i=0; i<margin_left; ++i)
      *stream<<' ';
    *stream << '\"';
    for(i=0; i<node->len; ++i)
      *stream<< node->str[i];
    *stream << "\" " << x08(addr, node)
            << "[" << addr << "] ";
    *stream << "\n";
  } else {
    for(i=0; i<margin_left; ++i)
      *stream<<' ';
    *stream << '\"';
    for(i=0; i<node->len; ++i)
      *stream<< node->str[i];
    *stream << "\" " << x08(addr, node)
            << "[" << addr << "] ";
    *stream << ": {\n";
    for(i=0, I=node->branches.size(); i<I; ++i) {
      __printSuffixTreeNode(node->branches[i],
                            margin_left+padding_left,
                            padding_left, stream);
    }
    for(i=0; i<margin_left; ++i)
      *stream<<' ';
    *stream << "} \n";
  }
}

template <class CharT, class T>
void SuffixTree<CharT,T>::print(std::basic_ostream<CharT> * stream)
{
  //assert(!"function not supported");
}

template <>
void SuffixTree<char,void*>::print(std::ostream * stream)
//                                       ||
//                             std::basic_ostream<char>
{
  *stream<<"Root ";
  __printSuffixTreeNode<char,void*>(root_, 0,2, stream);
}

template <class T>
static void __dotPrintNode(Node<char,T>const*node,std::ostream*stream)
{
  char buf[16];
  size_t i,I,j;
  for(i=0,I=node->branches.size(); i<I; ++i) {
    Node<char,T> const* p = node->branches[i];
    *stream<<"node"<<x08(buf,p)<<" [label=\"\"";
    if(p->branches.empty())
      *stream<<" peripheries=2 width=0.1 height=0.1";
    *stream<<"];\nnode"<<x08(buf,node);
    *stream<<" -> node"<<x08(buf,p)
            <<" [label=\"";
    for(j=0;j<p->len;++j)
      if(p->str[j])
        *stream<<p->str[j];
      else
        *stream<<"$";
    *stream<<"\"];\n";
    __dotPrintNode(p,stream);
  }
  if(!node->branches.empty()) {
    *stream<<"node"<<x08(buf,node);
    *stream<<" -> node"<<x08(buf, (node->link))<<" "
            <<"[color=red];\n";
  }
}

template <class CharT, class T>
void SuffixTree<CharT,T>::dot(std::basic_ostream<CharT> *) {
  //assert(!"function not supported");
}

template <>
void SuffixTree<char,void*>::dot(std::ostream * stream)
{
  char buf[16];
  *stream<<"digraph stree {\n";
  *stream<<"node"<<x08(buf, root_)<<" [label=\"root\"];\n";
  *stream<<"node"<<buf<<" -> node"<<buf<<";\n";
  *stream<<"node [width=0.2 height=0.2] \n";
  __dotPrintNode<void*>(root_, stream);
  *stream<<"}\n";
}


#endif // SUFFIXTREEV3_H
