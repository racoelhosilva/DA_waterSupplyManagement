#ifndef DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H
#define DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H

#include "ServicePoint.h"
#include <string>

class ServicePoint;

class PumpingStation : public ServicePoint {
public:
    /**
     * @brief Constructor of the PumpingStation class
     * @param id Unique identifier of the pumping station
     * @param code Code of the pumping station
     */
    PumpingStation(int id, const std::string &code);

    std::string getDescription() const;
};


#endif //DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H
