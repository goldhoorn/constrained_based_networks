#pragma once

#include <list>
#include <vector>
#include "Component.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Task.hpp"

namespace constrained_based_networks {

#define ID_ROOT_KNOT 1
#define ID_START 2

class Pool{
    public:
        Pool(std::string filename){ if(pool) throw std::runtime_error("Pool already created"); pool= this; load(filename);}
        void addComponent(Component *c);
        static Pool* getInstance();
        template<typename T> unsigned int getCount(){
            unsigned int cnt=0;
            for(auto c : components){
                auto comp = dynamic_cast<T>(c);
                if(comp != nullptr){
                    cnt++;
                }
            }
            return cnt;
        }

        unsigned int size() const{
            return components.size();
        }

        unsigned int getNonAbstractCount();
        unsigned int getComponentCount();
        Component* operator[](unsigned int id);
        template<typename T> std::vector<T> getItems(){
            std::vector<T> erg;
            for(auto c : components){
                auto comp = dynamic_cast<T>(c);
                if(comp != nullptr){
                    erg.push_back(comp);
                }
            }
            return erg;
        }

        void load(std::string filename);
        void save(std::string filename);
        Component * getComponent(std::string);

        unsigned int getId(const Component*) const;
    private:
        std::vector<Component*> components;
        static Pool *pool;
        Pool();
        friend class boost::serialization::access;

        template<class Archive>
        void serialize(Archive & ar, const unsigned int version){
            for(Component* f : components){
                auto task = dynamic_cast<constrained_based_networks::Task*>(f);
                if(task){
                    std::cerr << "Writing a task" << std::endl;
                    ar & *task;
                }
            }
        }
};
        


};
