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
class CompositionObj : public constrained_based_networks::ComponentObj, public constrained_based_networks::PortHandler
{
   protected:
    /**
     * The type as an int.
     */
    int type;

    std::map<std::string, Component> children;

   private:
    std::map<std::string, Forwards> argument_forwards;
    std::map<std::string, Forwards> event_forwards;
    // std::map<Component *, Forwards> argument_forwards;
    // std::map<Component *, Forwards> event_forwards;
    std::list<std::string> emitations;

    //    size_t cmp_id;

    /**
     * The configuration consists of a variable sized vector of strings, naming one configuration profile.
     *
     * empty means not configured / no constraint on configuration.
     */
    //    std::vector<std::string> configurations;

   protected:
    /**
     *Default constructor to be able to use components as map value type.
     */
    CompositionObj(Pool *pool);
    /**
     * Construct component with type and name.
     */
    CompositionObj(std::string name, Pool *pool);

   public:
    static std::shared_ptr<CompositionObj> make(Pool *pool, std::string name);

    virtual ~CompositionObj()
    {
    }

    /*
     * Warning this function is sligly wrong becasue it does not separate between a child and a component,
     * rework this but this might have a larger influence
     */
    virtual Forwards getArgumentForwards(Component child, std::string name);

    virtual const std::map<std::string, Forwards> &getArgumentForwards() const;
    virtual const std::map<std::string, Forwards> &getEventForwards() const;

    /*
     * Warning this function is sligly wrong becasue it does not separate between a child and a component,
     * rework this but this might have a larger influence
     */
    virtual Forwards getEventForwards(Component child, std::string name);
    bool hasChild(Component child);

    virtual std::shared_ptr<constrained_based_networks::ComponentObj> clone(Pool *p) const;

    virtual std::string getClassName() const
    {
        return "constrained_based_networks::CompositionObj";
    }

    // SpecializedComponentBase getSpecialized(std::shared_ptr<ComponentObj> obj, std::string name="");
    std::shared_ptr<SpecializedComponentObjBase> getSpecialized(std::shared_ptr<ComponentObj> obj, std::string name = "");
#if 0
    void addConfig(std::string name, std::string value)
    {
        if (auto c = dynamic_cast<CompositionObj *>(this)) {
            c->addConfig(name, value);
        } else {
            throw std::runtime_error("Called addConfig on invalid class");
        }
    }
#endif
    Component searchCorresponding(Component c, Pool *pool);
    void updateInternals(Pool *pool);

    Gecode::IntVarArray getPossibleTaskAssignments(Gecode::Space *space);

    void addArgumentForwards(std::string child, std::string source, std::string target);
    void addEventForwards(std::string child, std::string source, std::string target);

    virtual std::vector<std::pair<std::string, Component>> getChildren();
    // std::list<Component*> getChildren();

    /**
     * Components are equal if their name and type equal.
     */
    bool operator==(const CompositionObj &comp) const;

    std::vector<std::string> unsolveableChildren();

    /* Limit a child usage to a specific component */
    void addConstraint(std::string child, std::string target);

    /** Get the CompositionObj ID, NOT the id within the pool*/
    //    size_t getCmpID() const;

    /**
     * String representation of a component
     */
    // std::string toString() const;

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
    void addChild(Component child, std::string name);
    virtual void replaceChild(Component child, std::string name);

    std::list<std::string> getEmitations()
    {
        return emitations;
    }

    void addSelfEmitations(std::string name)
    {
        emitations.push_back(name);
    }

    bool abstract() const
    {
        return false;
    };
};

typedef std::shared_ptr<CompositionObj> Composition;

}  // end namespace constrained_based_networks

#endif  // GECODE_COMPOSITION_MANAGEMENT_COMPONENT_H
