#include "WaterSupplyNetwork.h"
#include <fstream>
#include <sstream>
#include "Reservoir.h"
#include "PumpingStation.h"
#include "DeliverySite.h"
#include "Pipe.h"
#include "AugmentingPath.h"
#include <iostream>
#include <queue>
#include <algorithm>
#include <cmath>

using namespace std;

WaterSupplyNetwork::WaterSupplyNetwork() : maxFlowNetwork(nullptr), superSource(nullptr), superSink(nullptr) {};

WaterSupplyNetwork::~WaterSupplyNetwork() {
    delete maxFlowNetwork;
}

bool WaterSupplyNetwork::parseData(const string& reservoirPath, const string& stationsPath, const string& citiesPath, const string& pipesPath) {
    if(!parseReservoir(reservoirPath)) {
        cout << "ERROR READING RESERVOIRS\n";
        return false;
    }
    if(!parseStations(stationsPath)) {
        cout << "ERROR READING PUMPING STATIONS\n";
        return false;
    }
    if(!parseCities(citiesPath)) {
        cout << "ERROR READING CITIES\n";
        return false;
    }
    if(!parsePipes(pipesPath)) {
        cout << "ERROR READING PIPES\n";
        return false;
    }
    createSuperSourceAndSuperSink(true);
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

double WaterSupplyNetwork::getMaxFlow(bool saveAugmentingPaths) {
    if (saveAugmentingPaths)
        augmentingPaths.clear();
    for (ServicePoint *v: getServicePoints()) {
        for (Pipe *p: v->getAdj()) {
            p->setFlow(0);
            if (saveAugmentingPaths)
                p->getAugmentingPaths().clear();
        }
    }

    edmondsKarp(superSource, superSink, saveAugmentingPaths);

    double maxFlow = 0;
    for (Pipe *p: superSink->getIncoming()) {
        maxFlow += p->getFlow();
    }

    return maxFlow;
}

double WaterSupplyNetwork::loadCachedMaxFlow() {
    double maxFlow = 0;
    if (maxFlowNetwork == nullptr) {
        unhideAllPipes();
        unhideAllServicePoints();
        maxFlow = getMaxFlow(true);
        maxFlowNetwork = new WaterSupplyNetwork();
        copyGraph(this, maxFlowNetwork);
        return maxFlow;
    }

    copyFlows(maxFlowNetwork, this);

    for (Pipe *p: superSink->getIncoming()) {
        maxFlow += p->getFlow();
    }

    return maxFlow;
}

double WaterSupplyNetwork::recalculateMaxFlow() {
    edmondsKarp(superSource, superSink, false);

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
    for (Reservoir *reservoir: getReservoirs())
        addEdge(superSource->getInfo(), reservoir->getInfo(), reservoir->getMaxDelivery());
    for (DeliverySite *city: getDeliverySites())
        addEdge(city->getInfo(), superSink->getInfo(), city->getDemand());
}

void WaterSupplyNetwork::edmondsKarp(ServicePoint *source, ServicePoint *sink, bool savePaths) {
    if (source->getCode() == sink->getCode())
        return;

    while (true) {
        edmondsKarpBfs(source, sink);
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

void WaterSupplyNetwork::edmondsKarpBfs(ServicePoint *srcSp, ServicePoint *sinkSp) {
    for (ServicePoint* sp: getServicePoints()) {
        sp->setVisited(false);
        sp->setPath(nullptr);
    }

    queue<ServicePoint*> spQueue;
    spQueue.push(srcSp);
    srcSp->setVisited(true);

    ServicePoint *u, *v;
    while (!spQueue.empty() && !sinkSp->isVisited()) {
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

void WaterSupplyNetwork::subtractAugmentingPath(const AugmentingPath& augmentingPath) {
    for (auto pair: augmentingPath.getPipes()) {
        Pipe *p = pair.first;
        bool incoming = pair.second;
        double flowToRemove = augmentingPath.getCapacity();
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
        subtractAugmentingPath(augmentingPath);
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
            Pipe *pipe2 = network2->findPipe(pipe1->getOrig()->getCode(), pipe1->getDest()->getCode());
            pipe2->setFlow(pipe1->getFlow());
        }
    }
}

void WaterSupplyNetwork::storeNetwork() {
    if (maxFlowNetwork == nullptr) {
        maxFlowNetwork = new WaterSupplyNetwork();
        copyGraph(this, maxFlowNetwork);
    } else {
        copyFlows(this, maxFlowNetwork);
    }
}

void WaterSupplyNetwork::loadNetwork() {
    if (maxFlowNetwork == nullptr)
        return;
    copyFlows(maxFlowNetwork, this);
}

double WaterSupplyNetwork::getMaxFlowWithoutPipes(const std::vector<Pipe *> &pipes) {
    unhideAllPipes();
    getMaxFlow(true);
    unselectAllAugmentingPaths();

    for (Pipe *pipe: pipes) {
        pipe->selectAugmentingPaths();
        pipe->setHidden(true);
        if (pipe->getReverse() != nullptr)
            pipe->getReverse()->setHidden(true);
    }

    subtractAugmentingPaths();
    return recalculateMaxFlow();
}

double WaterSupplyNetwork::getMaxFlowWithoutReservoir(Reservoir *reservoir) {
    vector<Pipe*> pipes;
    for (Pipe *pipe: reservoir->getAdj())
        pipes.push_back(pipe);
    for (Pipe *pipe: reservoir->getIncoming())
        pipes.push_back(pipe);
    return getMaxFlowWithoutPipes(pipes);
}

double WaterSupplyNetwork::getMaxFlowWithoutStation(PumpingStation *station) {
    return getMaxFlowWithoutPipes(station->getAdj());
}

double WaterSupplyNetwork::getMaxFlowWithoutPipesBF(const std::vector<Pipe *> &pipes) {
    unhideAllPipes();
    for (auto p : pipes){
        p->setHidden(true);
        if (p->getReverse() != nullptr){
            p->getReverse()->setHidden(true);
        }
    }
    return getMaxFlow();
}

double WaterSupplyNetwork::getMaxFlowWithoutReservoirBF(Reservoir* reservoir) {
    unhideAllServicePoints();
    reservoir->setHidden(true);
    return getMaxFlow();
}

double WaterSupplyNetwork::getMaxFlowWithoutStationBF(PumpingStation* station) {
    unhideAllServicePoints();
    station->setHidden(true);
    return getMaxFlow();
}

std::vector<Pipe *> WaterSupplyNetwork::getCriticalPipesToCity(DeliverySite *city) {
    unhideAllPipes();
    getMaxFlow(true);

    std::vector<Pipe *> possiblePipes, res;

    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            pipe->setSelected(false);
        }
    }
    for (Pipe *pipe: city->getIncoming()) {
        for (AugmentingPath *path: pipe->getAugmentingPaths()) {
            for (auto pair: path->getPipes()) {
                Pipe* p = pair.first;
                p->setSelected(true);
            }
        }
    }
    for (ServicePoint *sp: getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            if (!pipe->isSelected() || *pipe->getOrig() == *superSource || *pipe->getDest() == *superSink)
                continue;
            possiblePipes.push_back(pipe);
        }
    }

    for (Pipe *pipe: possiblePipes) {
        loadNetwork();
        unselectAllAugmentingPaths();
        pipe->selectAugmentingPaths();
        subtractAugmentingPaths();
        pipe->setHidden(true);
        if (pipe->getReverse() != nullptr)
            pipe->setHidden(true);
        recalculateMaxFlow();
        pipe->setHidden(false);
        if (pipe->getReverse() != nullptr)
            pipe->setHidden(false);
        if (city->getSupplyRate() < maxFlowNetwork->findDeliverySite(city->getCode())->getSupplyRate())
            res.push_back(pipe);
    }
    return res;
}

void compute_metrics(const vector<double> &v, tuple<double, double, double> &metrics) {
    metrics = {0,0,0};

    // Compute max and mean
    for (double value: v) {
        get<1>(metrics) += value;
        if(value > get<0>(metrics)) get<0>(metrics) = value;
    }
    get<1>(metrics) /= (double)v.size();

    // Compute variance
    for(double value: v) {
        get<2>(metrics) += (value - get<1>(metrics)) * (value - get<1>(metrics));
    }
    get<2>(metrics) /= (double)v.size();
}

void WaterSupplyNetwork::getMetrics(tuple<double, double, double> &metrics){
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

    compute_metrics(differences, metrics);

    unhideAllPipes();
}

void WaterSupplyNetwork::balance(double value) {
    WaterSupplyNetwork auxNetwork;
    double initFlow = getMaxFlow();
    copyGraph(this, &auxNetwork);
    bool found = true;
    while (found) {
        found = false;
        copyFlows(this, &auxNetwork);
        vector<Pipe *> pipes;

        for(ServicePoint *sp: getServicePoints()) {
            for (Pipe *p: sp->getAdj()) {
                if(p->getFlow() > 0 && p->getCapacity() > 0 && p->getRemainingFlow() <= value && !(*p->getOrig() == *superSource) && !(*p->getDest() == *superSink))
                    pipes.push_back(p);
            }
        }
        if (pipes.empty())
            break;

        sort(pipes.begin(), pipes.end(), [&](Pipe *a, Pipe *b){ return (a->getRemainingFlow() < b->getRemainingFlow() || (a->getRemainingFlow() == b->getRemainingFlow() && a->getCapacity() > b->getCapacity())); });

        for (Pipe *targetPipe: pipes) {
            double originalCapacity = targetPipe->getCapacity();
            targetPipe->setCapacity(max(originalCapacity - floor(value), 0.0));
            if (targetPipe->getReverse() != nullptr)
                targetPipe->setCapacity(max(originalCapacity - floor(value), 0.0));
            double finalFlow = getMaxFlow(false);

            if (finalFlow < initFlow) {
                copyFlows(&auxNetwork, this);
                targetPipe->setCapacity(originalCapacity);
            } else {
                found = true;
                break;
            }
        }
    }

    for (ServicePoint *sp: auxNetwork.getServicePoints()) {
        for (Pipe *pipe: sp->getAdj()) {
            findPipe(sp->getCode(), pipe->getDest()->getCode())->setCapacity(pipe->getCapacity());
        }
    }
}