#ifndef DA_WATERSUPPLYMANAGEMENT_SERVICEPOINT_H
#define DA_WATERSUPPLYMANAGEMENT_SERVICEPOINT_H

#include "Pipe.h"
#include <string>

class Pipe;

class ServicePoint : public Vertex<std::string> {
public:
    ServicePoint(int id, const std::string &code);

    int getId() const;
    std::string getCode() const;

    std::vector<Pipe*> getAdj() const;
    std::vector<Pipe*> getIncoming() const;
    Pipe *getPath() const;
    bool isHidden() const;

    void setHidden(bool hidden);

    Edge<std::string> *addEdge(Vertex<std::string> *dest, double w) override;

private:
    int id;
    bool hidden;
};

#endif //DA_WATERSUPPLYMANAGEMENT_SERVICEPOINT_H
