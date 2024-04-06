#ifndef DA_WATERSUPPLYMANAGEMENT_WATERSUPPLYNETWORK_H
#define DA_WATERSUPPLYMANAGEMENT_WATERSUPPLYNETWORK_H

#include <string>
#include <queue>
#include <stack>
#include "Graph.h"
#include "ServicePoint.h"
#include "Reservoir.h"
#include "PumpingStation.h"
#include "DeliverySite.h"
#include "AugmentingPath.h"

/**
 * @brief Class representation of a water supply network
 * @details It represents the data in a specialized graph.
 */
class WaterSupplyNetwork : private Graph<std::string> {
public:
    /**
     * @brief Constructor of the WaterSupplyNetwork class
     */
    WaterSupplyNetwork();
    /**
     * @brief Destructor of the WatterSupplyNetwork class
     * @details Complexity: O(V*E), where V is the number of vertices (service points) and E the number of edges (pipes) in the graph.
     */
    ~WaterSupplyNetwork() override;

    /**
     * @brief Parses the data files and initializes appropriate data structures
     * @details Complexity://TODO
     * @param reservoirPath Path to reservoirs data file
     * @param stationsPath Path to pumping stations data file
     * @param citiesPath Path to cities (delivery sites) data file
     * @param pipesPath Path to pipes data file
     * @return Returns true if successful, and false if it fails to read the files
     */
    bool parseData(const std::string& reservoirPath, const std::string& stationsPath, const std::string& citiesPath, const std::string& pipesPath);

    /**
     * @brief Returns a vector of pointers to all service points in the network
     * @details Complexity: //TODO
     * @return Vector of pointers to all service points in the network
     */
    std::vector<ServicePoint*> getServicePoints();

    /**
     * @brief Returns a vector of pointers to all reservoirs in the network
     * @details Complexity:
     * @return Vector of pointers to all reservoirs in the network
     */
    std::vector<Reservoir*> getReservoirs();

    /**
     * @brief Returns a vector of pointers to all pumping stations
     * @details Complexity:
     * @return Vector of pointers to all reservoirs in the network
     */
    std::vector<PumpingStation*> getPumpingStations();

    /**
     * @brief Returns a vector of pointers to all delivery sites
     * @details Complexity:
     * @return Vector of pointers to all reservoirs in the network
     */
    std::vector<DeliverySite*> getDeliverySites();

    /**
     * @brief Finds a service point given its code
     * @details Complexity: O(1).
     * @param code Code of the service point to find
     * @return Pointer to the found service point if it exists, or nullptr otherwise
     */
    ServicePoint *findServicePoint(const std::string &code);

    /**
     * @brief Finds a delivery site given its code
     * @details Complexity: O(1).
     * @param code Code of the delivery site to find
     * @return Pointer to the found delivery site if it exists, or nullptr otherwise
     */
    DeliverySite *findDeliverySite(const std::string &code);

    /**
     * @brief Finds a reservoir given its code
     * @details Complexity: O(1).
     * @param code Code of the reservoir to find
     * @return Pointer to the found reservoir if it exists, or nullptr otherwise
     */
    Reservoir *findReservoir(const std::string &code);

    /**
     * @brief Finds a pumping station given its code
     * @details Complexity: O(1).
     * @param code Code of the pumping station to find
     * @return Pointer to the found pumping station if it exists, or nullptr otherwise
     */
    PumpingStation *findPumpingStation(const std::string &code);

    /**
     * @brief Finds a pipe given its source and destination codes
     * @details Complexity: O(n), where n is the number of outgoing edges of the source vertex (service point)
     * @param src Code of the source service point
     * @param dest Code of the destination service point
     * @return Pointer to the found service pipe if it exists, or nullptr otherwise
     */
    Pipe *findPipe(const std::string &src, const std::string &dest);

    /**
     * @brief
     * @details Complexity:
     * @param saveAugmentingPaths
     * @return Max flow
     */
    double getMaxFlow(bool saveAugmentingPaths = false);

    /**
     * @brief
     * @details Complexity:
     * @return
     */
    double loadCachedMaxFlow();

    /**
     * @brief Function to calculate the max flow without some of the pipes (optimized)
     * @details Complexity: //TODO
     * @param pipes Vector of the pipes that shouldn't be considered
     * @return The max flow of the entire network
     */
    double getMaxFlowWithoutPipes(const std::vector<Pipe*> &pipes);

    /**
     * @brief Function to calculate the max flow without one of the reservoirs (optimized)
     * @details Complexity: //TODO
     * @param reservoir Reservoir that shouldn't be considered
     * @return The max flow of the entire network
     */
    double getMaxFlowWithoutReservoir(Reservoir* reservoir);

    /**
     * @brief Function to calculate the max flow without one of the pumping stations (optimized)
     * @details Complexity: //TODO
     * @param station Pumping station that shouldn't be considered
     * @return The max flow of the entire network
     */
    double getMaxFlowWithoutStation(PumpingStation* station);

    /**
     * @brief Function to calculate the max flow without some of the pipes (brute-force)
     * @details Complexity: //TODO
     * @param pipes Vector of the pipes that shouldn't be considered
     * @return The max flow of the entire network
     */
    double getMaxFlowWithoutPipesBF(const std::vector<Pipe*> &pipes);

    /**
     * @brief Function to calculate the max flow without one of the reservoirs (brute-force)
     * @details Complexity: //TODO
     * @param reservoir Reservoir that shouldn't be considered
     * @return The max flow of the entire network
     */
    double getMaxFlowWithoutReservoirBF(Reservoir* reservoir);

    /**
     * @brief Function to calculate the max flow without one of the pumping stations (brute-force)
     * @details Complexity: //TODO
     * @param station Pumping station that shouldn't be considered
     * @return The max flow of the entire network
     */
    double getMaxFlowWithoutStationBF(PumpingStation* station);

    /**
     * @brief Function to obtain the critical pipes for a given city
     * @details Complexity: //TODO
     * @param city Pointer to the city to be considered for the critical pipes
     * @return Vector containing pointers to all the pipes that are critical for the given city
     */
    std::vector<Pipe*> getCriticalPipesToCity(DeliverySite *city);

    /**
     * @brief Marks all service points as not hidden
     * @details Complexity: O(V), where E is the number of service points in the water supply network
     */
    void unhideAllServicePoints();

    /**
     * @brief Marks all pipes as not hidden
     * @details Complexity: O(V*E), where V is the number of service points, and E is the number of pipes
     */
    void unhideAllPipes();

    /**
     * @brief Marks all but one delivery site as hidden
     * @details Complexity: //TODO
     * @param code String corresponding to the code of the delivery site to not mark as hidden
     */
    void hideAllButOneDeliverySite(const std::string &code);

    /**
     * @brief Marks one reservoir as hidden
     * @details Complexity: //TODO
     * @param code String corresponding to the code of the reservoir to mark as hidden
     */
    void hideReservoir(const std::string &code);

    /**
     * @brief Marks one pumping station as hidden
     * @details Complexity: //TODO
     * @param code String corresponding to the code of the pumping station to mark as hidden
     */
    void hidePumpingStation(const std::string &code);

    /**
     * @brief Marks one service point as hidden
     * @details Complexity: //TODO
     * @param code String corresponding to the code of the service point to mark as hidden
     */
    void hideServicePoint(const std::string &code);

    /**
     * @brief Marks one pipe as hidden
     * @details Complexity: O(1).
     * @param pipe Pointer to the pipe to mark as hidden
     */
    void hidePipe(Pipe *pipe);

    /**
     * @brief Marks one pipe as not hidden
     * @details Complexity: O(1).
     * @param pipe Pointer to the pipe to mark as not hidden
     */
    void unhidePipe(Pipe *pipe);

    /**
     * @brief Marks as hidden the pipes with negative flows or to the supersource/supersink and calculates metrics
     * @details Complexity: O(V*E), where V is the number of service points, and E is the number of adjacent pipes.
     * @param metrics Tuple with values that will hold the metrics
     */
    void getMetrics(std::tuple<double, double, double> &metrics);

    /**
     * @brief Heuristic algorithm to balance the flow between the pipes on the network, whilst keeping the total flow
     * @details Complexity: O(V*E^3*C), where V is number of service points, E is the number of pipes, and C is the total sum capacity of the graph
     * @param value Reference value used to update the flow in each pipe. The smaller the value, the more precise the results are but the algorithm is slower.
     */
    void balance(double value);

    void storeNetwork();
    void loadNetwork();

private:
    /**
     * @brief Parses the file with the information of the reservoirs
     * @details Complexity: O(n), where n is the number of lines in the file
     * @param reservoirPath Path to the reservoirs file
     * @return True upon successfully parsing the file, and false otherwise
     */
    bool parseReservoir(const std::string& reservoirPath);

    /**
     * @brief Parses the file with the information of the stations
     * @details Complexity: O(n), where n is the number of lines in the file
     * @param stationsPath Path to the stations file
     * @return True upon successfully parsing the file, and false otherwise
     */
    bool parseStations(const std::string& stationsPath);

    /**
     * @brief Parses the file with the information of the cities
     * @details Complexity: O(n), where n is the number of lines in the file
     * @param citiesPath Path to the cities file
     * @return True upon successfully parsing the file, and false otherwise
     */
    bool parseCities(const std::string& citiesPath);

    /**
     * @brief Parses the file with the information of the pipes
     * @details Complexity: O(n), where n is the number of lines in the file
     * @param pipesPath Path to the pipes file
     * @return True upon successfully parsing the file, and false otherwise
     */
    bool parsePipes(const std::string& pipesPath);

    /**
     * @brief Filters the vertices of the network by a specified type
     * @details Complexity: O(V), where V is the number of vertices in the network.
     * @tparam T The type to filter
     * @return Vector with the references of the filtered vertices, properly converted to the type
     */
    template<class T >
    std::vector<T*> filterVerticesByType();

    /**
     * @brief Creates a super source and a super sink
     * @details If createPipes is set to true, it also creates the pipes that connects them to the reservoirs and
     * delivery sites. Complexity: O(1)
     * @param createPipes
     */
    void createSuperSourceAndSuperSink(bool createPipes = true);

    /**
     * @brief
     * @param source
     * @param sink
     * @param savePaths
     */
    void edmondsKarp(ServicePoint *source, ServicePoint *sink, bool savePaths = false);
    void edmondsKarpBfs(ServicePoint* srcSp, ServicePoint *sinkSp);
    AugmentingPath reduceAugmentingPath(ServicePoint *source, ServicePoint* sink);
    void subtractAugmentingPath(const AugmentingPath& augmentingPath);
    void subtractAugmentingPaths();
    double recalculateMaxFlow();
    void unselectAllAugmentingPaths();

    void copyGraph(WaterSupplyNetwork *network1, WaterSupplyNetwork *network2);
    void copyFlows(WaterSupplyNetwork *network1, WaterSupplyNetwork *network2);


    WaterSupplyNetwork *maxFlowNetwork;
    ServicePoint *superSource;
    ServicePoint *superSink;
    std::vector<AugmentingPath> augmentingPaths;
};


#endif //DA_WATERSUPPLYMANAGEMENT_WATERSUPPLYNETWORK_H
