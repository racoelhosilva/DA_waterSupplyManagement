#ifndef DA_WATERSUPPLYMANAGEMENT_SERVICEPOINT_H
#define DA_WATERSUPPLYMANAGEMENT_SERVICEPOINT_H

#include "Pipe.h"
#include <string>

class Pipe;

class ServicePoint : public Vertex<std::string> {
public:
    /**
     * @brief Constructor of the ServicePoint class
     * @param id Unique identifier of the service point
     * @param code Code of the service point
     */
    ServicePoint(int id, const std::string &code);

    /**
     * Returns the id (unique identifier) of the service point
     * @return ID (unique identifier) of the service point
     */
    int getId() const;

    /**
     * Returns the code of the service point
     * @return Code of the service point
     */
    std::string getCode() const;

    /**
     * @brief Returns the outgoing edges of the pipe
     * @return Vector of pointers to the pipe's outgoing pipes
     */
    std::vector<Pipe*> getAdj() const;

    /**
     * @brief Returns the incoming edges of the pipe
     * @return Vector of pointers to the pipe's incoming pipes
     */
    std::vector<Pipe*> getIncoming() const;

    /**
     * @brief Returns the parent's pipe that connects to this service point (auxiliary to graph searches)
     * @return Pointer to the parent's pipe to this service point
     */
    Pipe *getPath() const;

    /**
     * @brief Returns whether the service point is hidden or not
     * @return True if the service point is hidden, false otherwise
     */
    bool isHidden() const;

    /**
     * @brief Sets whether the service point is hidden or not
     * @param hidden True to set the service point as selected, false to set it as unselected
     */
    void setHidden(bool hidden);

    /**
     * @brief //TODO
     * @param dest
     * @param w The capacity of the pipe
     * @return
     */
    Edge<std::string> *addEdge(Vertex<std::string> *dest, double w) override;

    /**
     * @brief //TODO
     * @return
     */
    std::string getDescription() const;

    /**
     * @brief Equality operator
     * @param sp Service point
     * @return True if the service points have the same code, false otherwise
     */
    bool operator==(const ServicePoint &sp) const;

private:
    int id;
    bool hidden;
};

#endif //DA_WATERSUPPLYMANAGEMENT_SERVICEPOINT_H
