#include "DataService.hpp"
#include <sstream>
#include <stdexcept>

namespace constrained_based_networks {
    
DataService::DataService(): Component(Pool::getInstance()) {}

DataService::DataService(std::string name) 
    : Component(Pool::getInstance())
{
    this->name = name;
}

bool DataService::operator ==( const DataService &comp ) const
{
    return name == comp.name;
}

std::string DataService::toString() const
{
    std::ostringstream ss;
//    ss << "DataService " << name << " of type " << type << ".\n";
    ss << "  Configuration: [";
    for(int i = 0; i < configurations.size(); i++)
    {
        ss << configurations[i] << " ";
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
    for(std::multimap<OutgoingPort, std::string>::const_iterator it = outgoingConnections.begin(); it != outgoingConnections.end(); ++it)
    {
        ss  << it->first.toString() << it->second << " ";
    }
    ss << "]\n";
        
    return ss.str();
}
/*
int DataService::getType() const 
{ 
    return type;
}
*/

const std::string& DataService::getName() const 
{ 
    return name;
}

void DataService::setName(const std::string& name) 
{ 
    this->name = name;
}

const std::vector<std::string>& DataService::getConfiguration() const 
{
    return configurations;
}

void DataService::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}

const std::vector<IncomingPort>& DataService::getInPorts() const 
{
    return inPorts;
}

void DataService::pushBackInPort(const IncomingPort& inPort)
{
    inPorts.push_back(inPort);
}

const std::vector<OutgoingPort>& DataService::getOutPorts() const
{ 
    return outPorts;
}

void DataService::pushBackOutPort(const OutgoingPort& outPort)
{
    outPorts.push_back(outPort);
}

const IncomingPort& DataService::getIncomingPortByName(const std::string& name) const
{
    for(int i = 0; i < inPorts.size(); i++)
    {
        if(inPorts[i].name == name)
        {
            return inPorts[i];
        }
    }
    throw std::runtime_error("DataService getIncomingPortByName: no port named " + name);
}

const OutgoingPort& DataService::getOutgoingPortByName(const std::string& name) const
{
    for(int i = 0; i < outPorts.size(); i++)
    {
        if(outPorts[i].name == name)
        {
            return outPorts[i];
        }
    }
    throw std::runtime_error("DataService getOutgoingPortByName: no port named " + name);
}

const std::map<IncomingPort, std::string>& DataService::getIncomingConnections() const 
{ 
    return incomingConnections;
}

void DataService::putIncomingConnection(const IncomingPort& inPort, const std::string& componentName)
{
    incomingConnections[inPort] = componentName;
}

const std::multimap<OutgoingPort,std::string>& DataService::getOutgoingConnections() const 
{ 
    return outgoingConnections;
}

void DataService::putOutgoingConnection(const OutgoingPort& outPort, const std::string& componentName)
{
    outgoingConnections.insert(std::pair<OutgoingPort,std::string>(outPort, componentName));
}

    


} // end namespace constrained_based_networks
