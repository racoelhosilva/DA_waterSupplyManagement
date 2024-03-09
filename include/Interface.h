#ifndef DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
#define DA_WATERSUPPLYMANAGEMENT_INTERFACE_H

#include "WaterSupplyNetwork.h"

class Interface {
public:
    bool init();

private:
    WaterSupplyNetwork wsn;
};


#endif //DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
