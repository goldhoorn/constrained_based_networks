#include "DataService.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"
#include "Composition.hpp"
#include "SpecializedComponent.hpp"

namespace constrained_based_networks {

DataServiceObj::DataServiceObj(Pool *pool): ComponentObj(pool) {}

DataServiceObj::DataServiceObj(const std::string &name, Pool *pool)
    : ComponentObj(pool)
{
    this->name = name;
    //auto c = new Composition(name + "_cmp", pool);
    //c->addChild(this,"virtual_main_child");
}

Component DataServiceObj::clone(Pool *p) const {
    throw std::runtime_error("Not implemented");
    return 0;
}

DataService DataServiceObj::make(Pool *pool, std::string name)
{
    auto res = std::shared_ptr<DataServiceObj>(new DataServiceObj(name, pool));
    pool->addComponent(res);
    return res;
}

bool DataServiceObj::operator ==( const DataServiceObj &comp ) const
{
    return name == comp.name;
}

#if 0
std::string DataServiceObj::toString() const
{
    std::ostringstream ss;
//    ss << "DataServiceObj " << name << " of type " << type << ".\n";
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
int DataServiceObj::getType() const
{
    return type;
}
*/

/*
const std::vector<std::string>& DataServiceObj::getConfiguration() const
{
    return configurations;
}
*/
/*
void DataServiceObj::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}
*/

std::shared_ptr<SpecializedComponentObjBase> DataServiceObj::getSpecialized(std::shared_ptr<ComponentObj> _obj, std::string name){
    DataService obj = std::dynamic_pointer_cast<DataServiceObj>(_obj);
    assert(obj.get());
    return SpecializedComponentObj<DataServiceObj>::make(obj, pool, name);
}

} // end namespace constrained_based_networks
