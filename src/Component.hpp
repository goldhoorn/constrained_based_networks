#pragma once

#include <string>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace constrained_based_networks {

class Pool;

struct Configuration{
    std::vector<std::string> name;
    std::vector<std::string> value;

    void add(std::string name, std::string value){
        this->name.push_back(name);
        this->value.push_back(value);
    }
};

template<class T>
class SpecializedComponent : public T{
    public:
    SpecializedComponent(T *t, Pool *pool): T(*t){
    }
    Configuration configuration;
        
    void addConfig(std::string name, std::string value){
        configuration.add(name,value);
    };
};


class ConfigurationModel{
    public:
        enum Type{
            INT=0,
            DOUBLE,
            BOOL,
            STRING,

        };

        ConfigurationModel(std::string name, Type t):name(name),t(t){}

        std::string name;

        Type t;

};

class Component{
    public:
        Component(Pool *pool);
        virtual ~Component();
        unsigned int getID() const;
        virtual bool abstract() const=0;

        void addFullfillment(std::string name);
        bool isFullfilling(std::string name);
        virtual bool isActive();
        virtual void setActive(bool active=true);
        std::string getName();
       
        //Made for testing purposes
        bool isIgnored();

        void addEvent(const std::string &name);

        Component* getComponent(std::string s);

        void addProperty(const std::string &name, ConfigurationModel::Type t){
            properties.push_back(ConfigurationModel(name,t));
        }

        virtual void addConfig(std::string name, std::string value)=0;
        virtual Component* getSpecialized() =0;

        const std::vector<ConfigurationModel> &getProperties() const{
            return properties;
        }

    protected:
        /*
         * DataServices that this component fullfills
         */
        std::vector<std::string> fullfillments;
        std::vector<std::string> events;
        std::vector<ConfigurationModel> properties;

        Pool *pool;
        bool active;
        std::string name;

    private:

        unsigned int id;

    friend class Pool;
};


}
