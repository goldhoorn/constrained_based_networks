#ifndef GECODE_COMPOSITION_MANAGEMENT_PORT_H
#define GECODE_COMPOSITION_MANAGEMENT_PORT_H

#include <sstream>
#include <string>

namespace constrained_based_networks {

    /**
     * An incoming port defined by datatype and name.
     */
    struct InputPort
    {
        std::string datatype;
        std::string name;

        InputPort(const std::string& _datatype, const std::string& _name)
            : datatype(_datatype)
            , name(_name)
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
#if 0
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
#endif
    };

    /**
     * An outgoing port defined by datatype and name.
     */
    struct OutputPort
    {
        std::string datatype;
        std::string name;

        OutputPort(const std::string& _datatype, const std::string& _name)
            : datatype(_datatype)
            , name(_name)
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
    };

} // end namespace constrained_based_networks


#endif // GECODE_COMPOSITION_MANAGEMENT_PORTS_H
