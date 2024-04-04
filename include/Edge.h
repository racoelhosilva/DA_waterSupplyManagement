#ifndef DA_WATERSUPPLYMANAGEMENT_EDGE_H
#define DA_WATERSUPPLYMANAGEMENT_EDGE_H


#include "Vertex.h"

template <class T>
class Vertex;

template <class T>
class Edge {
public:
    Edge(Vertex<T> *orig, Vertex<T> *dest, double weight);
    virtual ~Edge();

    Vertex<T> *getDest() const;
    Vertex<T> *getOrig() const;
    double getWeight() const;
    double getFlow() const;
    bool isSelected() const;
    Edge *getReverse() const;

    void setWeight(double weight);
    void setFlow(double flow);
    void setSelected(bool selected);
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
