#ifndef GECODE_COMPOSITION_MANAGEMENT_QUERY_H
#define GECODE_COMPOSITION_MANAGEMENT_QUERY_H

#include<deque>
#include<vector>

#include "Composition.hpp"
#include "Port.hpp"

namespace constrained_based_networks {

/**
 * A query can is any list of Compositions, which can be configured and connected somehow.
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
    std::deque<Composition> components;
    
    /**
     * Check if there is a component with that name already
     */
    bool existsCompositionWithName(const std::string& name);
public:
    /**
     * Create a new query.
     */
    Query(std::string name);
    
    /**
     * Gets the components. This is readonly. To modify, e.g. configure Compositions, use getComposition
     */
    const std::deque<Composition>& getCompositions() const;
    
    /**
     * Gets the component with the given name, so it can be modified, e.g. configured.
     * 
     * \throws Exception if there is no such component.
     */
    Composition& getComposition(std::string name);
    
    /**
     * Gets the index of the component with the given name.
     * 
     * \throws Exception if there is no such component.
     */
    int getCompositionIndex(std::string name) const;
    
    /**
     * Gets the name.
     */
    std::string getName() const;
    
    /**
     * Inserts one component, making sure the type order is maintained.
     * It is inserted after the last component of the same type, if there is one.
     * 
     * \throws Exception if there is already a component with that name.
     */
    void addComposition(const Composition& component);
    
    /**
     * Integrates a sub-query into this query. All components are copied into this query.
     * 
     * After that, the components may be connected with other components.
     * If there is a dependency on the components, it should not be done before
     * the integration.
     */
    void integrateSubQuery(const Query& subQuery);
    
    /**
     * This adds a new connection, if the data types are compatible.
     * 
     * \throws Exception if the datatypes differ, there are no such components or ports
     */
    void addConnection(const std::string& outCompName, const std::string& outPortName, const std::string& inCompName, const std::string& inPortName);
    
    /**
     * Transforms the query/pool into a std::string.
     */
    std::string toString() const;
};

} // end namespace constrained_based_networks

#endif // GECODE_COMPOSITION_MANAGEMENT_QUERY_H
