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

void Component::addFullfillment(std::string str){
    fullfillments.push_back(str);
}

bool Component::isFullfilling(std::string name){
    for(auto v : fullfillments){
        if( v == name){
            return true;
        }
    }
    return false;
}
