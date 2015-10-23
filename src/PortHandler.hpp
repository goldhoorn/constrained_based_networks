#pragma once

#include <vector>
#include <map>
#include <string>

#include "Port.hpp"


#include <gecode/int.hh>
#include <gecode/search.hh>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace constrained_based_networks {

/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class PortHandler{
protected:
    /**
     * All incoming ports.
     */
    std::vector<constrained_based_networks::InputPort*> inPorts;

    /**
     * All outgoing ports.
     */
    std::vector<constrained_based_networks::OutputPort*> outPorts;

    /**
     * The incoming connections. Ports mapped to the names of the component, they are connected to.
     */
    std::map<constrained_based_networks::InputPort, std::string> incomingConnections;

    /**
     * The outgoing connections. Ports mapped to the names of the component, they are connected to.
     */
    std::multimap<constrained_based_networks::OutputPort, std::string> outgoingConnections;

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


public:
    ~PortHandler();

    /**
     * Components are equal if their name and type equal.
     */
    bool operator ==( const PortHandler &comp ) const;

    /**
     * String representation of a component
     */
//    std::string toString() const;

    /**
     * Get the incoming ports
     */
    const std::vector<constrained_based_networks::InputPort*>& getInPorts() const;

    /**
     * Gets an incoming port by name.
     *
     * \throws Exception if there is no such port
     */
    const constrained_based_networks::InputPort& getInputPortByName(const std::string& name) const;

    /**
     * Push back an incoming port
     */
    void createInput(const std::string name, const std::string type);

    /**
     * Push back an incoming port
     */
    void createOutput(const std::string name, const std::string type);

    /**
     * Get the outgoing ports
     */
    const std::vector<constrained_based_networks::OutputPort*>& getOutPorts() const;

    /**
     * Gets an outgoing port by name.
     *
     * \throws Exception if there is no such port
     */
    const constrained_based_networks::OutputPort& getOutputPortByName(const std::string& name) const;

    /**
     * Get the incoming connections
     */
    const std::map<constrained_based_networks::InputPort, std::string>& getIncomingConnections() const;

    /**
     * Put an incoming connection into the map
     */
    void putIncomingConnection(const constrained_based_networks::InputPort& inPort, const std::string& componentName);

    /**
     * Get the outgoing connections
     */
    const std::multimap<constrained_based_networks::OutputPort, std::string>& getOutgoingConnections() const;

    /**
     * Put an outgoing connection into the map
     */
    void putOutgoingConnection(const constrained_based_networks::OutputPort& outPort, const std::string& componentName);
};

}

