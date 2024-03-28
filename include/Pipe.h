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

    bool isHidden();
    void setHidden(bool hidden);

private:
    bool hidden;
};

#endif //DA_WATERSUPPLYMANAGEMENT_PIPE_H
