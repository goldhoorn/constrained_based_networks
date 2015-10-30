#include "PortHandler.hpp"
#include <sstream>
#include <stdexcept>

namespace constrained_based_networks {


PortHandler::~PortHandler(){
    for(auto p : inPorts){
        delete p;
    }
    for(auto p : outPorts){
        delete p;
    }
}

bool PortHandler::operator ==( const PortHandler &comp ) const
{
    (void)comp;
//    return name == comp.name;
    throw std::runtime_error("Implement portHandler operator==");
    //TODO implement me
    return false;
}

#if 0
std::string PortHandler::toString() const
{
    std::ostringstream ss;
    ss << "  Inports: [";
    for(int i = 0; i < inPorts.size(); i++)
    {
        ss << inPorts[i]->toString() << " ";
    }
    ss << "]\n";
    ss << "  Outports: [";
    for(int i = 0; i < outPorts.size(); i++)
    {
        ss << outPorts[i]->toString() << " ";
    }
    ss << "]\n";
    ss << "  Inconnections: [";
    for(std::map<InputPort, std::string>::const_iterator it = incomingConnections.begin(); it != incomingConnections.end(); ++it)
    {
        ss << it->second << it->first.toString() << " ";
    }
    ss << "]\n";
    ss << "  Outconnections: [";
    for(std::multimap<OutputPort, std::string>::const_iterator it = outgoingConnections.begin(); it != outgoingConnections.end(); ++it)
    {
        ss  << it->first.toString() << it->second << " ";
    }
    ss << "]\n";

    return ss.str();
}
#endif

const std::vector<InputPort*>& PortHandler::getInPorts() const
{
    return inPorts;
}

void PortHandler::createInput(const std::string name, const std::string type){
    inPorts.push_back(new InputPort(type,name));
}

void PortHandler::createOutput(const std::string name, const std::string type){
    outPorts.push_back(new OutputPort(type,name));
}

const std::vector<OutputPort*>& PortHandler::getOutPorts() const
{
    return outPorts;
}


const InputPort& PortHandler::getInputPortByName(const std::string& name) const
{
    for(const auto &p : inPorts)
    {
        if(p->name == name)
        {
            return *p;
        }
    }
    throw std::runtime_error("PortHandler getInputPortByName: no port named " + name);
}

const OutputPort& PortHandler::getOutputPortByName(const std::string& name) const
{
    for(const auto &p : outPorts)
    {
        if(p->name == name)
        {
            return *p;
        }
    }
    throw std::runtime_error("PortHandler getOutputPortByName: no port named " + name);
}

const std::map<InputPort, std::string>& PortHandler::getIncomingConnections() const
{
    return incomingConnections;
}

void PortHandler::putIncomingConnection(const InputPort& inPort, const std::string& componentName)
{
    incomingConnections[inPort] = componentName;
}

const std::multimap<OutputPort,std::string>& PortHandler::getOutgoingConnections() const
{
    return outgoingConnections;
}

void PortHandler::putOutgoingConnection(const OutputPort& outPort, const std::string& componentName)
{
    outgoingConnections.insert(std::pair<OutputPort,std::string>(outPort, componentName));
}


} // end namespace constrained_based_networks
