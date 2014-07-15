#ifndef GECODE_COMPOSITION_MANAGEMENT_QUERY_H
#define GECODE_COMPOSITION_MANAGEMENT_QUERY_H

#include<map>
#include<stdexcept>
#include<deque>
#include <sys/socket.h>

#include "component.h"
#include "port.h"

using namespace Gecode;

namespace composition_management {

/**
 * A query can is any list of Components, which can be configured and connected somehow.
 * Any actual current situation, like the component pool, can and is also modelled using this class.
 */
class Query
{
protected:
    /**
     * These are be ordered in ascending type order.
     */
    std::deque<Component> components;
public:
    /**
     * Create a new query.
     */
    Query()
    {
    }
    
    /**
     * Gets the components.
     */
    const std::deque<Component>& getComponents() const { return components; }
    
    /**
     * Inserts one component, making sure the type order is maintained.
     * It is inserted after the last component of the same type, if there is one.
     */
    void addComponent(const Component& component)
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
    
    /**
     * This adds a new connection, if the data types are compatible.
     * 
     * \throws Exception if the datatypes differ.
     */
    void addConnection(int outCompIndex, const OutgoingPort& out, int inCompIndex, const IncomingPort& in)
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
    
    std::string toString() const
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
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_QUERY_H