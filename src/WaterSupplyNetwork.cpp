#include "WaterSupplyNetwork.h"
#include <fstream>
#include <sstream>
#include "Reservoir.h"
#include "PumpingStation.h"
#include "DeliverySite.h"
#include "Pipe.h"
#include "AugmentingPath.h"
#include <iostream>
#include <limits>
#include <queue>
#include <algorithm>

using namespace std;

WaterSupplyNetwork::WaterSupplyNetwork() : auxNetwork(nullptr), superSource(nullptr), superSink(nullptr) {};

WaterSupplyNetwork::~WaterSupplyNetwork() {
    delete auxNetwork;
}

bool WaterSupplyNetwork::parseData(const string& reservoirPath, const string& stationsPath, const string& citiesPath, const string& pipesPath) {
    parseReservoir(reservoirPath);
    parseStations(stationsPath);
    parseCities(citiesPath);
    parsePipes(pipesPath);
    //TODO: display error message if any of these functions return false
    createSuperSourceAndSuperSink();
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
    std::sort(res.begin(), res.end(), [](T* a, T* b) {return a->getCode() < b->getCode();});
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
    augmentingPaths.clear();
    for (Pipe *pipe: superSource->getAdj()) {
        auto *reservoir = dynamic_cast<Reservoir*>(pipe->getDest());
        pipe->setCapacity(/*theoretical ? numeric_limits<double>::infinity() : */reservoir->getMaxDelivery());
    }
    for (Pipe *pipe: superSink->getIncoming()) {
        auto *city = dynamic_cast<DeliverySite*>(pipe->getOrig());
        pipe->setCapacity(/*theoretical ? numeric_limits<double>::infinity() : */theoretical ? auxNetwork->findPipe(city->getCode(), superSink->getCode())->getFlow() : city->getDemand());
    }
    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj()) {
            p->setSelected(true);
            p->setFlow(0);
            p->getAugmentingPaths().clear();
        }
    }

    edmondsKarp(superSource, superSink);

    double maxFlow = 0;
    for (Pipe *p: superSink->getIncoming()) {
        maxFlow += p->getFlow();
    }

    return maxFlow;
}

double WaterSupplyNetwork::calculateMaxFlowAndAugmentingPathsThroughPipe(Pipe *pipe) {
    for (Pipe *p: superSource->getAdj()) {
        auto *reservoir = dynamic_cast<Reservoir*>(p->getDest());
        p->setWeight(reservoir->getMaxDelivery());
    }
    for (Pipe *p: superSink->getIncoming()) {
        auto *city = dynamic_cast<DeliverySite*>(p->getOrig());
        p->setWeight(city->getDemand());
    }
    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj()) {
            p->setSelected(*p == *pipe);
            p->setFlow(0);
        }
    }

    edmondsKarp(superSource, superSink);

    double maxFlow = 0;
    for (Pipe *p: superSink->getIncoming()) {
        maxFlow += p->getFlow();
    }

    return maxFlow;
}

double WaterSupplyNetwork::calculateMaxFlowAndAugmentingPathsToCity(DeliverySite *city) {
    for (Pipe *p: superSource->getAdj()) {
        auto *reservoir = dynamic_cast<Reservoir*>(p->getDest());
        p->setWeight(reservoir->getMaxDelivery());
    }
    for (Pipe *p: superSink->getIncoming()) {
        auto *city = dynamic_cast<DeliverySite*>(p->getOrig());
        p->setWeight(city->getDemand());
    }
    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj()) {
            p->setSelected(*v == *city);
            p->setFlow(0);
        }
    }

    edmondsKarp(superSource, superSink);

    double maxFlow = 0;
    for (Pipe *p: superSink->getIncoming()) {
        maxFlow += p->getFlow();
    }

    return maxFlow;
}

double WaterSupplyNetwork::recalculateMaxFlow() {
    for (Pipe *p: superSource->getAdj()) {
        auto *reservoir = dynamic_cast<Reservoir*>(p->getDest());
        p->setCapacity(reservoir->getMaxDelivery());
    }
    for (Pipe *p: superSink->getIncoming()) {
        auto *city = dynamic_cast<DeliverySite*>(p->getOrig());
        p->setCapacity(auxNetwork->findPipe(city->getCode(), superSink->getCode())->getFlow());
    }
    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj())
            p->setSelected(false);
    }

    edmondsKarp(superSource, superSink);

    double maxFlow = 0;
    for (Pipe *p: superSink->getIncoming()) {
        maxFlow += p->getFlow();
    }

    return maxFlow;
}

void WaterSupplyNetwork::createSuperSourceAndSuperSink() {
    if (superSource != nullptr && superSink != nullptr)
        return;
    superSource = new ServicePoint(0, "__super_source__");
    superSink = new ServicePoint(0, "__super_sink__");
    addVertex(superSource);
    addVertex(superSink);

    for (Reservoir *r: getReservoirs()) {
        if (r->getCode() != superSource->getCode())
            addEdge(superSource->getInfo(), r->getInfo(), numeric_limits<double>::infinity());
    }
    for (DeliverySite *d: getDeliverySites()) {
        if (d->getCode() != superSink->getCode())
            addEdge(d->getInfo(), superSink->getInfo(), numeric_limits<double>::infinity());
    }
}

void WaterSupplyNetwork::edmondsKarp(ServicePoint *source, ServicePoint *sink) {
    if (source->getCode() == sink->getCode())
        return;

    while (true) {
        edmondsKarpBfs(source);
        if (!sink->isVisited())
            break;

        augmentingPaths.push_back(reduceAugmentingPath(source, sink));
        for (auto pair: augmentingPaths.back().getPipes()) {
            Pipe *pipe = pair.first;
            pipe->getAugmentingPaths().push_back(&augmentingPaths.back());
            if (pipe->getReverse() != nullptr)
                pipe->getReverse()->getAugmentingPaths().push_back(&augmentingPaths.back());
        }
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
            if (p->isHidden() || p->getRemainingFlow() <= 0)
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
            if (p->isHidden() || p->getFlow() <= 0)
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

AugmentingPath WaterSupplyNetwork::reduceAugmentingPath(ServicePoint *source, ServicePoint *sink) {
    ServicePoint *sp = sink;
    AugmentingPath augmentingPath;

    while (sp->getCode() != source->getCode()) {
        Pipe *path = sp->getPath();
        bool incoming = *sp == *path->getDest();
//        if (path->getOrig() != source && path->getDest() != sink)
        augmentingPath.addPipe(path, incoming);
        sp = incoming ? path->getOrig() : path->getDest();
    }

    for (auto pair: augmentingPath.getPipes()) {
        Pipe *pipe = pair.first;
        bool incoming = pair.second;
        pipe->setFlow(incoming ? pipe->getFlow() + augmentingPath.getCapacity() : pipe->getFlow() - augmentingPath.getCapacity());
        if (pipe->getReverse() != nullptr) {
            pipe->getReverse()->setFlow(incoming ? pipe->getReverse()->getFlow() - augmentingPath.getCapacity() :
                                        pipe->getReverse()->getFlow() + augmentingPath.getCapacity());
        }
    }

    return augmentingPath;
}

void WaterSupplyNetwork::subtractAugmentingPaths() {
    for (const AugmentingPath &augmentingPath: augmentingPaths) {
        if (!augmentingPath.isSelected())
            continue;
        for (auto pair: augmentingPath.getPipes()) {
            Pipe *p = pair.first;
            bool incoming = pair.second;
            p->setFlow(incoming ? p->getFlow() - augmentingPath.getCapacity() : p->getFlow() + augmentingPath.getCapacity());
            if (p->getReverse() != nullptr) {
                p->getReverse()->setFlow(incoming ? p->getReverse()->getFlow() + augmentingPath.getCapacity() :
                                         p->getReverse()->getFlow() - augmentingPath.getCapacity());
            } else if (p->getFlow() < 0) {
                p->selectAugmentingPaths();
            }
        }
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

void WaterSupplyNetwork::destroySuperSourceAndSuperSink() {
//    removeVertex("__super_source__");
//    removeVertex("__super_sink__");
}

void WaterSupplyNetwork::copyGraph(WaterSupplyNetwork *network1, WaterSupplyNetwork *network2) {
    VertexSet<string> vertexes2 = network2->getVertexSet();
    for (Vertex<string> *v: vertexes2)
        network2->removeVertex(v->getInfo());

    for (Reservoir *reservoir: network1->getReservoirs()) {
        auto *newReservoir = new Reservoir(reservoir->getName(), reservoir->getMunicipality(), reservoir->getId(), reservoir->getCode(), reservoir->getMaxDelivery());
        network2->addVertex(newReservoir);
    }
    for (PumpingStation *pumpingStation: network1->getPumpingStations()) {
        auto *newPumpingStation = new PumpingStation(pumpingStation->getId(), pumpingStation->getCode());
        network2->addVertex(newPumpingStation);
    }
    for (DeliverySite *deliverySite: network1->getDeliverySites()) {
        auto *newDeliverySite = new DeliverySite(deliverySite->getCity(), deliverySite->getId(), deliverySite->getCode(), deliverySite->getDemand(), deliverySite->getPopulation());
        network2->addVertex(newDeliverySite);
    }
    network2->createSuperSourceAndSuperSink();

    for (ServicePoint *sp: network1->getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            if (pipe->getReverse() != nullptr && network2->findPipe(sp->getCode(), pipe->getDest()->getCode()) == nullptr)
                network2->addBidirectionalEdge(sp->getCode(), pipe->getDest()->getCode(), pipe->getCapacity());
            else if (pipe->getReverse() == nullptr)
                network2->addEdge(sp->getCode(), pipe->getDest()->getCode(), pipe->getCapacity());
            network2->findPipe(sp->getCode(), pipe->getDest()->getCode())->setFlow(pipe->getFlow());
        }
    }
}

void WaterSupplyNetwork::copyFlows(WaterSupplyNetwork *network1, WaterSupplyNetwork *network2) {
    for (ServicePoint *sp1: network1->getServicePoints()) {
        for (Pipe *pipe1: sp1->getAdj()) {
            Pipe *pipe2 = network2->findPipe(sp1->getCode(), pipe1->getDest()->getCode());
            pipe2->setFlow(pipe1->getFlow());
        }
    }
}

void WaterSupplyNetwork::storeNetwork() {
    if (auxNetwork == nullptr) {
        auxNetwork = new WaterSupplyNetwork();
        copyGraph(this, auxNetwork);
    } else {
        copyFlows(this, auxNetwork);
    }
}

void WaterSupplyNetwork::loadNetwork() {
    if (auxNetwork == nullptr)
        return;
    copyFlows(auxNetwork, this);
}


// TODO: Remove this function
void WaterSupplyNetwork::print() {
    cout << getMaxFlow() << '\n';
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj())
            cout << sp->getCode() << " " << pipe->getDest()->getCode() << " " << pipe->getFlow() << '\n';
    }

    storeNetwork();
    unordered_map<string, unordered_map<string, double>> flows;
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj())
            flows.insert({ pipe->getOrig()->getCode() + " " + pipe->getDest()->getCode(), unordered_map<string, double>() });
    }

//    for (ServicePoint *sp: getServicePoints()) {
//        for (Pipe *pipe: sp->getAdj()) {
//            cout << sp->getCode() << " " << pipe->getDest()->getCode() << " " << pipe->getFlow() << " " << auxNetwork->findPipe(sp->getCode(), pipe->getDest()->getCode())->getFlow() << '\n';
//        }
//    }

    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            pipe->setHidden(true);
            if (pipe->getReverse() != nullptr)
                pipe->getReverse()->setHidden(true);
            getMaxFlow(true);
            pipe->setHidden(false);
            if (pipe->getReverse() != nullptr)
                pipe->getReverse()->setHidden(false);
            for (DeliverySite *ds: getDeliverySites()) {
                flows[pipe->getOrig()->getCode() + " " + pipe->getDest()->getCode()][ds->getCode()] =
                        auxNetwork->findDeliverySite(ds->getCode())->getDemand() - ds->getSupplyRate();
            }
        }
    }

    cout << "Hello\n";

    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            getMaxFlow(false);

            double temp = pipe->getFlow();
            pipe->selectAugmentingPaths();
            subtractAugmentingPaths();
            pipe->setHidden(true);
            if (pipe->getReverse() != nullptr)
                pipe->getReverse()->setHidden(true);
            recalculateMaxFlow();
            pipe->setHidden(false);

            if (pipe->getReverse() != nullptr)
                pipe->getReverse()->setHidden(false);
            for (DeliverySite *ds: getDeliverySites()) {
                if (auxNetwork->findDeliverySite(ds->getCode())->getDemand() - ds->getSupplyRate() != flows[pipe->getOrig()->getCode() + " " + pipe->getDest()->getCode()][ds->getCode()])
                    cout << "Error: " << pipe->getOrig()->getCode() << " " << pipe->getDest()->getCode() << " " << ds->getCity() << " " << auxNetwork->findDeliverySite(ds->getCode())->getDemand() - ds->getSupplyRate() << " != " << flows[pipe->getOrig()->getCode() + " " + pipe->getDest()->getCode()][ds->getCode()] << '\n';
            }
        }
    }
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

// TODO: Fix this to skip super source and super sink
void WaterSupplyNetwork::getMetrics(double &max, double &mean, double &variance){
    vector<double> differences;

    for(ServicePoint *v: getServicePoints()) {
        for(Pipe *p: v->getAdj()) {
            p->setHidden(p->getReverse() != nullptr && p->getFlow() <= 0 && !(p->getReverse()->isHidden()));
        }
    }

    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj()) {
            if(p->isHidden()) continue;
            differences.push_back(p->getCapacity() - p->getFlow());
        }
    }

    compute_metrics(differences, max, mean, variance);

    // Reset the edges back to normal
    for(ServicePoint *v: getServicePoints()) {
        for(Pipe *p: v->getAdj()) {
            p->setHidden(false);
        }
    }
}
