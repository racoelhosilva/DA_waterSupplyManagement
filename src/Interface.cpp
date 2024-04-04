#include "Interface.h"
#include "input.h"
#include "ansi.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <codecvt>
#include <algorithm>
#include <fstream>

using namespace std;

bool Interface::init(){
    this->wsn = WaterSupplyNetwork();
    wsn.parseData("../datasetLarge/Reservoir.csv","../datasetLarge/Stations.csv","../datasetLarge/Cities.csv","../datasetLarge/Pipes.csv");
    std::ofstream ofs;
    ofs.open(fileName, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    system("cls || clear");
    return true;
}

void Interface::printMenuOptions(const std::vector<std::string> &options, int choice) {
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << options[options.size()-1] << "│" << '\n';

    for (int idx = 1; idx < options.size() - 2; idx++){
        if (choice == idx){
            std::cout << "│" << BOLD << GREEN << " [" << idx << "] " << RESET << BOLD << std::setw(73) << std::left << options[idx] << RESET << "│" << '\n';
        }
        else {
            std::cout << "│" << GREEN << " [" << idx << "] " << RESET << FAINT << std::setw(73) << std::left << options[idx] << RESET << "│" << '\n';
        }
    }
    if (choice == options.size()-2){
        std::cout << "│" << BOLD << YELLOW << " [" << options.size()-2 << "] " << RESET << BOLD << std::setw(73) << std::left << options[options.size()-2] << RESET "│" << '\n';
    }
    else {
        std::cout << "│" << YELLOW << " [" << options.size()-2 << "] " << RESET << FAINT << std::setw(73) << std::left << options[options.size()-2] << RESET << "│" << '\n';
    }

    if (choice == 0){
        std::cout << "│" << BOLD << RED << " [0] " << RESET << BOLD << std::setw(73) << std::left << options[0] << RESET "│" << '\n';
    }
    else {
        std::cout << "│" << RED << " [0] " << RESET << FAINT << std::setw(73) << std::left << options[0] << RESET << "│" << '\n';
    }
}

void Interface::printOptionsPages(const std::vector<std::string> &options, const std::string &title, int choice, int page) {
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << title << "│" << '\n';


    for (int idx = page * 10 + 1; idx < min(page * 10 + 11, (int)options.size()); idx++){
        int spaceLength = 73;
        bool previous = false;
        for (int j = 0; j < options[idx].size(); j++){
            char c = options[idx][j];
            if (!isalpha(c) && !isblank(c) && !isdigit(c) && (c != '_')){
                if (!previous){
                    spaceLength++;
                    previous = true;
                }
                else {
                    previous = false;
                }
            }
        }
        if (idx >= 10){
            spaceLength--;
        }
        if (idx >= 100){
            spaceLength--;
        }

        if (choice == idx){
            std::cout << "│" << BOLD << GREEN << " [" << idx << "] " << RESET << BOLD << std::setw(spaceLength) << std::left << options[idx] << RESET << "│" << '\n';
        }
        else {
            std::cout << "│" << GREEN << " [" << idx << "] " << RESET << FAINT << std::setw(spaceLength) << std::left << options[idx] << RESET << "│" << '\n';
        }
    }
    if (choice == min(page * 10 + 11, (int)options.size())){
        std::cout << "│" << BOLD << RED << " [0] " << RESET << BOLD << std::setw(73) << std::left << options[0] << RESET "│" << '\n';
    }
    else {
        std::cout << "│" << RED << " [0] " << RESET << FAINT << std::setw(73) << std::left << options[0] << RESET << "│" << '\n';
    }
}

void Interface::printWriteBuffer(const std::string &buffer){
    std::string s;
    for (int _ = 0; _ < 68; _++){
        s += "─";
    }

    std::cout << "│" << std::string(4, ' ') << "┌" << s << "┐" << std::string(4, ' ') << "│" << '\n';
    if (buffer.empty()){
        std::cout << "│" << std::string(4, ' ') << "│" << ' ' << FAINT << std::setw(66) << std::left << "Insert text here:" << RESET << ' ' << "│" << std::string(4, ' ') << "│" << '\n';
    }
    else {
        std::cout << "│" << std::string(4, ' ') << "│" << ' ' << std::setw(66) << std::left << buffer
        << ' ' << "│" << std::string(4, ' ') << "│" << '\n';
    }
    std::cout << "│" << std::string(4, ' ') << "└" << s << "┘" << std::string(4, ' ') << "│" << '\n';
}

void Interface::printTop() {
    std::string s;
    for (int _ = 0; _ < 78; _++){
        s += "─";
    }
    std::cout << "┌" << s << "┐" << '\n';
    std::string title = "Water Supply Management (G15_02)";
    std::cout << "│" << std::string(23, ' ') << BLUE << BOLD << title << RESET << std::string(23, ' ') << "│" << '\n';
}

void Interface::printBottom() {
    std::string s;
    for (int _ = 0; _ < 78; _++){
        s += "─";
    }
    std::cout << "└" << s << "┘" << '\n';
}

void Interface::waitInput() {
    initCapture();
    std::cout << HIDE_CURSOR;
    cout << '\n' << std::string(25, ' ') << FAINT << "< Press " << RESET << BOLD << "ENTER" << RESET << FAINT << " to continue >\n" << RESET;

    Press press;
    do {
        press = getNextPress();
    } while  (press != RET);
    endCapture();
}

void Interface::printTitle(const std::string &title) {
    system("cls || clear");
    cout << '\n' << std::string((width/2) - (title.size()/2), ' ') << BOLD << CYAN << title << RESET << "\n\n";
}

void Interface::printNetworkFlow(double flow, bool compare) {
    cout << std::string(infoSpacing, ' ') << "Total Network Flow: "
         << BOLD << MAGENTA << flow << RESET;
    if (compare){
        cout << ' ' << FAINT << '/' << ' ' << defaultNetworkFlow << RESET;
    }
    cout << '\n';
}

void Interface::saveGeneralMaxFlowToFile(const std::string& title) {
    std::ofstream output;
    output.open(fileName, std::ios::app);
    output << "===>  " << title << '\n';
    for (auto c : wsn.getDeliverySites()){
        output << c->getDescription() << ',' << cityToDefaultFlow[c->getCity()] << '\n';
    }
    output.close();
}

void Interface::saveAllMaxFlowToFile(const std::string& title) {
    std::ofstream output;
    output.open(fileName, std::ios::app);
    output << "===>  " << title << '\n';
    for (auto c : wsn.getDeliverySites()){
        double diff = c->getSupplyRate() - cityToDefaultFlow[c->getCity()];
        output << c->getDescription() << ',' << cityToDefaultFlow[c->getCity()] << ',' << c->getSupplyRate() << ',' << diff << '\n';
    }
    output.close();
}

void Interface::saveSingleMaxFlowToFile(const DeliverySite* city, const std::string& title) {
    std::ofstream output;
    output.open(fileName);
    output << "===>  " << title << '\n';
    output << city->getDescription() << ',' << city->getSupplyRate() << '\n';
    output.close();
}

void Interface::saveDeficitsToFile(const std::string& title) {
    std::ofstream output;
    output.open(fileName, std::ios::app);
    output << "===>  " << title << '\n';
    for (auto c : wsn.getDeliverySites()){
        double diff = c->getDemand() - c->getSupplyRate();
        if (diff != 0){
            output << c->getDescription() << ',' << c->getDemand() << ',' << c->getSupplyRate() << ',' << diff << '\n';
        }
    }
    output.close();
}

void Interface::mainMenu() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Quit",
             "Maximum Delivery for All Cities",
             "Maximum Delivery for Specific City",
             "Cities in Deficit",
             "Test Reservoir Out of Commission",
             "Test Pumping Stations Out of Service",
             "Test Pipe Failures",
             "Blank Function",
             outputToFile ? "Set output to Console" : "Set output to File (output.txt)",
             "Choose your operation:"};

    int choice = 1;
    Press press;
    do {
        system("cls || clear");
        printTop();
        printMenuOptions(options, choice);
        printBottom();
        press = getNextPress();
        if (press == UP) {choice -= 1; choice += (options.size()-1);}
        else if (press == DOWN) {choice += 1;}
            choice = choice % (options.size()-1);
    } while (press != RET);

    endCapture();
    switch (choice) {
        case 1:{
            double networkFlow = wsn.getMaxFlow(false);
            defaultNetworkFlow = networkFlow;
            if (cityToDefaultFlow.empty()){
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            std::string title = "Normal Max Flow (All Cities)";
            if (outputToFile){
                saveGeneralMaxFlowToFile(title);
            }
            else {
                printTitle(title);
                cityDisplay(wsn.getDeliverySites());
                printNetworkFlow(networkFlow, false);
            }
            waitInput();
            break;
        }
        case 2:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.getMaxFlow(false);
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            DeliverySite *city = citySelection();
            if (city == nullptr){
                break;
            }
            wsn.hideAllButOneDeliverySite(city->getCode());
            wsn.getMaxFlow(false);
            std::string title = "Focused Max Flow (" + city->getCity() + ")";
            if (outputToFile){
                saveSingleMaxFlowToFile(city, title);
            }
            else {
                printTitle(title);
                cityDisplayComparison({city});
            }
            wsn.unhideAll();
            waitInput();
            break;
        }
        case 3:{
            // TODO: should we calculate again? or check first if the values are in the map?
            wsn.getMaxFlow(false);
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.getMaxFlow(false);
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            std::string title = "Cities with Demand Deficits (Normal Flow considered)";
            if (outputToFile){
                saveDeficitsToFile(title);
            }
            else {
                printTitle(title);
                displaySupplyDemand();
            }
            waitInput();
            break;
        }
        case 4:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.getMaxFlow(false);
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            Reservoir *r = reservoirSelection();
            if (r == nullptr){
                break;
            }
            wsn.hideReservoir(r->getCode());
            double networkFlow = wsn.getMaxFlow(false);
            std::string title = "Max Flow without Reservoir " + r->getCode();
            if (outputToFile){
                saveAllMaxFlowToFile(title);
            }
            else {
                printTitle(title);
                displayServicePointEffects();
                printNetworkFlow(networkFlow);
            }
            wsn.unhideAll();
            waitInput();
            break;
        }
        case 5:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.getMaxFlow(false);
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            PumpingStation *p = pumpingStationSelection();
            if (p == nullptr){
                break;
            }
            wsn.hidePumpingStation(p->getCode());
            double networkFlow = wsn.getMaxFlow(false);
            std::string title = "Max Flow without Pumping Station " + p->getCode();
            if (outputToFile) {
                saveAllMaxFlowToFile(title);
            }
            else {
                printTitle(title);
                displayServicePointEffects();
                printNetworkFlow(networkFlow);
            }
            wsn.unhideAll();
            waitInput();
            break;
        }
        case 6:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.getMaxFlow(false);
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            ServicePoint *src = servicePointSelection();
            if (src == nullptr){
                break;
            }
            ServicePoint *dest = servicePointSelection(src);
            if (dest == nullptr){
                break;
            }

            Pipe *p = wsn.findPipe(src->getCode(), dest->getCode());
            wsn.hidePipe(p);
            double networkFlow = wsn.getMaxFlow(false);
            std::string title = "Max Flow without Pipe " + src->getCode() + " -> " + dest->getCode();
            if (outputToFile){
                saveAllMaxFlowToFile(title);
            }
            else {
                printTitle(title);
                displayServicePointEffects();
                printNetworkFlow(networkFlow);
            }
            wsn.unhidePipe(p);
            waitInput();
            break;
        }
        case 7:
            std::cout << readInputText();
            break;
        case 8:
            outputToFile = not outputToFile;
            break;
        case 0:
            exitMenu();
            break;
    }
    mainMenu();
}

DeliverySite * Interface::citySelection() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back"};
    std::vector<std::string> codes = {""};
    for (DeliverySite* ds : wsn.getDeliverySites()){
        options.push_back(ds->getCity());
        codes.push_back(ds->getCode());
    }
    std::string title = "Choose a city:";

    int choice = 1, page = 0, llimit = 1, hlimit = min(11, (int)options.size()), page_limit = ((int)options.size() - 1) / 10;
    Press press;
    do {
        system("cls || clear");
        printTop();
        printOptionsPages(options, title, choice, page);
        printBottom();
        press = getNextPress();

        if (press == UP) {choice = (choice - 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == DOWN) {choice = (choice + 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == LEFT) {page = max(page - 1, 0); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
        else if (press == RIGHT) {page = min(page + 1, page_limit); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
    } while (press != RET);

    endCapture();
    if (choice == hlimit)
        return nullptr;
    else
        return wsn.findDeliverySite(codes[choice]);
}

Reservoir * Interface::reservoirSelection() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back"};
    std::vector<std::string> codes = {""};
    for (Reservoir* r : wsn.getReservoirs()){
        options.push_back(r->getName());
        codes.push_back(r->getCode());
    }
    std::string title = "Choose a city:";


    int choice = 1, page = 0, llimit = 1, hlimit = min(11, (int)options.size()), page_limit = ((int)options.size() - 1) / 10;
    Press press;
    do {
        system("cls || clear");
        printTop();
        printOptionsPages(options, title, choice, page);
        printBottom();
        press = getNextPress();

        if (press == UP) {choice = (choice - 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == DOWN) {choice = (choice + 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == LEFT) {page = max(page - 1, 0); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
        else if (press == RIGHT) {page = min(page + 1, page_limit); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
    } while (press != RET);

    endCapture();
    if (choice == hlimit)
        return nullptr;
    else
        return wsn.findReservoir(codes[choice]);
}

PumpingStation * Interface::pumpingStationSelection() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back"};
    std::vector<std::string> codes = {""};
    for (PumpingStation* p : wsn.getPumpingStations()){
        options.push_back(to_string(p->getId()));
        codes.push_back(p->getCode());
    }
    std::string title = "Choose a pumping station:";


    int choice = 1, page = 0, llimit = 1, hlimit = min(11, (int)options.size()), page_limit = ((int)options.size() - 1) / 10;
    Press press;
    do {
        system("cls || clear");
        printTop();
        printOptionsPages(options, title, choice, page);
        printBottom();
        press = getNextPress();

        if (press == UP) {choice = (choice - 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == DOWN) {choice = (choice + 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == LEFT) {page = max(page - 1, 0); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
        else if (press == RIGHT) {page = min(page + 1, page_limit); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
    } while (press != RET);

    endCapture();
    if (choice == hlimit)
        return nullptr;
    else
        return wsn.findPumpingStation(codes[choice]);
}

ServicePoint * Interface::servicePointSelection() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back"};
    std::vector<std::string> codes = {""};
    for (ServicePoint* sp : wsn.getServicePoints()){
        if (!sp->getAdj().empty())
            options.push_back(sp->getCode());
    }
    std::string title = "Choose a service point:";


    int choice = 1, page = 0, llimit = 1, hlimit = min(11, (int)options.size()), page_limit = ((int)options.size() - 1) / 10;
    Press press;
    do {
        system("cls || clear");
        printTop();
        printOptionsPages(options, title, choice, page);
        printBottom();
        press = getNextPress();

        if (press == UP) {choice = (choice - 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == DOWN) {choice = (choice + 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == LEFT) {page = max(page - 1, 0); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
        else if (press == RIGHT) {page = min(page + 1, page_limit); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
    } while (press != RET);

    endCapture();
    if (choice == hlimit)
        return nullptr;
    else
        return wsn.findServicePoint(options[choice]);
}

ServicePoint * Interface::servicePointSelection(ServicePoint *src) {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back"};
    std::vector<std::string> codes = {""};
    for (Pipe *p : src->getAdj()){
        ServicePoint *sp = p->getDest();
        options.push_back(sp->getCode());
    }
    std::string title = "Choose a service point:";


    int choice = 1, page = 0, llimit = 1, hlimit = min(11, (int)options.size()), page_limit = ((int)options.size() - 1) / 10;
    Press press;
    do {
        system("cls || clear");
        printTop();
        printOptionsPages(options, title, choice, page);
        printBottom();
        press = getNextPress();

        if (press == UP) {choice = (choice - 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == DOWN) {choice = (choice + 1 - llimit + (hlimit - llimit + 1)) % (hlimit - llimit + 1) + llimit;}
        else if (press == LEFT) {page = max(page - 1, 0); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
        else if (press == RIGHT) {page = min(page + 1, page_limit); llimit = choice = page * 10 + 1; hlimit = min(llimit + 10, (int)options.size());}
    } while (press != RET);

    endCapture();
    if (choice == hlimit)
        return nullptr;
    else
        return wsn.findServicePoint(options[choice]);
}

std::string Interface::readInputText(){
    initCapture();
    std::cout << HIDE_CURSOR;
    clearBuffer();

    Press press;
    do {
        system("cls || clear");
        printTop();
        printWriteBuffer(getBuffer());
        printBottom();
        press = getNextPress();
    } while (press != RET);
    endCapture();
    return getBuffer();
}

wstring toWstring(string str) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

void Interface::printTable(const vector<int> &colLens, const vector<string> &headers, const vector<vector<string>> &cells) {
    cout << BOLD << INVERT << "  ";
    for (int i = 0; i < headers.size(); i++)
        cout << left << setw(colLens[i]) << headers[i] << ' ';
    cout << "  " << RESET << "\n";
    for (int i = 0; i < cells.size(); i++) {
        cout << "│ ";
        for (int j = 0; j < cells[i].size(); j++){
            int formatting = colLens[j];
            string text = (cells[i][j]);
            bool previous = false;
            for (const char &c : text){
                if (!isalpha(c) && !isblank(c) && !isdigit(c) && (c != '_') && (c != '.')){
                    if (!previous){
                        formatting++;
                        previous = true;
                    }
                    else {
                        previous = false;
                    }
                }
            }
            cout << left << setw(formatting) << text << ' ';
        }
        cout << " │\n";
    }
    printBottom();
}

string doubleToString(double val, int precision = 5) {
    if ((int)val == val)
        return to_string((int)val);
    ostringstream ss;
    ss << setprecision(precision) << val;
    return ss.str();
}

void Interface::cityDisplay(const std::vector<DeliverySite *> &cities) {
    vector<int> colLens = {6, 4, 28, 10, 12, 10};
    vector<string> headers = {"Code", "Id", "City", "Demand", "Flow", "Population"};
    vector<vector<string>> cells(cities.size(), vector<string>());
    for (int i = 0; i < cities.size(); i++) {
        const DeliverySite *city = cities[i];
        cells[i] = {city->getCode(), to_string(city->getId()), city->getCity(), doubleToString(city->getDemand()),
                    doubleToString(city->getSupplyRate()), to_string(city->getPopulation())};
    }
    printTable(colLens, headers, cells);
}

void Interface::reservoirDisplay(const std::vector<Reservoir*> &reservoirs) {
    vector<int> colLens = {6, 6, 20, 20, 8};
    vector<string> headers = {"Code", "Id", "Name", "Municipality", "Max Delivery"};
    vector<vector<string>> cells(reservoirs.size(), vector<string>());
    for (int i = 0; i < reservoirs.size(); i++) {
        const Reservoir *reservoir = reservoirs[i];
        cells[i] = {reservoir->getCode(), doubleToString(reservoir->getId()), reservoir->getName(),
                    reservoir->getMunicipality(), doubleToString(reservoir->getMaxDelivery())};
    }
    printTable(colLens, headers, cells);
}

void Interface::pumpingStationDisplay(const std::vector<PumpingStation *> &pumpingStations) {
    vector<int> colLens = {6, 6};
    vector<string> headers = {"Code", "Id"};
    vector<vector<string>> cells(pumpingStations.size(), vector<string>());
    for (int i = 0; i < pumpingStations.size(); i++) {
        const PumpingStation *pumpingStation = pumpingStations[i];
        cells[i] = {pumpingStation->getCode(), doubleToString(pumpingStation->getId())};
    }
    printTable(colLens, headers, cells);
}

void Interface::pipeDisplay(const ServicePoint *servicePoint) {
    cout << "│    Outgoing pipes: ";
    vector<int> colLens = {20, 13, 8};
    vector<string> headers = {"Destination", "Bidirectional", "Capacity"};
    vector<vector<string>> cells(servicePoint->getAdj().size(), vector<string>());
    for (int i = 0; i < servicePoint->getAdj().size(); i++) {
        Pipe *pipe = servicePoint->getAdj()[i];
        cells[i] = {pipe->getDest()->getDescription(), pipe->getReverse() == nullptr ? "No" : "Yes", doubleToString(pipe->getCapacity())};
    }
    printTable(colLens, headers, cells);

    cout << "│    Incoming pipes: ";
    colLens = {30, 13, 8};
    headers = {"Origin", "Bidirectional", "Capacity"};
    cells = vector<vector<string>>(servicePoint->getIncoming().size(), vector<string>());
    for(int i = 0; i < servicePoint->getIncoming().size(); i++) {
        Pipe *pipe = servicePoint->getIncoming()[i];
        cells[i] = {pipe->getOrig()->getDescription(), pipe->getReverse() == nullptr ? "No" : "Yes", doubleToString(pipe->getCapacity())};
    }
    printTable(colLens, headers, cells);
}

void Interface::cityDisplayComparison(const std::vector<DeliverySite *> &cities) {
    vector<int> colLens = {6, 20, 10, 12, 12, 10};
    vector<string> headers = {"Code", "City", "Demand", "Normal", "Focused", "Population"};
    vector<vector<string>> cells(cities.size(), vector<string>());
    for (int i = 0; i < cities.size(); i++) {
        const DeliverySite *city = cities[i];
        cells[i] = {city->getCode(), city->getCity(), doubleToString(city->getDemand()),
                    doubleToString(cityToDefaultFlow[city->getCity()]),
                    doubleToString(city->getSupplyRate()), to_string(city->getPopulation())};
    }
    printTable(colLens, headers, cells);
}

void Interface::displaySupplyDemand(){
    vector<int> colLens = {6, 22, 10, 12, 10, 10};
    vector<string> headers = {"Code", "City", "Demand", "Flow", "Deficit", "Deficit %"};
    vector<vector<string>> cells;
    for (const DeliverySite *ds : wsn.getDeliverySites()) {
        if (ds->getDemand() > ds->getSupplyRate()) {
            vector<string> row = {ds->getCode(),
                                  ds->getCity(), doubleToString(ds->getDemand()),
                                  doubleToString(ds->getSupplyRate()),
                                  doubleToString(ds->getDemand() - ds->getSupplyRate()),
                                  doubleToString(((ds->getDemand() - ds->getSupplyRate())/ds->getDemand())*100)};
            cells.push_back(row);
        }
    }
    if (cells.empty()){
        cout << std::string(infoSpacing, ' ') << "There are " << BOLD << YELLOW << "no" << RESET << " cities in deficit!\n";
    }
    else {
        printTable(colLens, headers, cells);
        if (cells.size() == 1){
            cout << std::string(infoSpacing, ' ') << "There is " << BOLD << YELLOW << cells.size() << RESET << " city in deficit!\n";
        }
        else {
            cout << std::string(infoSpacing, ' ') << "There are " << BOLD << YELLOW << cells.size() << RESET << " cities in deficit!\n";
        }
    }
}

void Interface::displayServicePointEffects() {
    vector<int> colLens = {6, 20, 12, 12, 10, 10};
    vector<string> headers = {"Code", "City", "Normal", "Affected", "Delta", "Delta %"};
    vector<vector<string>> cells;
    for (const DeliverySite *ds : wsn.getDeliverySites()) {
        if (ds->getSupplyRate() < cityToDefaultFlow[ds->getCity()]) {
            vector<string> row = {ds->getCode(),
                                  ds->getCity(), doubleToString(cityToDefaultFlow[ds->getCity()]),
                                  doubleToString(ds->getSupplyRate()),
                                  doubleToString(cityToDefaultFlow[ds->getCity()] - ds->getSupplyRate()),
                                  doubleToString(((cityToDefaultFlow[ds->getCity()] - ds->getSupplyRate())/cityToDefaultFlow[ds->getCity()])*100)};
            cells.push_back(row);
        }
    }
    if (cells.empty()){
        cout << std::string(infoSpacing, ' ') << "There are " << BOLD << YELLOW << "no" << RESET << " cities affected!\n";
    }
    else {
        printTable(colLens, headers, cells);
        if (cells.size() == 1){
            cout << std::string(infoSpacing, ' ') << "There is " << BOLD << YELLOW << cells.size() << RESET << " city affected!\n";
        }
        else {
            cout << std::string(infoSpacing, ' ') << "There are " << BOLD << YELLOW << cells.size() << RESET << " cities affected!\n";
        }
    }
}

void Interface::exitMenu() {
    std::cout << "Closing the app...\n";
    exit(0);
}