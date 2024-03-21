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

void Interface::printOptionsCity(const std::vector<std::string> &options, int choice) {
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << options[options.size()-1] << "│" << '\n';

    for (int idx = 1; idx < options.size() - 1; idx++){
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
    if (choice == 0){
        std::cout << "│" << BOLD << RED << " [0] " << RESET << BOLD << std::setw(73) << std::left << options[0] << RESET "│" << '\n';
    }
    else {
        std::cout << "│" << RED << " [0] " << RESET << FAINT << std::setw(73) << std::left << options[0] << RESET << "│" << '\n';
    }
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

    int choice = 1;
    Press press;
    do {
        std::cout << RESET << CLEAR_SCREEN << MOVE_CURSOR(0,0) << RESET;
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
            citySelection();
            break;
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

void Interface::citySelection() {
    std::cout << "\033[?25l" ;
    initCapture();
    std::vector<std::string> options =
            {"Back"};
    for (DeliverySite* ds : wsn.getDeliverySites()){
        options.push_back(ds->getCity());
    }
    options.push_back("Choose the city:");

    int choice = 1;
    Press press;
    do {
        std::cout << RESET << CLEAR_SCREEN << MOVE_CURSOR(0,0) << RESET;
        printTop();
        printOptionsCity(options, choice);
        printBottom();
        press = getNextPress();
        if (press == UP) {choice -= 1; choice += (options.size()-1);}
        else if (press == DOWN) {choice += 1;}
        choice = choice % (options.size()-1);
    } while (press != RET);

    endCapture();
    switch (choice) {
        case 0:
            mainMenu();
            break;
        default:
            std::cout << "\nCity " << options[choice] << " selected\n";
            break;
    }
    mainMenu();
}

void Interface::exitMenu() {
    std::cout << "Closing the app...\n";
    exit(0);
}