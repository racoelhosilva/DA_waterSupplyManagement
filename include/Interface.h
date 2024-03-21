#ifndef DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
#define DA_WATERSUPPLYMANAGEMENT_INTERFACE_H

#include "WaterSupplyNetwork.h"

class Interface {
public:

    bool init();

    void printOptions(const std::vector<std::string> &options, int choice);
    void printOptionsCity(const std::vector<std::string> &options, int choice);
    void printTop();
    void printBottom();

    void mainMenu();
    void exitMenu();

    void citySelection(); // This function is currently just a test for a future feature

private:
    int width = 80;
    WaterSupplyNetwork wsn;
};


#endif //DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
