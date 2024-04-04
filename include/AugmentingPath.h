//
// Created by bruno on 3/31/24.
//

#ifndef DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H
#define DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H


#include <vector>
#include "Pipe.h"

class Pipe;

class AugmentingPath {
public:
    AugmentingPath();

    void addPipe(Pipe *pipe, bool incoming);
    const std::vector<std::pair<Pipe *, bool>> &getPipes() const;
    double getCapacity() const;
    bool isSelected() const;

    void setSelected(bool selected);

private:
    std::vector<std::pair<Pipe *, bool>> pipes;
    double capacity;
    bool selected;
};


#endif //DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H
