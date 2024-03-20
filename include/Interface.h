#ifndef DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
#define DA_WATERSUPPLYMANAGEMENT_INTERFACE_H

#include "WaterSupplyNetwork.h"

class Interface {
public:

    bool init();

    void printLine(const std::string &s);
    void printOptions(const std::vector<std::string> &options);
    void printOption();
    void printTop();
    void printBottom();
    bool validOption(unsigned long size, const std::string &choice);
    int readOption(int i);

    void mainMenu();
    void exitMenu();

private:
    int width = 80;
    WaterSupplyNetwork wsn;
};


#endif //DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
