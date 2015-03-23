#include "Composition.hpp"
#include <sstream>
#include <stdexcept>

namespace constrained_based_networks {
    
Composition::Composition(): Component(Pool::getInstance()) {}

Composition::Composition(std::string name) 
    : Component(Pool::getInstance())
{
    this->name = name;
}

bool Composition::operator ==( const Composition &comp ) const
{
    return name == comp.name;
}

std::string Composition::toString() const
{
    std::ostringstream ss;
    ss << "Composition " << name << ".\n";
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

int Composition::getType() const 
{ 
    return 0;
}

const std::string& Composition::getName() const 
{ 
    return name;
}

void Composition::setName(const std::string& name) 
{ 
    this->name = name;
}

const std::vector<std::string>& Composition::getConfiguration() const 
{
    return configurations;
}

void Composition::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}

const std::vector<IncomingPort>& Composition::getInPorts() const 
{
    return inPorts;
}

void Composition::pushBackInPort(const IncomingPort& inPort)
{
    inPorts.push_back(inPort);
}

const std::vector<OutgoingPort>& Composition::getOutPorts() const
{ 
    return outPorts;
}

void Composition::pushBackOutPort(const OutgoingPort& outPort)
{
    outPorts.push_back(outPort);
}

const IncomingPort& Composition::getIncomingPortByName(const std::string& name) const
{
    for(int i = 0; i < inPorts.size(); i++)
    {
        if(inPorts[i].name == name)
        {
            return inPorts[i];
        }
    }
    throw std::runtime_error("Composition getIncomingPortByName: no port named " + name);
}

const OutgoingPort& Composition::getOutgoingPortByName(const std::string& name) const
{
    for(int i = 0; i < outPorts.size(); i++)
    {
        if(outPorts[i].name == name)
        {
            return outPorts[i];
        }
    }
    throw std::runtime_error("Composition getOutgoingPortByName: no port named " + name);
}

const std::map<IncomingPort, std::string>& Composition::getIncomingConnections() const 
{ 
    return incomingConnections;
}

void Composition::putIncomingConnection(const IncomingPort& inPort, const std::string& componentName)
{
    incomingConnections[inPort] = componentName;
}

const std::multimap<OutgoingPort,std::string>& Composition::getOutgoingConnections() const 
{ 
    return outgoingConnections;
}

void Composition::putOutgoingConnection(const OutgoingPort& outPort, const std::string& componentName)
{
    outgoingConnections.insert(std::pair<OutgoingPort,std::string>(outPort, componentName));
}

    
Gecode::IntVarArray Composition::getPossibleTaskAssignments(Gecode::Space *space)
{
        auto arr = Gecode::IntVarArray(*space,children.size(),0,pool->getNonAbstractCount()-1);
        return arr;
}

void Composition::addChild(Component *c,std::string name){
    children[name] = c;
}

std::list<std::pair<std::string, Component*> > Composition::getChildren(){
    std::list<std::pair<std::string, Component*> > erg;
    for(auto i : children){
        erg.push_back(std::pair<std::string,Component*>(i.first,i.second));
    }
    return erg;
};

} // end namespace constrained_based_networks
