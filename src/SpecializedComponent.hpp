#pragma once

#include "Pool.hpp"
#include <string>
#include <vector>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>
#include "Composition.hpp"

namespace constrained_based_networks
{
class Pool;

struct Configuration : public std::map<std::string, std::string>
{
    //    std::vector<std::string> name;
    //    std::vector<std::string> value;

    void add(std::string _name, std::string _value)
    {
        operator[](_name) = _value;
    }
};

class SpecializedComponentObjBase : private boost::noncopyable
{
   public:
    SpecializedComponentObjBase()
    {
    }
    virtual ~SpecializedComponentObjBase()
    {
    }

    Configuration configuration;

    void addConfig(std::string name, std::string value)
    {
        configuration.add(name, value);
    };

    virtual constrained_based_networks::Component getComponent() = 0;
    virtual Component getOrginal() = 0;
    virtual bool isActive() = 0;
    virtual void setActive(bool active = true) = 0;
    virtual unsigned int getID() const = 0;
    virtual const std::string& getName(bool b) const = 0;
    virtual const std::string& getName() const = 0;

    std::map<std::string, Component> replaced_children;

    unsigned int id;
    /*
    operator const Component*() const{
        std::cerr << "FUSEL" << std::endl;
        return 0;
    };
    */

    friend class Pool;
};

typedef std::shared_ptr<SpecializedComponentObjBase> SpecializedComponentBase;

template <class T>
class SpecializedComponentObj : public SpecializedComponentObjBase, public T
{
   private:
    SpecializedComponentObj(std::shared_ptr<T> t, Pool* pool, std::string name)
        : T(*t.get())
        , pool(pool)
    {
        // classScope = false;
        org = t;
        active = false;
        //        std::cout << "Should add a specialized component with name: '" << name << "' and its base-class '" << t->getName() << "'" << std::endl;
    };

   public:
    static SpecializedComponentBase make(std::shared_ptr<T> t, Pool* pool, std::string name)
    {
        auto c = new SpecializedComponentObj<T>(t,pool,name);
        if (name.empty()) {
            std::stringstream s;
            s << t->T::getName() << "_" << pool->size();
            c->specialized_name = s.str();
        } else {
            if (pool->hasComponent(name)){
                throw std::invalid_argument("Cannot add Specialized component " + name + ", it already exists");
            }
            c->specialized_name = name;
        }

        auto ptr = std::shared_ptr<SpecializedComponentObj<T> >(c);
        auto c_ptr =  std::static_pointer_cast<ComponentObj>(ptr);
        pool->addComponent(c_ptr);
        return std::static_pointer_cast<SpecializedComponentObjBase>(ptr);
    }

    virtual Component clone(Pool* p) const
    {
        (void)p;
        throw std::runtime_error("Implement me");
        return 0;
    };

    virtual bool isActive()
    {
        //        std::cerr << "Is Active on specialized called " << active << " on " << getName(false) << " " << dynamic_cast<SpecializedComponentObjBase*>(this) << std::endl;
        return active;
    };

    virtual void setActive(bool active = true)
    {
        //       std::cerr << "----------------############ Set active on specialized called " << active << std::endl;
        //      std::cerr << "on " << getName(false) << " " << dynamic_cast<SpecializedComponentObjBase*>(this) << std::endl;
        this->active = active;
        if (active && id != ID_ROOT_KNOT) {  // ID for the root-knot-composition
            assert((*pool)[id].get() == this);
            Composition c = std::dynamic_pointer_cast<CompositionObj>((*pool)[1]);
            assert(c);
            c->addChild((*pool)[id], getName(true));
        }
        //        std::cerr << " " << active << " " << this->active << " " << T::active  << std::endl;
    }

    virtual ~SpecializedComponentObj() {};

    virtual constrained_based_networks::Component getComponent()
    {
        assert((*pool)[id].get() == this);
        return (*pool)[id];
    };


    virtual const std::string& getName() const
    {
        return getName(false);
    }

    virtual const std::string& getName(bool base) const
    {
        if (base) return T::getName();
        return specialized_name;
    }

    // This is ugly helper function to recover the IDs from mergeComponent
    // should not used somewhere else, a cast should be sufficent
    virtual constrained_based_networks::Component getOrginal()
    {
        return org;
    };

    virtual unsigned int getID() const
    {
        return SpecializedComponentObjBase::id;
        /*
      if (classScope) {
        return T::getID();
      } else {
        return SpecializedComponentObjBase::id;
      }
    */
    };

   private:
    std::shared_ptr<T> org;
    bool active;
    Pool* pool;
    std::string specialized_name;
    //  bool classScope;
};

template <class T>
using SpecializedComponent = std::shared_ptr<SpecializedComponentObj<T> >;

};
