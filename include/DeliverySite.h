#ifndef DA_WATERSUPPLYMANAGEMENT_DELIVERYSITE_H
#define DA_WATERSUPPLYMANAGEMENT_DELIVERYSITE_H


#include "ServicePoint.h"

class DeliverySite : public ServicePoint {
public:
    DeliverySite(const std::string &city, int id, const std::string &code, double demand, int population);

    const std::string &getCity() const;
    double getDemand() const;
    double getSupplyRate() const;
    int getPopulation() const;

    std::string getDescription() const;

private:
    std::string city;
    double demand;
    int population;
};


#endif //DA_WATERSUPPLYMANAGEMENT_DELIVERYSITE_H
