#include "Reservoir.h"
using namespace std;

Reservoir::Reservoir(const string &name, const string &municipality, int id, const string &code, double maxDelivery) : name(name), municipality(municipality), ServicePoint(id, code), maxDelivery(maxDelivery) {}

string Reservoir::getName() const {
    return this->name;
}

string Reservoir::getMunicipality() const {
    return this->municipality;
}

double Reservoir::getMaxDelivery() const {
    return this->maxDelivery;
}

double Reservoir::getDelivery() const {
    double delivery = 0;
    for (Pipe *pipe: ServicePoint::getAdj())
        delivery += pipe->getFlow();
    for (Pipe *pipe: ServicePoint::getIncoming())
        delivery -= pipe->getFlow();
    return delivery;
}
