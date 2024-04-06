#ifndef DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
#define DA_WATERSUPPLYMANAGEMENT_INTERFACE_H

#include "WaterSupplyNetwork.h"

class Interface {
public:

    /**
     * @brief Starts the interface by loading the dataset and cleaning the output file
     * @return True if the water supply network was loaded accordingly and the output file was created/resetted
     */
    bool init();

    /**
     * @brief Wrapper for the system call to clear the screen based on the Operating System
     */
    void clearScreen();

    /**
     * @brief Prints the options parts in the menu interface along with the highlighted option
     * @param options The vector of options to show in the menu
     * @param choice The option that is currently selected (highlighted)
     */
    void printMenuOptions(const std::vector<std::string> &options, int choice);


    /**
     * @brief Prints the options parts in the menu interface along with the highlighted option, but the second last option is not shown in yellow
     * @param options The vector of options to show in the menu
     * @param choice The option that is currently selected (highlighted)
     */
    void printMenuOptionsNoBottom(const std::vector<std::string> &options, int choice);

    /**
     * @brief Prints the options in the menu interface for selection purposes, features multiple pages and moving
     * @param options The vector of options to show in the menu
     * @param title The title message to write on top of each page
     * @param choice The option that is currently selected (highlighted)
     * @param page The number of the current page selected
     * @param num_pages The total number of pages for the given options
     */
    void printOptionsPages(const std::vector<std::string> &options, const std::string &title, int choice, int page, int num_pages);

    /**
     * @brief Prints the top part of the menu interface box
     */
    void printTop();

    /**
     * @brief Prints the bottom part of the menu interface box
     */
    void printBottom();

    /**
     * @brief Writes the wait message and halts until ENTER is pressed
     */
    void waitInput();

    /**
     * @brief Prints the title at the top of the display functions
     * @param title The message/title to print in the function
     */
    void printTitle(const std::string& title);

    /**
     * @brief Prints the title at the top of the display functions without clearing the screen
     * @param title The message/title to print in the function
     */
    void printTitleNoClear(const std::string& title);

    /**
     * @brief Prints the network flow returned from various functions from the wsn
     * @param flow The flow value to print
     * @param compare Whether the value should be compared to the default or not
     */
    void printNetworkFlow(double flow, bool compare = true);
    /**
     * @brief Prints the selected the pipes to be hidden in the menu
     */
    void printSelectedPipes();

    /**
     * @brief Prints the hidden pipes when showing the results
     */
    void printHiddenPipes();

    /**
     * @brief Saves the General Max Flow output for each city to the output file
     * @param title Text to be written to the file as the title
     */
    void saveGeneralMaxFlowToFile(const std::string& title);

    /**
     * @brief Saves the max flow for each city after some restrictions, compared to the normal flow
     * @param title Text to be written to the file as the title
     */
    void saveAllMaxFlowToFile(const std::string& title);

    /**
     * @brief Saves the max flow for a specific city, compared to the normal flow
     * @param city The city selected to save to the file
     * @param title Text to be written to the file as the title
     */
    void saveSingleMaxFlowToFile(const DeliverySite* city, const std::string& title);

    /**
     * @brief Saves the max flow and deficit for each city, compared to the normal flow
     * @param title Text to be written to the file as the title
     */
    void saveDeficitsToFile(const std::string& title);

    /**
     * @brief Saves the critical pipes for a specific city
     * @param title Text to be written to the file as the title
     * @param pipes The pipes selected to save to the file
     */
    void saveCriticalPipesToFile(const std::string& title, const std::vector<Pipe *> pipes);

    /**
     * @brief Saves the metrics calculated
     * @param metrics Tuple containing the values in order: Max, Mean, Variance
     */
    void saveMetricsToFile(std::tuple<double, double, double> &metrics);

    /**
     * @brief Writes just the title to the file
     * @param title Text to be written to the file as the title
     */
    void saveTitleToFile(const std::string& title);

    /**
     * @brief Menu to select whether to add more pipes or process the operation
     * @return True if the operation should be processed, False otherwise
     */
    bool pipeMenu();

    /**
     * @brief Main menu to select which function should be processed
     */
    void mainMenu();

    /**
     * @brief Menu containing the options to view direct information of the network
     */
    void informationMenu();

    /**
     * @brief Menu that is presented when exiting the program
     */
    void exitMenu();

    /**
     * @brief Menu used to select which dataset should be loaded into the program
     * @return True if the dataset was loaded correctly, False otherwise
     */
    bool datasetMenu();

    std::vector<ServicePoint *> getSortedServicePoints();

    /**
     * @brief Menu with the options that can be selected for a city
     * @return Pointer to the city selected by the user
     */
    DeliverySite * citySelection();

    /**
     * @brief Menu with the options that can be selected for a reservoir
     * @return Pointer to the reservoir selected by the user
     */
    Reservoir * reservoirSelection();

    /**
     * @brief Menu with the options that can be selected for a pumping station
     * @return Pointer to the pumping station selected by the user
     */
    PumpingStation * pumpingStationSelection();

    /**
     * @brief Menu with the options that can be selected for a service point
     * @return Pointer to the service point selected by the user
     */
    ServicePoint * allServicePointSelection();

    /**
     * @brief Menu with the options that can be selected for a service point with pipes associated
     * @return Pointer to the source service point selected by the user
     */
    ServicePoint * servicePointSelection();

    /**
     * @brief Menu with the options that can be selected for a service point where the origin is the src
     * @param src Pointer to the source service point selected by the user (origin of the pipe)
     * @return Pointer to the dest service point selected by the user
     */
    ServicePoint * servicePointSelection(ServicePoint *src);

    /**
     * @brief Reads input text from the user
     * @return A string buffed with the text written by the user
     */
    std::string readInputText();

    /**
     * @brief Menu used to display/read text from the user
     * @param buffer Current text read from the user, character by character
     */
    void printWriteBuffer(const std::string &buffer);

    /**
     * @brief Prints the header for the metrics display
     */
    void printMetricsHeader();

    /**
     * @brief Prints a single table row for the metrics display
     * @param metrics Tuple containing the values in order: Max, Mean, Variance
     */
    void printMetricsRow(std::tuple<double, double, double> &metrics);

    /**
     * @brief Prints the tables for the displayed data
     * @param colLens Vector containing the size for each column in the table
     * @param headers Vector containing the headers for each column in the table
     * @param cells Vector containing the values for each row in the table
     */
    void printTable(const std::vector<int> &colLens, const std::vector<std::string> &headers, const std::vector<std::vector<std::string>> &cells);

    /**
     * @brief Displays the cities in the vector into a table
     * @param cities Vector containing the cities to be displayed
     */
    void cityDisplay(const std::vector<DeliverySite *> &cities);

    /**
     * @brief Displays a city's normal/focused flow comparison into a table
     * @param cities Vector containing the cities to be displayed
     */
    void cityDisplayComparison(const std::vector<DeliverySite *> &cities);

    /**
     * @brief Displays all cities' flow/demand comparison into a table
     */
    void displaySupplyDemand();

    /**
     * @brief Displays the effects certain restrictions for the cities that were affected
     */
    void displayServicePointEffects();

    /**
     * @brief Displays the critical pipes of a previously selected city
     * @param pipes Vector containing the critical pipes to display
     */
    void displayCriticalPipes(const std::vector<Pipe *> &pipes);

    /**
     * @brief Displays information for a set of cities
     * @param cities Vector containing the cities to display
     */
    void cityDisplayInfo(const std::vector<DeliverySite *> &cities);

    /**
     * @brief Displays information for a set of reservoirs
     * @param reservoirs Vector containing the reservoirs to display
     */
    void reservoirDisplay(const std::vector<Reservoir *> &reservoirs);

    /**
     * @brief Displays information for a set of pumping stations
     * @param pumpingStations Vector containing the pumping stations to display
     */
    void pumpingStationDisplay(const std::vector<PumpingStation *> &pumpingStations);

    /**
     * @brief Displays information for a set of pipes
     * @param servicePoint Service Point selected to view related pipes
     */
    void pipeDisplay(const ServicePoint *servicePoint);

private:
    int infoSpacing = 8;
    int width = 80;
    WaterSupplyNetwork wsn;
    double defaultNetworkFlow;
    std::unordered_map<std::string, double> cityToDefaultFlow;
    std::vector<Pipe *> selectedPipes;

    bool outputToFile = false;
    std::string fileName = "../output.txt";
};

#endif //DA_WATERSUPPLYMANAGEMENT_INTERFACE_H
