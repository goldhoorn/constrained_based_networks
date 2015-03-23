#pragma once

#include <string>
#include <vector>

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
        virtual void setActive();
        std::string getName();

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
