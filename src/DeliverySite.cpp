#include "DeliverySite.h"

using namespace std;

DeliverySite::DeliverySite(const string &city, int id, const string &code, double demand, int population)
        : ServicePoint(id, code), city(city), demand(demand), population(population) {}

const string &DeliverySite::getCity() const {
    return city;
}

double DeliverySite::getDemand() const {
    return demand;
}

int DeliverySite::getPopulation() const {
    return population;
}

double DeliverySite::getSupplyRate() const {
    double rate = 0;
    for (Pipe *pipe: ServicePoint::getIncoming())
        rate += pipe->getFlow();
    return rate;
}

std::string DeliverySite::getDescription() const {
    return getCode() + ' ' + getCity();
}