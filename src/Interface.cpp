#include "Interface.h"
#include "input.h"
#include "ansi.h"
#include <iostream>
#include <iomanip>

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
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << buffer << RESET << "│" << '\n';
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << buffer << RESET << "│" << '\n';
    std::cout << "│" << std::string(4, ' ') << std::setw(74) << std::left << buffer << RESET << "│" << '\n';
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
    initCapture();
    std::cout << HIDE_CURSOR;
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
        case 1:
            citySelection();
            break;
        case 2:
            std::cout << readInputText();
            break;
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
    initCapture();
    std::cout << HIDE_CURSOR;
    std::vector<std::string> options =
            {"Back"};
    for (DeliverySite* ds : wsn.getDeliverySites()){
        options.push_back(ds->getCity());
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
        mainMenu();
    else
        std::cout << "\nCity " << options[choice] << " selected\n";
    mainMenu();
}

std::string Interface::readInputText(){
    initCapture();
    std::cout << SHOW_CURSOR;
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

void Interface::cityDisplay(const std::vector<DeliverySite> &cities) {

}

void Interface::exitMenu() {
    std::cout << "Closing the app...\n";
    exit(0);
}