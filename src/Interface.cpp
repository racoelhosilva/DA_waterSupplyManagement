#include "Interface.h"
#include "input.h"
#include "ansi.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <codecvt>

using namespace std;

bool Interface::init(){
    this->wsn = WaterSupplyNetwork();
    wsn.parseData("../datasetLarge/Reservoir.csv","../datasetLarge/Stations.csv","../datasetLarge/Cities.csv","../datasetLarge/Pipes.csv");
    system("cls || clear");
    return true;
}

void Interface::printOptions(const std::vector<std::string> &options, int choice) {
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

void Interface::printOptionsCity(const std::vector<std::string> &options, const std::string &title, int choice, int page) {
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << title << "│" << '\n';


    for (int idx = page * 10 + 1; idx < min(page * 10 + 11, (int)options.size()); idx++){
        int spaceLength = 73;
        bool previous = false;
        for (int j = 0; j < options[idx].size(); j++){
            char c = options[idx][j];
            if (!isalpha(c) && !isblank(c)){
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
    cout << "Press ENTER to continue\n";

    Press press;
    do {
        press = getNextPress();
    } while  (press != RET);
    endCapture();
}

void Interface::mainMenu() {
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Quit",
             "Maximum Delivery for All Cities",
             "Maximum Delivery for Specific City",
             "Cities in Deficit",
             "Function 4",
             "Function 5",
             "Function 6",
             "Choose your operation:"};

    int choice = 1;
    Press press;
    do {
        system("cls || clear");
        printTop();
        printOptions(options, choice);
        printBottom();
        press = getNextPress();
        if (press == UP) {choice -= 1; choice += (options.size()-1);}
        else if (press == DOWN) {choice += 1;}
            choice = choice % (options.size()-1);
    } while (press != RET);

    endCapture();
    switch (choice) {
        case 1:{
            double supersinkFlow = wsn.getMaxFlow(false);
            cityDisplay(wsn.getDeliverySites());
            waitInput();
            break;
        }
        case 2:{
            DeliverySite *city = citySelection();
            if (city == nullptr){
                break;
            }
            wsn.hideAllButOneDeliverySite(city->getCode());
            double cityFlow = wsn.getMaxFlow(false);
            cityDisplay({city});
            wsn.unhideAll();
            waitInput();
            break;
        }
        case 3:{
            double supersinkFlow = wsn.getMaxFlow(false);
            displaySupplyDemand();
            waitInput();
            break;
        }
        case 4:
            std::cout << readInputText();
            break;
        case 5:
        case 6:
            std::cout << "\nChoice " << choice << " selected\n";
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
        printOptionsCity(options, title, choice, page);
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

void printTable(const vector<int> &colLens, const vector<string> &headers, const vector<vector<string>> &cells) {
    cout << "│    ";
    for (int i = 0; i < headers.size(); i++)
        cout << BOLD <<left << setw(colLens[i]) << headers[i] << RESET << ' ';
    cout << " │\n";
    for (int i = 0; i < cells.size(); i++) {
        cout << "│    ";
        for (int j = 0; j < cells[i].size(); j++)
            cout << left << setw(colLens[j]) << cells[i][j] << ' ';
        cout << "│\n";
    }
}

string doubleToString(double val, int precision = 5) {
    if ((int)val == val)
        return to_string((int)val);
    ostringstream ss;
    ss << setprecision(precision) << val;
    return ss.str();
}

wstring toWstring(string str) {
    wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
    return converter.from_bytes(str);
}

void Interface::cityDisplay(const std::vector<DeliverySite *> &cities) {
    vector<int> colLens = {6, 6, 20, 7, 13, 10};
    vector<string> headers = {"Code", "Id", "City", "Demand", "Incoming Flow", "Population"};
    vector<vector<string>> cells(cities.size(), vector<string>());
    for (int i = 0; i < cities.size(); i++) {
        const DeliverySite *city = cities[i];
        cells[i] = {city->getCode(), to_string(city->getId()), city->getCity(), doubleToString(city->getDemand()),
                    doubleToString(city->getSupplyRate()), to_string(city->getPopulation())};
    }
    printTable(colLens, headers, cells);
}

void Interface::displaySupplyDemand(){
    vector<int> colLens = {6, 6, 20, 7, 13, 7, 11};
    vector<string> headers = {"Code", "Id", "City", "Demand", "Incoming Flow", "Deficit", "Deficit (%)"};
    vector<vector<string>> cells;
    for (const DeliverySite *ds : wsn.getDeliverySites()) {
        if (ds->getDemand() > ds->getSupplyRate()) {
            vector<string> row = {ds->getCode(), to_string(ds->getId()),
                                  ds->getCity(), doubleToString(ds->getDemand()),
                                  doubleToString(ds->getSupplyRate()),
                                  doubleToString(ds->getDemand() - ds->getSupplyRate()),
                                  doubleToString(((ds->getDemand() - ds->getSupplyRate())/ds->getDemand())*100)};
            cells.push_back(row);
        }
    }
    if (cells.empty()){
        cout << "There are no cities in deficit!\n";
    }
    else {
        printTable(colLens, headers, cells);
        cout << "There are " << cells.size() << " cities in deficit!\n";
    }
}

void Interface::exitMenu() {
    std::cout << "Closing the app...\n";
    exit(0);
}