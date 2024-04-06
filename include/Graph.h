#ifndef DA_WATERSUPPLYMANAGEMENT_GRAPH_H
#define DA_WATERSUPPLYMANAGEMENT_GRAPH_H

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "Vertex.h"

/**
 * @brief Hash table of vertices
 * @tparam T Type of the vertices' information
 */
template <class T>
using VertexSet = std::unordered_set<Vertex<T>*, VertexHash<T>, VertexHash<T>>;

/**
 * @brief Class that represents a generic graph
 * @tparam T Type of the vertices' information
 */
template <class T>
class Graph {
public:
    /**
     * @brief Constructor of the Graph class
     */
    Graph();

    /**
     * @brief Destructor of the Graph class
     * @details Complexity: O(V*E), where V is the number of vertices and E the number of edges in the graph.
     */
    virtual ~Graph();

    /**
     * @brief Finds a vertex in the graph
     * @details Complexity: O(1).
     * @param in Info of the vertex to search
     * @return Reference to the vertex wanted, or nullptr of not found
     */
    Vertex<T> *findVertex(const T &in) const;

    /**
     * @brief Adds a vertex to the graph if it does not exist in the graph
     * @details Complexity: O(1).
     * @param v Reference to the vertex to remove
     * @return True if the vertex was successfully added, and false if it already exists
     */
    bool addVertex(Vertex<T>* v);

    /**
     * @brief Removes the vertex with info in from the graph, as well as its related edges
     * @details Complexity: O(E), where E is the number of edges in the graph.
     * @param in Info of the vertex
     * @return True if the vertex was successfully removed, and false if the vertex was not in the graph
     */
    bool removeVertex(const T &in);

    /**
     * @brief Adds an edge from the vertex with info src to the vertex with info dest
     * @details Complexity: O(1).
     * @param src Info of the source vertex
     * @param dest Info of the destination vertex
     * @param w Weight of the edge
     * @return True if the edge was successfully added, and false otherwise (if one of the vertices were not found)
     */
    bool addEdge(const T &src, const T &dest, double w);

    /**
     * @brief Removes the edges from the vertex with info src to the vertex with info dest
     * @details Complexity: O(E1+E2), where E1 is the number of outgoing edges from the source vertex, and E2 is the number of
     * incoming edges to the destination.
     * @param src Info of the source vertex
     * @param dest Info of the destination vertex
     * @return True if an edge was removed successfully, and false otherwise
     */
    bool removeEdge(const T &src, const T &dest);

    /**
     * @brief Adds a bidirectional edge between the vertices with infos src and dest
     * @details Complexity: O(1).
     * @param src Info of one vertex
     * @param dest Info of the other vertex
     * @param w Weight of the edge
     * @return True if the edge was successfully added, and false otherwise
     */
    bool addBidirectionalEdge(const T &src, const T &dest, double w);

    /**
     * @brief Returns a set with all the vertices of the graph
     * @return Set with the graph's vertices
     */
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
VertexSet<T> Graph<T>::getVertexSet() const {
    return vertexSet;
}

#endif //DA_WATERSUPPLYMANAGEMENT_GRAPH_H
