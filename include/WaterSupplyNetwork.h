#ifndef DA_WATERSUPPLYMANAGEMENT_WATERSUPPLYNETWORK_H
#define DA_WATERSUPPLYMANAGEMENT_WATERSUPPLYNETWORK_H

#include <string>
#include "Graph.h"
#include "ServicePoint.h"
#include "Reservoir.h"
#include "PumpingStation.h"
#include "DeliverySite.h"

class WaterSupplyNetwork : private Graph<std::string> {
public:
    WaterSupplyNetwork();
    ~WaterSupplyNetwork() override;

    bool parseData(const std::string& reservoirPath, const std::string& stationsPath, const std::string& citiesPath, const std::string& pipesPath);

    void print();

    std::vector<ServicePoint*> getServicePoints();
    std::vector<Reservoir*> getReservoirs();
    std::vector<PumpingStation*> getPumpingStations();
    std::vector<DeliverySite*> getDeliverySites();
    ServicePoint *findServicePoint(const std::string &code);
    Pipe *findPipe(const std::string &orig, const std::string &dest);

    double getMaxFlow(bool theoretical = false);
    double calculateMaxFlowAndAugmentingPathsThroughPipe(Pipe *pipe, bool theoretical = false);
    double calculateMaxFlowAndAugmentingPathsToCity(DeliverySite *city, bool theoretical = false);
    double recalculateMaxFlow(bool theoretical = false);

    void unhideAll();
    void hideAllButOneDeliverySite(const std::string &code);

    void storeNetwork();
    void loadNetwork();

private:
    bool parseReservoir(const std::string& reservoirPath);
    bool parseStations(const std::string& stationsPath);
    bool parseCities(const std::string& citiesPath);
    bool parsePipes(const std::string& pipesPath);

    template<class T >
    std::vector<T*> filterVerticesByType();

    void createSuperSourceAndSuperSink(ServicePoint *&superSource, ServicePoint *&superSink, bool theoretical);
    void edmondsKarp(ServicePoint *source, ServicePoint *sink);
    void edmondsKarpBfs(ServicePoint* src);
    void reduceAugmentingPath(ServicePoint *source, ServicePoint* sink);
    void destroySuperSourceAndSuperSink();

    void copyGraph(WaterSupplyNetwork *network1, WaterSupplyNetwork *network2);

    WaterSupplyNetwork *auxNetwork;
};


#endif //DA_WATERSUPPLYMANAGEMENT_WATERSUPPLYNETWORK_H
