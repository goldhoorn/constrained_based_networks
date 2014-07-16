#ifndef GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
#define GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H

#include<vector>
#include<map>
#include <string>

#include "port.h"

namespace composition_management {
    
/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class Component {
protected:
    /**
     * The type as an int.
     */
    int type;
    
    /**
     * Must be unique. This must be ensured by the user of the library!
     */
    std::string name;
    
    /**
     * The configuration consists of a variable sized vector of strings, naming one configuration profile.
     * 
     * empty means not configured / no constraint on configuration. 
     */
    std::vector<std::string> configuration;
    
    /**
     * All incoming ports.
     */
    std::vector<IncomingPort> inPorts;
    
    /**
     * All outgoing ports.
     */
    std::vector<OutgoingPort> outPorts;
    
    /**
     * The incoming connections.
     */
    std::map<IncomingPort, const Component*> incomingConnections;
    
    /**
     * The outgoing connections.
     */
    std::map<OutgoingPort, const Component*> outgoingConnections;
public:
    /**
     *Default constructor to be able to use components as map value type.
     */
    Component();
    
    /**
     * Construct component with type and name.
     */
    Component(int type, std::string name) ;
    
    /**
     * Components are equal if their name and type equal.
     */
    bool operator ==( const Component &comp ) const;
    
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
    void pushBackConfiguration(const std::string& configurationStr);
    
    /**
     * Get the incoming ports
     */
    const std::vector<IncomingPort>& getInPorts() const;
    
    /**
     * Push back an incoming port
     */
    void pushBackInPort(const IncomingPort& inPort);
    
    /**
     * Get the outgoing ports
     */
    const std::vector<OutgoingPort>& getOutPorts() const;
    
    /**
     * Push back an outgoing port
     */
    void pushBackOutPort(const OutgoingPort& outPort);
    
    /**
     * Get the incoming connections
     */
    const std::map<IncomingPort, const Component*>& getIncomingConnections() const;
    
    /**
     * Put an incoming connection into the map
     */
    void putIncomingConnection(const IncomingPort& inPort, const Component* component);
    
    /**
     * Get the outgoing connections
     */
    const std::map<OutgoingPort, const Component*>& getOutgoingConnections() const;
    
    /**
     * Put an outgoing connection into the map
     */
    void putOutgoingConnection(const OutgoingPort& outPort, const Component* component);
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H