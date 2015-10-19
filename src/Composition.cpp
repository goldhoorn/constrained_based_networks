#include "Composition.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"
#include "SpecializedComponent.hpp"

namespace constrained_based_networks
{

Composition::Composition(Pool *pool) : Component(pool)
{
}

Composition::Composition(std::string name, Pool *pool) : Component(pool)
{
    this->name = name;
    //cmp_id = pool->getItems<Composition *>().size() - 1;
//    std::cout << "Creating composition: " << name << std::endl;
}

Forwards Composition::getArgumentForwards(Component *child)
{
    return argument_forwards[child];
}

Forwards Composition::getEventForwards(Component *child)
{
    return event_forwards[child];
}

bool Composition::operator==(const Composition &comp) const
{
    return name == comp.name;
}

void Composition::addArgumentForwards(std::string child, std::string source, std::string target)
{
    try{
        auto &c = children.at(child);
        argument_forwards[c][source] = target;
    }catch(std::out_of_range e){
        std::cerr << "Could not find child \""<< child << "\"" << std::endl;
        std::cerr << "Possible children are: " << std::endl;
        for(auto child : children){
            std::cerr << "- " << child.first << std::endl;
        }
        std::cerr << std::endl;
        throw e;
    }
}

void Composition::addEventForwards(std::string child, std::string source, std::string target)
{
    event_forwards[children[child]][source] =  target;
}

std::string Composition::toString() const
{
    std::ostringstream ss;
    ss << name;
    /*
    if (configurations.size() != 0){
        ss << "  Configuration: [";
        for(int i = 0; i < configurations.size(); i++)
        {
            ss << configurations[i] << " ";
        }
        ss << "]" << std::endl;
    }
    */
    return ss.str();
}

/*
size_t Composition::getCmpID() const
{
    return cmp_id;
}
*/

int Composition::getType() const
{
    return 0;
}

#if 0
const std::vector<std::string>& Composition::getConfiguration() const
{
    return configurations;
}

void Composition::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}
#endif

void Composition::replaceChild(Component *c, std::string name)
{
    if (isIgnored()) return;
    if(children.find(name) == children.end()){
        throw std::runtime_error("Cannot replace child, the requested child does not exist so far: " + name);
    }
    children[name] = c;
}

void Composition::addChild(Component *c, std::string name)
{
    if (isIgnored()) return;
    children[name] = c;

    //Creating default forwards, this can be overridden by addEventForwards
    addEventForwards(name,"failed","failed");
    addEventForwards(name,"success","failed");
    addEventForwards(name,"aborted","failed");
    addEventForwards(name,"internal_error","failed");
    addEventForwards(name,"fatal_error","failed");
}

void Composition::addConstraint(std::string child, std::string target)
{

    if (children.find(child) == children.end()) {
        throw std::invalid_argument(child + " cannot be found on " + getName());
    }
    auto c = pool->getComponent(target);
    if (!c) {
        throw std::invalid_argument(getName() + " cannot redefine child " + name);
    }
    children.erase(child);

    // addChild(pool->getComponent("DepthReader::Task"),"z");
    // addChild(pool->getComponent("DepthReader::Task"),"z");

    // TODO better do this later on CSP level instead here...
    addChild(c, child);
    //   std::cout << child << "----" << children[child]->getName() <<std::endl;
    //    children[child] = pool->getComponent(name);
}

std::vector<std::string> Composition::unsolveableChildren()
{
    std::vector<std::string> res;
    //Pool *pool = Pool::getInstance();

    for (auto child : children) {
        bool valid = false;
        for (auto provider : pool->getItems<Component *>()) {
            if (provider->abstract()) {
                continue;
            }

            if (provider->isFullfilling(child.second->getName())) {
                //       std::cout << std::string("FULLFILLING: " + getName() +
                // "." + child.first + " = " + child.second->getName() + "==" +
                // provider->getName()) << std::endl;
                //                printf("GOT A FULLILLING! %s for
                // %s\n",getName().c_str(), provider->getName().c_str());
                valid = true;
                break;
            }
        }
        if (!valid) {
            // std::cout << std::string("NOT FULLFILLING:" + getName() + "." +
            // child.first + " = " + child.second->getName()) << std::endl;

            res.push_back(getName() + "." + child.first + " = " + child.second->getName());
        }
    }
    return res;
}

Gecode::IntVarArray Composition::getPossibleTaskAssignments(Gecode::Space *space)
{
    // auto arr =
    // Gecode::IntVarArray(*space,children.size(),0,pool->getNonAbstractCount()-1);
    auto arr = Gecode::IntVarArray(*space, getChildren().size(), 0, pool->size() - 1);
    return arr;
}

std::vector<std::pair<std::string, Component *>> Composition::getChildren()
{
    std::vector<std::pair<std::string, Component *>> erg;
    for (auto i : children) {
        erg.push_back(std::pair<std::string, Component *>(i.first, i.second));
    }
    return erg;
};

SpecializedComponentBase *Composition::getSpecialized()
{
    return new SpecializedComponent<Composition>(this, pool);
}

}  // end namespace constrained_based_networks
