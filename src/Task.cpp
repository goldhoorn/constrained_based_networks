#include "Task.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"

namespace constrained_based_networks {
    
Task::Task(): Component(Pool::getInstance()) {}

Task::Task(Pool *pool, std::string name) 
    : Component(pool)
{
    this->name = name;
    addFullfillment(name);
}

Task::Task(std::string name) 
    : Component(Pool::getInstance())
{
    this->name = name;
    addFullfillment(name);
}

bool Task::operator ==( const Task &comp ) const
{
    return name == comp.name;
}

std::string Task::toString() const
{
    std::ostringstream ss;
//    ss << "Task " << name << " of type " << type << ".\n";
    ss << "  Configuration: [";
    for(int i = 0; i < configurations.size(); i++)
    {
        ss << configurations[i] << " ";
    }
    ss << "]\n";
        
    return ss.str();
}
/*
int Task::getType() const 
{ 
    return type;
}
*/

const std::string& Task::getName() const 
{ 
    return name;
}

void Task::setName(const std::string& name) 
{ 
    this->name = name;
}

const std::vector<std::string>& Task::getConfiguration() const 
{
    return configurations;
}

void Task::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}


} // end namespace constrained_based_networks
