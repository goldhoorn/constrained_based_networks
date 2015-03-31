#include "Component.hpp"
#include "Pool.hpp"


using namespace constrained_based_networks;

Component::Component(Pool *pool):
    pool(pool)
{
    pool->addComponent(this);
    active=false;
}

Component::~Component(){
}

bool Component::isActive(){
    return active;
};

std::string Component::getName(){
    return name;
}

void Component::setActive(bool a){
    active = a;
}
        

unsigned int Component::getID() const{
    return id;
};

bool Component::isIgnored(){
    return
//        (getName().find("Hbridge") != std::string::npos) ||
//        (getName().find("Control") != std::string::npos) ||
//        (getName().find("Particle") != std::string::npos) ||
        (false);
}

void Component::addFullfillment(std::string str){
    if(isIgnored())return;
    fullfillments.push_back(str);
}

bool Component::isFullfilling(std::string name){

    if(name == getName()) return true; //Trivial check

    for(auto v : fullfillments){
        if( v == name){
            return true;
        }
    }
    return false;
}
