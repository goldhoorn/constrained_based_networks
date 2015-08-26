#include "StateMachine.hpp"
#include "Pool.hpp"
#include "SpecializedComponent.hpp"

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

void StateMachine::setStart(const Component *c){
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
//   std::cout << "hmh--- " << target->getName() << std::endl;

}

SpecializedComponentBase* StateMachine::getSpecialized()
{
    return new SpecializedComponent<StateMachine>(this, pool);
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

void StateMachine::addTransition(SpecializedComponentBase *source,  SpecializedComponentBase *target, SpecializedComponentBase *event_source, std::string ev){
    addTransition(source->getComponent(), target->getComponent(), event_source->getComponent(), ev);
}
void StateMachine::addTransition(Component *source,  SpecializedComponentBase *target, SpecializedComponentBase *event_source, std::string ev){
    addTransition(source, target->getComponent(), event_source->getComponent(), ev);
}
void StateMachine::addTransition(SpecializedComponentBase *source,  Component *target, SpecializedComponentBase *event_source, std::string ev){
    addTransition(source->getComponent(), target, event_source->getComponent(), ev);
}
void StateMachine::addTransition(SpecializedComponentBase *source,  SpecializedComponentBase *target, Component *event_source, std::string ev){
    addTransition(source->getComponent(), target->getComponent(), event_source, ev);
}
void StateMachine::addTransition(Component *source,  SpecializedComponentBase *target, Component *event_source, std::string ev){
    addTransition(source, target->getComponent(), event_source, ev);
}
void StateMachine::addTransition(SpecializedComponentBase *source,  Component *target, Component *event_source, std::string ev){
    addTransition(source->getComponent(), target, event_source, ev);
}
void StateMachine::setStart(SpecializedComponentBase *c){
    setStart(c->getComponent());
}
