#ifndef GECODE_COMPOSITION_MANAGEMENT_QUERY_H
#define GECODE_COMPOSITION_MANAGEMENT_QUERY_H

#include<deque>
#include<vector>

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
     * The name of this query. Must be unique.
     */
    std::string name;
    
    /**
     * These are ordered in ascending type order.
     */
    std::deque<Component> components;
    
    /**
     * Check if there is a component with that name already
     */
    bool existsComponentWithName(const std::string& name);
public:
    /**
     * Create a new query.
     */
    Query(std::string name);
    
    /**
     * Gets the components. This is readonly. To modify, e.g. configure Components, use getComponent
     */
    const std::deque<Component>& getComponents() const;
    
    /**
     * Gets the component with the given name, so it can be modified, e.g. configured.
     * 
     * \throws Exception if there is no such component.
     */
    Component& getComponent(std::string name);
    
    /**
     * Gets the index of the component with the given name.
     * 
     * \throws Exception if there is no such component.
     */
    int getComponentIndex(std::string name) const;
    
    std::string getName() const;
    
    /**
     * Inserts one component, making sure the type order is maintained.
     * It is inserted after the last component of the same type, if there is one.
     * 
     * \throws Exception if there is already a component with that name.
     */
    void addComponent(const Component& component);
    
    /**
     * Integrates a sub-query into this query. All components, except the main component are renamed.
     * Their names are prepended the subQuery's name. The main component must not exist in this query.
     * All components are copied into this query.
     * 
     * After that, the main component may be connected with other components.
     */
    void integrateSubQuery(const Query& subQuery, const std::string& mainComponentName);
    
    /**
     * This adds a new connection, if the data types are compatible.
     * 
     * \throws Exception if the datatypes differ.
     */
    void addConnection(const std::string& outCompName, const OutgoingPort& out, const std::string& inCompName, const IncomingPort& in);
    
    /**
     * Transforms the query/pool into a std::string.
     */
    std::string toString() const;
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_QUERY_H