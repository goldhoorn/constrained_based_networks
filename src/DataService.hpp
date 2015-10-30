#pragma once

#include <vector>
#include <map>
#include <string>

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

public:
    /**
     *Default constructor to be able to use components as map value type.
     */
    DataService(Pool *pool);
    
    virtual Component* clone(Pool *p) const;

    /**
     * Construct component with type and name.
     */
    DataService(const std::string &name, Pool *pool) ;

    void addConfig(std::string name, std::string value){
        if(auto c = dynamic_cast<DataService*>(this)){
            c->addConfig(name,value);
        }else{
            throw std::runtime_error("Called addConfig on invalid class");
        }
    }
    SpecializedComponentBase *getSpecialized(std::string name="");

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
//    std::string toString() const;
  
    virtual std::string getClassName() const { return "constrained_based_networks::DataService"; }

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

