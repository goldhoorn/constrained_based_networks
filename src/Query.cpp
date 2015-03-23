#include "Query.hpp"

#include <stdexcept>
#include <deque>

namespace constrained_based_networks {


Query::Query(std::string name)
    : name(name)
{
}

const std::deque<Composition>& Query::getCompositions() const 
{ 
    return components;
}

Composition& Query::getComposition(std::string name)
{
    for(std::deque<Composition>::iterator it = components.begin(); it != components.end(); ++it)
    {
        if(it->getName() == name)
        {
            return *it;
        }
    }
    throw std::runtime_error("Query getComposition: no component with that name: " + name);
}

int Query::getCompositionIndex(std::string name) const
{
    for(std::deque<Composition>::const_iterator it = components.begin(); it != components.end(); ++it)
    {
        if(it->getName() == name)
        {
            return std::distance(components.begin(), it);
        }
    }
    throw std::runtime_error("Query getCompositionIndex: no component with that name: " + name);
}

bool Query::existsCompositionWithName(const std::string& name)
{
    try
    {
        getComposition(name);
        return true;
    }
    catch(std::exception& e)
    {
        return false;
    }
}

void Query::addComposition(const Composition& component)
{
    if(existsCompositionWithName(component.getName()))
    {
        throw std::runtime_error("Query addComposition: there is already a component with that name: " + component.getName());
    }
    
    int type = component.getType();
    // insert at front if smallest type or list empty
    if(components.empty() || type < components[0].getType())
    {
        components.push_front(component);
        return;
    }
    
    // Iterate through components backwards
    for(std::deque<Composition>::reverse_iterator it = components.rbegin(); it != components.rend(); ++it)
    {
        if(it->getType() <= type)
        {
            components.insert(it.base(), component);
            break;
        }
    }
    
}

void Query::integrateSubQuery(const constrained_based_networks::Query& subQuery)
{
    // Loop through all components of the subquery
    for(std::deque<Composition>::const_iterator it = subQuery.components.begin(); it != subQuery.components.end(); ++it)
    {
        // Copy
        Composition comp = *it;
        // Rename
        //comp.setName(subQuery.name + "_" + comp.getName());
        // Also rename the components names for the connections.
        // As each component of the subquery can only be connected to those, all
        // can be renamed
//         std::map<IncomingPort, std::string> inConnections = comp.getIncomingConnections();
//         for(std::map<IncomingPort, std::string>::const_iterator it = inConnections.begin(); it != inConnections.end(); ++it)
//         {
//             comp.putIncomingConnection(it->first, subQuery.name + "_" + it->second);
//         }
//         std::map<OutgoingPort, std::string> outConnections = comp.getOutgoingConnections();
//         for(std::map<OutgoingPort, std::string>::const_iterator it = outConnections.begin(); it != outConnections.end(); ++it)
//         {
//             comp.putOutgoingConnection(it->first, subQuery.name + "_" + it->second);
//         }
        // Add to the component list (using the method to keep order!)
        addComposition(comp);
    }
}

std::string Query::getName() const
{
    return name;
}

void Query::addConnection(const std::string& outCompName, const std::string& outPortName, const std::string& inCompName, const std::string& inPortName)
{
    // find components
    Composition& inComp = getComposition(inCompName);
    Composition& outComp = getComposition(outCompName);
    
    // find ports
    const IncomingPort& in = inComp.getIncomingPortByName(inPortName);
    const OutgoingPort& out = outComp.getOutgoingPortByName(outPortName);
    
    if(out.datatype != in.datatype)
    {
        throw std::runtime_error("Query addConnection: datatypes incompatible.");
    }
    
    // add connections
    outComp.putOutgoingConnection(out, inCompName);
    inComp.putIncomingConnection(in, outCompName);
}

std::string Query::toString() const
{
    std::ostringstream ss;
    ss << "Query\n{\n";
    for(int i = 0; i < components.size(); i++)
    {
        ss << components.at(i).toString() << "\n";
    }
    ss << "}\n";
        
    return ss.str();
}


} // end namespace constrained_based_networks
