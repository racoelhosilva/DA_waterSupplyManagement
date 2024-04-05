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

class WaterSupplyNetwork : private Graph<std::string> {
public:
    WaterSupplyNetwork();
    ~WaterSupplyNetwork() override;

    bool parseData(const std::string& reservoirPath, const std::string& stationsPath, const std::string& citiesPath, const std::string& pipesPath);

    std::vector<ServicePoint*> getServicePoints();
    std::vector<Reservoir*> getReservoirs();
    std::vector<PumpingStation*> getPumpingStations();
    std::vector<DeliverySite*> getDeliverySites();
    ServicePoint *findServicePoint(const std::string &code);
    DeliverySite *findDeliverySite(const std::string &code);
    Reservoir *findReservoir(const std::string &code);
    PumpingStation *findPumpingStation(const std::string &code);
    Pipe *findPipe(const std::string &src, const std::string &dest);

    double getMaxFlow(bool theoretical = false);
    double calculateMaxFlowAndAugmentingPathsThroughPipe(Pipe *pipe);
    double calculateMaxFlowAndAugmentingPathsToCity(DeliverySite *city);
    void subtractAugmentingPaths();
    double recalculateMaxFlow();

    void unhideAll();
    void hideAllButOneDeliverySite(const std::string &code);
    void hideReservoir(const std::string &code);
    void hidePumpingStation(const std::string &code);
    void hideServicePoint(const std::string &code);

    void hidePipe(Pipe *pipe);
    void unhidePipe(Pipe *pipe);

    void getMetrics(double &max, double &mean, double &variance);


    void storeNetwork();
    void loadNetwork();

private:
    bool parseReservoir(const std::string& reservoirPath);
    bool parseStations(const std::string& stationsPath);
    bool parseCities(const std::string& citiesPath);
    bool parsePipes(const std::string& pipesPath);

    template<class T >
    std::vector<T*> filterVerticesByType();

    void createSuperSourceAndSuperSink();
    void edmondsKarp(ServicePoint *source, ServicePoint *sink);
    void edmondsKarpBfs(ServicePoint* src);
    AugmentingPath reduceAugmentingPath(ServicePoint *source, ServicePoint* sink);

    void destroySuperSourceAndSuperSink();

    void copyGraph(WaterSupplyNetwork *network1, WaterSupplyNetwork *network2);
    void copyFlows(WaterSupplyNetwork *network1, WaterSupplyNetwork *network2);

    WaterSupplyNetwork *auxNetwork;
    ServicePoint *superSource;
    ServicePoint *superSink;
    std::vector<AugmentingPath> augmentingPaths;
};


#endif //DA_WATERSUPPLYMANAGEMENT_WATERSUPPLYNETWORK_H
