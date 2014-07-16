#include "query.h"

#include<stdexcept>
#include<deque>

namespace composition_management {


Query::Query()
{
}

const std::deque<Component>& Query::getComponents() const 
{ 
    return components;
}

Component& Query::getComponent(int index)
{
    return components[index];
}

void Query::addComponent(const Component& component)
{
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

void Query::addConnection(int outCompIndex, const OutgoingPort& out, int inCompIndex, const IncomingPort& in)
{
    if(out.datatype != in.datatype)
    {
        throw std::runtime_error("Query addConnection: datatypes incompatible.");
    }
    Component& inComp = components[inCompIndex];
    Component& outComp = components[outCompIndex];
    
    outComp.putOutgoingConnection(out, &inComp);
    inComp.putIncomingConnection(in, &outComp);
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