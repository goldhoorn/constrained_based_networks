#ifndef GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
#define GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H

#include <gecode/int.hh>
#include<vector>

#include <sstream>

#include "port.h"

using namespace Gecode;

namespace composition_management {

// TODO refs and stuff. creation is very tedious ATM
    
/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class Component {
protected:
    /**
     * TODO use enum instead
     */
    int type;
    
    /**
     * (TODO) Must be unique
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
    
    // TODO use Pointers here?
    /**
     * The incoming connections.
     */
    std::map<IncomingPort, Component> incomingConnections;
    
    /**
     * The outgoing connections.
     */
    std::map<OutgoingPort, Component> outgoingConnections;
public:
    // To use as map value type
    Component() {}
    
    /**
     */
    Component(int type, std::string name,std::vector<IncomingPort> inPorts, std::vector<OutgoingPort> outPorts) 
        : type(type)
        , name(name)
        , inPorts(inPorts)
        , outPorts(outPorts)
    {
    }
    
    /**
     * Components are equal if their name and type equals.
     */
    bool operator ==( const Component &comp ) const
    {
        return type == comp.type && name == comp.name;
    }
    
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
        for(std::map<IncomingPort, Component>::const_iterator it = incomingConnections.begin(); it != incomingConnections.end(); ++it)
        {
            ss << it->second.getName() << it->first.toString() << " ";
        }
        ss << "]\n";
        ss << "  Outconnections: [";
        for(std::map<OutgoingPort, Component>::const_iterator it = outgoingConnections.begin(); it != outgoingConnections.end(); ++it)
        {
            ss  << it->first.toString() << it->second.getName() << " ";
        }
        ss << "]\n";
         
        return ss.str();
    }
    
    int getType() const { return type; }
    
    const std::string& getName() const { return name; }
    
    void setName(const std::string& name) { this->name = name; }
    
    std::vector<std::string>& getConfiguration() { return configuration; }
    
    std::vector<IncomingPort>& getInPorts() { return inPorts; }
    
    std::vector<OutgoingPort>& getOutPorts() { return outPorts; }
    
    std::map<IncomingPort, Component>& getIncomingConnections() { return incomingConnections; }
    
    std::map<OutgoingPort, Component>& getOutgoingConnections() { return outgoingConnections; }
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H