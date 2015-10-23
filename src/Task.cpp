#include "Task.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"
#include "SpecializedComponent.hpp"

namespace constrained_based_networks {

Task::Task(Pool *pool): Component(pool) {}

Task::Task(Pool *pool, std::string name)
    : Component(pool)
{
    this->name = name;
    addFullfillment(name);
}

Task::Task(std::string name, Pool *pool)
    : Component(pool)
{
    this->name = name;
    addFullfillment(name);
}

bool Task::operator ==( const Task &comp ) const
{
    return name == comp.name;
}

#if 0
std::string Task::toString() const
{
    std::ostringstream ss;
//    ss << "Task " << name << " of type " << type << ".\n";
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
int Task::getType() const
{
    return type;
}
*/

/*
const std::vector<std::string>& Task::getConfiguration() const
{
    return configurations;
}

void Task::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}
*/

SpecializedComponentBase* Task::getSpecialized()
{
    return new SpecializedComponent<Task>(this, pool);
}


} // end namespace constrained_based_networks
