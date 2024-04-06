#ifndef DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H
#define DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H

#include "ServicePoint.h"
#include <string>

class ServicePoint;

/**
 * @brief Class representation of a pumping station in the network
 */
class PumpingStation : public ServicePoint {
public:
    /**
     * @brief Constructor of the PumpingStation class
     * @param id Unique identifier of the pumping station
     * @param code Code of the pumping station
     */
    PumpingStation(int id, const std::string &code);

    /**
     * @brief Returns a description of the pumping station
     * @details Complexity: O(1).
     * @return String with a description (code) of the pumping station
     */
    std::string getDescription() const;
};


#endif //DA_WATERSUPPLYMANAGEMENT_PUMPINGSTATION_H
