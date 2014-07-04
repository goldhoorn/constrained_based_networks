#ifndef GECODE_COMPOSITION_MANAGEMENT_QUERY_H
#define GECODE_COMPOSITION_MANAGEMENT_QUERY_H

#include<map>
#include<vector>
#include <sys/socket.h>

#include "component.h"
#include "port.h"

using namespace Gecode;

namespace composition_management {

/**
 * A query can is any list of Components, which can be configured and connected somehow.
 * Any actual current situation, like the component pool, can and is also modelled using this class.
 */
class Query{
protected:
    /**
     * These must be ordered in ascending type order. TODO ensure that!
     */
    std::vector<Component> components;
public:
    /**
     * Create a new query.
     */
    Query(std::vector<Component> components)
        : components(components)
    {
    }
    
    std::vector<Component>& getComponents() { return components; }
    
    /**
     * This adds a new connection, if the data types are compatible
     */
    void addConnection(Component& outComp,OutgoingPort& out, Component& inComp, IncomingPort& in)
    {
        // TODO exceptions
        assert(out.datatype == in.datatype);
        outComp.getOutgoingConnections()[out] = inComp;
        inComp.getIncomingConnections()[in] = outComp;
    }
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_QUERY_H