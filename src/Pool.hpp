#pragma once

#include <list>
#include <vector>
#include "Component.hpp"
#include "StateMachine.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Task.hpp"
#include "Logger.hpp"

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

    size_t addComponent(Component* c);

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
        for (size_t i = 0; i < component_names.size(); i++) {
            if (component_names[i] == typeid(T).name()) {
                return *reinterpret_cast< std::vector<T>* >(&component_helper[i]);
            }
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

    std::map<Component*, size_t> component_ids;

    void load(std::string filename);
    void save(std::string filename);
    Component* getComponent(std::string);

    unsigned int getId(const Component *obj) const;

    template <typename T>
    unsigned int getTypeSpecificId(const T t){
        size_t cnt=0;
        for(auto i : getItems<T>()){
            if(i == t){
                return cnt;
            }
            cnt++;
        }
        throw std::runtime_error("Cannot found element " + t->getName() + " in Database");
    }

   private:
    std::vector<Component*> components;
    std::vector<StateMachine*> state_machines;
    static Pool* pool;
    Pool();
    friend class boost::serialization::access;
    void setDirty();

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
