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
    };
    
    struct OutgoingPort
    {
        std::string datatype;
        std::string name;
        
        OutgoingPort(std::string datatype, std::string name, bool incoming)
            : datatype(datatype)
            , name(name)
        { }
    };
    
} // end namespace composition_management


#endif // GECODE_COMPOSITION_MANAGEMENT_PORTS_H