#ifndef DA_WATERSUPPLYMANAGEMENT_VERTEX_H
#define DA_WATERSUPPLYMANAGEMENT_VERTEX_H

#include "Edge.h"
#include <vector>

template <class T>
class Edge;

/**
 * @brief Class that represents a vertex
 * @tparam T Type of the vertex's info
 */
template <class T>
class Vertex {
public:
    /**
     * @brief Constructor of the Vertex class
     * @param info The information of the vertex
     */
    explicit Vertex(T info);

    /**
     * @brief Destructor of the Vertex class
     */
    virtual ~Vertex();

    /**
     * @brief Returns the info of the vertex
     * @return The info of the vertex
     */
    T getInfo() const;

    /**
     * @brief Returns the outgoing edges of the vertex
     * @return Vector of references to the vertex's outgoing edges
     */
    std::vector<Edge<T> *> getAdj() const;

    /**
     * @brief Returns if the vertex was visited
     * @return True if the vertex was set as visited, and false otherwise
     */
    bool isVisited() const;

    /**
     * @brief Returns if the vertex is being processed
     * @return True if the vertex was set as processed, and false otherwise
     */
    bool isProcessing() const;

    /**
     * @brief Returns the parent's edge that connects to this vertex (auxiliary to graph searches)
     * @return The parent's edge to this vertex
     */
    Edge<T> *getPath() const;

    /**
     * @brief Returns the incoming edges to the vertex
     * @return Vector of references to the vertex's incoming edges
     */
    std::vector<Edge<T> *> getIncoming() const;

    /**
     * @brief Sets if the vertex has been visited
     * @param visited Whether the vertex was visited or not
     */
    void setVisited(bool visited);

    /**
     * @brief Sets if the vertex is being processed
     * @param processing Whether the vertex is being processed or not
     */
    void setProcessing(bool processing);

    /**
     * @brief Sets the parent's edge that connects to this vertex (auxiliary to graph searches)
     * @param path The parent's edge to this vertex
     */
    void setPath(Edge<T> *path);

    /**
     * @brief Adds an outgoing edge from this vertex to dest, and the same edge as an incoming edge of dest
     * Complexity: O(1).
     * @param dest Destination of the pipe
     * @param w Edge's weight
     * @return Pointer to the newly created edge
     */
    virtual Edge<T> * addEdge(Vertex<T> *dest, double w);

    /**
     * @brief Removes all the outgoing edges to the vertex with info in
     * Complexity: O(E1+E2), where E1 is the number of outgoing edges of this vertex and E2 is the number of incoming edges
     * edges of the destination vertex.
     * @param in The info of the destination vertex
     * @return True if an edge was removed or not, and false otherwise
     */
    bool removeEdge(T in);

    /**
     * @brief Removes all the outgoing edges, updating the destination vertexes accordingly
     * Complexity: O(E), where E is the number of edges in the graph.
     */
    void removeOutgoingEdges();

protected:
    T info;
    std::vector<Edge<T> *> adj;
    bool visited = false;
    bool processing = false;
    Edge<T> *path = nullptr;

    std::vector<Edge<T> *> incoming;

    /**
     * @brief Removes the edge from the list of incoming edges of the destination and frees it.
     * Complexity: O(E), where E is the number of incoming edges of the destination vertex
     * @param edge The edge to be deleted
     */
    void deleteEdge(Edge<T> *edge);
};

/**
 * @brief Hash functional object class for vertex object.
 * @tparam T Vertex's info type
 */
template <class T>
struct VertexHash {
    /**
     * @brief Returns the hash of a vertex
     * Complexity: O(1).
     * @param v Reference to the vertex
     * @return Hash code of the vertex
     */
    int operator()(const Vertex<T> *v) const;

    /**
     * @brief Returns whether two vertices are the same
     * @param v1 Reference to one vertex
     * @param v2 Reference to the other vertex
     * @return True if the two vertices are equal, and false otherwise
     */
    bool operator()(const Vertex<T> *v1, const Vertex<T> *v2) const;
};

template <class T>
Vertex<T>::Vertex(T in): info(in) {}

template<class T>
Vertex<T>::~Vertex() = default;

template <class T>
Edge<T> * Vertex<T>::addEdge(Vertex<T> *d, double w) {
    auto newEdge = new Edge<T>(this, d, w);
    adj.push_back(newEdge);
    d->incoming.push_back(newEdge);
    return newEdge;
}

template <class T>
bool Vertex<T>::removeEdge(T in) {
    bool removedEdge = false;
    auto it = adj.begin();
    while (it != adj.end()) {
        Edge<T> *edge = *it;
        Vertex<T> *dest = edge->getDest();
        if (dest->getInfo() == in) {
            it = adj.erase(it);
            deleteEdge(edge);
            removedEdge = true;
        } else {
            it++;
        }
    }
    return removedEdge;
}

template <class T>
void Vertex<T>::removeOutgoingEdges() {
    for(Edge<T> *edge: this->adj) {
        deleteEdge(edge);
    }
    adj.clear();
}

template <class T>
T Vertex<T>::getInfo() const {
    return this->info;
}

template <class T>
std::vector<Edge<T>*> Vertex<T>::getAdj() const {
    return this->adj;
}

template <class T>
bool Vertex<T>::isVisited() const {
    return this->visited;
}

template <class T>
bool Vertex<T>::isProcessing() const {
    return this->processing;
}

template <class T>
Edge<T> *Vertex<T>::getPath() const {
    return this->path;
}

template <class T>
std::vector<Edge<T> *> Vertex<T>::getIncoming() const {
    return this->incoming;
}

template <class T>
void Vertex<T>::setVisited(bool visited) {
    this->visited = visited;
}

template <class T>
void Vertex<T>::setProcessing(bool processing) {
    this->processing = processing;
}

template <class T>
void Vertex<T>::setPath(Edge<T> *path) {
    this->path = path;
}

template <class T>
void Vertex<T>::deleteEdge(Edge<T> *edge) {
    Vertex<T> *dest = edge->getDest();

    auto it = dest->incoming.begin();
    while (it != dest->incoming.end()) {
        if ((*it)->getOrig()->getInfo() == info) {
            it = dest->incoming.erase(it);
        }
        else {
            it++;
        }
    }
    delete edge;
}

template<class T>
int VertexHash<T>::operator()(const Vertex<T> *v) const {
    return std::hash<T>()(v->getInfo());
}

template<class T>
bool VertexHash<T>::operator()(const Vertex<T> *v1, const Vertex<T> *v2) const {
    return v1->getInfo() == v2->getInfo();
}

#endif //DA_WATERSUPPLYMANAGEMENT_VERTEX_H
