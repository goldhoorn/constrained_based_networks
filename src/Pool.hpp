#pragma once

#include <list>
#include <vector>
#include "Component.hpp"

namespace constrained_based_networks {

class Pool{
    public:
        void addComponent(Component *c);
        static Pool* getInstance();
        unsigned int getNonAbstractCount();
        unsigned int getComponentCount();
        Component* operator[](unsigned int id);
        template<typename T> std::list<T> getItems(){
            std::list<T> erg;

            for(auto c : components){
                auto comp = dynamic_cast<T>(c);
                if(comp != nullptr){
                    erg.push_back(comp);
                }
            }
            return erg;
        }

        unsigned int getId(const Component*) const;
    private:
        std::vector<Component*> components;
        static Pool *pool;
        Pool();
};
        


};
