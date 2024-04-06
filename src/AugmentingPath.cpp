//
// Created by bruno on 3/31/24.
//

#include "AugmentingPath.h"

#include <limits>

using namespace std;

AugmentingPath::AugmentingPath(): capacity(numeric_limits<double>::infinity()), selected(false) {};

void AugmentingPath::addPipe(Pipe *pipe, bool direct) {
    capacity = min(capacity, direct ? pipe->getRemainingFlow() : pipe->getFlow());
    pipes.emplace_back(pipe, direct);
}

const std::vector<std::pair<Pipe *, bool>> &AugmentingPath::getPipes() const {
    return pipes;
}

double AugmentingPath::getCapacity() const {
    return capacity;
}

bool AugmentingPath::isSelected() const {
    return selected;
}

void AugmentingPath::setSelected(bool selected) {
    AugmentingPath::selected = selected;
}


