#include "StateMachine.hpp"
#include "Pool.hpp"
#include "SpecializedComponent.hpp"
#include "Component.hpp"

using namespace constrained_based_networks;

StateMachine::StateMachine(std::string name, Pool *pool) : Composition(name, pool)
{
    addProperty("current_state", ConfigurationModel::INT);
    transitions.push_back(Transition(this,this,this,"no_start_state"));
}

StateMachine::~StateMachine()
{
}

void StateMachine::setStart(std::string name)
{
    setStart(pool->getComponent(name));
}

void StateMachine::setStart(Component *c)
{
    transitions[0] = Transition(this,c,this,"start");
}

std::vector<std::pair<std::string, Component *>> StateMachine::getChildren()
{
    std::vector<std::pair<std::string, Component *>> res;
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(this)) {
//        std::cout << "Have a specialized StateMachine " << getName() << std::endl;
        if (spec->configuration.find("current_state") != spec->configuration.end()) {
            auto current_state = atoi(spec->configuration["current_state"].c_str());
            auto states = getStates();
//            std::cout << "Current State should be: " << current_state << " and we have " << states.size() << std::endl;
            assert(0 <= current_state);
            assert(current_state < transitions.size());
            auto s = transitions[current_state].target;
            assert(s);
//            std::cout << "New child is: " << s->getName() << std::endl;
            res.push_back({"main", s});
        } else {
            if (!transitions[0].target) {
                std::cerr << "Warn it seems statemachine " << getName() << "HAs no states, cannot return children" << std::endl;
            } else {
                res.push_back({"main", transitions[0].target});
            }
        }
    } else {
        if (!transitions[0].target) {
            std::cerr << "Warn it seems statemachine " << getName() << "HAs no states, cannot return children" << std::endl;
        } else {
            res.push_back({"main", transitions[0].target});
        }
    }
    return res;
}

Component *StateMachine::searchCorresponding(Component *c, Pool *pool)
{
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(c)) {
        // search for object in DB
        for (auto pci : pool->getItems<Component *>()) {
            if (auto pc = dynamic_cast<SpecializedComponentBase *>(pci)) {
                if (pc->getName(true) == spec->getName(true) && pc->configuration == spec->configuration) {
                    //                            std::cout << "pc: " << pc << " pci: " << pci << std::endl;
                    //                            std::cout << "pc: " << sizeof(*pc) << " pci: " << sizeof(*pci)  << " " << std::abs((long int)pc-(long int)pci) << std::endl;
                    //                            throw std::runtime_error("bla");

                    //This check should not be needed
                    assert(pool->getComponent(pci->getName()) == pci);
                    return pci;
                }
            }
        }
        throw std::runtime_error("Could not find compatible component in pool");
    } else {

        //Sainity check would rase a error otherwise
        assert(pool->getComponent(c->getName()) == c);
        return c;
    }
}

void StateMachine::updateInternals(Pool *pool)
{
//    std::cout << "Update Internals for SM: " << this->getName() << std::endl;
    for (auto &t : transitions) {
        //If t.source == 0 then this is the invalid stating-state
//        if(t.source == this){
//            std::cout << "Was sourcesource: " << t.source->getName() << std::endl;
            t.source = searchCorresponding(t.source, pool);
//            std::cout << "Found for source: " << t.source->getName() << std::endl;
            t.target = searchCorresponding(t.target, pool);
            t.event_source = searchCorresponding(t.event_source, pool);
//        }
    }
#if 1 //Testing
    for (auto t : transitions) {
        pool->getComponent(t.source->getName());
        pool->getComponent(t.target->getName());
        pool->getComponent(t.event_source->getName());
    }
#endif
}

void StateMachine::addTransition(std::string s, std::string t, std::string event_s, std::string event_name)
{
    auto source = pool->getComponent(s);
    auto target = pool->getComponent(t);
    auto ev_s = pool->getComponent(event_s);
    addTransition(source, target, ev_s, event_name);
}

void StateMachine::addTransition(Component *source, Component *target, Component *event_source, std::string ev)
{
    transitions.push_back(Transition(source, target, event_source, ev));
    //   std::cout << "hmh--- " << target->getName() << std::endl;
}

SpecializedComponentBase *StateMachine::getSpecialized()
{
    return new SpecializedComponent<StateMachine>(this, pool);
}

int StateMachine::getNewState(Component *child, std::string event)
{
    for (size_t i = 0; i < transitions.size(); i++) {
        auto t = transitions[i];
        if (t.event_source == child && t.event == event) {
            return i;
        }
    }
    return -1;
}

Forwards StateMachine::getEventForwards(Component *child)
{
    auto forwards = Composition::getEventForwards(child);
    for (size_t i = 0; i < transitions.size(); i++) {
        auto t = transitions[i];
        // We only support this so far:
        if(t.event_source != t.source){
            std::cout << "Error we only support the source-state as a source for events" << std::endl;
            std::cout << "The was:" << t.event_source->getName() << " and " << t.source->getName() << std::endl;
            assert(false);
        }
        if (t.event_source == child) {
            std::stringstream s;
            s << "transition-" << i;
            forwards[t.event] = s.str();  // This overloads the normal propagation
        }
    }
    return forwards;
}

std::vector<Component *> StateMachine::getStates()
{
    std::vector<Component *> erg;
    for (auto tr : transitions) {
        // Make sure target states are all what we need
        auto t = tr.target;
        bool contains = false;
        for (auto t1 : erg) {
            if (t == t1) {
                contains = true;
                break;
            }
        }
        if (!contains) {
            erg.push_back(t);
        }
    }
    return erg;
}

void StateMachine::addTransition(SpecializedComponentBase *source, SpecializedComponentBase *target, SpecializedComponentBase *event_source, std::string ev)
{
    addTransition(source->getComponent(), target->getComponent(), event_source->getComponent(), ev);
}
void StateMachine::addTransition(Component *source, SpecializedComponentBase *target, SpecializedComponentBase *event_source, std::string ev)
{
    addTransition(source, target->getComponent(), event_source->getComponent(), ev);
}
void StateMachine::addTransition(SpecializedComponentBase *source, Component *target, SpecializedComponentBase *event_source, std::string ev)
{
    addTransition(source->getComponent(), target, event_source->getComponent(), ev);
}
void StateMachine::addTransition(SpecializedComponentBase *source, SpecializedComponentBase *target, Component *event_source, std::string ev)
{
    addTransition(source->getComponent(), target->getComponent(), event_source, ev);
}
void StateMachine::addTransition(Component *source, SpecializedComponentBase *target, Component *event_source, std::string ev)
{
    addTransition(source, target->getComponent(), event_source, ev);
}
void StateMachine::addTransition(SpecializedComponentBase *source, Component *target, Component *event_source, std::string ev)
{
    addTransition(source->getComponent(), target, event_source, ev);
}
void StateMachine::setStart(SpecializedComponentBase *c)
{
    setStart(c->getComponent());
}
