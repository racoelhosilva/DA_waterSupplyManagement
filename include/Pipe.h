#ifndef DA_WATERSUPPLYMANAGEMENT_PIPE_H
#define DA_WATERSUPPLYMANAGEMENT_PIPE_H

#include "Edge.h"
#include "ServicePoint.h"
#include "AugmentingPath.h"
#include <string>

class ServicePoint;
class AugmentingPath;

class Pipe : public Edge<std::string> {
public:
    /**
     * Constructor of the Pipe class
     * @param orig Pointer to the origin service point of the pipe
     * @param dest Pointer to the destination service point of the pipe
     * @param capacity The capacity of the pipe
     */
    Pipe(Vertex<std::string> *orig, Vertex<std::string> *dest, double capacity);

    /**
     * @brief Returns the capacity of the pipe
     * @return The capacity of the pipe
     */
    double getCapacity() const;

    /**
     * @brief Returns the origin service point of the pipe
     * @return Pointer to the origin service point of the pipe
     */
    ServicePoint *getDest() const;

    /**
     * @brief Returns the destination service point of the pipe
     * @return Pointer to the destination service point of the pipe
     */
    ServicePoint *getOrig() const;

    /**
     * @brief Returns the reverse pipe
     * @return Pointer to the reverse pipe
     */
    Pipe *getReverse() const;

    /**
     * @brief Returns the remaining flow (capacity - flow) of the pipe
     * @return The remaining flow of the pipe
     */
    double getRemainingFlow() const;

    /**
     * @brief Returns whether the pipe is hidden or not
     * @return True if the pipe was set as hidden, and false otherwise
     */
    bool isHidden() const;

    /**
     * @brief Sets whether the pipe is hidden or not
     * @param hidden True to set the pipe as selected, false to set it as unselected
     */
    void setHidden(bool hidden);

    /**
     * @brief Sets the capacity of the pipe
     * @param capacity The capacity of the pipe
     */
    void setCapacity(double capacity);

    /**
     * @brief Returns the augmenting paths associated with the pipe
     * @return Constant reference to a vector with pointers to the augmenting paths associated with the pipe
     */
    const std::vector<AugmentingPath*> &getAugmentingPaths() const;

    /**
     * @brief Returns the augmenting paths associated with the pipe
     * @return Reference to a vector with pointers to the augmenting paths associated with the pipe
     */
    std::vector<AugmentingPath*> &getAugmentingPaths();

    /**
     * @brief //TODO
     */
    void selectAugmentingPaths() const;

    /**
     * @brief Equality operator
     * @param pipe The pipe to compare with
     * @return True if the pipes have the same origin and destination vertices (service points), false otherwise
     */
    bool operator==(const Pipe &pipe) const;

private:
    std::vector<AugmentingPath*> augmentingPaths;
    bool hidden;
};

#endif //DA_WATERSUPPLYMANAGEMENT_PIPE_H
