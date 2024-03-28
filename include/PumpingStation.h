#ifndef DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H
#define DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H

#include "ServicePoint.h"
#include <string>

class ServicePoint;

class PumpingStation : public ServicePoint {
public:
    PumpingStation(int id, const std::string &code);

    std::string getDescription() const override;
};


#endif //DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H
