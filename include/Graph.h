#ifndef DA_WATERSUPPLYMANAGEMENT_GRAPH_H
#define DA_WATERSUPPLYMANAGEMENT_GRAPH_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Vertex.h"

template <class T>
using VertexSet = std::unordered_set<Vertex<T>*, VertexHash<T>, VertexHash<T>>;

template <class T>
class Graph {
public:
    Graph();
    virtual ~Graph();

    Vertex<T> *findVertex(const T &in) const;

    bool addVertex(Vertex<T>* v);
    bool removeVertex(const T &in);

    bool addEdge(const T &src, const T &dest, double w);
    bool removeEdge(const T &src, const T &dest);
    bool addBidirectionalEdge(const T &src, const T &dest, double w);

    int getNumVertex() const;
    VertexSet<T> getVertexSet() const;

private:
    VertexSet<T> vertexSet;
};

template <class T>
Graph<T>::Graph() = default;

template <class T>
Graph<T>::~Graph() {
    for (Vertex<T>* v: vertexSet)
        v->removeOutgoingEdges();
    for (Vertex<T>* v: vertexSet)
        delete v;
}

template<class T>
Vertex<T> *Graph<T>::findVertex(const T &in) const {
    Vertex<T> temp(in);
    auto res = vertexSet.find(&temp);
    return res != vertexSet.end() ? *res : nullptr;
}

template<class T>
bool Graph<T>::addVertex(Vertex<T> *v) {
    if (findVertex(v->getInfo()) != nullptr)
        return false;
    vertexSet.insert(v);
    return true;
}

template<class T>
bool Graph<T>::removeVertex(const T &in) {
    Vertex<T> temp(in);
    auto it = vertexSet.find(&temp);
    if (it == vertexSet.end())
        return false;

    auto v = *it;
    v->removeOutgoingEdges();
    for (Vertex<T>* u: vertexSet)
        u->removeEdge(v->getInfo());
    vertexSet.erase(it);
    delete v;
    return true;
}

template<class T>
bool Graph<T>::addEdge(const T &src, const T &dest, double w) {
    Vertex<T> *u = findVertex(src), *v = findVertex(dest);
    if (u == nullptr || v == nullptr)
        return false;
    u->addEdge(v, w);
    return true;
}

template<class T>
bool Graph<T>::removeEdge(const T &src, const T &dest) {
    Vertex<T> *v = findVertex(src);
    if (v == nullptr)
        return false;
    return v->removeEdge(dest);
}

template<class T>
bool Graph<T>::addBidirectionalEdge(const T &src, const T &dest, double w) {
    Vertex<T> *u = findVertex(src), *v = findVertex(dest);
    if (u == nullptr || v == nullptr)
        return false;
    Edge<T> *e = u->addEdge(v, w), *er = v->addEdge(u, w);
    e->setReverse(er);
    er->setReverse(e);
    return true;
}

template<class T>
int Graph<T>::getNumVertex() const {
    return vertexSet.size();
}

template<class T>
VertexSet<T> Graph<T>::getVertexSet() const {
    return vertexSet;
}

#endif //DA_WATERSUPPLYMANAGEMENT_GRAPH_H
