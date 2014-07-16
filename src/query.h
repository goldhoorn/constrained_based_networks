#ifndef GECODE_COMPOSITION_MANAGEMENT_QUERY_H
#define GECODE_COMPOSITION_MANAGEMENT_QUERY_H

#include<deque>

#include "component.h"
#include "port.h"

namespace composition_management {

/**
 * A query can is any list of Components, which can be configured and connected somehow.
 * Any actual current situation, like the component pool, can and is also modelled using this class.
 */
class Query
{
protected:
    /**
     * These are be ordered in ascending type order.
     */
    std::deque<Component> components;
public:
    /**
     * Create a new query.
     */
    Query();
    
    /**
     * Gets the components.
     */
    const std::deque<Component>& getComponents() const;
    
    /**
     * Inserts one component, making sure the type order is maintained.
     * It is inserted after the last component of the same type, if there is one.
     */
    void addComponent(const Component& component);
    
    /**
     * This adds a new connection, if the data types are compatible.
     * 
     * \throws Exception if the datatypes differ.
     */
    void addConnection(int outCompIndex, const OutgoingPort& out, int inCompIndex, const IncomingPort& in);
    
    /**
     * Transforms the query/pool into a std::string.
     */
    std::string toString() const;
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_QUERY_H