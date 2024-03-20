#include "Interface.h"
#include "input.h"
#include <iostream>
#include <sstream>
#include <iomanip>

bool Interface::init(){
    this->wsn = WaterSupplyNetwork();
    wsn.parseData("../datasetLarge/Reservoir.csv","../datasetLarge/Stations.csv","../datasetLarge/Cities.csv","../datasetLarge/Pipes.csv");
    return true;
}

void Interface::printLine(const std::string &s){
    std::cout << "│" << std::setw(width) << std::left << s << "│" << '\n';
}


void Interface::printOptions(const std::vector<std::string> &options) {
    std::ostringstream oss;
    oss << "     " << options[options.size()-1];
    printLine(oss.str());
    for (int idx = 1; idx < options.size() - 1; idx++ ){
        oss.str("");
        oss << " [" << idx << "] " << options[idx];
        printLine(oss.str());
    }
    oss.str("");
    oss << " [0] " << options[0];
    printLine(oss.str());
}

void Interface::printTop() {
    std::string s;
    for (int _ = 0; _ < width; _++){
        s += "─";
    }
    std::cout << "┌" << s << "┐" << '\n';
    std::ostringstream oss;
    oss << "                        " << "Water Supply Management (G15_02)";
    printLine(oss.str());
    oss.str("");
    printLine(oss.str());
}

void Interface::printBottom() {
    std::string s;
    for (int _ = 0; _ < width; _++){
        s += "─";
    }
    std::cout << "└" << s << "┘" << '\n';
}


bool Interface::validOption(unsigned long size, const std::string &choice) {
    return choice.size() == 1 && "0" <= choice && choice <= std::to_string(size-2);
}

int Interface::readOption(int max) {
    std::string choice;
    do {
        std::ostringstream oss;
        oss << "  Option" << ": ";
        printLine(oss.str());
        std::cin.clear();
        std::cin >> choice;
        std::cin.ignore();
    } while (!validOption(max, choice));
    return stoi(choice);
}

void Interface::mainMenu() {
    std::cout << "\033[?25l" ;
    initCapture();
    printTop();
    std::vector<std::string> options =
            {"Quit",
             "Function 1",
             "Function 2",
             "Function 3",
             "Function 4",
             "Function 5",
             "Function 6",
             "Choose your operation:"};
    printOptions(options);
    printBottom();

    //int choice = readOption(int(options.size()));
    int choice = 0;
    Press press;
    do {
        std::cout << "\033[8m" << "\x1b[2K" << "\r" << "   Option: " << choice;
        press = getNextPress();
        if (press == UP) {choice -= 1; choice += (options.size()-1);}
        else if (press == DOWN) {choice += 1;}
            choice = choice % (options.size()-1);
    } while (press != RET);

    endCapture();
    switch (choice) {
        case 1:
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            std::cout << "Choice " << choice << " selected\n";
            break;
        case 0:
            exitMenu();
            break;
    }
    mainMenu();
}

void Interface::exitMenu() {
    std::cout << "Closing the app...\n";
    exit(0);
}