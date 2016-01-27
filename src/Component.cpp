#include "Component.hpp"
#include "Pool.hpp"
#include "Composition.hpp"
#include "Task.hpp"

using namespace constrained_based_networks;

ComponentObj::ComponentObj(Pool *_pool) : pool(_pool)
{
    assert(pool);
    max_use = 10000;//TODO fixme arbitrary large number, numerix limits max is to large for gecode, here should be the gecode cariable used// std::numeric_limits<int>::max();
    active = false;
};

ComponentObj::~ComponentObj()
{
}

Component ComponentObj::clone(Pool *p) const{
    throw std::runtime_error("THIS SHOULD NOT HAPPEN, DELETE IMPLEMENTATION");
    return 0;//(*p)[0];
}


bool ComponentObj::isActive()
{
    return active;
}

bool ComponentObj::isInPool(Pool *pool) const
{
    if (pool == this->pool) {
        for (auto c : pool->getItems<ComponentObj>()) {
            if (c.get() == this) {
                return true;
            }
        }
        std::cerr << "Pool equals but cannot find component in pool" << std::endl;
        return false;
    } else {
        return false;
    }
}

const std::string &ComponentObj::getName() const
{
    return name;
}

void ComponentObj::setActive(bool a)
{
    active = a;
    if (a && id != ID_ROOT_KNOT) {  // ID for the root-knot-composition
        auto root = std::dynamic_pointer_cast<CompositionObj>((*pool)[1]);
        auto self = pool->operator[](id);
        assert(self.get() == this);
        assert(root);
        root->addChild(self, "child_" + getName());
    }
}

void ComponentObj::addEvent(const std::string &_name)
{
    events.push_back(_name);
}

unsigned int ComponentObj::getID() const
{
    return id;
}

bool ComponentObj::isIgnored()
{
    return (getName().find("Syskit::Component") != std::string::npos) || (getName().find("Syskit::Composition") != std::string::npos) || (getName().find("Roby::Task") != std::string::npos) ||
           (getName().find("Syskit::DataService") != std::string::npos) ||
           //        (getName().find("Control") != std::string::npos) ||
           //        (getName().find("Particle") != std::string::npos) ||
           (false);
}

const std::vector<std::string> &ComponentObj::getFullfillments() const
{
    return fullfillments;
}

void ComponentObj::addFullfillment(std::string str)
{
    if (isIgnored()) return;
    for (auto s : fullfillments) {
        if (s == str) return;
    }
    fullfillments.push_back(str);
}

bool ComponentObj::isFullfilling(std::string _name)
{

    if (_name == getName()) return true;  // Trivial check

    for (auto v : fullfillments) {
        if (v == _name) {
            return true;
        }
    }
    return false;
}
