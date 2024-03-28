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
    bool parseData(const std::string& reservoirPath, const std::string& stationsPath, const std::string& citiesPath, const std::string& pipesPath);

    void print();

    std::vector<ServicePoint*> getServicePoints();
    std::vector<Reservoir*> getReservoirs();
    std::vector<PumpingStation*> getPumpingStations();
    std::vector<DeliverySite*> getDeliverySites();
    ServicePoint* findServicePoint(const std::string &code);

    double getMaxFlow(bool theoretical);

    void unhideAll();
    void hideAllButOneDeliverySite(const std::string &code);

private:
    bool parseReservoir(const std::string& reservoirPath);
    bool parseStations(const std::string& stationsPath);
    bool parseCities(const std::string& citiesPath);
    bool parsePipes(const std::string& pipesPath);

    template<class T >
    std::vector<T*> filterVerticesByType();

    void edmondsKarp(ServicePoint *source, ServicePoint *sink);
    void edmondsKarpBfs(ServicePoint* src);
    void reduceAugmentingPath(ServicePoint *source, ServicePoint* sink);
};


#endif //DA_WATERSUPPLYMANAGEMENT_WATERSUPPLYNETWORK_H