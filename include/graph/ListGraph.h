#ifndef LISTGRAPH_H
#define LISTGRAPH_H


#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <queue>
#include <stack>

#include "MyCommon.h"
#include "MyUnionFind.h"

namespace graph {


// 邻接表实现的图
template <typename V,  //顶点key
          typename E,  //边权值
          typename VHash = std::hash<V>,
          typename VCmp = std::equal_to<V>,
          typename EHash = std::hash<E>,
          typename ECmp = std::equal_to<E>>
class ListGraph {
public:
    class Edge;
    class Vertex {
    public:
        static int used_count;
        V _value; //顶点的值
        std::unordered_set<Edge*> _inEdges;  //进入顶点的边集合
        std::unordered_set<Edge*> _outEdges; //出去顶点的边集合
        typedef typename std::unordered_set<Edge*>::iterator InEdgeIterator;
        typedef typename std::unordered_set<Edge*>::iterator OutEdgeIterator;
        Vertex() : _value(V(0)) {
            used_count++;
            std::cout << "Vertex(), Vertex Used Count: " << used_count << "\n";
        }
        Vertex(const V& value) : _value(value) {
            used_count++;
            std::cout << "Vertex(), Vertex Used Count: " << used_count << "\n";
        }
        ~Vertex() {
            _inEdges.clear();
            _outEdges.clear();
            used_count--;
            std::cout << "~Vertex(), Vertex Used Count: " << used_count << "\n";
        }

        friend bool operator==(const Vertex& v1, const Vertex& v2) {
            return v1._value == v2._value;
        }
        friend bool operator!=(const Vertex& v1, const Vertex& v2) {
            return v1._value != v2._value;
        }
        bool operator==(const Vertex& other) {
            return _value == other._value;
        }
        bool operator!=(const Vertex& other) {
            return _value != other._value;
        }
        bool operator==(const Vertex* other) {
            return _value == other->_value;
        }
        bool operator!=(const Vertex* other) {
            return _value != other->_value;
        }

        friend std::ostream& operator<<(std::ostream& out, Vertex& vertex) {
            out << vertex._value << "\nIN_EDGES: ";
            return out;
        }
        friend std::ostream& operator<<(std::ostream& out, Vertex* vertex) {
            out << vertex->_value << "\nIN: ";
            if (vertex->_inEdges.size() > 0) {
                for (auto edge : vertex->_inEdges) {
                    out << "[" << edge->_from->_value << " -> " << edge->_to->_value << "], ";
                }
            } else {
                out << "[], ";
            }

            out << "OUT: ";
            if (vertex->_outEdges.size() > 0) {
                for (auto edge : vertex->_outEdges) {
                    out << "[" << edge->_from->_value << " -> " << edge->_to->_value << "], ";
                }
            } else {
                out << "[], ";
            }
            return out;
        }

        Edge* RemoveInEdge(Edge* edge) {  //删除顶点的一条入边
            Edge* oldEdge = nullptr;
            InEdgeIterator iter = _inEdges.find(edge);
            if (iter != _inEdges.end()) {
                oldEdge = *iter;
                _inEdges.erase(iter);
            }
            return oldEdge;
        }
        Edge *RemoveOutEdge(Edge* edge) { //删除顶点的一条出边
            Edge* oldEdge = nullptr;
            OutEdgeIterator iter = _outEdges.find(edge);
            if (iter != _outEdges.end()) {
                oldEdge = *iter;
                _outEdges.erase(iter);
            }
            return oldEdge;
        }
    };

    class VertexHash {
    public:
        size_t operator()(const Vertex& rhs) const {
            return std::hash<V>()(rhs._value);
        }
        size_t operator()(const Vertex* rhs) const {
            return std::hash<V>()(rhs->_value);
        }
        size_t operator()(Vertex& rhs) {
            return std::hash<V>()(rhs._value);
        }
        size_t operator()(Vertex* rhs) {
            return std::hash<V>()(rhs->_value);
        }
    };
    class VertexCmp {
    public:
        bool operator()(const Vertex& lhs, const Vertex& rhs) const {
            return lhs._value == rhs._value;
        }
        bool operator()(const Vertex* lhs, const Vertex* rhs) const {
            return lhs->_value == rhs->_value;
        }
        bool operator()(Vertex& lhs, Vertex& rhs) {
            return lhs._value == rhs._value;
        }
        bool operator()(Vertex* lhs, Vertex* rhs) {
            return lhs->_value == rhs->_value;
        }
    };

    class Edge {
    public:
        static int used_count;
        Vertex* _from;
        Vertex* _to;
        E _weight;

        Edge(Vertex* from,
             Vertex* to)
            : _from(from), _to(to), _weight(E(0)) {
            used_count++;
            std::cout << "Edge(), Edge Used Count: " << used_count << "\n";
        }
        Edge(Vertex* from,
             Vertex* to,
             const E& weight)
            : _from(from), _to(to), _weight(weight){
            used_count++;
            std::cout << "Edge(E weight), Edge Used Count: " << used_count << "\n";
        }
        virtual ~Edge() {
            used_count--;
            std::cout << "~Edge(), Edge Used Count: " << used_count << "\n";
        }

        bool operator==(const Edge& other) {
            return _from == other._from && _to == other._to;
        }
        bool operator!=(const Edge& other) {
            return _from != other._from || _to != other._to;
        }
        friend std::ostream& operator<<(std::ostream& out, Edge& edge) {
            out << " Edge [" << edge._from << " --> " << edge._to << "], "
                << "weight = " << edge._weight;
            return out;
        }

        bool operator==(const Edge* other) { // 判断两条边是否相等？不考虑权值？
            return _from == other->_from && _to == other->_to;
        }
        bool operator!=(const Edge* other) {
            return _from != other->_from || _to != other->_to;
        }
        friend std::ostream& operator<<(std::ostream& out, Edge* edge) {
            out << "Edge [" << edge->_from << " --> " << edge->_to << "], "
                << "weight = " << edge->_weight;
            return out;
        }
    };

    struct EdgeHash {
    public:
        size_t operator()(const Edge& rhs) const {
            return VertexHash()(rhs._from) * 31 + VertexHash()(rhs._to);
        }
        size_t operator()(const Edge* rhs) const {
            return VertexHash()(rhs->_from) * 31 + VertexHash()(rhs->_to);
        }
        size_t operator()(Edge& rhs) {
            return VertexHash()(rhs._from) * 31 + VertexHash()(rhs._to);
        }
        size_t operator()(Edge* rhs) {
            return VertexHash()(rhs->_from) * 31 + VertexHash()(rhs->_to);
        }
    };
    class EdgeHash2 {
    public:
        size_t operator()(const Edge& rhs) const {
            return VertexHash()(rhs._from) + VertexHash()(rhs._to);
        }
        size_t operator()(const Edge* rhs) const {
            return VertexHash()(rhs->_from) + VertexHash()(rhs->_to);
        }
        size_t operator()(Edge& rhs) {
            return VertexHash()(rhs._from) + VertexHash()(rhs._to);
        }
        size_t operator()(Edge* rhs) {
            return VertexHash()(rhs->_from) + VertexHash()(rhs->_to);
        }
    };
    class EdgeHash3 {
    public:
        size_t operator()(const Edge& rhs) const {
            return VertexHash()(rhs._from) * 31 + VertexHash()(rhs._to) +
                    std::hash<E>()(rhs._weight);
        }
        size_t operator()(const Edge* rhs) const {
            return VertexHash()(rhs->_from) * 31 + VertexHash()(rhs->_to) +
                    std::hash<E>()(rhs->_weight);
        }
        size_t operator()(Edge& rhs) {
            return VertexHash()(rhs._from) * 31 + VertexHash()(rhs._to) +
                    std::hash<E>()(rhs._weight);
        }
        size_t operator()(Edge* rhs) {
            return VertexHash()(rhs->_from) * 31 + VertexHash()(rhs->_to) +
                    std::hash<E>()(rhs->_weight);
        }
    };
    class EdgeHash4 {
    public:
        size_t operator()(const Edge& rhs) const {
            return std::hash<V>()(rhs._from->_value) + std::hash<V>()(rhs._to->_value);
        }
        size_t operator()(const Edge* rhs) const {
            return std::hash<V>()(rhs->_from->_value) + std::hash<V>()(rhs->_to->_value);
        }
        size_t operator()(Edge& rhs) {
            return std::hash<V>()(rhs._from->_value) + std::hash<V>()(rhs._to->_value);
        }
        size_t operator()(Edge* rhs) {
            return std::hash<V>()(rhs->_from->_value) + std::hash<V>()(rhs->_to->_value);
        }
    };
    class EdgeCmp {
    public:
        bool operator()(const Edge& lhs, const Edge& rhs) const {
            return VertexCmp()(lhs._from, rhs._from) && VertexCmp()(lhs._to, rhs._to);
        }
        bool operator()(const Edge* lhs, const Edge* rhs) const {
            return VertexCmp()(lhs->_from, rhs->_from) && VertexCmp()(lhs->_to, rhs->_to);
        }
        bool operator()(Edge& lhs, Edge& rhs) {
            return VertexCmp()(lhs._from, rhs._from) && VertexCmp()(lhs._to, rhs._to);
        }
        bool operator()(Edge* lhs, Edge* rhs) {
            return VertexCmp()(lhs->_from, rhs->_from) && VertexCmp()(lhs->_to, rhs->_to);
        }
    };
    class EdgeCmp2 {
    public:
        bool operator()(const Edge& lhs, const Edge& rhs) const {
            return VertexCmp()(lhs._from, rhs._from) &&
                    VertexCmp()(lhs._to, rhs._to) &&
                    lhs._weight == rhs._weight;
        }
        bool operator()(const Edge* lhs, const Edge* rhs) const {
            return VertexCmp()(lhs->_from, rhs->_from) &&
                    VertexCmp()(lhs->_to, rhs->_to) &&
                    lhs->_weight == rhs->_weight;
        }
        bool operator()(Edge& lhs, Edge& rhs) {
            return VertexCmp()(lhs._from, rhs._from) &&
                    VertexCmp()(lhs._to, rhs._to) &&
                    lhs._weight == rhs._weight;
        }
        bool operator()(Edge* lhs, Edge* rhs) {
            return VertexCmp()(lhs->_from, rhs->_from) &&
                    VertexCmp()(lhs->_to, rhs->_to) &&
                    lhs->_weight == rhs->_weight;
        }
    };
    class EdgeCmp3 {
    public:
        bool operator()(const Edge& lhs, const Edge& rhs) const {
            return (VertexCmp()(lhs._from, rhs._from) && VertexCmp()(lhs._to, rhs._to)) ||
                    (VertexCmp()(lhs._from, rhs._to) && VertexCmp()(lhs._to, rhs._from));
        }
        bool operator()(const Edge* lhs, const Edge* rhs) const {
            return (VertexCmp()(lhs->_from, rhs->_from) && VertexCmp()(lhs->_to, rhs->_to)) ||
                    (VertexCmp()(lhs->_from, rhs->_to) && VertexCmp()(lhs->_to, rhs->_from));
        }
        bool operator()(Edge& lhs, Edge& rhs) {
            return (VertexCmp()(lhs._from, rhs._from) && VertexCmp()(lhs._to, rhs._to)) ||
                    (VertexCmp()(lhs._from, rhs._to) && VertexCmp()(lhs._to, rhs._from));
        }
        bool operator()(Edge* lhs, Edge* rhs) {
            return (VertexCmp()(lhs->_from, rhs->_from) && VertexCmp()(lhs->_to, rhs->_to)) ||
                    (VertexCmp()(lhs->_from, rhs->_to) && VertexCmp()(lhs->_to, rhs->_from));
        }
    };

    class EdgeWeightGreaterCmp { //根据权重比较边
    public:
        bool operator()(const Edge& lhs, const Edge& rhs) const {
            return lhs._weight > rhs._weight;
        }
        bool operator()(const Edge* lhs, const Edge* rhs) const {
            return lhs->_weight > rhs->_weight;
        }
        bool operator()(Edge& lhs, Edge& rhs) {
            return lhs._weight > rhs._weight;
        }
        bool operator()(Edge* lhs, Edge* rhs) {
            return lhs->_weight > rhs->_weight;
        }
    };

public:
    std::unordered_map<V, Vertex*, VHash, VCmp> _vertices;
    std::unordered_set<Edge*, EdgeHash, EdgeCmp> _edges;
    typedef typename std::unordered_map<V, Vertex*>::iterator VertexIterator;
    typedef typename std::unordered_set<Edge*, EdgeHash, EdgeCmp>::iterator EdgeIterator;

    //哈希的宽松性, EdgeHash4最为宽松
    //比较器EdgeCmp2比EdgeCmp更严格,必须在顶点相同的情况下权值也要相同才认为是一条边
    //比较器EdgeCmp3比EdgeCmp更宽松,不考虑方向,不考虑权值

    ListGraph() {

    }

    virtual ~ListGraph() {
        for(auto edge : _edges) {
            delete edge;
        }
        _edges.clear();

        for(auto& vertex : _vertices) {
            delete vertex.second;
        }
        _vertices.clear();
    }

    //顶点个数
    int NumVertices() {
        return _vertices.size();
    }

    //边个数
    int NumEdges() {
        return _edges.size();
    }

    //添加一个顶点
    void AddVertex(const V& v) {
        if (_vertices.find(v) == _vertices.end()) { // _vertices.count(v) == 0
            Vertex *newVertex = new Vertex(v);
            _vertices.insert(std::pair<V, Vertex*>(v, newVertex));
        }
    }

    //添加一条边
    void AddEdge(const V& from, const V& to, const E& weight) {
        Vertex *fromVertex = nullptr;
        if (_vertices.find(from) == _vertices.end()) {
            //顶点不存在
            fromVertex = new Vertex(from);
            _vertices.insert(std::pair<V, Vertex*>(from, fromVertex));
        } else {
            fromVertex = _vertices[from];
        }

        Vertex *toVertex = nullptr;
        if (_vertices.find(to) == _vertices.end()) {
            toVertex = new Vertex(to);
            _vertices.insert(std::pair<V, Vertex*>(to, toVertex));
        } else {
            toVertex = _vertices[to];
        }

        Edge *edge = new Edge(fromVertex, toVertex, weight);
        EdgeIterator iter = _edges.find(edge);
        if (iter == _edges.end()) {//如果不存在这条边
            fromVertex->_outEdges.insert(edge);
            toVertex->_inEdges.insert(edge);
            _edges.insert(edge);
        } else { //这条边已经存在了,只修改权值
            (*iter)->_weight = weight;
            delete edge;
        }
    }
    void AddEdge(const V& from, const V& to) {
        AddEdge(from, to, E(0));
    }


    //删除一个顶点
    void RemoveVertex(const V& v) {
        VertexIterator iter = _vertices.find(v);
        if (iter == _vertices.end()) {  //如果该顶点不存在,返回
            return;
        }

        Vertex *vertex = (*iter).second;

        // 与该顶点有关的边都要删除
        // 删除该节点为起始点的所有边
        while (vertex->_outEdges.size() > 0) {
            typename Vertex::OutEdgeIterator iter = vertex->_outEdges.begin();
            Edge* edge = *iter;    //得到该顶点的出边
            Vertex* toVertex = edge->_to; //得到出边的终点
            Edge* edge_to_be_del = toVertex->RemoveInEdge(edge); //出边的终点的入边集合里删除当前这条边
            vertex->_outEdges.erase(edge); //删除该出边

            {
                EdgeIterator iter = _edges.find(edge_to_be_del);
                if (iter != _edges.end()) {
                    _edges.erase(iter);
                    delete edge_to_be_del;
                }
            }
        }

        // 删除该节点为终止点的所有边
        while (vertex->_inEdges.size() > 0) {
            typename Vertex::InEdgeIterator iter = vertex->_inEdges.begin();
            Edge* edge = *iter;  //得到该顶点的入边
            Vertex* fromVertex = edge->_from; //得到入边的起点
            Edge* edge_to_be_del = fromVertex->RemoveOutEdge(edge); //入边的起点的出边集合里删除当前这条边
            vertex->_inEdges.erase(edge); //删除该入边

            {
                EdgeIterator iter = _edges.find(edge_to_be_del);
                if (iter != _edges.end()) {
                    _edges.erase(iter);
                    delete edge_to_be_del;
                }
            }
        }

        // 删除该顶点
        delete vertex;
        _vertices.erase(iter);
    }

    //删除一条边
    void RemoveEdge(const V& from, const V& to) {
        if (_vertices.find(from) == _vertices.end()) return;
        if (_vertices.find(to) == _vertices.end()) return;

        Vertex* fromVertex = _vertices[from];
        Vertex* toVertex = _vertices[to];
        Edge* tmpEdge = new Edge(fromVertex, toVertex);
        EdgeIterator iter = _edges.find(tmpEdge);
        if (iter != _edges.end()) {
            fromVertex->RemoveOutEdge(tmpEdge);
            toVertex->RemoveInEdge(tmpEdge);
            delete *iter;
            _edges.erase(iter);
        }
        delete tmpEdge;
    }

    //宽度优先遍历，类似于二叉树的层序遍历
    void BFS(const V& begin) {
        std::cout << "BFS: " << begin << "\n";

        VertexIterator iter = _vertices.find(begin);
        if (iter == _vertices.end()) return;

        Vertex* beginVertex = (*iter).second;
        std::unordered_set<Vertex*> visitedVertices;
        std::queue<Vertex*> q;
        q.push(beginVertex);
        visitedVertices.insert(beginVertex);

        while (!q.empty()) {
            Vertex* vertex = q.front();
            std::cout << vertex << "\n";

            if (vertex->_outEdges.size() > 0) {
                for (Edge* edge : vertex->_outEdges) {
                    if (visitedVertices.count(edge->_to) == 0) {
                        q.push(edge->_to);
                        visitedVertices.insert(edge->_to);
                    }
                }
            }
//            if (vertex->_inEdges.size() > 0) {
//                for (Edge* edge : vertex->_inEdges) {
//                    if (visitedVertices.count(edge->_from) == 0) {
//                        q.push(edge->_from);
//                        visitedVertices.insert(edge->_from);
//                    }
//                }
//            }

            q.pop();
        }
    }

    //深度优先遍历
    void DFS(const V& begin) {
        std::cout << "DFS: " << begin << "\n";

        VertexIterator iter = _vertices.find(begin);
        if (iter == _vertices.end()) return;

        Vertex* vertex = (*iter).second;
        std::unordered_set<Vertex*> visitedVertices;
        DFS(vertex, visitedVertices);
    }

    void DFS(Vertex* vertex, std::unordered_set<Vertex*> &visitedVertices) {
        std::cout << vertex << "\n";
        visitedVertices.insert(vertex);

        for (auto edge : vertex->_outEdges) {
            if (visitedVertices.count(edge->_to) > 0) continue;
            DFS(edge->_to, visitedVertices);
        }
    }

    void DFS_Iteratively(const V& begin) {
        std::cout << "DFS_Iteratively: " << begin << "\n";
        VertexIterator iter = _vertices.find(begin);
        if (iter == _vertices.end()) return;

        Vertex* beginVertex = (*iter).second;
        std::unordered_set<Vertex*> visitedVertices;
        std::stack<Vertex*> s;
        s.push(beginVertex);
        visitedVertices.insert(beginVertex);
        std::cout << beginVertex << "\n";

        while (!s.empty()) {
            Vertex* vertex = s.top();
            s.pop();
            for (auto edge : vertex->_outEdges) {
                if (visitedVertices.count(edge->_to) > 0) continue;

                s.push(edge->_from);
                s.push((edge->_to));
                std::cout << edge->_to << "\n";
                visitedVertices.insert(edge->_to);

                break;
            }
        }

    }

    //拓扑排序
    void TopologicalSort() {

        std::cout << "TopologicalSort: \n";

        //搞一个队列存储当前图中入度为0的顶点
        //搞一个map存储每个入度不为0的顶点，值为每个顶点的入度
        //不断从队列中取出顶点，将其放入结果列表
        //将该顶点的出边对应的终点，其入度减1
        //(1)如果终点入度为0，将其放入队列
        //(2)如果终点入度不为零，更新map对应节点的入度减一
        std::queue<Vertex*> q;   //存储入度为0的顶点
        std::unordered_map<Vertex*, int, VertexHash, VertexCmp> m;  //入度不为0的顶点集合
        std::vector<Vertex*> results;
        for (auto item : _vertices) {
            Vertex* vertex = item.second;
            int in = vertex->_inEdges.size();
            if (in == 0) {
                q.push(vertex);
            } else {
                m.insert(std::pair<Vertex*, int>(vertex, in));
            }
        }

        while (!q.empty()) {
            Vertex* vertex = q.front();
            q.pop();

            results.push_back(vertex);

            for (auto edge : vertex->_outEdges) {
                Vertex* toVertex = edge->_to;
                int inTo = m[toVertex] - 1;
                if (inTo == 0) {
                    q.push(toVertex);
                } else {
                    m[toVertex] = inTo;
                }
            }
        }

        for(auto vertex : results) {
            std::cout << vertex << "\n";
        }
    }

    void MST_Prim() {
        //无向连通图
        std::cout << "Prim: \n";
        if (NumEdges() == 0) return;

        //搞一个顶点集合
        std::unordered_set<Vertex*, VertexHash, VertexCmp> addedVertices;
        //搞一个边集合存储最小生成树的边
        std::unordered_set<Edge*, EdgeHash, EdgeCmp> mstEdges;

        VertexIterator iter = _vertices.begin();
        addedVertices.insert((*iter).second);

        while (true) {
            //获得从A到剩余顶点连接的所有边，找出权值最小的那条边
            Edge* minWeightEdge = nullptr;
            E minWeight = INT_MAX;
            for (auto vertex : addedVertices) {
                for (auto edge : vertex->_outEdges) {
                    Vertex* to = edge->_to;
                    if (addedVertices.count(to) > 0) continue;

                    if (edge->_weight < minWeight) {
                        minWeight = edge->_weight;
                        minWeightEdge = edge;
                    }
                }
            }

            if (minWeightEdge != nullptr) {
                addedVertices.insert(minWeightEdge->_to);
                mstEdges.insert(minWeightEdge);
            }

            if (addedVertices.size() == _vertices.size())
                break;
        }


        for (auto edge : mstEdges) {
            std::cout << edge->_from->_value << " --> " << edge->_to->_value
                      << ", weight = " << edge->_weight << "\n";
        }

    }


    void MST_Prim_UsingMinHeap() {  //求最值使用小顶堆实现
        //无向连通图
        std::cout << "MST_Prim_UsingMinHeap: \n";
        if (NumEdges() == 0) return;

        //搞一个顶点集合
        std::unordered_set<Vertex*, VertexHash, VertexCmp> addedVertices;
        //搞一个边集合存储最小生成树的边
        std::unordered_set<Edge*, EdgeHash, EdgeCmp> mstEdges;

        VertexIterator iter = _vertices.begin();
        addedVertices.insert((*iter).second);

        while (true) {
            //获得从A到剩余顶点连接的所有边，找出权值最小的那条边
            std::priority_queue<Edge*, std::vector<Edge*>, EdgeWeightGreaterCmp> q;  //小顶堆
            for (auto vertex : addedVertices) {
                for (auto edge : vertex->_outEdges) {
                    Vertex* to = edge->_to;
                    if (addedVertices.count(to) > 0) continue;

                    q.push(edge);
                }
            }

            if (!q.empty()) {
                Edge* minWeightEdge = q.top();
                addedVertices.insert(minWeightEdge->_to);
                mstEdges.insert(minWeightEdge);
            }

            if (addedVertices.size() == _vertices.size())
                break;
        }

        for (auto edge : mstEdges) {
            std::cout << edge->_from->_value << " --> " << edge->_to->_value
                      << ", weight = " << edge->_weight << "\n";
        }

    }


    void MST_Prim_UsingMinHeap2() {  //求最值使用小顶堆实现
        //无向连通图
        std::cout << "MST_Prim_UsingMinHeap2: \n";
        if (NumEdges() == 0) return;

        //搞一个顶点集合
        std::unordered_set<Vertex*, VertexHash, VertexCmp> addedVertices;
        //搞一个边集合存储最小生成树的边
        std::unordered_set<Edge*, EdgeHash, EdgeCmp> mstEdges;

        std::priority_queue<Edge*, std::vector<Edge*>, EdgeWeightGreaterCmp> q;  //小顶堆
        //选择一个顶点，先把出边加入到优先级队列中
        VertexIterator iter = _vertices.begin();
        Vertex* firstVertex = (*iter).second;
        addedVertices.insert(firstVertex);
        for (auto edge : firstVertex->_outEdges) {
            q.push(edge);
        }

        size_t mstEdgeSize = _vertices.size() - 1;
        while (!q.empty() && mstEdges.size() < mstEdgeSize) {
            Edge* minWeightEdge = q.top(); //优先级队列中取出最小权值的那条边，删除
            q.pop();

            Vertex* toVertex = minWeightEdge->_to;

            if (addedVertices.count(toVertex) > 0) continue;

            addedVertices.insert(toVertex);
            mstEdges.insert(minWeightEdge);

            for (auto edge : toVertex->_outEdges) {
                q.push(edge);
            }
        }


        for (auto edge : mstEdges) {
            std::cout << edge->_from->_value << " --> " << edge->_to->_value
                      << ", weight = " << edge->_weight << "\n";
        }

    }

    void MST_Kruskal() {
        std::cout << "MST_Kruskal: \n";
        //使用并查集
        size_t mstEdgeSize = _vertices.size() - 1;
        if (mstEdgeSize <= 0) return;

        //所有边加入小顶堆，权值最小的边在堆顶
        std::priority_queue<Edge*, std::vector<Edge*>, EdgeWeightGreaterCmp> q;
        for (auto edge : _edges) {
            q.push(edge);
        }

        //所有顶点添加到并查集中
        test::GenericUnionFind<Vertex*> vertex_uf;
        for (auto item : _vertices) {
            vertex_uf.MakeSet(item.second);
        }

        //保存最小生成树的所有边
        std::unordered_set<Edge*, EdgeHash, EdgeCmp> mstEdges;

        // 取出最小的边，看这条边的两个顶点是否属于同一个集合
        // 如果不是同一个集合，加入当前这条最小边
        // 将当前最小边的两个顶点集合合并
        // 循环直至终止条件
        while (!q.empty() && mstEdges.size() < mstEdgeSize) {
            Edge* edge = q.top();
            q.pop();

            if (vertex_uf.IsSame(edge->_from, edge->_to)) continue;

            mstEdges.insert(edge);

            vertex_uf.Union(edge->_from, edge->_to);
        }

        for (auto edge : mstEdges) {
            std::cout << edge->_from->_value << " --> " << edge->_to->_value
                      << ", weight = " << edge->_weight << "\n";
        }
    }

    //负权环不存在最短路径，但是只有负权边是可能存在最短路径的

    void ShortestPath_Dijkstra(const V& v) {
        std::cout << "ShortestPath_Dijkstra for " << v << " is: \n";
        //单源最短路径
        //不能有负权边
        //O(ElogV)
        //石头，绳子，离开桌面
        //松弛，尝试找出更短路径

        VertexIterator iter = _vertices.find(v);
        if (iter == _vertices.end()) return;

        Vertex* beginVertex = (*iter).second;

        std::unordered_map<Vertex*, E, VertexHash, VertexCmp> distMap; //存储到该顶点的权重和

        std::unordered_map<Vertex*, E, VertexHash, VertexCmp> selectedPath; //存储已经知道最短路径的那些顶点，及其最短路径权重和

        for (auto item : _vertices) {
            Vertex* vertex = item.second;
            distMap[vertex] = INT_MAX;
        }
        for (auto edge : beginVertex->_outEdges) {
            distMap[edge->_to] = edge->_weight;
        }
        distMap.erase(beginVertex);
        selectedPath[beginVertex] = INT_MIN;

        while (distMap.size() > 0) {
            //选择一条权重和最小的顶点
            Vertex* minVertex = nullptr;
            E minWeight = INT_MAX;
            for (auto item : distMap) {
                if (item.second < minWeight) {
                    minVertex = item.first;
                    minWeight = item.second;
                }
            }

            if (minVertex == nullptr) {
                //不存在最短路径
                break;
            }

            //找到了那个顶点之后，从distMap中删除该顶点，对其出边的顶点进行松弛
            distMap.erase(minVertex);
            selectedPath[minVertex] = minWeight;
            for (auto edge : minVertex->_outEdges) {
                Vertex* toVertex = edge->_to;
                if (selectedPath.count(toVertex) > 0) continue;
                E newWeight = minWeight + edge->_weight;
                if (newWeight < distMap[toVertex]) {
                    distMap[toVertex] = newWeight;
                }
            }

        }

        for (auto item : selectedPath) {
            std::cout << item.first->_value << ": " << item.second << "\n";
        }
        std::cout << "NO SHORTEST PATH: ";
        for (auto item : distMap) {
            std::cout << item.first->_value << ", ";
        }
        std::cout << "\n";

    }

    void ShortestPath_Dijkstra_WithPath(const V& v) {
        std::cout << "ShortestPath_Dijkstra_WithPath for " << v << " is: \n";
        //单源最短路径
        //不能有负权边
        //O(ElogV)
        //石头，绳子，离开桌面
        //松弛，尝试找出更短路径

        VertexIterator iter = _vertices.find(v);
        if (iter == _vertices.end()) return;

        Vertex* beginVertex = (*iter).second;

        //存储到该顶点的权重和,及当前最短路径上的前一个顶点
        std::unordered_map<Vertex*, std::pair<E, Vertex*>, VertexHash, VertexCmp> distMap;

        //存储已经知道最短路径的那些顶点，及其最短路径权重和，及到其最短路径的前一个顶点
        std::unordered_map<Vertex*, std::pair<E, Vertex*>, VertexHash, VertexCmp> selectedPath;

        //预先进行了初始化，但是有个bug是E可能不存在INT_MAX的转换
        for (auto item : _vertices) {
            Vertex* vertex = item.second;
            distMap[vertex] = std::make_pair(INT_MAX, nullptr);
        }
        for (auto edge : beginVertex->_outEdges) {
            distMap[edge->_to] = std::make_pair(edge->_weight, beginVertex);
        }
        distMap.erase(beginVertex);
        selectedPath[beginVertex] = std::make_pair(INT_MIN, nullptr);

        while (distMap.size() > 0) {
            //选择一条权重和最小的顶点
            Vertex* minVertex = nullptr;
            E minWeight = INT_MAX;
            Vertex* preVertex = nullptr;
            for (auto item : distMap) {
                if (item.second.first < minWeight) {
                    minVertex = item.first;
                    minWeight = item.second.first;
                    preVertex = item.second.second;
                }
            }

            if (minVertex == nullptr) {  // 如果用这种写法，这里要特别注意
                //不存在最短路径
                break;
            }

            //找到了那个顶点之后，从distMap中删除该顶点，对其出边的顶点进行松弛
            distMap.erase(minVertex);
            selectedPath[minVertex] = std::make_pair(minWeight, preVertex);
            for (auto edge : minVertex->_outEdges) {
                Vertex* toVertex = edge->_to;
                if (selectedPath.count(toVertex) > 0) continue;
                E newWeight = minWeight + edge->_weight;  // E必须实现operator+,<,>等操作符
                if (newWeight < distMap[toVertex].first) {
                    distMap[toVertex].first = newWeight;
                    distMap[toVertex].second = minVertex;
                }
            }

        }

        for (auto item : selectedPath) {
            std::cout << item.first->_value << ": " << item.second.first << ", Path is: ";
            std::cout << item.first->_value;
            Vertex* vertex = item.second.second;
            while (vertex != nullptr) {
                std::cout << " <-- " << vertex->_value;
                vertex = selectedPath[vertex].second;
            }
            std::cout << "\n";
        }
        std::cout << "NO SHORTEST PATH: ";
        for (auto item : distMap) {
            std::cout << item.first->_value << ", ";
        }
        std::cout << "\n";

    }


    void ShortestPath_Dijkstra_WithPath2(const V& v) {
        std::cout << "ShortestPath_Dijkstra_WithPath2 for " << v << " is: \n";
        //单源最短路径
        //不能有负权边
        //O(ElogV)
        //石头，绳子，离开桌面
        //松弛，尝试找出更短路径

        VertexIterator iter = _vertices.find(v);
        if (iter == _vertices.end()) return;

        Vertex* beginVertex = (*iter).second;

        //存储到该顶点的权重和,及当前最短路径上的前一个顶点
        std::unordered_map<Vertex*, std::pair<E, Vertex*>, VertexHash, VertexCmp> distMap;

        //存储已经知道最短路径的那些顶点，及其最短路径权重和，及到其最短路径的前一个顶点
        std::unordered_map<Vertex*, std::pair<E, Vertex*>, VertexHash, VertexCmp> selectedPath;

        for (auto edge : beginVertex->_outEdges) {
            distMap[edge->_to] = std::make_pair(edge->_weight, beginVertex);
        }
        selectedPath[beginVertex] = std::make_pair(INT_MIN, nullptr);//把起点加入集合

        while (distMap.size() > 0) {
            //选择一条权重和最小的顶点
            Vertex* minVertex = nullptr;
            E minWeight = INT_MAX;
            Vertex* preVertex = nullptr;
            for (auto item : distMap) {
                if (item.second.first < minWeight) { //E必须实现比较器，这里换成比较器更加合理
                    minVertex = item.first;
                    minWeight = item.second.first;
                    preVertex = item.second.second;
                }
            }

            if (minVertex == nullptr) {
                //不存在最短路径
                break;
            }

            //找到了那个顶点之后，从distMap中删除该顶点，对其出边的顶点进行松弛
            distMap.erase(minVertex);
            selectedPath[minVertex] = std::make_pair(minWeight, preVertex);
            //松弛
            for (auto edge : minVertex->_outEdges) {
                Vertex* toVertex = edge->_to;
                if (selectedPath.count(toVertex) > 0) continue; //如果已经找到了那个点的最短路径，跳过

                E newWeight = minWeight + edge->_weight;  // E必须实现operator+,<,>等操作符
                if (distMap.count(toVertex) == 0 || newWeight < distMap[toVertex].first) {
                    //如果还没有计算过这个点的最短路径，或者新的权值和小于旧的最短路径权值和，则进行更新
                    distMap[toVertex].first = newWeight;
                    distMap[toVertex].second = minVertex;
                }
            }

        }

        for (auto item : selectedPath) {
            std::cout << item.first->_value << ": " << item.second.first << ", Path is: ";
            std::cout << item.first->_value;
            Vertex* vertex = item.second.second;
            while (vertex != nullptr) {
                std::cout << " <-- " << vertex->_value;
                vertex = selectedPath[vertex].second;
            }
            std::cout << "\n";
        }
        std::cout << "NO SHORTEST PATH: ";
        for (auto item : _vertices) {
            if (selectedPath.count(item.second) == 0) {
                std::cout << item.first << ", ";
            }
        }
        std::cout << "\n";

    }

    void ShortestPath_BellmanFord(const V& v) {
        //单源最短路径
        //可以有负权边，能检测出负权环
        //O(EV)

        std::cout << "ShortestPath_BellmanFord for " << v << " is: \n";

        VertexIterator iter = _vertices.find(v);
        if (iter == _vertices.end()) return;

        Vertex* beginVertex = (*iter).second;
        std::unordered_map<Vertex*, std::pair<E, Vertex*>, VertexHash, VertexCmp> selectedPath;
        selectedPath[beginVertex] = std::make_pair(0, nullptr);

        size_t count = _vertices.size() - 1;
        for (size_t i = 0; i < count; i++) {
            for (auto edge : _edges) {
                Vertex* fromVertex = edge->_from;
                Vertex* toVertex = edge->_to;
                if (selectedPath.count(fromVertex) == 0) continue;

                E newWeight = selectedPath[fromVertex].first + edge->_weight;
                if (selectedPath.count(toVertex) == 0 || newWeight < selectedPath[toVertex].first) {
                    selectedPath[toVertex].first = newWeight;
                    selectedPath[toVertex].second = fromVertex;
                }
            }
        }

        //再进行一次松弛，判断是否具有负权环
        bool isHaveNegRing = false;
        for (auto edge : _edges) {
            Vertex* fromVertex = edge->_from;
            Vertex* toVertex = edge->_to;
            if (selectedPath.count(fromVertex) == 0) continue;

            E newWeight = selectedPath[fromVertex].first + edge->_weight;
            if (selectedPath.count(toVertex) == 0 || newWeight < selectedPath[toVertex].first) {
                selectedPath[toVertex].first = newWeight;
                selectedPath[toVertex].second = fromVertex;
                isHaveNegRing = true;
                break;
            }
        }
        if (isHaveNegRing) {
            std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~HAVE NEGATIVE WEIGHT RING!!!!~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        }

        //如果没有负权环，输出路径
        if (!isHaveNegRing) {
            for (auto item : selectedPath) {
                std::cout << item.first->_value << ": " << item.second.first << ", Path is: ";
                std::cout << item.first->_value;
                Vertex* vertex = item.second.second;
                while (vertex != nullptr) {
                    std::cout << " <-- " << vertex->_value;
                    vertex = selectedPath[vertex].second;
                }
                std::cout << "\n";
            }
            std::cout << "NO SHORTEST PATH: ";
            for (auto item : _vertices) {
                if (selectedPath.count(item.second) == 0) {
                    std::cout << item.first << ", ";
                }
            }
            std::cout << "\n";
        }

    }

    void ShortestPath_Floyd() {
        //多源最短路径
        //任意两个顶点，支持负权边
        //O(V^3),比执行V次Dijkstra要好

        std::cout << "ShortestPath_Floyd is:\n";
        std::unordered_map<Vertex*,
                std::unordered_map<Vertex*, E, VertexHash, VertexCmp>,
                VertexHash, VertexCmp> dists;

        //初始化
        for (auto edge : _edges) {
            Vertex* fromVertex = edge->_from;
            Vertex* toVertex = edge->_to;
            if (dists.count(fromVertex) == 0) {
                dists[fromVertex] = std::unordered_map<Vertex*, E, VertexHash, VertexCmp>();
                dists[fromVertex][toVertex] = edge->_weight;
            } else {
                if (dists[fromVertex].count(toVertex) == 0 || edge->_weight < dists[fromVertex][toVertex]) {
                    dists[fromVertex][toVertex] = edge->_weight;
                }
            }
        }

        for (auto item1 : _vertices) {
            Vertex* v1 = item1.second;
            for (auto item3 : _vertices) {
                Vertex* v3 = item3.second;
                if (v1 == v3) continue;
                for (auto item2 : _vertices) {
                    Vertex* v2 = item2.second;
                    if (v1 == v2 || v2 == v3) continue;

                    if (dists[v1].count(v2) == 0) continue;
                    if (dists[v2].count(v3) == 0) continue;

                    E v1v2_w = dists[v1][v2];
                    E v2v3_w = dists[v2][v3];
                    if (dists[v1].count(v3) == 0) {
                        dists[v1][v3] = v1v2_w + v2v3_w;
                    } else {
                        E v1v3_w = dists[v1][v3];
                        if ( (v1v2_w + v2v3_w) < v1v3_w) {
                            dists[v1][v3] = v1v2_w + v2v3_w;
                        }
                    }
                }
            }
        }

        for (auto item1 : dists) {
            Vertex* v1 = item1.first;
            for (auto item2 : item1.second) {
                Vertex* v2 = item2.first;
                std::cout << v1->_value << " --> " << v2->_value << ": " << item2.second << "\n";
            }
        }

    }

    void ShortestPath_Floyd_WithPath() {
        //多源最短路径
        //任意两个顶点，支持负权边
        //O(V^3),比执行V次Dijkstra要好

        std::cout << "ShortestPath_Floyd_WithPath is:\n";
        std::unordered_map<Vertex*,
                std::unordered_map<Vertex*, std::pair<E, Vertex*>, VertexHash, VertexCmp>,
                VertexHash, VertexCmp> dists;

        //初始化
        for (auto edge : _edges) {
            Vertex* fromVertex = edge->_from;
            Vertex* toVertex = edge->_to;
            if (dists.count(fromVertex) == 0) {
                dists[fromVertex] = std::unordered_map<Vertex*, std::pair<E, Vertex*>, VertexHash, VertexCmp>();
                dists[fromVertex][toVertex] = std::make_pair(edge->_weight, fromVertex);
            } else {
                if (dists[fromVertex].count(toVertex) == 0 || edge->_weight < dists[fromVertex][toVertex].first) {
                    dists[fromVertex][toVertex] = std::make_pair(edge->_weight, fromVertex);
                }
            }
        }

        for (auto item1 : _vertices) {
            Vertex* v1 = item1.second;
            for (auto item3 : _vertices) {
                Vertex* v3 = item3.second;
                if (v1 == v3) continue;
                for (auto item2 : _vertices) {
                    Vertex* v2 = item2.second;
                    if (v1 == v2 || v2 == v3) continue;

                    if (dists[v1].count(v2) == 0) continue;
                    if (dists[v2].count(v3) == 0) continue;

                    E v1v2_w = dists[v1][v2].first;
                    E v2v3_w = dists[v2][v3].first;
                    if (dists[v1].count(v3) == 0) {//v1-->v3还没有最短路径，则当前找到了一条v1-->v2,v2-->v3的路径
                        dists[v1][v3] = std::make_pair(v1v2_w + v2v3_w, v2);
                    } else {  //v1-->v3已经存在了一条最短路径，看哪条路径更短
                        E v1v3_w = dists[v1][v3].first;
                        if ((v1v2_w + v2v3_w) < v1v3_w) {
                            dists[v1][v3].first = v1v2_w + v2v3_w;
                            dists[v1][v3].second = v2;
                        }
                    }
                }
            }
        }

        //输出最短路径
        for (auto item1 : dists) {
            Vertex* v1 = item1.first;
            for (auto item2 : item1.second) {
                Vertex* v2 = item2.first;
                std::cout << v1->_value << " --> " << v2->_value << ": " << item2.second.first << ", Path is: ";

                Vertex* currentVertex = v2;    // currentVertex
                Vertex* preVertex = item2.second.second;  // preVertex
                do {
                    std::cout << currentVertex->_value << " <-- ";
                    currentVertex = preVertex;
                    if (dists[v1].count(currentVertex) == 0) {
                        break;
                    } else {
                        preVertex = dists[v1][currentVertex].second;
                    }
                } while (currentVertex != v1);
                std::cout << v1->_value << "\n";
            }
        }

    }



    friend std::ostream& operator<<(std::ostream& out, ListGraph* listGraph) {
        out << "===================================================\n";
        out << "NumVertices: " << listGraph->NumVertices()
            << ", NumEdges: " << listGraph->NumEdges() << "\n";
        out << "Vertices: " << "\n";
        for (auto vertex : listGraph->_vertices) {
            out << vertex.second << "\n";
        }
        out << "\n";

//        out << "Edges: " << "\n";
//        for (auto edge : listGraph->_edges) {
//            out << edge << "\n";
//        }
        return out;
    }

};


}

#endif // LISTGRAPH_H
