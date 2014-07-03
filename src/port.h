#ifndef GECODE_COMPOSITION_MANAGEMENT_PORT_H
#define GECODE_COMPOSITION_MANAGEMENT_PORT_H

namespace composition_management {
    
    struct IncomingPort
    {
        std::string datatype;
        std::string name;
        
        IncomingPort(std::string datatype, std::string name, bool incoming)
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
    };
    
    struct OutgoingPort
    {
        std::string datatype;
        std::string name;
        
        OutgoingPort(std::string datatype, std::string name, bool incoming)
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
    };
    
} // end namespace composition_management


#endif // GECODE_COMPOSITION_MANAGEMENT_PORTS_H