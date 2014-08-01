#include "Component.hpp"
#include <sstream>
#include <stdexcept>

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
    for(std::map<IncomingPort, std::string>::const_iterator it = incomingConnections.begin(); it != incomingConnections.end(); ++it)
    {
        ss << it->second << it->first.toString() << " ";
    }
    ss << "]\n";
    ss << "  Outconnections: [";
    for(std::map<OutgoingPort, std::string>::const_iterator it = outgoingConnections.begin(); it != outgoingConnections.end(); ++it)
    {
        ss  << it->first.toString() << it->second << " ";
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

const IncomingPort& Component::getIncomingPortByName(const std::string& name) const
{
    for(int i = 0; i < inPorts.size(); i++)
    {
        if(inPorts[i].name == name)
        {
            return inPorts[i];
        }
    }
    throw std::runtime_error("Component getIncomingPortByName: no port named " + name);
}

const OutgoingPort& Component::getOutgoingPortByName(const std::string& name) const
{
    for(int i = 0; i < outPorts.size(); i++)
    {
        if(outPorts[i].name == name)
        {
            return outPorts[i];
        }
    }
    throw std::runtime_error("Component getOutgoingPortByName: no port named " + name);
}

const std::map<IncomingPort, std::string>& Component::getIncomingConnections() const 
{ 
    return incomingConnections;
}

void Component::putIncomingConnection(const IncomingPort& inPort, const std::string& componentName)
{
    incomingConnections[inPort] = componentName;
}

const std::map<OutgoingPort, std::string>& Component::getOutgoingConnections() const 
{ 
    return outgoingConnections;
}

void Component::putOutgoingConnection(const OutgoingPort& outPort, const std::string& componentName)
{
    outgoingConnections[outPort] = componentName;
}

} // end namespace composition_management