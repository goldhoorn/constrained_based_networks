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
     * 
     */
    std::string name;
    
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
    Component(std::string name, std::vector<IntVar> configuration, std::vector<IncomingPort> inPorts, std::vector<OutgoingPort> outPorts) 
        : name(name)
        , configuration(configuration)
        , inPorts(inPorts)
        , outPorts(outPorts)
    {
    }
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H