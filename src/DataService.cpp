#include "DataService.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"
#include "Composition.hpp"
#include "SpecializedComponent.hpp"

namespace constrained_based_networks {

DataService::DataService(Pool *pool): Component(pool) {}

DataService::DataService(const std::string &name, Pool *pool)
    : Component(pool)
{
    this->name = name;
    auto c = new Composition(name + "_cmp", pool);
    c->addChild(this,"virtual_main_child");
}

Component* DataService::clone(Pool *p) const {
    auto ds = new DataService(name,p);
    return ds;
}

bool DataService::operator ==( const DataService &comp ) const
{
    return name == comp.name;
}

#if 0
std::string DataService::toString() const
{
    std::ostringstream ss;
//    ss << "DataService " << name << " of type " << type << ".\n";
/*
    ss << "  Configuration: [";
    for(int i = 0; i < configurations.size(); i++)
    {
        ss << configurations[i] << " ";
    }
    ss << "]\n";
  */
    return ss.str();
}
#endif

/*
int DataService::getType() const
{
    return type;
}
*/

/*
const std::vector<std::string>& DataService::getConfiguration() const
{
    return configurations;
}
*/
/*
void DataService::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}
*/

SpecializedComponentBase *DataService::getSpecialized(std::string name){
    return new SpecializedComponent<DataService>(this, pool,name);
}

} // end namespace constrained_based_networks
