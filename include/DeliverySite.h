#ifndef DA_WATERSUPPLYMANAGEMENT_DELIVERYSITE_H
#define DA_WATERSUPPLYMANAGEMENT_DELIVERYSITE_H


#include "ServicePoint.h"

/**
 * @brief Class representaion of a delivery site in the network
 */
class DeliverySite : public ServicePoint {
public:
    /**
     * @brief Constructor of the DeliverySite class
     * @param city City of the delivery site
     * @param id Unique identifier of the delivery site
     * @param code Code of the delivery site
     * @param demand Rate demand or need of delivery site in m^3/sec
     * @param population Total population of the delivery site
     */
    DeliverySite(const std::string &city, int id, const std::string &code, double demand, int population);

    /**
     * @brief Returns the city of the delivery site
     * @return Constant reference to a string with the name of the city of the delivery site
     */
    const std::string &getCity() const;

    /**
     * @brief Returns the demand rate or need of the delivery site
     * @return Demand rate of the delivery site in m^3/sec
     */
    double getDemand() const;

    /**
     * @brief Returns the supply rate of the delivery site
     * @details Complexity: O(n) where n is the number of incoming edges.
     * @return Supply rate of the delivery site in m^3/sec
     */
    double getSupplyRate() const;

    /**
     * @brief Returns the population of the delivery site
     * @return Total population of the delivery site
     */
    int getPopulation() const;

    /**
     * @brief Returns a description of the delivery site
     * @details Complexity: O(1).
     * @return String with a description (code and city) of the delivery site
     */
    std::string getDescription() const;

private:
    std::string city;
    double demand;
    int population;
};


#endif //DA_WATERSUPPLYMANAGEMENT_DELIVERYSITE_H
