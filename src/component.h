#ifndef GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
#define GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H

#include <gecode/int.hh>
#include<vector>

#include <sstream>

#include "port.h"

using namespace Gecode;

namespace composition_management {

// TODO split component and query into cpp and h
    
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
    Component() {}
    
    /**
     * Construct component with type and name.
     */
    Component(int type, std::string name) 
        : type(type)
        , name(name)
    {
    }
    
    /**
     * Components are equal if their name and type equal.
     */
    bool operator ==( const Component &comp ) const
    {
        return type == comp.type && name == comp.name;
    }
    
    /**
     * String representation of a component
     */
    std::string toString() const
    {
        std::ostringstream ss;
        ss << "Component " << name << " of type " << type << ".\n";
        ss << "  Configuration: [";
        for(int i = 0; i < configuration.size(); i++)
        {
            ss << configuration[i] << " ";
        }
        ss << "]\n";
        ss << "  Inports: [";
        for(int i = 0; i < inPorts.size(); i++)
        {
            ss << inPorts[i].toString() << " ";
        }
        ss << "]\n";
        ss << "  Outports: [";
        for(int i = 0; i < outPorts.size(); i++)
        {
            ss << outPorts[i].toString() << " ";
        }
        ss << "]\n";
        ss << "  Inconnections: [";
        for(std::map<IncomingPort, const Component*>::const_iterator it = incomingConnections.begin(); it != incomingConnections.end(); ++it)
        {
            ss << it->second->getName() << it->first.toString() << " ";
        }
        ss << "]\n";
        ss << "  Outconnections: [";
        for(std::map<OutgoingPort, const Component*>::const_iterator it = outgoingConnections.begin(); it != outgoingConnections.end(); ++it)
        {
            ss  << it->first.toString() << it->second->getName() << " ";
        }
        ss << "]\n";
         
        return ss.str();
    }
    
    /**
     * Get the type
     */
    int getType() const { return type; }
    
    /**
     * Get the name
     */
    const std::string& getName() const { return name; }
    
    /**
     * Set the name
     */
    void setName(const std::string& name) { this->name = name; }
    
    /**
     * Get the configuration
     */
    const std::vector<std::string>& getConfiguration() const { return configuration; }
    
    /**
     * Push back a configuration value
     */
    void pushBackConfiguration(const std::string& configurationStr)
    {
        configuration.push_back(configurationStr);
    }
    
    /**
     * Get the incoming ports
     */
    const std::vector<IncomingPort>& getInPorts() const { return inPorts; }
    
    /**
     * Push back an incoming port
     */
    void pushBackInPort(const IncomingPort& inPort)
    {
        inPorts.push_back(inPort);
    }
    
    /**
     * Get the outgoing ports
     */
    const std::vector<OutgoingPort>& getOutPorts() const { return outPorts; }
    
    /**
     * Push back an outgoing port
     */
    void pushBackOutPort(const OutgoingPort& outPort)
    {
        outPorts.push_back(outPort);
    }
    
    /**
     * Get the incoming connections
     */
    const std::map<IncomingPort, const Component*>& getIncomingConnections() const { return incomingConnections; }
    
    /**
     * Put an incoming connection into the map
     */
    void putIncomingConnection(const IncomingPort& inPort, const Component* component)
    {
        incomingConnections[inPort] = component;
    }
    
    /**
     * Get the outgoing connections
     */
    const std::map<OutgoingPort, const Component*>& getOutgoingConnections() const { return outgoingConnections; }
    
    /**
     * Put an outgoing connection into the map
     */
    void putOutgoingConnection(const OutgoingPort& outPort, const Component* component)
    {
        outgoingConnections[outPort] = component;
    }
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H