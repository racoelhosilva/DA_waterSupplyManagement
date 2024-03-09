#include "WaterSupplyNetwork.h"
#include <fstream>
#include <sstream>
#include "Reservoir.h"
#include "PumpingStation.h"
#include "DeliverySite.h"
#include "Pipe.h"
#include <iostream>
#include <cassert>

using namespace std;

WaterSupplyNetwork::WaterSupplyNetwork() = default;

bool WaterSupplyNetwork::parseData(const string& reservoirPath, const string& stationsPath, const string& citiesPath, const string& pipesPath) {
    parseReservoir(reservoirPath);
    parseStations(stationsPath);
    parseCities(citiesPath);
    parsePipes(pipesPath);
    //TODO: display error message if any of these functions return false
    return true;
}

void trimString(string &str) {
    while (!str.empty() && (str.back() == '\r' || str.back() == ','))
        str.pop_back();
}

bool WaterSupplyNetwork::parseReservoir(const std::string& reservoirPath) {
    ifstream inputFile(reservoirPath);
    istringstream iss;
    string line, name, municipality, id, code, maxDelivery;

    if(!getline(inputFile, line)) {
        return false;
    }

    while(getline(inputFile, line)) {
        iss.clear();
        iss.str(line);
        getline(iss, name, ',');
        getline(iss, municipality, ',');
        getline(iss, id, ',');
        getline(iss, code, ',');
        getline(iss, maxDelivery, ',');
        trimString(maxDelivery);

        if (name.empty() || municipality.empty() || id.empty() || code.empty() || maxDelivery.empty())
            break;

        auto reservoir = new Reservoir(name, municipality, stoi(id), code, stod(maxDelivery));
        this->addVertex(reservoir);
    }

    return true;
}

bool WaterSupplyNetwork::parseStations(const std::string& stationsPath) {
    ifstream inputFile(stationsPath);
    istringstream iss;
    string line, id, code;

    if(!getline(inputFile, line)) {
        return false;
    }

    while(getline(inputFile, line)) {
        iss.clear();
        iss.str(line);
        getline(iss, id, ',');
        getline(iss, code);
        trimString(code);
        if (id.empty() || code.empty())
            break;

        auto pumpingStation = new PumpingStation(stoi(id), code);
        this->addVertex(pumpingStation);
    }

    return true;
}

bool WaterSupplyNetwork::parseCities(const std::string& citiesPath) {
    ifstream inputFile(citiesPath);
    istringstream iss;
    string line, city, id, code, demand, populationStr;

    if(!getline(inputFile, line)) {
        return false;
    }

    while(getline(inputFile, line)) {
        iss.clear();
        iss.str(line);
        getline(iss, city, ',');
        getline(iss, id, ',');
        getline(iss, code, ',');
        getline(iss, demand, ',');
        getline(iss, populationStr);
        trimString(populationStr);
        if (city.empty() || id.empty() || code.empty() || demand.empty() || populationStr.empty())
            break;

        int population;
        if (!populationStr.empty() && populationStr[0] == '"') {
            istringstream populationIss(populationStr.substr(1, populationStr.size() - 2));
            population = 0;
            string section;
            while (getline(populationIss, section, ','))
                population = population * 1000 + stoi(section);
        } else {
            population = stoi(populationStr);
        }

        auto deliverySite = new DeliverySite(city, stoi(id), code, stod(demand), population);
        this->addVertex(deliverySite);
    }

    return true;
}

bool WaterSupplyNetwork::parsePipes(const std::string& pipesPath) {
    ifstream inputFile(pipesPath);
    istringstream iss;
    string line, Service_Point_A, Service_Point_B, capacity, direction;

    if(!getline(inputFile, line)) {
        return false;
    }

    while(getline(inputFile, line)) {
        iss.clear();
        iss.str(line);
        getline(iss, Service_Point_A, ',');
        getline(iss, Service_Point_B, ',');
        getline(iss, capacity, ',');
        getline(iss, direction);
        trimString(direction);

        if (Service_Point_A.empty() || Service_Point_B.empty() || capacity.empty() || direction.empty())
            break;

        if (direction[0] == '0')
            addBidirectionalEdge(Service_Point_A, Service_Point_B, stod(capacity));
        else
            addEdge(Service_Point_A, Service_Point_B, stod(capacity));
    }

    return true;
}

void WaterSupplyNetwork::print(){
    for (auto v: getPumpingStations())
        std::cout << ' ' << v->getId() << ' ' << v->getCode() << endl;
    std::cout << getPumpingStations().size() << endl;
}

std::vector<Reservoir *> WaterSupplyNetwork::getReservoirs() {
    return filterVerticesByType<Reservoir>();
}

std::vector<PumpingStation *> WaterSupplyNetwork::getPumpingStations() {
    return filterVerticesByType<PumpingStation>();
}

std::vector<DeliverySite *> WaterSupplyNetwork::getDeliverySites() {
    return filterVerticesByType<DeliverySite>();
}

std::vector<ServicePoint *> WaterSupplyNetwork::getServicePoints() {
    return filterVerticesByType<ServicePoint>();
}

template<class T>
vector<T *> WaterSupplyNetwork::filterVerticesByType() {
    std::vector<T*> res;
    for (Vertex<string>* v: getVertexSet()) {
        auto r = dynamic_cast<T*>(v);
        if (r != nullptr)
            res.push_back(r);
    }
    return res;
}


