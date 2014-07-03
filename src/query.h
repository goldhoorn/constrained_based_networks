#ifndef GECODE_COMPOSITION_MANAGEMENT_QUERY_H
#define GECODE_COMPOSITION_MANAGEMENT_QUERY_H

#include<map>
#include<vector>
#include <sys/socket.h>

#include "component.h"
#include "port.h"

using namespace Gecode;

namespace composition_management {

class Query{
protected:
    /**
     * These must be ordered in ascending type order.
     */
    std::vector<Component> components;
public:    
    Query(std::vector<Component> components, std::map<IncomingPort, OutgoingPort> connections)
        : components(components)
    {
    }
    
    std::vector<Component>& getComponents() { return components; }
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_QUERY_H