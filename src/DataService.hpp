#pragma once

#include <vector>
#include <map>
#include <string>

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>


#include "Port.hpp"
#include "Component.hpp"


#include <gecode/int.hh>
#include <gecode/search.hh>

namespace constrained_based_networks {

/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class DataService : public constrained_based_networks::Component{
protected:

    /**
     * The configuration consists of a variable sized vector of strings, naming one configuration profile.
     *
     * empty means not configured / no constraint on configuration.
     */
    std::vector<std::string> configurations;

    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & ar, const unsigned int version){
        /*
            ar & fullfillments;
            ar & active;
            ar & name;
            ar & configurations;
            */
    }

public:
    /**
     *Default constructor to be able to use components as map value type.
     */
    DataService();

    /**
     * Construct component with type and name.
     */
    DataService(std::string name) ;

    void addConfig(std::string name, std::string value){
        if(auto c = dynamic_cast<DataService*>(this)){
            c->addConfig(name,value);
        }else{
            throw std::runtime_error("Called addConfig on invalid class");
        }
    }
    SpecializedComponentBase *getSpecialized();

    /*
    Component* getSpecialized()
    {
        return new SpecializedComponent<DataService>(this, pool);
    }
    */

    /**
     * Components are equal if their name and type equal.
     */
    bool operator ==( const DataService &comp ) const;

    /**
     * String representation of a component
     */
    std::string toString() const;

    /**
     * Get the name
     */
    virtual std::string getName() const;

    /**
     * Set the name
     */
    void setName(const std::string& name);

    /**
     * Get the configuration
     */
    const std::vector<std::string>& getConfiguration() const;

    /**
     * Push back a configuration value
     */
    void setConfiguration(const std::vector<std::string>& configuration);

    virtual bool abstract() const{return true;};
};

} // end namespace constrained_based_networks

