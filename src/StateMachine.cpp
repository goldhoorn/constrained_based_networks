#include "StateMachine.hpp"
#include "Pool.hpp"

using namespace constrained_based_networks;

StateMachine::StateMachine(std::string name, Pool *pool):
    Component(pool)
{
    this->name = name;
}

StateMachine::~StateMachine(){
}
            
void StateMachine::setStart(std::string name){
    setStart(pool->getComponent(name));
}

void StateMachine::setStart(Component *c){
    start = c;
}


void StateMachine::addTransition(std::string s,  std::string t, std::string event_s, std::string event_name){
    auto source = pool->getComponent(s);
    auto target = pool->getComponent(t);
    auto ev_s = pool->getComponent(event_s);
    addTransition(source,target,ev_s, event_name);
}
            
void StateMachine::addTransition(Component *source,  Component *target, Component *event_source, std::string ev)
{
   transitions.push_back(Transition(source,target,event_source,ev)); 

}

std::vector<Component*> StateMachine::getStates(){
    std::vector<Component*> erg;
    for(auto tr : transitions){
        //Make sure target states are all what we need
        auto t = tr.target;
        bool contains = false;
        for(auto t1 : erg){
            if(t == t1){
                contains = true;
                break;
            }
        }
        if(!contains){
            erg.push_back(t);
        }
    }
    return erg;
}
