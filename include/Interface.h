#ifndef DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
#define DA_WATERSUPPLYMANAGEMENT_INTERFACE_H

#include "WaterSupplyNetwork.h"

class Interface {
public:

    bool init();

    void printOptions(const std::vector<std::string> &options, int choice);
    void printOptionsCity(const std::vector<std::string> &options, const std::string &title, int choice, int page);
    void printTop();
    void printBottom();
    void waitInput();

    void mainMenu();
    void exitMenu();

    DeliverySite * citySelection();

    std::string readInputText();
    void printWriteBuffer(const std::string &buffer);

    void cityDisplay(const std::vector<DeliverySite *> &cities);
    void displaySupplyDemand();
    void reservoirDisplay(const std::vector<Pipe> &pipe);

private:
    int width = 80;
    WaterSupplyNetwork wsn;
};


#endif //DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
