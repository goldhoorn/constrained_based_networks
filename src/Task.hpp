#pragma once

#include <vector>
#include <map>
#include <string>

#include "Component.hpp"
#include "PortHandler.hpp"



#include <gecode/int.hh>
#include <gecode/search.hh>

namespace constrained_based_networks {

    class Pool;
/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class Task : public constrained_based_networks::Component, public constrained_based_networks::PortHandler{
protected:
    /**
     * The configuration consists of a variable sized vector of strings, naming one configuration profile.
     *
     * empty means not configured / no constraint on configuration.
     */
    //std::vector<std::string> configurations;


public:
    /**
     *Default constructor to be able to use components as map value type.
     */
    Task(Pool *pool);
    Task(Pool *pool, std::string name);

    /**
     * Construct component with type and name.
     */
    Task(std::string name, Pool *pool) ;

    SpecializedComponentBase* getSpecialized();
/*
    void addConfig(std::string name, std::string value){
        if(auto c = dynamic_cast<Task*>(this)){
            c->addConfig(name,value);
        }else{
            throw std::runtime_error("Called addConfig on invalid class");
        }
    }
    */


    /**
     * Components are equal if their name and type equal.
     */
    bool operator ==( const Task &comp ) const;

    /**
     * String representation of a component
     */
    std::string toString() const;

    virtual std::string getClassName() const { return "constrained_based_networks::Task"; }

#if 0
    /**
     * Get the configuration
     */
    const std::vector<std::string>& getConfiguration() const;

    /**
     * Push back a configuration value
     */
    void setConfiguration(const std::vector<std::string>& configuration);
#endif
    bool abstract() const{return false;};

};

} // end namespace constrained_based_networks

