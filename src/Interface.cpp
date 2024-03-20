#include "Interface.h"
#include "input.h"
#include "ansi.h"
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


void Interface::printOptions(const std::vector<std::string> &options, int choice) {
    std::ostringstream oss;
    oss << "     " << options[options.size()-1];
    printLine(oss.str());
    oss.str(std::to_string(choice));
    printLine(oss.str());
    for (int idx = 1; idx < options.size() - 1; idx++){
        if (idx == choice){
            std::cout << BOLD;
        }
        oss.str("");
        oss << " [" << idx << "] " << options[idx];
        printLine(oss.str());
        if (idx == choice){
            std::cout << RESET;
        }
    }
    if (0 == choice){
        std::cout << BOLD;
    }
    oss.str("");
    oss << " [0] " << options[0];
    printLine(oss.str());
    if (0 == choice){
        std::cout << RESET;
    }
}

void Interface::printTop() {
    std::string s;
    for (int _ = 0; _ < width; _++){
        s += "─";
    }
    std::cout << "┌" << s << "┐" << '\n';
    std::string title = "Water Supply Management (G15_02)";
    std::cout << "│" << std::string(24, ' ') << BLUE << BOLD << title << RESET << std::string(24, ' ') << "│" << '\n';
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
    std::vector<std::string> options =
            {"Quit",
             "Function 1",
             "Function 2",
             "Function 3",
             "Function 4",
             "Function 5",
             "Function 6",
             "Choose your operation:"};


    int choice = 0;
    Press press;
    do {
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
        case 1:
        case 2:
        case 3:
        case 4:
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

void Interface::exitMenu() {
    std::cout << "Closing the app...\n";
    exit(0);
}