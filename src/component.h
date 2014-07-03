#ifndef GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
#define GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H

#include <gecode/int.hh>
#include<vector>

#include "port.h"

using namespace Gecode;

namespace composition_management {
   
class Component {
protected:
    /**
     * TODO use enum instead
     */
    int type;
    
    /**
     * The configuration consists of a variable sized vector of IntVars.
     * TODO there should be support for any data types, not just int.
     */
    std::vector<IntVar> configuration;
    
    /**
     * All incoming ports.
     */
    std::vector<IncomingPort> inPorts;
    
    /**
     * All incoming ports.
     */
    std::vector<OutgoingPort> outPorts;
public:
    Component(int type, std::vector<IntVar> configuration, std::vector<IncomingPort> inPorts, std::vector<OutgoingPort> outPorts) 
        : type(type)
        , configuration(configuration)
        , inPorts(inPorts)
        , outPorts(outPorts)
    {
    }
    
    int getType() { return type; }
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H