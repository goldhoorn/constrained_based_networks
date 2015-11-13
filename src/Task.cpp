#include "Task.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"
#include "SpecializedComponent.hpp"

namespace constrained_based_networks
{

TaskObj::TaskObj(Pool *pool) : ComponentObj(pool)
{
}

TaskObj::TaskObj(Pool *pool, std::string name) : ComponentObj(pool)
{
    this->name = name;
    addFullfillment(name);
}

TaskObj::TaskObj(std::string name, Pool *pool) : ComponentObj(pool)
{
    this->name = name;
    addFullfillment(name);
}

Task TaskObj::make(Pool *pool, std::string name)
{
    auto res = std::shared_ptr<TaskObj>(new TaskObj(pool, name));
    pool->addComponent(res);
    return res;
}

bool TaskObj::operator==(const TaskObj &comp) const
{
    return name == comp.name;
}

Component TaskObj::clone(Pool *p) const
{
    throw std::runtime_error("IMPLEMENT ME");
    return 0;
    /*
    Component::clone(p);
    auto *c = new TaskObj(name,p);
    return c;
    */
};

#if 0
std::string TaskObj::toString() const
{
    std::ostringstream ss;
//    ss << "TaskObj " << name << " of type " << type << ".\n";
    ss << name;
    /*<< "  Configuration: [";
    for(int i = 0; i < configurations.size(); i++)
    {
        ss << configurations[i] << " ";
    }
    ss << "]";
*/
    return ss.str();
}
#endif

/*
int TaskObj::getType() const
{
    return type;
}
*/

/*
const std::vector<std::string>& TaskObj::getConfiguration() const
{
    return configurations;
}

void TaskObj::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}
*/

SpecializedComponentBase TaskObj::getSpecialized(std::shared_ptr<ComponentObj> _obj, std::string name)
{
    Task obj = std::dynamic_pointer_cast<TaskObj>(_obj);
    assert(obj.get());
    return SpecializedComponentObj<TaskObj>::make(obj, pool, name);
}

}  // end namespace constrained_based_networks
