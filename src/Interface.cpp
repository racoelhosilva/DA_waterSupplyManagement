#include "Interface.h"
#include <iostream>

bool Interface::init(){
    this->wsn = WaterSupplyNetwork();
    wsn.parseData("../datasetLarge/Reservoir.csv","../datasetLarge/Stations.csv","../datasetLarge/Cities.csv","../datasetLarge/Pipes.csv");
    wsn.print();
    return true;
}

void Interface::printOptions(const std::vector<std::string> &options) {
    std::cout << "     " << options[options.size()-1] << "\n";
    for (int idx = 1; idx < options.size() - 1; idx++ ){
        std::cout << " [" << idx << "] " << options[idx] << '\n';
    }
    std::cout << " [0] " << options[0] << '\n';
}

bool Interface::validOption(unsigned long size, const std::string &choice) {
    return choice.size() == 1 && "0" <= choice && choice <= std::to_string(size-2);
}

int Interface::readOption(int max) {
    std::string choice;
    do {
        std::cout << "  Option" << ": ";
        std::cin.clear();
        std::cin >> choice;
        std::cin.ignore();
    } while (!validOption(max, choice));
    return stoi(choice);
}

void Interface::mainMenu() {
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

    int choice = readOption(int(options.size()));

    switch (choice) {
        default:
            std::cout << "Choice " << choice << " selected\n";
            break;
    }
    mainMenu();
}
