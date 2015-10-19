#ifndef GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
#define GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H

#include <vector>
#include <map>
#include <string>

#include "PortHandler.hpp"
#include "Component.hpp"

#include <gecode/int.hh>
#include <gecode/search.hh>

namespace constrained_based_networks
{

typedef std::map<std::string, std::string> Forwards;

/**
 * A component is uniquely defined by its type and name. It can be configured. It has output and input ports,
 * on which it can be connected to other components.
 */
class Composition : public constrained_based_networks::Component, public constrained_based_networks::PortHandler
{
   protected:
    /**
     * The type as an int.
     */
    int type;

    std::map<std::string, Component *> children;
   private:
    std::map<Component *, Forwards> argument_forwards;
    std::map<Component *, Forwards> event_forwards;

//    size_t cmp_id;

    /**
     * The configuration consists of a variable sized vector of strings, naming one configuration profile.
     *
     * empty means not configured / no constraint on configuration.
     */
    //    std::vector<std::string> configurations;

   public:
    /**
     *Default constructor to be able to use components as map value type.
     */
    Composition(Pool *pool);

    /**
     * Construct component with type and name.
     */
    Composition(std::string name, Pool *pool);

    virtual Forwards getArgumentForwards(Component *child);
    virtual Forwards getEventForwards(Component *child);

    virtual std::string getClassName() const { return "constrained_based_networks::Composition"; }

    SpecializedComponentBase *getSpecialized();
#if 0
    void addConfig(std::string name, std::string value)
    {
        if (auto c = dynamic_cast<Composition *>(this)) {
            c->addConfig(name, value);
        } else {
            throw std::runtime_error("Called addConfig on invalid class");
        }
    }
#endif

    Gecode::IntVarArray getPossibleTaskAssignments(Gecode::Space *space);

    void addArgumentForwards(std::string child, std::string source, std::string target);
    void addEventForwards(std::string child, std::string source, std::string target);

    virtual std::vector<std::pair<std::string, Component *>> getChildren();
    // std::list<Component*> getChildren();

    /**
     * Components are equal if their name and type equal.
     */
    bool operator==(const Composition &comp) const;

    std::vector<std::string> unsolveableChildren();

    /* Limit a child usage to a specific component */
    void addConstraint(std::string child, std::string target);

    /** Get the Composition ID, NOT the id within the pool*/
//    size_t getCmpID() const;

    /**
     * String representation of a component
     */
    std::string toString() const;

    /**
     * Get the type
     */
    int getType() const;

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
    /**
     * Push back an incoming port
     */
    void addChild(Component *child, std::string name);
    void replaceChild(Component *child, std::string name);

    bool abstract() const
    {
        return false;
    };
};

}  // end namespace constrained_based_networks

#endif  // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
