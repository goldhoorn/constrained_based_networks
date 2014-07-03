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
     * 
     * 0 means not configured / no constraint on configuration. A number means it has to that
     * configuration / is that configuration.
     */
    std::vector<int> configuration;
    
    /**
     * All incoming ports.
     */
    std::vector<IncomingPort> inPorts;
    
    /**
     * All incoming ports.
     */
    std::vector<OutgoingPort> outPorts;
public:
    /**
     * \param configurationSize the number of things to configure
     */
    Component(int type, int configurationSize, std::vector<IncomingPort> inPorts, std::vector<OutgoingPort> outPorts) 
        : type(type)
        , inPorts(inPorts)
        , outPorts(outPorts)
    {
        for(int i = 0; i < configurationSize; i++)
        {
            configuration.push_back(0);
        }
    }
    
    int getType() { return type; }
    
    std::vector<int>& getConfiguration() { return configuration; }
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H