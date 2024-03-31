//
// Created by bruno on 3/31/24.
//

#include "AugmentingPath.h"

#include <limits>

using namespace std;

AugmentingPath::AugmentingPath(): capacity(numeric_limits<double>::infinity()) {};

void AugmentingPath::addPipe(Pipe *pipe, bool incoming) {
    capacity = min(capacity, incoming ? pipe->getRemainingFlow() : pipe->getFlow());
    pipes.emplace_back(pipe, incoming);
}

const std::vector<std::pair<Pipe *, bool>> &AugmentingPath::getPipes() const {
    return pipes;
}

double AugmentingPath::getCapacity() const {
    return capacity;
}


