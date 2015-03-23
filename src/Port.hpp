#ifndef GECODE_COMPOSITION_MANAGEMENT_PORT_H
#define GECODE_COMPOSITION_MANAGEMENT_PORT_H

#include <sstream>
#include <string>

namespace constrained_based_networks {
    
    /**
     * An incoming port defined by datatype and name.
     */
    struct IncomingPort
    {
        std::string datatype;
        std::string name;
        
        IncomingPort(const std::string& datatype, const std::string& name)
            : datatype(datatype)
            , name(name)
        { }
        
        // to use it as map key
        bool operator <( const IncomingPort &port ) const
        {
            if(name != port.name)
            {
                return name < port.name;
            }
            return datatype < port.datatype;
        }
        
        std::string toString() const
        {
            std::ostringstream ss;
            ss << ">>[" << name << ", " << datatype << "]";
            return ss.str();
        }
    };
    
    /**
     * An outgoing port defined by datatype and name.
     */
    struct OutgoingPort
    {
        std::string datatype;
        std::string name;
        
        OutgoingPort(const std::string& datatype, const std::string& name)
            : datatype(datatype)
            , name(name)
        { }
        
        // to use it as map key
        bool operator <( const OutgoingPort &port ) const
        {
            if(name != port.name)
            {
                return name < port.name;
            }
            return datatype < port.datatype;
        }
        
        std::string toString() const
        {
            std::ostringstream ss;
            ss << "[" << name << ", " << datatype << "]>>";
            return ss.str();
        }
    };
    
} // end namespace constrained_based_networks


#endif // GECODE_COMPOSITION_MANAGEMENT_PORTS_H
