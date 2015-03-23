#include "Task.hpp"
#include <sstream>
#include <stdexcept>

namespace constrained_based_networks {
    
Task::Task(): Component(Pool::getInstance()) {}

Task::Task(Pool *pool, std::string name) 
    : Component(pool)
{
    this->name = name;
    addFullfillment(name);
}

Task::Task(std::string name) 
    : Component(Pool::getInstance())
{
    this->name = name;
    addFullfillment(name);
}

bool Task::operator ==( const Task &comp ) const
{
    return name == comp.name;
}

std::string Task::toString() const
{
    std::ostringstream ss;
//    ss << "Task " << name << " of type " << type << ".\n";
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
int Task::getType() const 
{ 
    return type;
}
*/

const std::string& Task::getName() const 
{ 
    return name;
}

void Task::setName(const std::string& name) 
{ 
    this->name = name;
}

const std::vector<std::string>& Task::getConfiguration() const 
{
    return configurations;
}

void Task::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}

const std::vector<IncomingPort>& Task::getInPorts() const 
{
    return inPorts;
}

void Task::pushBackInPort(const IncomingPort& inPort)
{
    inPorts.push_back(inPort);
}

const std::vector<OutgoingPort>& Task::getOutPorts() const
{ 
    return outPorts;
}

void Task::pushBackOutPort(const OutgoingPort& outPort)
{
    outPorts.push_back(outPort);
}

const IncomingPort& Task::getIncomingPortByName(const std::string& name) const
{
    for(int i = 0; i < inPorts.size(); i++)
    {
        if(inPorts[i].name == name)
        {
            return inPorts[i];
        }
    }
    throw std::runtime_error("Task getIncomingPortByName: no port named " + name);
}

const OutgoingPort& Task::getOutgoingPortByName(const std::string& name) const
{
    for(int i = 0; i < outPorts.size(); i++)
    {
        if(outPorts[i].name == name)
        {
            return outPorts[i];
        }
    }
    throw std::runtime_error("Task getOutgoingPortByName: no port named " + name);
}

const std::map<IncomingPort, std::string>& Task::getIncomingConnections() const 
{ 
    return incomingConnections;
}

void Task::putIncomingConnection(const IncomingPort& inPort, const std::string& componentName)
{
    incomingConnections[inPort] = componentName;
}

const std::multimap<OutgoingPort,std::string>& Task::getOutgoingConnections() const 
{ 
    return outgoingConnections;
}

void Task::putOutgoingConnection(const OutgoingPort& outPort, const std::string& componentName)
{
    outgoingConnections.insert(std::pair<OutgoingPort,std::string>(outPort, componentName));
}

    


} // end namespace constrained_based_networks
