#pragma once

#include <list>
#include <vector>
#include "Component.hpp"
#include "StateMachine.hpp"
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

#include "Task.hpp"
#include "Logger.hpp"

namespace constrained_based_networks
{

#define ID_NIL 0
#define ID_ROOT_KNOT 1
#define ID_START 0

class Pool
{
   public:
    ~Pool();
    Pool(bool empty = false);

    std::string source_of_model;

    Pool* clone();

    //std::string (*dupFunction)(constrained_based_networks::Pool*);

    size_t addComponent(Component c);

    //    static Pool* getInstance();

    template <typename T>
    unsigned int getCount()
    {
        unsigned int cnt = 0;
        for (auto c : components) {
            auto comp = dynamic_cast<T>(c);
            if (comp != nullptr) {
                cnt++;
            }
        }
        return cnt;
    }

    void checkConsistency();

    unsigned int size() const
    {
        return components.size();
    }

    void mergeDoubles();
    //  void updateLookups();
    unsigned int getNonAbstractCount();
    unsigned int getComponentCount();

    Component operator[](unsigned int id);

    template <typename T>
    std::vector<std::shared_ptr<T>> getItems()
    {
        if(0){
            //getName is not valid for boot pointer prevent this struct called with a non-ClassSolution Object
            ((T*)(nullptr))->getName();
        }
        for (size_t i = 0; i < component_names.size(); i++) {
            if (component_names[i] == typeid(T).name()) {
                return *reinterpret_cast<std::vector<std::shared_ptr<T>>*>(&component_helper[i]);
            }
        }

        std::vector<Component> erg;
        for (auto c : components) {
            auto comp = std::dynamic_pointer_cast<T>(c);
            if (comp.get()) {
                Component entry = std::static_pointer_cast<ComponentObj>(comp);
                erg.push_back(entry);
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
    std::vector<std::vector<Component>> component_helper;

    std::map<Component, size_t> component_ids;

    Component getComponent(std::string);

    bool hasComponent(std::string);

    unsigned int getId(const Component obj) const;

    template <typename T>
    unsigned int getTypeSpecificId(const std::shared_ptr<T> t)
    {
        if(0){
            //getName is not valid for boot pointer prevent this struct called with a non-ClassSolution Object
            ((T*)(nullptr))->getName();
        }
        size_t cnt = 0;
        for (auto i : getItems<T>()) {
            if (i == t) {
                return cnt;
            }
            cnt++;
        }
        throw std::runtime_error("Cannot found element " + t->getName() + " in Database");
    }

   private:
    std::vector<Component> components;
    std::vector<StateMachine> state_machines;
    // static Pool* pool;
    void setDirty();
};
};
