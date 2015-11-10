#include "StateMachine.hpp"
#include "Pool.hpp"
#include "SpecializedComponent.hpp"
#include "Component.hpp"

using namespace constrained_based_networks;

StateMachine::StateMachine(std::string name, Pool *pool) : Composition(name, pool)
{
    addProperty("current_state", ConfigurationModel::INT);
    transitions.push_back(Transition(this, this, this, "no_start_state"));
    assert(name != "SM1_5");
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
    transitions[0] = Transition(this, c, this, "start");
}

std::vector<Transition> StateMachine::getTransitions()
{
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(this)) {
        std::vector<Transition> res = transitions;
        for (const auto &replacement : spec->replaced_children) {
            auto old = pool->getComponent(replacement.first);
            auto &child = replacement.second;

            for (auto &t : res) {
                if (t.source == old) {
                    t.source = child;
                }
                if (t.target == old) {
                    t.target = child;
                }
                if (t.event_source == old) {
                    t.event_source = child;
                }
            }
        }
        return res;
    } else {
        return transitions;
    }
}

unsigned int StateMachine::getCurrentTransition()
{
    //std::vector<std::pair<std::string, Component *>> res;
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(this)) {
        //        std::cout << "Have a specialized StateMachine " << getName() << std::endl;
        if (spec->configuration.find("current_state") != spec->configuration.end()) {
            auto current_state = (size_t)atoi(spec->configuration["current_state"].c_str());
            auto states = getStates();
            //            std::cout << "Current State should be: " << current_state << " and we have " << states.size() << std::endl;
            assert(0 <= current_state);
            assert(current_state < getTransitions().size());
            auto s = getTransitions()[current_state].target;
            assert(s);
            return current_state;
            //            std::cout << "New child is: " << s->getName() << std::endl;
            //res.push_back({"main", s});
        } else {
            if (!getTransitions()[0].target) {
                throw std::runtime_error("State machine has a transition but no target");
//                std::cerr << "Warn it seems statemachine " << getName() << "HAs no states, cannot return children" << std::endl;
            } else {
                return 0;
            }
        }
    } else {
        if (!getTransitions()[0].target) {
            throw std::runtime_error("State machine has no transitions");
        } else {
            return 0;
        }
    }
    throw std::runtime_error("Unknown error in getCurrentTransition()");
}

std::vector<std::pair<std::string, Component *>> StateMachine::getChildren()
{
    std::vector<std::pair<std::string, Component *>> res;
    res.push_back({"main", getTransitions()[getCurrentTransition()].target});
    // std::cout << "Returning child " << res[0].second->getName() << " for " << getName() << " current transition is: " << getCurrentTransition() << std::endl;
    // std::cout << "Transition size is: " << transitions.size() << std::endl;
    // for(auto t: transitions){
    //    std::cout << "\t- " << t.target->getName() << std::endl;
    //}
    return res;
}

Component *StateMachine::searchCorresponding(Component *c, Pool *pool)
{
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(c)) {
        // search for object in DB
        for (auto pci : pool->getItems<Component *>()) {
            if (auto pc = dynamic_cast<SpecializedComponentBase *>(pci)) {
                if (pc->getName(true) == spec->getName(true) && pc->configuration == spec->configuration && pc->replaced_children == spec->replaced_children) {

                    // This check should not be needed
                    assert(pool->getComponent(pci->getName()) == pci);
                    return pci;
                }
            }
        }
        throw std::runtime_error("Could not find compatible component in pool");
    } else {
        // Sainity check would rase a error otherwise
        assert(pool->getComponent(c->getName()) == c);
        return c;
    }
}

void StateMachine::updateInternals(Pool *pool)
{
    // Update the specialized child replacement
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(this)) {
        for (auto &replacement : spec->replaced_children) {
            replacement.second = searchCorresponding(replacement.second, pool);
        }
    }

    //    std::cout << "Update Internals for SM: " << this->getName() << std::endl;
    for (auto &t : transitions) {
        t.source = searchCorresponding(t.source, pool);
        t.target = searchCorresponding(t.target, pool);
        t.event_source = searchCorresponding(t.event_source, pool);
    }
#if 1  // Testing
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
}

SpecializedComponentBase *StateMachine::getSpecialized(std::string name)
{
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(this)) {
        auto s = new SpecializedComponent<StateMachine>(dynamic_cast<StateMachine*>(spec->getOrginal()), pool, name);
        s->configuration = spec->configuration;
        s->replaced_children = spec->replaced_children;
        s->setActive(spec->isActive());
        return s;
//        throw std::runtime_error("We cannot specilaize a specialized component");
    }
    return new SpecializedComponent<StateMachine>(this, pool, name);
}

int StateMachine::getNewState(Component *child, std::string event)
{
    for (size_t i = 0; i < getTransitions().size(); i++) {
        auto t = getTransitions()[i];
        if (t.event_source == child && t.event == event) {
            return i;
        }
    }
    return -1;
}

Component *StateMachine::clone(Pool *p) const
{
    auto *c = new StateMachine(name, p);
    throw std::runtime_error("Implement me");
    return c;
};

Forwards StateMachine::getEventForwards(Component *child, std::string name)
{
    (void)child;
    Forwards forwards;
    //    auto forwards = Composition::getEventForwards(child, name);
    /*
    forwards["failed"] = "failed";
    forwards["success"] = "failed";
    forwards["aborted"] = "failed";
    forwards["internal_error"] = "failed";
    forwards["fatal_error"] = "failed";
    */

    if (name != "main") {
        throw std::runtime_error("We can only handle event forwards for our main-child currently");
        // Could be extended but is not nessescary
    }

    auto current_transition_id = getCurrentTransition();
//    auto current_state = getTransitions()[current_transition_id].target;
    auto current_state = child;
    std::cerr << "STATE MACHINE IS IN STATE: " << current_transition_id << " THE STATE IS " << current_state->getName() << " Child is " << child->getName() << std::endl;

    for (size_t i = 0; i < getTransitions().size(); i++) {
        //auto t = getTransitions()[i];
        auto t = transitions[i];
        // We only support this so far:
        if (t.event_source != t.source) {
            throw std::runtime_error("Event source must be the source state");
        }
        if (t.event_source == current_state) {
            std::stringstream s;
            s << "transition-" << i;
            std::cerr << "success" << t.event_source->getName() << " -> " << s.str() << std::endl;
            forwards[t.event] = s.str();  // This overloads the normal propagation
        }else{
            std::cerr << "Failed " << t.event_source->getName() << "vs. " << current_state->getName() << std::endl;
        }
    }
    return forwards;
}
#if 0




    std::cerr << "begin: " << getName() << std::endl;
    for (size_t i = 0; i < getTransitions().size(); i++) {
        auto t = getTransitions()[i];
        std::string event_source = t.event_source->getName();
        if(auto es = dynamic_cast<SpecializedComponentBase*>(t.event_source)){
            event_source = es->getName(true);
        }

        // We only support this so far:
        if (t.event_source != t.source) {
            std::cout << "Error we only support the source-state as a source for events" << std::endl;
            std::cout << "The was:" << t.event_source->getName() << " and " << t.source->getName() << std::endl;
            assert(false);
        }
        if (event_source == child->getName()) {
            std::stringstream s;
            s << "transition-" << i;
            std::cerr << "success" << t.event_source->getName() << " -> " << s.str() << std::endl;
            forwards[t.event] = s.str();  // This overloads the normal propagation
        }else{
            std::cerr << "Failed " << t.event_source->getName() << "vs. " << child->getName() << std::endl;
        }
    }

    //if(getName() == "AuvControl::SimpleMove"){
        std::cerr << "EV Name: " << name << std::endl;
    //    assert(false);
    //}
    return forwards;
}
#endif

std::vector<Component *> StateMachine::getStates()
{
    std::vector<Component *> erg;
    for (auto tr : getTransitions()) {
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

void StateMachine::replaceChild(Component *child, Component *old)
{
    (void)child;
    (void)old;

    throw std::runtime_error("Unsupported");
    /*
    bool done = false;
    for (auto &t : transitions) {
        if (t.source == old) {
            done = true;
            t.source = child;
        }
        if (t.target == old) {
            done = true;
            t.target = child;
        }
        if (t.event_source == old) {
            done = true;
            t.event_source = child;
        }
    }
    assert(done);
    */
}

void StateMachine::replaceChild(Component *child, std::string name)
{
    (void)name;
    (void)child;
    throw std::runtime_error("Unsupported");
    /*
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(this)) {
        bool found=false;
        for(size_t i = 0; i< transitions.size() ;i++){
            auto c = transitions[i];
            if(c.target == child){
                spec->configuration["current_state"] = std::to_string(i);
                found = true;
            }
        }
        if(!found){
            throw std::invalid_argument("Cannot replace child, it does not exist as target on this SM");
        }
    }else{
        throw std::runtime_error("Cannot replace a component on a non-specialized state-machine");
    }
    */
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
