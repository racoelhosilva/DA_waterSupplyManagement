#ifndef DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
#define DA_WATERSUPPLYMANAGEMENT_INTERFACE_H

#include "WaterSupplyNetwork.h"

class Interface {
public:

    bool init();

    static void printOptions(const std::vector<std::string> &options);
    bool validOption(unsigned long size, const std::string &choice);
    int readOption(int i);

    void mainMenu();

private:
    unsigned int width = 40;
    unsigned int height = 12;
    WaterSupplyNetwork wsn;
};


#endif //DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
