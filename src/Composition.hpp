#ifndef GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
#define GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H

#include <vector>
#include <map>
#include <string>

#include "Pool.hpp"
#include "PortHandler.hpp"

#include <gecode/int.hh>
#include <gecode/search.hh>

namespace constrained_based_networks {
    
/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class Composition : public constrained_based_networks::Component, public constrained_based_networks::PortHandler{
protected:
    /**
     * The type as an int.
     */
    int type;
    
    std::map<std::string, Component*> children;

    /**
     * The configuration consists of a variable sized vector of strings, naming one configuration profile.
     * 
     * empty means not configured / no constraint on configuration. 
     */
    std::vector<std::string> configurations;
    
public:
    /**
     *Default constructor to be able to use components as map value type.
     */
    Composition();
   
    /**
     * Construct component with type and name.
     */
    Composition(std::string name) ;
   
    Gecode::IntVarArray getPossibleTaskAssignments(Gecode::Space *space);
    
    std::list<std::pair<std::string, Component*> > getChildren();
    //std::list<Component*> getChildren();

    /**
     * Components are equal if their name and type equal.
     */
    bool operator ==( const Composition &comp ) const;
    
    /**
     * String representation of a component
     */
    std::string toString() const;
    
    /**
     * Get the type
     */
    int getType() const;
    
    /**
     * Get the name
     */
    const std::string& getName() const;
    
    /**
     * Set the name
     */
    void setName(const std::string& name);
    
    /**
     * Get the configuration
     */
    const std::vector<std::string>& getConfiguration() const;
    
    /**
     * Push back a configuration value
     */
    void setConfiguration(const std::vector<std::string>& configuration);
    
    /**
     * Push back an incoming port
     */
    void addChild(Component *child, std::string name);

    bool abstract() const{return false;};
};

} // end namespace constrained_based_networks

#endif // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
