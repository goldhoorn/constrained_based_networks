#pragma once

#include <vector>
#include <map>
#include <string>

#include "Port.hpp"
#include "Pool.hpp"


#include <gecode/int.hh>
#include <gecode/search.hh>

namespace constrained_based_networks {
    
/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class DataService : public constrained_based_networks::Component{
protected:
    
    /**
     * The configuration consists of a variable sized vector of strings, naming one configuration profile.
     * 
     * empty means not configured / no constraint on configuration. 
     */
    std::vector<std::string> configurations;
    
    /**
     * All incoming ports.
     */
    std::vector<constrained_based_networks::IncomingPort> inPorts;
    
    /**
     * All outgoing ports.
     */
    std::vector<constrained_based_networks::OutgoingPort> outPorts;
    
    /**
     * The incoming connections. Ports mapped to the names of the component, they are connected to.
     */
    std::map<constrained_based_networks::IncomingPort, std::string> incomingConnections;
    
    /**
     * The outgoing connections. Ports mapped to the names of the component, they are connected to.
     */
    std::multimap<constrained_based_networks::OutgoingPort, std::string> outgoingConnections;
public:
    /**
     *Default constructor to be able to use components as map value type.
     */
    DataService();
   
    /**
     * Construct component with type and name.
     */
    DataService(std::string name) ;
   

    /**
     * Components are equal if their name and type equal.
     */
    bool operator ==( const DataService &comp ) const;
    
    /**
     * String representation of a component
     */
    std::string toString() const;
    
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
     * Get the incoming ports
     */
    const std::vector<constrained_based_networks::IncomingPort>& getInPorts() const;
    
    bool abstract() const{return true;};
    
    /**
     * Gets an incoming port by name.
     * 
     * \throws Exception if there is no such port
     */
    const constrained_based_networks::IncomingPort& getIncomingPortByName(const std::string& name) const;
    
    /**
     * Push back an incoming port
     */
    void pushBackInPort(const constrained_based_networks::IncomingPort& inPort);
    
    /**
     * Get the outgoing ports
     */
    const std::vector<constrained_based_networks::OutgoingPort>& getOutPorts() const;
    
    /**
     * Gets an outgoing port by name.
     * 
     * \throws Exception if there is no such port
     */
    const constrained_based_networks::OutgoingPort& getOutgoingPortByName(const std::string& name) const;
    
    /**
     * Push back an outgoing port
     */
    void pushBackOutPort(const constrained_based_networks::OutgoingPort& outPort);
    
    /**
     * Get the incoming connections
     */
    const std::map<constrained_based_networks::IncomingPort, std::string>& getIncomingConnections() const;
    
    /**
     * Put an incoming connection into the map
     */
    void putIncomingConnection(const constrained_based_networks::IncomingPort& inPort, const std::string& componentName);
    
    /**
     * Get the outgoing connections
     */
    const std::multimap<constrained_based_networks::OutgoingPort, std::string>& getOutgoingConnections() const;
    
    /**
     * Put an outgoing connection into the map
     */
    void putOutgoingConnection(const constrained_based_networks::OutgoingPort& outPort, const std::string& componentName);
};

} // end namespace constrained_based_networks

