#ifndef DA_WATERSUPPLYMANAGEMENT_RESERVOIR_H
#define DA_WATERSUPPLYMANAGEMENT_RESERVOIR_H

#include "ServicePoint.h"

/**
 * @brief Class representation of a reservoir in the network
 */
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

    /**
     * @brief Returns the water reservoir's name
     * @return String representing the name of the water reservoir
     */
    std::string getName() const;

    /**
     * @brief Returns the municipality where the water reservoir is located
     * @return String representing the municipality where the water reservoir is located
     */
    std::string getMunicipality() const;

    /**
     * @brief Returns the minimum delivery capacity of water reservoir
     * @return Double representing the delivery capacity of the water reservoir
     */
    double getMaxDelivery() const;

    /**
     * @brief Returns the current delivery of the water reservoir
     * @details Complexity: O(E), where E is the number of outgoing pipes from the reservoir
     * @return Double representing the net delivery of the water reservoir in m^3/sec (i.e., total flow leaving the pipe [outgoing pipes])
     */
    double getDelivery() const;

    /**
     * @brief Returns a description of the water reservoir
     * @details Complexity: O(1)
     * @return String with a description (code and name) of the water reservoir
     */
    std::string getDescription() const;

private:
    std::string name;
    std::string municipality;
    double maxDelivery;
};

#endif //DA_WATERSUPPLYMANAGEMENT_RESERVOIR_H
