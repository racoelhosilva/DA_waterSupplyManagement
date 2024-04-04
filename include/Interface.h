#ifndef DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
#define DA_WATERSUPPLYMANAGEMENT_INTERFACE_H

#include "WaterSupplyNetwork.h"

class Interface {
public:

    bool init();

    void printMenuOptions(const std::vector<std::string> &options, int choice);
    void printOptionsPages(const std::vector<std::string> &options, const std::string &title, int choice, int page);
    void printTop();
    void printBottom();
    void waitInput();
    void printTitle(const std::string& title);
    void printNetworkFlow(double flow, bool compare = true);
    void printSelectedPipes();
    void printHiddenPipes();

    void saveGeneralMaxFlowToFile(const std::string& title);
    void saveAllMaxFlowToFile(const std::string& title);
    void saveSingleMaxFlowToFile(const DeliverySite* city, const std::string& title);
    void saveDeficitsToFile(const std::string& title);

    bool pipeMenu();
    void mainMenu();
    void exitMenu();

    DeliverySite * citySelection();
    Reservoir * reservoirSelection();
    PumpingStation * pumpingStationSelection();
    ServicePoint * servicePointSelection();
    ServicePoint * servicePointSelection(ServicePoint *src);

    std::string readInputText();
    void printWriteBuffer(const std::string &buffer);

    void printTable(const std::vector<int> &colLens, const std::vector<std::string> &headers, const std::vector<std::vector<std::string>> &cells);
    void cityDisplay(const std::vector<DeliverySite *> &cities);
    void cityDisplayComparison(const std::vector<DeliverySite *> &cities);
    void displaySupplyDemand();
    void displayServicePointEffects();
    void reservoirDisplay(const std::vector<Reservoir *> &reservoirs);
    void pumpingStationDisplay(const std::vector<PumpingStation *> &pumpingStations);
    void pipeDisplay(const ServicePoint *servicePoint);

private:
    int infoSpacing = 8;
    int width = 80;
    WaterSupplyNetwork wsn;
    double defaultNetworkFlow;
    std::unordered_map<std::string, double> cityToDefaultFlow;
    std::unordered_set<Pipe *> selectedPipes;

    bool outputToFile = false;
    std::string fileName = "../output.txt";
};


#endif //DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
