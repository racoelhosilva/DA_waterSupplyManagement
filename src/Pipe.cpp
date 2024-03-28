#include "Pipe.h"

Pipe::Pipe(Vertex<std::string> *orig, Vertex<std::string> *dest, double capacity) : Edge(orig, dest, capacity), hidden(false) {}

double Pipe::getCapacity() const {
    return getWeight();
}

ServicePoint *Pipe::getDest() const {
    return dynamic_cast<ServicePoint*>(Edge::getDest());
}

ServicePoint *Pipe::getOrig() const {
    return dynamic_cast<ServicePoint*>(Edge::getOrig());
}

Pipe *Pipe::getReverse() const {
    return dynamic_cast<Pipe*>(Edge::getReverse());
}

double Pipe::getRemainingFlow() const {
    return getWeight() - getFlow();
}

bool Pipe::isHidden() {
    return hidden;
}

void Pipe::setHidden(bool hidden) {
    this->hidden = hidden;
    if (getReverse() != nullptr)
        getReverse()->hidden = hidden;
}


