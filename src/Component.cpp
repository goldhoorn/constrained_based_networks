#include "Component.hpp"
#include "Pool.hpp"
#include "Composition.hpp"
#include "Task.hpp"

using namespace constrained_based_networks;

Component::Component(Pool *_pool) : pool(_pool)
{
    assert(pool);
    pool->addComponent(this);
    active = false;
    getPtr();
}

Component::~Component()
{
}

Component* Component::clone(Pool *p) const{
    throw std::runtime_error("THIS SHOULD NOT HAPPEN, DELETE IMPLEMENTATION");
    return (*p)[0];
}


bool Component::isActive()
{
    return active;
};

bool Component::isInPool(Pool *pool) const
{
    if (pool == this->pool) {
        for (auto c : pool->getItems<Component *>()) {
            if (c == this) {
                return true;
            }
        }
        std::cerr << "Pool equals but cannot find component in pool" << std::endl;
        return false;
    } else {
        return false;
    }
}

const std::string &Component::getName() const
{
    return name;
}

void Component::setActive(bool a)
{
    active = a;
    if (a && id != ID_ROOT_KNOT) {  // ID for the root-knot-composition
        auto root = dynamic_cast<Composition *>((*pool)[1]);
        assert(root);
        root->addChild(this, "child_" + getName());
    }
}

void Component::addEvent(const std::string &_name)
{
    events.push_back(_name);
}

unsigned int Component::getID() const
{
    return id;
};

bool Component::isIgnored()
{
    return (getName().find("Syskit::Component") != std::string::npos) || (getName().find("Syskit::Composition") != std::string::npos) || (getName().find("Roby::Task") != std::string::npos) ||
           (getName().find("Syskit::DataService") != std::string::npos) ||
           //        (getName().find("Control") != std::string::npos) ||
           //        (getName().find("Particle") != std::string::npos) ||
           (false);
}

const std::vector<std::string> &Component::getFullfillments() const
{
    return fullfillments;
}

void Component::addFullfillment(std::string str)
{
    if (isIgnored()) return;
    for (auto s : fullfillments) {
        if (s == str) return;
    }
    fullfillments.push_back(str);
}

bool Component::isFullfilling(std::string _name)
{

    if (_name == getName()) return true;  // Trivial check

    for (auto v : fullfillments) {
        if (v == _name) {
            return true;
        }
    }
    return false;
}
