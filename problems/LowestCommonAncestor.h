#ifndef LOWESTCOMMONANCESTOR_H
#define LOWESTCOMMONANCESTOR_H


namespace problems {

/* 最低公共祖先问题
 *
 * https://cp-algorithms.com/graph/lca.html
 * 树的欧拉周游（不一定是二叉树），
 * 用DFS，生成一个euler数组，存储每次访问到的节点的及其高度
 * 问题转变为RMQ问题,range-min-query
 * 1. 使用Sqrt-Decomposition，O(sqrt(N)) time with O(N) preprocessing time
 * 2. 使用线段树, O(logN) time with O(N) preprocessing time
 * 3. 使用稀疏表Sparse-Table，O(1) query with O(NlogN) build time
 */
class LowestCommonAncestor
{
public:
    LowestCommonAncestor() {

    }
    virtual ~LowestCommonAncestor() {

    }
};

}


#endif // LOWESTCOMMONANCESTOR_H
