//
// Created by bruno on 3/31/24.
//

#ifndef DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H
#define DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H


#include <vector>
#include "Pipe.h"

class Pipe;

/**
 * @brief Class that represents an augmenting path
 */
class AugmentingPath {
public:
    /**
     * @brief Constructor of the AugmentingPath class
     */
    AugmentingPath();

    /**
     * @brief Adds another pipe to the augmenting path
     * @param pipe Pointer to the pipe to add to the augmenting path
     * @param direct Indicates if the pipe is direct in the augmenting path or reversed
     */
    void addPipe(Pipe *pipe, bool direct);

    /**
     * @brief Returns the pipes in the augmenting path
     * @return Vector with the pairs of pipes and booleans (indicating the direction) in the augmenting path
     */
    const std::vector<std::pair<Pipe *, bool>> &getPipes() const;

    /**
     * @brief Returns the capacity of the augmenting path
     * @return Total capacity of the augmenting path
     */
    double getCapacity() const;

    /**
     * @brief Checks if the augmenting path is selected
     * @return True if the path is selected, False otherwise
     */
    bool isSelected() const;

    /**
     * @brief Sets the selected flag to the value passed
     * @param selected Boolean value to update to the flag
     */
    void setSelected(bool selected);

private:
    std::vector<std::pair<Pipe *, bool>> pipes;
    double capacity;
    bool selected;
};


#endif //DA_WATERSUPPLYMANAGEMENT_AUGMENTINGPATH_H
