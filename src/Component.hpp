#pragma once

#include <string>
#include <list>
#include <vector>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/noncopyable.hpp>
#include <graph_analysis/GraphAnalysis.hpp>
//#include "SpecializedComponent.hpp"
//#include <graph_analysis/lemon/Graph.hpp>

namespace constrained_based_networks
{

class Pool;
class SpecializedComponentObjBase;
class ComponentObj;

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

    std::string typeAsString()
    {
        switch (t) {
            case INT:
                return "ConfigurationModel::INT";
            case DOUBLE:
                return "ConfigurationModel::DOUBLE";
            case BOOL:
                return "ConfigurationModel::BOOL";
            case STRING:
                return "ConfigurationModel::STRING";
            default:
                throw std::runtime_error("Invalid type");
        }
    }
    std::string name;

    Type t;
};

class ComponentObj : public graph_analysis::Vertex
{
   public:
    ComponentObj(Pool *pool);

    virtual ~ComponentObj();
    virtual unsigned int getID() const;
    virtual bool abstract() const = 0;

    void addFullfillment(std::string name);
    bool isFullfilling(std::string name);
    virtual bool isActive();
    virtual void setActive(bool active = true);
    virtual const std::string &getName() const;

    virtual std::shared_ptr<ComponentObj> clone(Pool *p) const = 0;

    std::string toString() const
    {
        return getName();
    }

    virtual std::string getClassName() const
    {
        return "constrained_based_networks::ComponentObj";
    }

    virtual const std::string &getLabel() const
    {
        return getName();
    }

    // Made for testing purposes
    bool isIgnored();

    void addEvent(const std::string &name);

    std::shared_ptr<ComponentObj> getComponent(std::string s);

    void addProperty(const std::string &_name, std::string t)
    {
        if (t == "ConfigurationModel::DOUBLE") {
            addProperty(_name, ConfigurationModel::DOUBLE);
        } else if (t == "ConfigurationModel::BOOL") {
            addProperty(_name, ConfigurationModel::BOOL);
        } else if (t == "ConfigurationModel::INT") {
            addProperty(_name, ConfigurationModel::INT);
        } else if (t == "ConfigurationModel::STRING") {
            addProperty(_name, ConfigurationModel::STRING);
        } else {
            throw std::runtime_error("unnsuported config type: " + t);
        }
    }

    void addProperty(const std::string &_name, ConfigurationModel::Type t)
    {
        properties.push_back(ConfigurationModel(_name, t));
    }

    // virtual void addConfig(std::string name, std::string value)=0;
    //virtual SpecializedComponentBase getSpecialized(std::shared_ptr<ComponentObj> obj, std::string name="") = 0;
    virtual std::shared_ptr<SpecializedComponentObjBase>  getSpecialized(std::shared_ptr<ComponentObj> obj, std::string name="") = 0;

    ConfigurationModel::Type getProperty(std::string _name) const
    {
        for (auto p : properties) {
            if (p.name == _name) {
                return p.t;
            }
        }
        throw std::runtime_error("Could not find property on underlaying task: " + _name + " on task " + getName());
    }
    const std::vector<ConfigurationModel> &getProperties() const
    {
        return properties;
    }
/*
    void prepareDelete(){
        self.reset();
    }
    graph_analysis::Vertex::Ptr getPtr()
    {
        //TODO hacky  please check why we need to check the pointer consistency here
        if (self.get() == nullptr || self.get() != this ) {
            self = graph_analysis::Vertex::Ptr(this);
        }
        return self;
    }
    */

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

    const std::vector<std::string> & getFullfillments() const;

    const std::vector<std::string> &getEvents() const
    {
        return events;
    }

    bool isInPool(Pool *pool) const;

   protected:
    /*
     * DataServices that this component fullfills
     */
    std::vector<std::string> fullfillments;
    std::vector<std::string> events;
    std::vector<ConfigurationModel> properties;
    //graph_analysis::Vertex::Ptr self;

    std::map<std::string, std::map<std::string, std::string>> saved_conf;

    Pool *pool;
    bool active;
    std::string name;

   private:
    unsigned int id;

    friend class Pool;
};

typedef std::shared_ptr<ComponentObj> Component;

}
