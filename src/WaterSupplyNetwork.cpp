#include "WaterSupplyNetwork.h"
#include <fstream>
#include <sstream>
#include "Reservoir.h"
#include "PumpingStation.h"
#include "DeliverySite.h"
#include "Pipe.h"
#include <iostream>
#include <limits>
#include <queue>

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

ServicePoint *WaterSupplyNetwork::findServicePoint(const std::string &code) {
    return dynamic_cast<ServicePoint*>(findVertex(code));
}

DeliverySite *WaterSupplyNetwork::findDeliverySite(const std::string &code) {
    return dynamic_cast<DeliverySite*>(findVertex(code));
}

Reservoir *WaterSupplyNetwork::findReservoir(const std::string &code) {
    return dynamic_cast<Reservoir*>(findVertex(code));
}

PumpingStation *WaterSupplyNetwork::findPumpingStation(const std::string &code) {
    return dynamic_cast<PumpingStation*>(findVertex(code));
}

Pipe *WaterSupplyNetwork::findPipe(const std::string &src, const std::string &dest) {
    ServicePoint *srcSp = findServicePoint(src);
    if (srcSp == nullptr)
        return nullptr;
    for (Pipe* pipe: srcSp->getAdj()) {
        if (pipe->getDest()->getCode() == dest)
            return pipe;
    }
    return nullptr;
}

double WaterSupplyNetwork::getMaxFlow(bool theoretical) {
    ServicePoint *superSource = new Reservoir("", "", 0, "__super_source__", numeric_limits<double>::infinity()),
        *superSink = new DeliverySite("", 0, "__super_sink__", numeric_limits<double>::infinity(), 0);
    addVertex(superSource);
    addVertex(superSink);

    for (Reservoir *r: getReservoirs()) {
        if (r->getCode() != superSource->getCode())
            addEdge(superSource->getInfo(), r->getInfo(), theoretical ? numeric_limits<double>::infinity() : r->getMaxDelivery());
    }
    for (DeliverySite *d: getDeliverySites()) {
        if (d->getCode() != superSink->getCode())
            addEdge(d->getInfo(), superSink->getInfo(), theoretical ? numeric_limits<double>::infinity(): d->getDemand());
    }

    edmondsKarp(superSource, superSink);

    double maxFlow = 0;
    for (Pipe *p: superSink->getIncoming()) {
        maxFlow += p->getFlow();
    }

    removeVertex(superSource->getCode());
    removeVertex(superSink->getCode());

    return maxFlow;
}

void WaterSupplyNetwork::edmondsKarp(ServicePoint *source, ServicePoint *sink) {
    if (source->getCode() == sink->getCode())
        return;

    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj())
            p->setFlow(0);
    }

    while (true) {
        edmondsKarpBfs(source);
        if (!sink->isVisited())
            break;

        reduceAugmentingPath(source, sink);
    }
}

void WaterSupplyNetwork::edmondsKarpBfs(ServicePoint *srcSp) {
    for (ServicePoint* sp: getServicePoints()) {
        sp->setVisited(false);
        sp->setPath(nullptr);
    }

    queue<ServicePoint*> spQueue;
    spQueue.push(srcSp);
    srcSp->setVisited(true);

    ServicePoint *u, *v;
    while (!spQueue.empty()) {
        u = spQueue.front();
        spQueue.pop();

        for (Pipe *p: u->getAdj()) {
            if (p->isHidden() || p->getRemainingFlow() == 0)
                continue;
            v = p->getDest();
            if (v->isHidden())
                continue;
            if (!v->isVisited()) {
                v->setVisited(true);
                v->setPath(p);
                spQueue.push(v);
            }
        }

        for (Pipe *p: u->getIncoming()) {
            if (p->isHidden() || p->getFlow() == 0)
                continue;
            v = p->getOrig();
            if (v->isHidden())
                continue;
            if (!v->isVisited()) {
                v->setVisited(true);
                v->setPath(p);
                spQueue.push(v);
            }
        }
    }
}

void WaterSupplyNetwork::reduceAugmentingPath(ServicePoint *source, ServicePoint *sink) {
    double pathCapacity = numeric_limits<double>::infinity();
    ServicePoint *sp = sink;
    while (sp->getCode() != source->getCode()) {
        Pipe *path = sp->getPath();
        bool incoming = sp->getInfo() == path->getDest()->getInfo();
        pathCapacity = min(pathCapacity, incoming ? path->getRemainingFlow() : path->getFlow());
        sp = incoming ? path->getOrig() : path->getDest();
    }

    sp = sink;
    while (sp->getCode() != source->getCode()) {
        Pipe *path = sp->getPath();
        bool incoming = sp->getInfo() == path->getDest()->getInfo();
        path->setFlow(incoming ? path->getFlow() + pathCapacity : path->getFlow() - pathCapacity);
        sp = incoming ? path->getOrig() : path->getDest();
    }
}

void WaterSupplyNetwork::unhideAll() {
    for (ServicePoint *sp: getServicePoints())
        sp->setHidden(false);
}

void WaterSupplyNetwork::hideAllButOneDeliverySite(const string &code) {
    for (DeliverySite *ds: getDeliverySites()) {
        ds->setHidden(ds->getCode() != code);
    }
}

void WaterSupplyNetwork::hideReservoir(const string &code) {
    for (Reservoir *r: getReservoirs()) {
        r->setHidden(r->getCode() == code);
    }
}

void WaterSupplyNetwork::hidePumpingStation(const std::string &code) {
    for (PumpingStation *p: getPumpingStations()) {
        p->setHidden(p->getCode() == code);
    }
}

void WaterSupplyNetwork::hideServicePoint(const string &code) {
    for (ServicePoint *sp: getServicePoints()) {
        sp->setHidden(sp->getCode() == code);
    }
}

void WaterSupplyNetwork::hidePipe(Pipe *pipe) {
    pipe->setHidden(true);
}

void WaterSupplyNetwork::unhidePipe(Pipe *pipe) {
    pipe->setHidden(false);
}


// TODO: Remove this function
void WaterSupplyNetwork::print() {
    hideAllButOneDeliverySite("C_17");
    std::cout << "Max flow: " << getMaxFlow(false) << "\n\n";
    std::cout << "Reservoirs:\n";

    for (Reservoir* reservoir: getReservoirs())
        std::cout << reservoir->getName() << ' ' << reservoir->getDelivery() << '/' << reservoir->getMaxDelivery() << '\n';

    std::cout << "\nCities:\n";

    for (DeliverySite* site: getDeliverySites())
        std::cout << site->getCity() << ' ' << site->getSupplyRate() << '/' << site->getDemand() << '\n';
}

void compute_metrics(const vector<double> &v, double &max, double &mean, double &variance) {
    max = 0, mean = 0, variance = 0;

    // Compute max and mean
    for (double value: v) {
        mean += value;
        if(value > max) max = value;
    }
    mean /= (double)v.size();

    // Compute variance
    for(double value: v) {
        variance += (value - mean) * (value - mean);
    }
    variance /= (double)v.size();
}
