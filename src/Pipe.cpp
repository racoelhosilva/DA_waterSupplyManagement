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

const std::vector<AugmentingPath*> &Pipe::getAugmentingPaths() const {
    return augmentingPaths;
}

std::vector<AugmentingPath*> &Pipe::getAugmentingPaths() {
    return augmentingPaths;
}

bool Pipe::operator==(const Pipe &pipe) const {
    return getOrig() == pipe.getOrig() && getDest() == pipe.getDest();
}

bool Pipe::isHidden() const {
    return hidden;
}

void Pipe::setHidden(bool hidden) {
    this->hidden = hidden;
    if (getReverse() != nullptr)
        getReverse()->hidden = hidden;
}

void Pipe::setCapacity(double capacity) {
    setWeight(capacity);
}

void Pipe::selectAugmentingPaths() const {
    for (AugmentingPath* path : augmentingPaths)
        path->setSelected(true);
}


