#ifndef DA_WATERSUPPLYMANAGEMENT_EDGE_H
#define DA_WATERSUPPLYMANAGEMENT_EDGE_H


#include "Vertex.h"

template <class T>
class Vertex;

/**
 * @brief Class that represents an edge in a graph
 * @tparam T Type of the info of the vertices that the edge connects
 */
template <class T>
class Edge {
public:
    /**
     * @brief Constructor of the Edge class
     * @param orig Pointer to the origin vertex of the edge
     * @param dest Pointer to the destination vertex of the edge
     * @param weight The weight of the edge
     */
    Edge(Vertex<T> *orig, Vertex<T> *dest, double weight);

    /**
     * @brief Destructor of the Edge class
     */
    virtual ~Edge();

    /**
     * @brief Returns destination vertex of the edge
     * @return Pointer to the destination vertex
     */
    Vertex<T> *getDest() const;

    /**
     * @brief Returns the origin vertex of the edge
     * @return Pointer to the origin vertex
     */
    Vertex<T> *getOrig() const;

    /**
     * @brief Returns the weight of the edge
     * @return The weight of the edge
     */
    double getWeight() const;

    /**
     * @brief Returns the flow through the edge
     * @return The flow through the edge
     */
    double getFlow() const;

    /**
     * @brief Returns if the edge is selected
     * @return True if the edge was set as selected, and false otherwise
     */
    bool isSelected() const;

    /**
     * @brief Returns the reverse edge
     * @return Pointer to the reverse edge
     */
    Edge *getReverse() const;

    /**
     * @brief Sets the weight of the edge
     * @param weight The new weight of the edge
     */
    void setWeight(double weight);

    /**
     * @brief Sets the flow through the edge
     * @param flow The new flow through the edge
     */
    void setFlow(double flow);

    /**
     * @brief Sets if the edge is selected
     * @param selected True to set the edge as selected, false to set it as unselected
     */
    void setSelected(bool selected);

    /**
     * @brief Sets the reverse edge
     * @param reverse Pointer to the reverse edge
     */
    void setReverse(Edge *reverse);

private:
    Vertex<T>* orig;
    Vertex<T>* dest;
    double weight;
    double flow;
    bool selected;
    Edge* reverse;
};

template <class T>
Edge<T>::Edge(Vertex<T> *orig, Vertex<T> *dest, double weight) : orig(orig), dest(dest), weight(weight), reverse(nullptr) {}

template<class T>
Edge<T>::~Edge() = default;

template <class T>
Vertex<T> *Edge<T>::getDest() const {
    return dest;
}

template <class T>
Vertex<T> *Edge<T>::getOrig() const {
    return orig;
}

template <class T>
double Edge<T>::getWeight() const {
    return weight;
}

template <class T>
double Edge<T>::getFlow() const {
    return flow;
}

template <class T>
bool Edge<T>::isSelected() const {
    return selected;
}

template <class T>
Edge<T> *Edge<T>::getReverse() const {
    return reverse;
}

template<class T>
void Edge<T>::setWeight(double weight) {
    Edge::weight = weight;
}

template <class T>
void Edge<T>::setFlow(double flow) {
    Edge<T>::flow = flow;
}

template <class T>
void Edge<T>::setSelected(bool selected) {
    Edge<T>::selected = selected;
}
template <class T>
void Edge<T>::setReverse(Edge<T> *reverse) {
    Edge<T>::reverse = reverse;
}


#endif //DA_WATERSUPPLYMANAGEMENT_EDGE_H
