#ifndef DA_WATERSUPPLYMANAGEMENT_PIPE_H
#define DA_WATERSUPPLYMANAGEMENT_PIPE_H

#include "Edge.h"
#include "ServicePoint.h"
#include <string>

class ServicePoint;

class Pipe : public Edge<std::string> {
public:
    Pipe(Vertex<std::string> *orig, Vertex<std::string> *dest, double capacity);

    double getCapacity() const;

    ServicePoint *getDest() const;
    ServicePoint *getOrig() const;
    Pipe *getReverse() const;
    double getRemainingFlow() const;

    const std::vector<std::vector<Pipe *>> &getAugmentingPaths() const;
    std::vector<std::vector<Pipe *>> &getAugmentingPaths();

    bool operator==(const Pipe &pipe) const;

private:
    std::vector<std::vector<Pipe *>> augmentingPaths;
};

#endif //DA_WATERSUPPLYMANAGEMENT_PIPE_H
