#include "Pool.hpp"

#include "Composition.hpp"
#include "DataService.hpp"
#include "Task.hpp"
#include <fstream>
#include "SpecializedComponent.hpp"

namespace constrained_based_networks
{

/*
Pool *Pool::pool = 0;

Pool *Pool::getInstance()
{
    if (pool == 0) {
        pool = new Pool();
        new Task(pool, "NIL-Task");
        auto c = new Composition("root-knot");
        c->setActive(true);
    }
    return pool;
}
*/

/*
void Pool::updateLookups(){
    getItems<Composition*>();
    getItems<Component*>();
    getItems<Task*>();
    getItems<SpecializedComponentBase*>();
}
*/

Pool::~Pool()
{

};

void Pool::checkConsistency()
{
    for (auto c : getItems<ComponentObj>()) {
        if (c != operator[](c->getID())) {
            std::cout << "Failig id: " << c->getID() << " for component with name: " << c->getName() << std::endl;
            auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c);
            if (spec.get()) {
                std::cout << "It is a specialized one: " << spec->getID() << " and base id: " << spec->getComponent()->getID() << std::endl;
            }
            throw std::runtime_error("Pool is inconsistent to IDs");
        }
    }

    std::string cmp_name;
    std::string child_name;
    std::string child_role;
    try
    {
        for (auto c : components) {
            auto cmp = std::dynamic_pointer_cast<CompositionObj>(c);
            if (cmp.get()) {
                cmp_name = cmp->getName();
                for (auto child : cmp->getChildren()) {
                    child_name = child.second->getName();
                    child_role = child.first;
                    // This line would throw a runtime error if it cannot be found
                    auto result = getComponent(child.second->getName());
                    (void)result;
                }
            }
        }
    }
    catch (std::invalid_argument e)
    {
        std::cout << "Pool consistency failed, cannot find either child or compopsition:" << std::endl;
        std::cout << "Composition: " << cmp_name << std::endl;
        std::cout << "\tChild: " << child_name << " in role: " << child_role << std::endl;
        throw std::runtime_error("Pool is inconsistent");
    }
}

Pool::Pool(bool empty)
{
    if (!empty) {
        auto t = TaskObj::make(this, "NIL-Task");
        auto c = CompositionObj::make(this, "root-knot");
        c->setActive(true);

        // Prevent memory leak:
        //        t->getPtr();
        //        c->getPtr();
    }
}

bool Pool::hasComponent(std::string name)
{
    for (auto v : components) {
        // std::cout << "Have component wirh name: " << v->getName() << std::endl;
        if (v->getName() == name) {
            return true;
        }
    }
    return false;
}

Component Pool::getComponent(std::string name)
{
    for (auto v : components) {
        // std::cout << "Have component wirh name: " << v->getName() << std::endl;
        if (v->getName() == name) {
            return v;
        }
    }
    throw std::invalid_argument(name + " is unknown as component");
}

void Pool::mergeDoubles()
{
    checkConsistency();
    /*
    std::cout << "Pool before merge:" << std::endl;
    for (size_t i = 0; i < components.size(); i++) {
        std::cout << "\t- " << components[i]->getName() << std::endl;
    }
    */

    std::vector<Component> new_components;
    for (size_t i = 0; i < components.size(); i++) {
        auto &c = components[i];
        //        std::cout << "Checking: " << c->getName() << std::endl;
        auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c);
        if (spec.get()) {
            bool valid = true;
            Component base = 0;
            for (auto &existing : new_components) {
                bool name_equals = false;
                auto s2 = std::dynamic_pointer_cast<SpecializedComponentObjBase>(existing);
                if (s2.get()) {
                    name_equals = spec->getName(true) == s2->getName(true);
                    // std::cout << spec->getComponent()->getName() << " and " << s2->getComponent()->getName() << std::endl;
                } else {
                    // name_equals = spec->getComponent()->getName() == existing->getName();
                    name_equals = spec->getName(true) == existing->getName();
                }

                // If the name equals we have on both sides a specialized component, so we have to check the configuration
                if (name_equals) {
                    auto cmp = std::dynamic_pointer_cast<SpecializedComponentObjBase>(existing);
                    if (!cmp) {
                        // We found the base-class of this specialized one
                        // Okay this existing is the base component for spec
                        // std::cout << "Got base component" << std::endl;
                        base = existing;
                    } else {
                        // This mean it's a different specialized component with the same base-class
                        if (cmp->configuration == spec->configuration && cmp->isActive() == spec->isActive() && cmp->replaced_children == spec->replaced_children) {
                            //                          std::cout << "Got a equal component between " << cmp->getName() << " and " << spec->getName() << std::endl;
                            // Skip this it already is part of the database, we can stop here
                            /*
                            if (c->isActive()) {  // Don't do this here, this would cause other specilaized one to start
                                existing->setActive(true);
                                std::cout << "!!!!!!!!!         Discarding active component  !!!!!!!!!!!!!!!" << cmp->getName() << " and " << spec->getName() << std::endl;
                                std::cout << cmp << " and " << spec << std::endl;
                            }
                            */
                            valid = false;
                            //                            break;
                        }
                    }
                }
            }
            if (valid) {
                if (base) {
                    /*
                    // TODO re-thing if this makes sense here, why was this added this re-specialization?!
                    auto spec_new = base->getSpecialized();
                    std::cout << "Adding specialized: " << spec_new->getName() << std::endl;
                    spec_new->configuration = spec->configuration;
                    spec_new->setActive(spec->isActive());
                    //new_components.push_back(dynamic_cast<Component *>(spec_new));
                    */
                    new_components.push_back(c);
                } else {
                    throw std::runtime_error("Cannot re-add new component, base-class is unknown");
                }
            }
        } else {
            //           std::cout << "Adding non specialied: " << c->getName() << std::endl;
            new_components.push_back(c);
        }
    }
    /*
    std::cout << "Pool after merge:" << std::endl;
    for (size_t i = 0; i < new_components.size(); i++) {
        std::cout << "\t- " << new_components[i]->getName() << std::endl;
    }
    */

    for (size_t i = 0; i < new_components.size(); i++) {
        auto &c = new_components[i];
        // printf("Component: %s, %u, %lu\n",c->getName().c_str(),c->id, i);
        auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c);
        if (spec.get()) {
            spec->id = i;
        } else {
            c->id = i;
        }
    }

    // Cleanup orginal pointer of inherited objects
    for (size_t i = 0; i < new_components.size(); i++) {
        auto &c = new_components[i];
        auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c);
        if (spec.get()) {
            c->id = spec->getOrginal()->getID();
        }
    }

    // This needs to be done BEFORE updatInternals of the StateMachines
    // is called. The updateInternals uses (this) pool to search
    // for corresponding components
    components = new_components;
    setDirty();

    // Cleanup orginal pointer of state-machines
    for (size_t i = 0; i < components.size(); i++) {
        auto sm = std::dynamic_pointer_cast<StateMachineObj>(components[i]);
        if (sm.get()) {
            sm->updateInternals(this);
        }
    }

    // Sainitry check, should not needed after update the SMs
    setDirty();

    // Cleanup orginal pointer of state-machines
    for (size_t i = 0; i < components.size(); i++) {
        auto sm = std::dynamic_pointer_cast<CompositionObj>(components[i]);
        if (sm) {
            sm->updateInternals(this);
        }
    }

    // Sainitry check, should not needed after update the SMs
    setDirty();
    checkConsistency();
}

void Pool::setDirty()
{
    component_helper.clear();
    component_names.clear();
    component_ids.clear();
}

Pool *Pool::clone()
{
    Pool *res = new Pool();
    throw std::runtime_error("IMPLEMENT ME");
    return res;
}

size_t Pool::addComponent(Component c)
{
    if (hasComponent(c->getName())) {
        throw std::runtime_error("Cannot add a component with name '" + c->getName() + "' a second time");
    }

    auto b = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c);
    size_t id = components.size();
    if (b) {
        b->id = id;
    } else {
        c->id = id;
    }
    components.push_back(c);
    setDirty();
    return id;
}

unsigned int Pool::getId(const Component obj) const
{
    return obj->getID();
    /*
        unsigned int erg=0;
        for(auto c : components){
            if(c == obj){
                return  erg;
            }
            erg++;
        }
        throw std::invalid_argument("Cannot get index for requested object");
    */
}

unsigned int Pool::getComponentCount()
{
    return components.size();
}

unsigned int Pool::getNonAbstractCount()
{
    unsigned int count = 0;
    for (const auto &c : components) {
        if (c->abstract()) {
            count++;
        }
    }
    //    printf("We have %u non-abstract components\n",count);
    return count;
}

Component Pool::operator[](unsigned int id)
{
    return components[id];
}
};
