#include "ServicePoint.h"

using namespace std;

ServicePoint::ServicePoint(int id, const std::string &code) : Vertex<std::string>(code), id(id), hidden(false) {}

int ServicePoint::getId() const{
    return id;
}

std::string ServicePoint::getCode() const{
    return this->getInfo();
}

std::vector<Pipe *> ServicePoint::getAdj() const {
    vector<Pipe*> adj;
    for (Edge<std::string> *e: Vertex::getAdj()) {
        auto pipe = dynamic_cast<Pipe*>(e);
        if (pipe != nullptr)
            adj.push_back(pipe);
    }
    return adj;
}

std::vector<Pipe *> ServicePoint::getIncoming() const {
    vector<Pipe*> incoming;
    for (Edge<std::string> *e: Vertex::getIncoming()) {
        auto pipe = dynamic_cast<Pipe*>(e);
        if (pipe != nullptr)
            incoming.push_back(pipe);
    }
    return incoming;
}

Pipe *ServicePoint::getPath() const {
    return dynamic_cast<Pipe*>(Vertex::getPath());
}

Edge<std::string> *ServicePoint::addEdge(Vertex<std::string> *dest, double w) {
    auto spDest = dynamic_cast<ServicePoint*>(dest);
    if (spDest == nullptr)
        return nullptr;
    auto newEdge = new Pipe(this, dest, w);
    adj.push_back(newEdge);
    spDest->incoming.push_back(newEdge);
    return newEdge;
}

bool ServicePoint::isHidden() const {
    return hidden;
}

void ServicePoint::setHidden(bool hidden) {
    ServicePoint::hidden = hidden;
}




