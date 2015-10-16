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

Pool::Pool()
{
    new Task(this, "NIL-Task");
    auto c = new Composition("root-knot",this);
    c->setActive(true);
}

Component *Pool::getComponent(std::string name)
{
    for (auto v : components) {
        if (v->getName() == name) {
            return v;
        }
    }
    throw std::invalid_argument(name + " is unknown as component");
}

void Pool::mergeDoubles()
{
    std::vector<Component *> new_components;
    for (size_t i = 0; i < components.size(); i++) {
        auto &c = components[i];
        auto spec = dynamic_cast<SpecializedComponentBase *>(c);
        if (spec) {
            bool valid = true;
            Component *base = 0;
            for (auto &existing : new_components) {
                bool name_equals = false;

                if (auto s2 = dynamic_cast<SpecializedComponentBase *>(existing)) {
                    name_equals = spec->getName(true) == s2->getName(true);
                    // std::cout << spec->getComponent()->getName() << " and " << s2->getComponent()->getName() << std::endl;
                } else {
                    // name_equals = spec->getComponent()->getName() == existing->getName();
                    name_equals = spec->getName(true) == existing->getName();
                }

                if (name_equals) {
                    // auto cmp = dynamic_cast<SpecializedComponent<Composition> *>(existing);
                    auto cmp = dynamic_cast<SpecializedComponentBase *>(existing);
                    if (!cmp) {
                        std::cout << "Got base component" << std::endl;
                        base = existing;
                        // We found the base-class of this specialized one
                    } else {
                        if (cmp->configuration == spec->configuration) {
                            // Skip this it already is part of the database, we can stop here
                            if (c->isActive()) {  // Don't do this here, this would cause other specilaized one to start
                                existing->setActive(true);
                            }
                            valid = false;
                            //                            break;
                        }
                    }
                }
            }
            if (valid) {
                if (base) {
                    auto spec_new = base->getSpecialized();
                    spec_new->configuration = spec->configuration;
                    new_components.push_back(dynamic_cast<Component *>(spec_new));
                } else {
                    throw std::runtime_error("Cannot re-add new component, base-class is unknown");
                }
            }
        } else {
            new_components.push_back(c);
        }
    }

    for (size_t i = 0; i < new_components.size(); i++) {
        auto &c = new_components[i];
        // printf("Component: %s, %u, %lu\n",c->getName().c_str(),c->id, i);
        if (auto spec = dynamic_cast<SpecializedComponentBase *>(c)) {
            spec->id = i;
        } else {
            c->id = i;
        }
    }

    // Cleanup orginal pointer of inherited objects
    for (size_t i = 0; i < new_components.size(); i++) {
        auto &c = new_components[i];
        if (auto spec = dynamic_cast<SpecializedComponentBase *>(c)) {
            c->id = spec->getOrginal()->getID();
        }
    }

    // This needs to be done BEFORE updatInternals of the StateMachines
    // is called. The updateInternals uses (this) pool to search
    // for corresponding components
    components = new_components;
    setDirty();

    // Cleanup orginal pointer of state-machines
    for (size_t i = 0; i < new_components.size(); i++) {
        if (auto sm = dynamic_cast<StateMachine *>(new_components[i])) {
            sm->updateInternals(this);
        }
    }

    // Sainitry check, should not needed after update the SMs
    setDirty();
}

void Pool::setDirty()
{
    component_helper.clear();
    component_names.clear();
    component_ids.clear();
}

size_t Pool::addComponent(Component *c)
{
    auto b = dynamic_cast<SpecializedComponentBase *>(c);
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

unsigned int Pool::getId(const Component *obj) const
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
    for (int i = 0; i < components.size(); i++) {
        if (!components[i]->abstract()) {
            count++;
        }
    }
    //    printf("We have %u non-abstract components\n",count);
    return count;
}

Component *Pool::operator[](unsigned int id)
{
    return components[id];
}

void Pool::save(std::string filename)
{
    std::ofstream ofs(filename);
    boost::archive::text_oarchive oa(ofs);
    oa << *this;
    ofs.close();
}

void Pool::load(std::string filename)
{
    std::ifstream ifs(filename);
    boost::archive::text_iarchive ia(ifs);
    ia >> (*this);
    ifs.close();

    //    std::cout << "Loading finish" << std::endl;
    //    for (auto c : components) {
    //        std::cout << c->getName() << std::endl;
    //    }
}
};
