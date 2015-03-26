#ifndef GECODE_COMPOSITION_MANAGEMENT_PORT_H
#define GECODE_COMPOSITION_MANAGEMENT_PORT_H

#include <sstream>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace constrained_based_networks {
    
    /**
     * An incoming port defined by datatype and name.
     */
    struct InputPort
    {
        std::string datatype;
        std::string name;
        
        InputPort(const std::string& datatype, const std::string& name)
            : datatype(datatype)
            , name(name)
        { }
        
        // to use it as map key
        bool operator <( const InputPort &port ) const
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
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
        /*
            ar & inPorts;
            ar & outPorts;
            ar & incomingConnections;
            ar & outgoingConnections;
            */
        };
    };
    
    /**
     * An outgoing port defined by datatype and name.
     */
    struct OutputPort
    {
        std::string datatype;
        std::string name;
        
        OutputPort(const std::string& datatype, const std::string& name)
            : datatype(datatype)
            , name(name)
        { }
        
        // to use it as map key
        bool operator <( const OutputPort &port ) const
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
        friend class boost::serialization::access;
        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
        /*
            ar & inPorts;
            ar & outPorts;
            ar & incomingConnections;
            ar & outgoingConnections;
            */
        };
    };
    
} // end namespace constrained_based_networks


#endif // GECODE_COMPOSITION_MANAGEMENT_PORTS_H
