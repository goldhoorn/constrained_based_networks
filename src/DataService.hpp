#pragma once

#include <vector>
#include <map>
#include <string>

#include "Port.hpp"
#include "Component.hpp"

#include <gecode/int.hh>
#include <gecode/search.hh>

namespace constrained_based_networks
{

/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class DataServiceObj : public constrained_based_networks::ComponentObj
{
   protected:
    /**
     * The configuration consists of a variable sized vector of strings, naming one configuration profile.
     *
     * empty means not configured / no constraint on configuration.
     */
    std::vector<std::string> configurations;
    /**
     *Default constructor to be able to use components as map value type.
     */
    DataServiceObj(Pool *pool);
    /**
     * Construct component with type and name.
     */
    DataServiceObj(const std::string &name, Pool *pool);

   public:
    virtual ~DataServiceObj() {};

    static std::shared_ptr<DataServiceObj> make(Pool *pool, std::string name);

    virtual Component clone(Pool *p) const;


    void addConfig(std::string name, std::string value)
    {
        if (auto c = dynamic_cast<DataServiceObj *>(this)) {
            c->addConfig(name, value);
        } else {
            throw std::runtime_error("Called addConfig on invalid class");
        }
    }
    std::shared_ptr<SpecializedComponentObjBase> getSpecialized(std::shared_ptr<ComponentObj> obj, std::string name = "");

    /*
    Component* getSpecialized()
    {
        return new SpecializedComponent<DataServiceObj>(this, pool);
    }
    */

    /**
     * Components are equal if their name and type equal.
     */
    bool operator==(const DataServiceObj &comp) const;

    /**
     * String representation of a component
     */
    //    std::string toString() const;

    virtual std::string getClassName() const
    {
        return "constrained_based_networks::DataService";
    }

    /**
     * Get the configuration
     */
    const std::vector<std::string> &getConfiguration() const;

    /**
     * Push back a configuration value
     */
    void setConfiguration(const std::vector<std::string> &configuration);

    virtual bool abstract() const
    {
        return true;
    };
};

typedef std::shared_ptr<DataServiceObj> DataService;


}  // end namespace constrained_based_networks
