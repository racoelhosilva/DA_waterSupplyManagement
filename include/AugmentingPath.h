//
// Created by bruno on 3/31/24.
//

#ifndef DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H
#define DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H


#include <vector>
#include "Pipe.h"

class AugmentingPath {
public:
    AugmentingPath();

    void addPipe(Pipe *pipe, bool incoming);
    const std::vector<std::pair<Pipe *, bool>> &getPipes() const;
    double getCapacity() const;

private:
    std::vector<std::pair<Pipe *, bool>> pipes;
    double capacity;
};


#endif //DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H
