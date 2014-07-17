#include "query.h"

#include<stdexcept>
#include<deque>

namespace composition_management {


Query::Query(std::string name)
    : name(name)
{
}

const std::deque<Component>& Query::getComponents() const 
{ 
    return components;
}

Component& Query::getComponent(std::string name)
{
    for(std::deque<Component>::iterator it = components.begin(); it != components.end(); ++it)
    {
        if(it->getName() == name)
        {
            return *it;
        }
    }
    throw std::runtime_error("Query getComponent: no component with that name: " + name);
}

int Query::getComponentIndex(std::string name) const
{
    for(std::deque<Component>::const_iterator it = components.begin(); it != components.end(); ++it)
    {
        if(it->getName() == name)
        {
            return std::distance(components.begin(), it);
        }
    }
    throw std::runtime_error("Query getComponentIndex: no component with that name: " + name);
}

bool Query::existsComponentWithName(const std::string& name)
{
    try
    {
        getComponent(name);
        return true;
    }
    catch(std::exception& e)
    {
        return false;
    }
}

void Query::addComponent(const Component& component)
{
    if(existsComponentWithName(component.getName()))
    {
        throw std::runtime_error("Query addComponent: there is already a component with that name: " + component.getName());
    }
    
    int type = component.getType();
    // insert at front if smallest type or list empty
    if(components.empty() || type < components[0].getType())
    {
        components.push_front(component);
        return;
    }
    
    // Iterate through components backwards
    for(std::deque<Component>::reverse_iterator it = components.rbegin(); it != components.rend(); ++it)
    {
        if(it->getType() <= type)
        {
            components.insert(it.base(), component);
            break;
        }
    }
    
}

void Query::integrateSubQuery(const composition_management::Query& subQuery)
{
    // Loop through all components of the subquery
    for(std::deque<Component>::const_iterator it = subQuery.components.begin(); it != subQuery.components.end(); ++it)
    {
        // Copy
        Component comp = *it;
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
        addComponent(comp);
    }
}

std::string Query::getName() const
{
    return name;
}

void Query::addConnection(const std::string& outCompName, const OutgoingPort& out, const std::string& inCompName, const IncomingPort& in)
{
    if(out.datatype != in.datatype)
    {
        throw std::runtime_error("Query addConnection: datatypes incompatible.");
    }
    // find components
    Component& inComp = getComponent(inCompName);
    Component& outComp = getComponent(outCompName);
    
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


} // end namespace composition_management