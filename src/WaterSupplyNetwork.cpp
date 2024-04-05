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
#include <cmath>

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
        pipe->setCapacity(theoretical ? numeric_limits<double>::infinity() : reservoir->getMaxDelivery());
    }
    for (Pipe *pipe: superSink->getIncoming()) {
        auto *city = dynamic_cast<DeliverySite*>(pipe->getOrig());
        pipe->setCapacity(theoretical ? numeric_limits<double>::infinity() : city->getDemand());
    }
    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj()) {
            p->setFlow(0);
            p->getAugmentingPaths().clear();
        }
    }

    edmondsKarp(superSource, superSink, true);

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

    edmondsKarp(superSource, superSink);

    double maxFlow = 0;
    for (Pipe *p: superSink->getIncoming()) {
        maxFlow += p->getFlow();
    }

    return maxFlow;
}

void WaterSupplyNetwork::createSuperSourceAndSuperSink(bool createPipes) {
    if (superSource != nullptr && superSink != nullptr)
        return;
    superSource = new ServicePoint(0, "__super_source__");
    superSink = new ServicePoint(0, "__super_sink__");
    addVertex(superSource);
    addVertex(superSink);

    if (!createPipes)
        return;
    for (Reservoir *r: getReservoirs()) {
        if (r->getCode() != superSource->getCode())
            addEdge(superSource->getInfo(), r->getInfo(), numeric_limits<double>::infinity());
    }
    for (DeliverySite *d: getDeliverySites()) {
        if (d->getCode() != superSink->getCode())
            addEdge(d->getInfo(), superSink->getInfo(), numeric_limits<double>::infinity());
    }
}

void WaterSupplyNetwork::edmondsKarp(ServicePoint *source, ServicePoint *sink, bool savePaths) {
    if (source->getCode() == sink->getCode())
        return;

    while (true) {
        edmondsKarpBfs(source);
        if (!sink->isVisited())
            break;

        AugmentingPath augmentingPath = reduceAugmentingPath(source, sink);
        if (savePaths) {
            augmentingPaths.push_back(augmentingPath);
            for (auto pair: augmentingPaths.back().getPipes()) {
                Pipe *pipe = pair.first;
                pipe->getAugmentingPaths().push_back(&augmentingPaths.back());
                if (pipe->getReverse() != nullptr)
                    pipe->getReverse()->getAugmentingPaths().push_back(&augmentingPaths.back());
            }
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

void WaterSupplyNetwork::subtractAugmentingPath(const AugmentingPath& augmentingPath, double maxToRemove) {
    for (auto pair: augmentingPath.getPipes()) {
        Pipe *p = pair.first;
        bool incoming = pair.second;
        double flowToRemove = min(maxToRemove, augmentingPath.getCapacity());
        p->setFlow(incoming ? p->getFlow() - flowToRemove : p->getFlow() + flowToRemove);
        if (p->getReverse() != nullptr) {
            p->getReverse()->setFlow(incoming ? p->getReverse()->getFlow() + flowToRemove :
                                     p->getReverse()->getFlow() - flowToRemove);
        } else if (p->getFlow() < 0) {
            p->selectAugmentingPaths();
        }
    }
}

void WaterSupplyNetwork::subtractAugmentingPaths() {
    for (const AugmentingPath &augmentingPath: augmentingPaths) {
        if (!augmentingPath.isSelected())
            continue;
        subtractAugmentingPath(augmentingPath, numeric_limits<double>::infinity());
    }
}

void WaterSupplyNetwork::unselectAllAugmentingPaths() {
    for (AugmentingPath &augmentingPath: augmentingPaths)
        augmentingPath.setSelected(false);
}

void WaterSupplyNetwork::unhideAllServicePoints() {
    for (ServicePoint *sp: getServicePoints())
        sp->setHidden(false);
}

void WaterSupplyNetwork::unhideAllPipes() {
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj())
            pipe->setHidden(false);
    }
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
    network2->createSuperSourceAndSuperSink(false);

    for (ServicePoint *sp: network1->getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            if (pipe->getReverse() != nullptr && network2->findPipe(sp->getCode(), pipe->getDest()->getCode()) == nullptr) {
                network2->addBidirectionalEdge(sp->getCode(), pipe->getDest()->getCode(), pipe->getCapacity());
            } else if (pipe->getReverse() == nullptr) {
                network2->addEdge(sp->getCode(), pipe->getDest()->getCode(), pipe->getCapacity());
            }
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

double WaterSupplyNetwork::getMaxFlowWithoutPipes(std::vector<Pipe *> pipes) {
    unhideAllPipes();
    getMaxFlow();

    for (Pipe *pipe: pipes)
        pipe->selectAugmentingPaths();
    subtractAugmentingPaths();
    return recalculateMaxFlow();
}

double WaterSupplyNetwork::getMaxFlowWithoutReservoir(Reservoir *reservoir) {
    return getMaxFlowWithoutPipes(reservoir->getAdj());
}

double WaterSupplyNetwork::getMaxFlowWithoutStation(PumpingStation *station) {
    return getMaxFlowWithoutPipes(station->getAdj());
}

std::vector<Pipe *> WaterSupplyNetwork::getCriticalPipesToCity(DeliverySite *city) {
    unhideAllPipes();
    getMaxFlow();
    storeNetwork();

    std::vector<Pipe *> possiblePipes, res;

    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            pipe->setSelected(false);
        }
    }
    for (Pipe *pipe: city->getIncoming())
        for (AugmentingPath *path: pipe->getAugmentingPaths())
            for (auto p: path->getPipes())
                p.first->setSelected(true);
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj())
            possiblePipes.push_back(pipe);
    }

    for (Pipe *pipe: possiblePipes) {
        loadNetwork();
        unselectAllAugmentingPaths();
        unhideAllPipes();
        pipe->selectAugmentingPaths();
        subtractAugmentingPaths();
        pipe->setHidden(true);
        recalculateMaxFlow();
        if (city->getSupplyRate() < auxNetwork->findDeliverySite(city->getCode())->getSupplyRate())
            res.push_back(pipe);
    }
}

// TODO: Remove this function
void WaterSupplyNetwork::print() {
    cout << getMaxFlow() << '\n';
    storeNetwork();
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj())
            cout << sp->getCode() << " " << pipe->getDest()->getCode() << " " << pipe->getFlow() << " " << auxNetwork->findPipe(sp->getCode(), pipe->getDest()->getCode())->getFlow() << '\n';
    }
    loadNetwork();
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj())
            cout << sp->getCode() << " " << pipe->getDest()->getCode() << " " << pipe->getFlow() << " " << auxNetwork->findPipe(sp->getCode(), pipe->getDest()->getCode())->getFlow() << '\n';
    }

    unordered_map<string, unordered_map<string, double>> flows;
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj())
            flows.insert({ pipe->getOrig()->getCode() + " " + pipe->getDest()->getCode(), unordered_map<string, double>() });
    }


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
                        auxNetwork->findDeliverySite(ds->getCode())->getSupplyRate() - ds->getSupplyRate();
            }
        }
    }

    getMaxFlow();
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            loadNetwork();
            unselectAllAugmentingPaths();
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
                if (auxNetwork->findDeliverySite(ds->getCode())->getSupplyRate() - ds->getSupplyRate() != flows[pipe->getOrig()->getCode() + " " + pipe->getDest()->getCode()][ds->getCode()])
                    cout << "Error: " << pipe->getOrig()->getCode() << " " << pipe->getDest()->getCode() << " " << ds->getCity() << " " << auxNetwork->findDeliverySite(ds->getCode())->getSupplyRate() - ds->getSupplyRate() << " != " << flows[pipe->getOrig()->getCode() + " " + pipe->getDest()->getCode()][ds->getCode()] << '\n';
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

    unhideAllPipes();

    for(ServicePoint *v: getServicePoints()) {
        for(Pipe *p: v->getAdj()) {
            // Hide Super-source or Super-sink
            if (p->getDest()->getId() == 0 || p->getOrig()->getId() == 0) {p->setHidden(true); continue;}
            // Hide negative flows
            if (p->getFlow() < 0) {p->setHidden(true); continue;}

            // Hide 0 flow bidirectional
            if (p->getReverse() != nullptr){
                if (p->getReverse()->getFlow() == 0 && p->getFlow() == 0){
                    if (p->getReverse()->isHidden() && !p->isHidden()) continue;
                    if (!p->getReverse()->isHidden() && p->isHidden()) continue;
                    p->setHidden(true);
                }
            }
        }
    }

    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj()) {
            if(p->isHidden()) continue;
            differences.push_back(p->getCapacity() - p->getFlow());
        }
    }

    compute_metrics(differences, max, mean, variance);

    unhideAllPipes();
}

void WaterSupplyNetwork::balance(double mean) {
    double initFlow = getMaxFlow();
    bool found = true;
    while (found) {
        found = false;
        storeNetwork();
        vector<Pipe *> pipes;

        for(ServicePoint *sp: getServicePoints()) {
            for (Pipe *p: sp->getAdj()) {
                if(p->getFlow() > 0 && p->getCapacity() > 0 && p->getRemainingFlow() <= mean && !(*p->getOrig() == *superSource) && !(*p->getDest() == *superSink))
                    pipes.push_back(p);
            }
        }
        if (pipes.empty())
            break;

        sort(pipes.begin(), pipes.end(), [&](Pipe *a, Pipe *b){ return (a->getRemainingFlow() < b->getRemainingFlow() || (a->getRemainingFlow() == b->getRemainingFlow() && a->getCapacity() > b->getCapacity())); });

        Pipe* targetPipe = pipes[0];
        for (Pipe *targetPipe: pipes) {
            double originalCapacity = targetPipe->getCapacity();
            targetPipe->setCapacity(max(originalCapacity - floor(mean), 0.0));
            if (targetPipe->getReverse() != nullptr)
                targetPipe->setCapacity(max(originalCapacity - floor(mean), 0.0));
            double finalFlow = getMaxFlow();

            if (finalFlow < initFlow) {
                loadNetwork();
                targetPipe->setCapacity(originalCapacity);
            } else {
                found = true;
                break;
            }
        }
    }

    for (ServicePoint *sp: auxNetwork->getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            findPipe(sp->getCode(), pipe->getDest()->getCode())->setCapacity(pipe->getCapacity());
        }
    }
}