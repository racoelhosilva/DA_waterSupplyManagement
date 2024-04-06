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
     * @brief Returns if the pipe is hidden
     * @return True if the pipe was set as hidden, and false otherwise
     */
    bool isHidden() const;

    /**
     * @brief Sets if the edge is being hidden
     * @param hidden Whether the edge is hidden or not
     */
    void setHidden(bool hidden);

    /**
     * @brief Sets the capacity of the pipe
     * @param capacity The capacity of the pipe
     */
    void setCapacity(double capacity);

    /**
     * @brief Returns a vector with pointers to the augmenting paths associated with the pipe
     * @return Vector with pointers to the augmenting paths associated with the pipe
     */
    const std::vector<AugmentingPath*> &getAugmentingPaths() const;

    std::vector<AugmentingPath*> &getAugmentingPaths();
    void selectAugmentingPaths() const;

    bool operator==(const Pipe &pipe) const;

private:
    std::vector<AugmentingPath*> augmentingPaths;
    bool hidden;
};

#endif //DA_WATERSUPPLYMANAGEMENT_PIPE_H
