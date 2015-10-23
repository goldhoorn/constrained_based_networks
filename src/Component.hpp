#pragma once

#include <string>
#include <vector>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/noncopyable.hpp>
#include <graph_analysis/GraphAnalysis.hpp>
#include <graph_analysis/lemon/Graph.hpp>

namespace constrained_based_networks
{

class Pool;
class SpecializedComponentBase;
class Component;

class ConfigurationModel
{
   public:
    enum Type {
        INT = 0,
        DOUBLE,
        BOOL,
        STRING,
    };

    ConfigurationModel(std::string _name, Type _t)
        : name(_name)
        , t(_t)
    {
    }

    std::string name;

    Type t;
};

class Component : public graph_analysis::Vertex
{
   public:
    Component(Pool *pool);
    virtual ~Component();
    virtual unsigned int getID() const;
    virtual bool abstract() const = 0;

    void addFullfillment(std::string name);
    bool isFullfilling(std::string name);
    virtual bool isActive();
    virtual void setActive(bool active = true);
    virtual const std::string &getName() const;
    std::string toString() const
    {
        return getName();
    }

    virtual std::string getClassName() const
    {
        return "constrained_based_networks::Component";
    }

    virtual const std::string &getLabel() const
    {
        return getName();
    }

    // Made for testing purposes
    bool isIgnored();

    void addEvent(const std::string &name);

    Component *getComponent(std::string s);

    void addProperty(const std::string &_name, ConfigurationModel::Type t)
    {
        properties.push_back(ConfigurationModel(_name, t));
    }

    // virtual void addConfig(std::string name, std::string value)=0;
    virtual SpecializedComponentBase *getSpecialized() = 0;

    const ConfigurationModel::Type getProperty(std::string _name) const
    {
        for (auto p : properties) {
            if (p.name == _name) {
                return p.t;
            }
        }
        throw std::runtime_error("Could not find property on underlaying task");
    }
    const std::vector<ConfigurationModel> &getProperties() const
    {
        return properties;
    }

    graph_analysis::Vertex::Ptr getPtr()
    {
        if (self.get() == nullptr) {
            self = graph_analysis::Vertex::Ptr(this);
        }
        return self;
    }

    void addConfFileProperty(std::string section, std::string name, std::string value)
    {
        // TODO Model-checking?!
        saved_conf[section][name] = value;
    }

    std::string getConfFileProperty(std::string section, std::string name)
    {
        return saved_conf.at(section).at(name);
    }

    std::list<std::string> getSections()
    {
        std::list<std::string> erg;
        for (auto elem : saved_conf) {
            erg.push_back(elem.first);
        }
        return erg;
    }

    const std::vector<std::string> &getEvents() const
    {
        return events;
    }

   protected:
    /*
     * DataServices that this component fullfills
     */
    std::vector<std::string> fullfillments;
    std::vector<std::string> events;
    std::vector<ConfigurationModel> properties;
    graph_analysis::Vertex::Ptr self;

    std::map<std::string, std::map<std::string, std::string>> saved_conf;

    Pool *pool;
    bool active;
    std::string name;

   private:
    unsigned int id;

    friend class Pool;
};
}
