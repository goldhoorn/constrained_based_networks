#ifndef GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
#define GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H

#include <gecode/int.hh>
#include<vector>

#include "port.h"

using namespace Gecode;

namespace composition_management {
   
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
     * The configuration consists of a variable sized vector of IntVars.
     * TODO there should be support for any data types, not just int.
     * 
     * 0 means not configured / no constraint on configuration. A number means it has to that
     * configuration / is that configuration.
     */
    std::vector<int> configuration;
    
    /**
     * All incoming ports.
     */
    std::vector<IncomingPort> inPorts;
    
    /**
     * All outgoing ports.
     */
    std::vector<OutgoingPort> outPorts;
    
    // TODO use Pointers here?
    std::map<IncomingPort, Component> incomingConnections;
    
    
    std::map<OutgoingPort, Component> outgoingConnections;
public:
    // To use as map value type
    Component() {}
    
    /**
     * \param configurationSize the number of things to configure
     */
    Component(int type, std::string name, int configurationSize, std::vector<IncomingPort> inPorts, std::vector<OutgoingPort> outPorts) 
        : type(type)
        , name(name)
        , inPorts(inPorts)
        , outPorts(outPorts)
    {
        for(int i = 0; i < configurationSize; i++)
        {
            configuration.push_back(0);
        }
    }
    
    bool operator ==( const Component &comp ) const
    {
        return type == comp.type && name == comp.name;
    }
    
    int getType() { return type; }
    
    const std::string& getName() { return name; }
    
    void setName(const std::string& name) { this->name = name; }
    
    std::vector<int>& getConfiguration() { return configuration; }
    
    std::vector<IncomingPort>& getInPorts() { return inPorts; }
    
    std::vector<OutgoingPort>& getOutPorts() { return outPorts; }
    
    std::map<IncomingPort, Component>& getIncomingConnections() { return incomingConnections; }
    
    std::map<OutgoingPort, Component>& getOutgoingConnections() { return outgoingConnections; }
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H