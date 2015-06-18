#pragma once

#include <string>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace constrained_based_networks {

class Pool;

class Component;

struct Configuration{
    std::vector<std::string> name;
    std::vector<std::string> value;

    void add(std::string _name, std::string _value){
        this->name.push_back(_name);
        this->value.push_back(_value);
    }
};

class SpecializedComponentBase{
    public:
    SpecializedComponentBase(){
    }
    virtual ~SpecializedComponentBase(){
    }

    Configuration configuration;
        
    void addConfig(std::string name, std::string value){
        configuration.add(name,value);
    };
//    virtual operator constrained_based_networks::Component*() = 0;
    //virtual constrained_based_networks::Component* foo() = 0;
    operator const Component*() const{
        std::cerr << "FUSEL" << std::endl;
        return 0;
    };
    
    
};

template<class T>
class SpecializedComponent : public T, public SpecializedComponentBase{
    public:
    SpecializedComponent(T *t, Pool *pool): T(*t){
    }
    
    virtual constrained_based_networks::Component* foo(){
        return this;
    };
    
    //virtual operator constrained_based_networks::Component*(){
    operator Component*() const{
        printf("Hallo gaga let's dance\n");
        return 0;
 //       return dynamic_cast<Component*>(this);
//
    }

};



class ConfigurationModel{
    public:
        enum Type{
            INT=0,
            DOUBLE,
            BOOL,
            STRING,

        };

        ConfigurationModel(std::string _name, Type _t):name(_name),t(_t){}

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

        void addProperty(const std::string &_name, ConfigurationModel::Type t){
            properties.push_back(ConfigurationModel(_name,t));
        }

        //virtual void addConfig(std::string name, std::string value)=0;
        virtual SpecializedComponentBase* getSpecialized() =0;

        const ConfigurationModel::Type getProperty(std::string _name) const{
            for(auto p : properties){
                if(p.name == _name){
                    return p.t;
                }
            }
            throw std::runtime_error("Could not find property on underlaying task");
        }
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
