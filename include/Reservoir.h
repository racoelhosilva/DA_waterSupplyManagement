#ifndef DA_WATERSUPPLYMANAGEMENT_RESERVOIR_H
#define DA_WATERSUPPLYMANAGEMENT_RESERVOIR_H

#include "ServicePoint.h"

class Reservoir : public ServicePoint {
public:
    /**
     * @brief Constructor of the Reservoir class
     * @param name Name of the water reservoir
     * @param municipality Municipality where the water reservoir is located
     * @param id Unique identifier of the water reservoir
     * @param code Code of the water reservoir
     * @param maxDelivery Maximum delivery of water reservoir in m^3/sec
     */
    Reservoir(const std::string &name, const std::string &municipality, int id, const std::string &code, double maxDelivery);

    std::string getName() const;
    std::string getMunicipality() const;
    double getMaxDelivery() const;
    double getDelivery() const;

    std::string getDescription() const;

private:
    std::string name;
    std::string municipality;
    double maxDelivery;
};

#endif //DA_WATERSUPPLYMANAGEMENT_RESERVOIR_H
