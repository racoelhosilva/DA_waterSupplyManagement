#ifndef DA_WATERSUPPLYMANAGEMENT_PIPE_H
#define DA_WATERSUPPLYMANAGEMENT_PIPE_H

#include "Edge.h"
#include "ServicePoint.h"
#include "AugmentingPath.h"
#include <string>

class ServicePoint;
class AugmentingPath;

class Pipe : public Edge<std::string> {
public:
    Pipe(Vertex<std::string> *orig, Vertex<std::string> *dest, double capacity);

    double getCapacity() const;

    ServicePoint *getDest() const;
    ServicePoint *getOrig() const;
    Pipe *getReverse() const;
    double getRemainingFlow() const;

    const std::vector<AugmentingPath> &getAugmentingPaths() const;
    std::vector<AugmentingPath> &getAugmentingPaths();

    bool isHidden() const;
    void setHidden(bool hidden);

    bool operator==(const Pipe &pipe) const;

private:
    std::vector<AugmentingPath> augmentingPaths;
    bool hidden;
};

#endif //DA_WATERSUPPLYMANAGEMENT_PIPE_H
