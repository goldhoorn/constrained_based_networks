#include "component.h"
#include <sstream>

namespace composition_management {
    
Component::Component() {}

Component::Component(int type, std::string name) 
    : type(type)
    , name(name)
{
}

bool Component::operator ==( const Component &comp ) const
{
    return type == comp.type && name == comp.name;
}

std::string Component::toString() const
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

int Component::getType() const 
{ 
    return type;
}

const std::string& Component::getName() const 
{ 
    return name;
}

void Component::setName(const std::string& name) 
{ 
    this->name = name;
}

const std::vector<std::string>& Component::getConfiguration() const 
{
    return configuration;
}

void Component::setConfiguration(const std::vector<std::string>& configuration)
{
    this->configuration = configuration;
}

const std::vector<IncomingPort>& Component::getInPorts() const 
{
    return inPorts;
}

void Component::pushBackInPort(const IncomingPort& inPort)
{
    inPorts.push_back(inPort);
}

const std::vector<OutgoingPort>& Component::getOutPorts() const
{ 
    return outPorts;
}

void Component::pushBackOutPort(const OutgoingPort& outPort)
{
    outPorts.push_back(outPort);
}

const std::map<IncomingPort, const Component*>& Component::getIncomingConnections() const 
{ 
    return incomingConnections;
}

void Component::putIncomingConnection(const IncomingPort& inPort, const Component* component)
{
    incomingConnections[inPort] = component;
}

const std::map<OutgoingPort, const Component*>& Component::getOutgoingConnections() const 
{ 
    return outgoingConnections;
}

void Component::putOutgoingConnection(const OutgoingPort& outPort, const Component* component)
{
    outgoingConnections[outPort] = component;
}

} // end namespace composition_management