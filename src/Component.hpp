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
    Configuration c;
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
   
        virtual Component* getSpecialized() =0;

        void addConfig(std::string name, std::string value);

    protected:
        /*
         * DataServices that this component fullfills
         */
        Configuration configuration;
        std::vector<std::string> fullfillments;
        std::vector<std::string> events;
        Pool *pool;
        bool active;
        std::string name;

    private:

        unsigned int id;

    friend class Pool;
};


}
