#include "PumpingStation.h"

PumpingStation::PumpingStation(int id, const std::string &code) : ServicePoint(id, code) {}

std::string PumpingStation::getDescription() const {
    return getCode();
}