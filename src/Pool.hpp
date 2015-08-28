#pragma once

#include <list>
#include <vector>
#include "Component.hpp"
#include "StateMachine.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Task.hpp"

namespace constrained_based_networks {

#define ID_NIL 0
#define ID_ROOT_KNOT 1
#define ID_START 0

class Pool {
   public:
    Pool(std::string filename) {
        if (pool) throw std::runtime_error("Pool already created");
        pool = this;
        load(filename);
    }

    void addComponent(Component* c);

    static Pool* getInstance();

    template <typename T>
    unsigned int getCount() {
        unsigned int cnt = 0;
        for (auto c : components) {
            auto comp = dynamic_cast<T>(c);
            if (comp != nullptr) {
                cnt++;
            }
        }
        return cnt;
    }

    unsigned int size() const { return components.size(); }

    void mergeDoubles();
    //  void updateLookups();
    unsigned int getNonAbstractCount();
    unsigned int getComponentCount();

    Component* operator[](unsigned int id);

    template <typename T>
    std::vector<T> getItems(bool first = true) {
        int id = -1;
        for (size_t i = 0; i < component_names.size(); i++) {
            if (component_names[i] == typeid(T).name()) {
                id = i;
            }
        }
        if (id != -1) {
            /*
            void* foo = &component_helper[id];
            std::vector<T>* bla = (std::vector<T>*)foo;
            return *bla;
            */

            //return *((std::vector<T>*)((void*)&component_helper[id]));
            return *reinterpret_cast< std::vector<T>* >(&component_helper[id]);
        }

        std::vector<Component*> erg;
        for (auto c : components) {
            T comp = dynamic_cast<T>(c);
            if (comp != nullptr) {
                erg.push_back(comp);
            }
        }

        // Trivial check to prevent generated zero results for components that
        // does not exist
        // if(erg.size() == 0) return std::vector<T>();

        component_helper.push_back(erg);
        component_names.push_back(typeid(T).name());

        // Now we filled out databse, let the conversion be done recursivly
        return getItems<T>();
    }

    std::vector<std::string> component_names;
    std::vector<std::vector<Component*>> component_helper;

    void load(std::string filename);
    void save(std::string filename);
    Component* getComponent(std::string);
    unsigned int getId(const Component*) const;

   private:
    std::vector<Component*> components;
    std::vector<StateMachine*> state_machines;
    static Pool* pool;
    Pool();
    friend class boost::serialization::access;

    template <class Archive>
    void serialize(Archive& ar, const unsigned int version) {
        for (Component* f : components) {
            auto task = dynamic_cast<constrained_based_networks::Task*>(f);
            if (task) {
                std::cerr << "Writing a task" << std::endl;
                ar&* task;
            }
        }
    }
};
};
