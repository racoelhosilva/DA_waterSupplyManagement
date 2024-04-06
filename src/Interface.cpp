#include "Interface.h"
#include "input.h"
#include "ansi.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <codecvt>
#include <algorithm>
#include <fstream>
#include <cmath>

using namespace std;

// std::sort(res.begin(), res.end(), [](T* a, T* b) {return a->getCode() < b->getCode();});

bool Interface::init(){
    this->wsn = WaterSupplyNetwork();
    if (!datasetMenu())
        return false;
    std::ofstream ofs;
    ofs.open(fileName, std::ofstream::out | std::ofstream::trunc);
    ofs.close();
    clearScreen();
    return true;
}

void Interface::clearScreen() {
#ifdef __unix__
    system("clear");
#else
    // assume windows
    system("cls");
#endif
}

void Interface::printMenuOptions(const std::vector<std::string> &options, int choice) {
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << options[options.size()-1] << "│" << '\n';

    for (int idx = 1; idx < options.size() - 2; idx++){
        int space = 73;
        if (idx >= 10){
            space--;
        }
        if (choice == idx){
            std::cout << "│" << BOLD << GREEN << " [" << idx << "] " << RESET << BOLD << std::setw(space) << std::left << options[idx] << RESET << "│" << '\n';
        }
        else {
            std::cout << "│" << GREEN << " [" << idx << "] " << RESET << FAINT << std::setw(space) << std::left << options[idx] << RESET << "│" << '\n';
        }
    }

    int space = 73;
    if (options.size() - 2 >= 10){
        space--;
    }

    if (choice == options.size()-2){
        std::cout << "│" << BOLD << YELLOW << " [" << options.size()-2 << "] " << RESET << BOLD << std::setw(space) << std::left << options[options.size()-2] << RESET "│" << '\n';
    }
    else {
        std::cout << "│" << YELLOW << " [" << options.size()-2 << "] " << RESET << FAINT << std::setw(space) << std::left << options[options.size()-2] << RESET << "│" << '\n';
    }

    if (choice == 0){
        std::cout << "│" << BOLD << RED << " [0] " << RESET << BOLD << std::setw(73) << std::left << options[0] << RESET "│" << '\n';
    }
    else {
        std::cout << "│" << RED << " [0] " << RESET << FAINT << std::setw(73) << std::left << options[0] << RESET << "│" << '\n';
    }
}

void Interface::printMenuOptionsNoBottom(const std::vector<std::string> &options, int choice) {
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << options[options.size()-1] << "│" << '\n';

    for (int idx = 1; idx < options.size() - 1; idx++){
        if (choice == idx){
            std::cout << "│" << BOLD << GREEN << " [" << idx << "] " << RESET << BOLD << std::setw(73) << std::left << options[idx] << RESET << "│" << '\n';
        }
        else {
            std::cout << "│" << GREEN << " [" << idx << "] " << RESET << FAINT << std::setw(73) << std::left << options[idx] << RESET << "│" << '\n';
        }
    }
    if (choice == 0){
        std::cout << "│" << BOLD << RED << " [0] " << RESET << BOLD << std::setw(73) << std::left << options[0] << RESET "│" << '\n';
    }
    else {
        std::cout << "│" << RED << " [0] " << RESET << FAINT << std::setw(73) << std::left << options[0] << RESET << "│" << '\n';
    }
}

void Interface::printOptionsPages(const std::vector<std::string> &options, const std::string &title, int choice, int page, int num_pages) {
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

    int leftSpacing = 32, rightSpacing = 33;
    if (page + 1 >= 10){
        rightSpacing--;
    }
    if (num_pages + 1 >= 10){
        rightSpacing--;
    }

    std::string leftMark = "<   ", rightMark = "   >";

    if (page == 0){
        leftMark = "    ";
    }
    if (page == num_pages){
        rightMark = "    ";
    }
    std::cout << "│" << FAINT << std::string(leftSpacing, ' ')
              << leftMark << RESET
              << page+1 << " / " << num_pages+1
              << FAINT << rightMark
              << RESET << std::string(rightSpacing, ' ') << "│" << '\n';
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
    clearScreen();
    cout << '\n' << std::string((width/2) - (title.size()/2), ' ') << BOLD << BLUE << title << RESET << "\n\n";
}

void Interface::printTitleNoClear(const std::string &title) {
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

void Interface::printSelectedPipes() {
    cout << "│  " << BOLD << left << setw(76) << "Hidden Pipes:" << RESET << "│\n";
    for (const Pipe * p : selectedPipes){
        std::string option = p->getOrig()->getCode() + " -> " + p->getDest()->getCode();
        cout << "│" << std::string(4, ' ') << BOLD << YELLOW << "-" << RESET
             << std::string(1, ' ') << left << setw(72) << option << "│\n";
    }
}

void Interface::printHiddenPipes() {
    cout << std::string(infoSpacing, ' ') << "Hidden Pipes: " << CYAN;
    for (Pipe * pipe : selectedPipes){
        cout << pipe->getOrig()->getCode() << "->" << pipe->getDest()->getCode() << "  ";
    }
    cout << RESET << '\n';
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

void Interface::saveCriticalPipesToFile(const std::string& title, const std::vector<Pipe *> pipes) {
    std::ofstream output;
    output.open(fileName, std::ios::app);
    output << "===>  " << title << '\n';
    for (auto p : pipes){
        output << p->getOrig() << ',' << p->getDest() << '\n';
    }
    output.close();
}

void Interface::saveMetricsToFile(std::tuple<double, double, double> &metrics) {
    std::ofstream output;
    output.open(fileName, std::ios::app);
    output << get<0>(metrics) << ',' << get<1>(metrics) << ',' << get<2>(metrics) << '\n';
    output.close();
}

void Interface::saveTitleToFile(const std::string &title) {
    std::ofstream output;
    output.open(fileName, std::ios::app);
    output << "===>  " << title << '\n';
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
             "Test Reservoir Out of Commission (Brute-Force)",
             "Test Pumping Stations Out of Service",
             "Test Pumping Stations Out of Service (Brute-Force)",
             "Test Pipe Failures",
             "Test Pipe Failures (Brute-Force)",
             "Critical Pipes for Specific City",
             "Network Balancing",
             "Display Network Information",
             outputToFile ? "Set output to Console" : "Set output to File (output.txt)",
             "Choose your operation:"};


    int choice = 1;
    Press press;
    do {
        clearScreen();
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
            double networkFlow = wsn.loadCachedMaxFlow();
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
                defaultNetworkFlow = wsn.loadCachedMaxFlow();
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            DeliverySite *city = citySelection();
            if (city == nullptr){
                break;
            }
            wsn.hideAllButOneDeliverySite(city->getCode());
            wsn.getMaxFlow();
            std::string title = "Focused Max Flow (" + city->getCity() + ")";
            if (outputToFile){
                saveSingleMaxFlowToFile(city, title);
            }
            else {
                printTitle(title);
                cityDisplayComparison({city});
            }
            wsn.unhideAllServicePoints();
            waitInput();
            break;
        }
        case 3:{
            wsn.getMaxFlow();
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.loadCachedMaxFlow();
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
                defaultNetworkFlow = wsn.loadCachedMaxFlow();
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            Reservoir *r = reservoirSelection();
            if (r == nullptr){
                break;
            }
            double networkFlow = wsn.getMaxFlowWithoutReservoir(r);
            std::string title = "Max Flow without Reservoir " + r->getCode();
            if (outputToFile){
                saveAllMaxFlowToFile(title);
            }
            else {
                printTitle(title);
                displayServicePointEffects();
                printNetworkFlow(networkFlow);
            }
            wsn.unhideAllPipes();
            waitInput();
            break;
        }
        case 5:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.loadCachedMaxFlow();
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            Reservoir *r = reservoirSelection();
            if (r == nullptr){
                break;
            }
            double networkFlow = wsn.getMaxFlowWithoutReservoirBF(r);
            std::string title = "Max Flow without Reservoir " + r->getCode();
            if (outputToFile){
                saveAllMaxFlowToFile(title);
            }
            else {
                printTitle(title);
                displayServicePointEffects();
                printNetworkFlow(networkFlow);
            }
            wsn.unhideAllServicePoints();
            waitInput();
            break;
        }
        case 6:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.loadCachedMaxFlow();
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            PumpingStation *p = pumpingStationSelection();
            if (p == nullptr){
                break;
            }
            double networkFlow = wsn.getMaxFlowWithoutStation(p);
            std::string title = "Max Flow without Pumping Station " + p->getCode();
            if (outputToFile) {
                saveAllMaxFlowToFile(title);
            }
            else {
                printTitle(title);
                displayServicePointEffects();
                printNetworkFlow(networkFlow);
            }
            wsn.unhideAllPipes();
            waitInput();
            break;
        }
        case 7:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.loadCachedMaxFlow();
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            PumpingStation *p = pumpingStationSelection();
            if (p == nullptr){
                break;
            }
            double networkFlow = wsn.getMaxFlowWithoutStationBF(p);
            std::string title = "Max Flow without Pumping Station " + p->getCode();
            if (outputToFile) {
                saveAllMaxFlowToFile(title);
            }
            else {
                printTitle(title);
                displayServicePointEffects();
                printNetworkFlow(networkFlow);
            }
            wsn.unhideAllServicePoints();
            waitInput();
            break;
        }
        case 8:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.loadCachedMaxFlow();
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
            selectedPipes.push_back(p);

            bool process = pipeMenu();
            if (process){
                double networkFlow = wsn.getMaxFlowWithoutPipes(selectedPipes);
                std::string title = "Max Flow without Pipes";
                if (outputToFile){
                    title.append(": ");
                    int count = 0;
                    for (Pipe* pipe : selectedPipes){
                        if (count != 0){
                            title.append(", ");
                        }
                        title.append(pipe->getOrig()->getCode());
                        title.append(" -> ");
                        title.append(pipe->getDest()->getCode());
                        count++;
                    }
                    saveAllMaxFlowToFile(title);
                }
                else {
                    printTitle(title);
                    displayServicePointEffects();
                    printHiddenPipes();
                    printNetworkFlow(networkFlow);
                }
            }

            wsn.unhideAllPipes();
            selectedPipes.clear();

            if (process){
                waitInput();
            }
            break;
        }
        case 9:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.getMaxFlow();
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
            selectedPipes.push_back(p);

            bool process = pipeMenu();
            if (process){
                double networkFlow = wsn.getMaxFlowWithoutPipesBF(selectedPipes);
                std::string title = "Max Flow without Pipes";
                if (outputToFile){
                    title.append(": ");
                    int count = 0;
                    for (Pipe* pipe : selectedPipes){
                        if (count != 0){
                            title.append(", ");
                        }
                        title.append(pipe->getOrig()->getCode());
                        title.append(" -> ");
                        title.append(pipe->getDest()->getCode());
                        count++;
                    }
                    saveAllMaxFlowToFile(title);
                }
                else {
                    printTitle(title);
                    displayServicePointEffects();
                    printHiddenPipes();
                    printNetworkFlow(networkFlow);
                }
            }

            wsn.unhideAllPipes();
            selectedPipes.clear();

            if (process){
                waitInput();
            }
            break;
        }
        case 10:{
            if (cityToDefaultFlow.empty()){
                defaultNetworkFlow = wsn.getMaxFlow();
                for (DeliverySite *ds : wsn.getDeliverySites()){
                    cityToDefaultFlow[ds->getCity()] = ds->getSupplyRate();
                }
            }
            DeliverySite *city = citySelection();
            if (city == nullptr){
                break;
            }
            vector<Pipe *> pipes = wsn.getCriticalPipesToCity(city);
            std::string title = "Essential Pipes for City (" + city->getCity() + ")";
            if (outputToFile){
                saveCriticalPipesToFile(title, pipes);
            }
            else {
                printTitle(title);
                displayCriticalPipes(pipes);
            }
            wsn.unhideAllServicePoints();
            waitInput();
            break;
        }
        case 11:{
            wsn.getMaxFlow();
            std::vector<std::tuple<double, double, double>> allMetrics;
            std::tuple<double, double, double> metrics;
            wsn.getMetrics(metrics);
            allMetrics.push_back(metrics);
            int attempts = 5;

            std::string title = "Balancing Metrics";
            if (outputToFile){
                saveTitleToFile(title);
            }
            else {
                title.append(" (this may take a while)");
                printTitle(title);
                printMetricsHeader();
            }

            std::tuple<double, double, double> prevMetrics;
            do {
                if (outputToFile){
                    saveMetricsToFile(metrics);
                }
                else {
                    printMetricsRow(metrics);
                }
                wsn.balance(get<1>(metrics));
                prevMetrics = metrics;
                wsn.getMetrics(metrics);
                allMetrics.push_back(metrics);
                attempts--;
            } while (abs(get<2>(prevMetrics) - get<2>(metrics)) > (0.001 * get<2>(prevMetrics)) && attempts > 0);

            if (outputToFile){
                saveMetricsToFile(metrics);
            }
            else {
                printMetricsRow(metrics);
                printBottom();
            }

            waitInput();
            break;
        }
        case 12:
            informationMenu();
            break;
        case 13:
            outputToFile = not outputToFile;
            break;
        case 0:
            exitMenu();
            break;
    }
}

void Interface::informationMenu() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {
             "Back",
             "List All Delivery Sites (Cities)",
             "List All Reservoirs",
             "List All Pumping Stations",
             "List Pipes from Service Point",
             "Information to check:"
            };

    int choice = 1;
    Press press;
    do {
        clearScreen();
        printTop();
        printMenuOptionsNoBottom(options, choice);
        printBottom();
        press = getNextPress();
        if (press == UP) {choice -= 1; choice += (options.size()-1);}
        else if (press == DOWN) {choice += 1;}
        choice = choice % (options.size()-1);
    } while (press != RET);

    endCapture();

    switch (choice) {
        case 1:
            printTitle("Delivery Site (City) Display");
            cityDisplayInfo(wsn.getDeliverySites());
            waitInput();
            break;
        case 2:
            printTitle("Reservoir Display");
            reservoirDisplay(wsn.getReservoirs());
            waitInput();
            break;
        case 3:
            printTitle("Pumping Station Display");
            pumpingStationDisplay(wsn.getPumpingStations());
            waitInput();
            break;
        case 4: {
            ServicePoint *p = allServicePointSelection();
            if (p == nullptr) {
                break;
            }
            printTitle("Pipe Display for Service Point " + p->getCode());
            pipeDisplay(p);
            waitInput();
            break;
        }
        default:
            mainMenu();
    }
}

bool Interface::pipeMenu() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back",
             "Add more Pipes",
             "Process Operation",
             "Choose your operation:"};

    int choice = 1;
    Press press;
    do {
        clearScreen();
        printTop();
        printMenuOptions(options, choice);
        printSelectedPipes();
        printBottom();
        press = getNextPress();
        if (press == UP) {choice -= 1; choice += (options.size()-1);}
        else if (press == DOWN) {choice += 1;}
        choice = choice % (options.size()-1);
    } while (press != RET);

    endCapture();
    switch (choice) {
        case 1: {
            ServicePoint *src = servicePointSelection();
            if (src == nullptr){
                break;
            }
            ServicePoint *dest = servicePointSelection(src);
            if (dest == nullptr){
                break;
            }
            Pipe *p = wsn.findPipe(src->getCode(), dest->getCode());
            selectedPipes.push_back(p);
            return pipeMenu();
        }
        case 2:
            return true;
    }
    return false;
}

bool Interface::datasetMenu() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Quit",
            "Large Data Set (Portugal)",
            "Small Data Set (Madeira)",
            "Custom Data Set (Check README)",
            "Choose the Dataset"
            };

    int choice = 1;
    Press press;
    do {
        clearScreen();
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
        case 1:
            return wsn.parseData("../datasetLarge/Reservoir.csv","../datasetLarge/Stations.csv","../datasetLarge/Cities.csv","../datasetLarge/Pipes.csv");
        case 2:
            return wsn.parseData("../datasetSmall/Reservoirs_Madeira.csv","../datasetSmall/Stations_Madeira.csv","../datasetSmall/Cities_Madeira.csv","../datasetSmall/Pipes_Madeira.csv");
        case 3:
            return wsn.parseData("../dataset/Reservoir.csv","../dataset/Stations.csv","../dataset/Cities.csv","../dataset/Pipes.csv");
        default:
            exitMenu();
    }
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
        clearScreen();
        printTop();
        printOptionsPages(options, title, choice, page, page_limit);
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
        clearScreen();
        printTop();
        printOptionsPages(options, title, choice, page, page_limit);
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
        clearScreen();
        printTop();
        printOptionsPages(options, title, choice, page, page_limit);
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

ServicePoint * Interface::allServicePointSelection() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back"};
    std::vector<std::string> codes = {""};
    for (ServicePoint* sp : wsn.getServicePoints()){
        options.push_back(sp->getCode());
    }
    std::string title = "Choose a service point:";


    int choice = 1, page = 0, llimit = 1, hlimit = min(11, (int)options.size()), page_limit = ((int)options.size() - 1) / 10;
    Press press;
    do {
        clearScreen();
        printTop();
        printOptionsPages(options, title, choice, page, page_limit);
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

ServicePoint * Interface::servicePointSelection() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back"};
    std::vector<std::string> codes = {""};
    for (ServicePoint* sp : wsn.getServicePoints()){
        if (sp->getId() == 0){
            continue;
        }
        if (sp->getAdj().size() == 1){
            if (sp->getAdj()[0]->getDest()->getCode() == "__super_sink__"){
                continue;
            }
        }
        if (!sp->getAdj().empty())
            options.push_back(sp->getCode());
    }
    std::string title = "Choose a service point:";


    int choice = 1, page = 0, llimit = 1, hlimit = min(11, (int)options.size()), page_limit = ((int)options.size() - 1) / 10;
    Press press;
    do {
        clearScreen();
        printTop();
        printOptionsPages(options, title, choice, page, page_limit);
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
        clearScreen();
        printTop();
        printOptionsPages(options, title, choice, page, page_limit);
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
        clearScreen();
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

string doubleToString(double val, int precision = 5) {
    if ((int)val == val)
        return to_string((int)val);
    ostringstream ss;
    ss << setprecision(precision) << val;
    return ss.str();
}

void Interface::printMetricsHeader(){
    cout << INVERT << BOLD << std::string(2, ' ') << setw(25) << "Maximum Difference"
         << setw(25) << "Mean Difference"
         << setw(28) << "Variance" << RESET << '\n';
}

void Interface::printMetricsRow(std::tuple<double, double, double> &metrics){
    cout << "│ ";
    cout << setw(25) << doubleToString(get<0>(metrics))
         << setw(25) << doubleToString(get<1>(metrics))
         << setw(26) << get<2>(metrics);
    cout << " │\n";
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
                if (!isalpha(c) && !isblank(c) && !isdigit(c) && (c != '_') && (c != '.') && (c != '+')){
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

void Interface::cityDisplayInfo(const std::vector<DeliverySite *> &cities) {
    vector<int> colLens = {6, 4, 28, 16, 17};
    vector<string> headers = {"Code", "Id", "City", "Demand", "Population"};
    vector<vector<string>> cells(cities.size(), vector<string>());
    for (int i = 0; i < cities.size(); i++) {
        const DeliverySite *city = cities[i];
        cells[i] = {city->getCode(), to_string(city->getId()), city->getCity(), doubleToString(city->getDemand()), to_string(city->getPopulation())};
    }
    printTable(colLens, headers, cells);
}

void Interface::reservoirDisplay(const std::vector<Reservoir*> &reservoirs) {
    vector<int> colLens = {6, 4, 26, 23, 12};
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
    vector<int> colLens = {37, 37};
    vector<string> headers = {"Code", "Id"};
    vector<vector<string>> cells(pumpingStations.size(), vector<string>());
    for (int i = 0; i < pumpingStations.size(); i++) {
        const PumpingStation *pumpingStation = pumpingStations[i];
        cells[i] = {pumpingStation->getCode(), doubleToString(pumpingStation->getId())};
    }
    printTable(colLens, headers, cells);
}

void Interface::pipeDisplay(const ServicePoint *servicePoint) {
    printTitleNoClear("Outgoing pipes");
    vector<int> colLens = {25, 24, 24};
    vector<string> headers = {"Destination", "Bidirectional", "Capacity"};
    vector<vector<string>> cells(servicePoint->getAdj().size(), vector<string>());
    for (int i = 0; i < servicePoint->getAdj().size(); i++) {
        Pipe *pipe = servicePoint->getAdj()[i];
        cells[i] = {pipe->getDest()->getDescription(), pipe->getReverse() == nullptr ? "No" : "Yes", doubleToString(pipe->getCapacity())};
    }
    printTable(colLens, headers, cells);

    printTitleNoClear("Incoming pipes");
    colLens = {25, 24, 24};
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

void Interface::displayCriticalPipes(const std::vector<Pipe *> &pipes) {
    vector<int> colLens = {20, 20, 16, 16};
    vector<string> headers = {"Origin", "Destination", "Bidirectional", "Capacity"};
    vector<vector<string>> cells;
    for (const Pipe *p : pipes) {
        cells.push_back({p->getOrig()->getDescription(), p->getDest()->getDescription(),
                         p->getReverse() == nullptr ? "No" : "Yes", doubleToString(p->getCapacity())});
    }
    printTable(colLens, headers, cells);
}

void Interface::exitMenu() {
    std::cout << "Closing the app...\n";
    exit(0);
}