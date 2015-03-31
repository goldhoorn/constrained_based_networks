#pragma once

#include <string>
#include <vector>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace constrained_based_networks {

class Pool;

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

        Component* getComponent(std::string s);
    protected:
        /*
         * DataServices that this component fullfills
         */
        std::vector<std::string> fullfillments;
        Pool *pool;
        bool active;
        std::string name;

    private:

        unsigned int id;

    friend class Pool;
};

}
