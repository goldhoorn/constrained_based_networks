#include "Composition.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"

namespace constrained_based_networks {
    
Composition::Composition(): Component(Pool::getInstance()) {}

Composition::Composition(std::string name) 
    : Component(Pool::getInstance())
{
    this->name = name;
}

bool Composition::operator ==( const Composition &comp ) const
{
    return name == comp.name;
}

std::string Composition::toString() const
{
    std::ostringstream ss;
    ss << "Composition " << name << ".\n";
    ss << "  Configuration: [";
    for(int i = 0; i < configurations.size(); i++)
    {
        ss << configurations[i] << " ";
    }
    ss << "]\n";
        
    return ss.str();
}

int Composition::getType() const 
{ 
    return 0;
}

const std::string& Composition::getName() const 
{ 
    return name;
}

void Composition::setName(const std::string& name) 
{ 
    this->name = name;
}

const std::vector<std::string>& Composition::getConfiguration() const 
{
    return configurations;
}

void Composition::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}

void Composition::addChild(Component *c,std::string name){
    children[name] = c;
}

std::vector<std::string> Composition::unsolveableChildren(){
    std::vector<std::string> res;
    Pool *pool = Pool::getInstance();

    for(auto child : children){
        bool valid=false;
        for(auto provider : pool->getItems<Component*>()){
            if(provider->abstract()){
                continue;
            }

            if(provider->isFullfilling(child.second->getName())){
         //       std::cout << std::string("FULLFILLING: " + getName() + "." + child.first + " = " + child.second->getName() + "==" +  provider->getName()) << std::endl;
//                printf("GOT A FULLILLING! %s for %s\n",getName().c_str(), provider->getName().c_str());
                valid = true;
                break;
            }
        }
        if(!valid){
           // std::cout << std::string("NOT FULLFILLING:" + getName() + "." + child.first + " = " + child.second->getName()) << std::endl;

            res.push_back(getName() + "." + child.first + " = " + child.second->getName());
        }
    }
    return res;
}

Gecode::IntVarArray Composition::getPossibleTaskAssignments(Gecode::Space *space)
{
        //auto arr = Gecode::IntVarArray(*space,children.size(),0,pool->getNonAbstractCount()-1);
        auto arr = Gecode::IntVarArray(*space,children.size(),0,pool->size()-1);
        return arr;
}

std::vector<std::pair<std::string, Component*> > Composition::getChildren(){
    std::vector<std::pair<std::string, Component*> > erg;
    for(auto i : children){
        erg.push_back(std::pair<std::string,Component*>(i.first,i.second));
    }
    return erg;
};

} // end namespace constrained_based_networks
