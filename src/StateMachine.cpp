#include "StateMachine.hpp"
#include "Pool.hpp"
#include "SpecializedComponent.hpp"
#include "Component.hpp"

using namespace constrained_based_networks;

StateMachineObj::StateMachineObj(Pool *pool, std::string name) : CompositionObj(name, pool)
{
    addProperty("current_state", ConfigurationModel::INT);
//    transitions[0].push_back(Transition(0, 0, 0, "no_start_state"));
}

StateMachineObj::~StateMachineObj()
{
}

StateMachine StateMachineObj::make(Pool *pool, std::string name)
{
    auto res = std::shared_ptr<StateMachineObj>(new StateMachineObj(pool, name));
    pool->addComponent(res);
    return res;
}
/*
void StateMachineObj::setStart(std::string name)
{
    setStart(pool->getComponent(name));
}

void StateMachineObj::setStart(Component c)
{
    // std::cout << "Set start called on " << getName() << " start state: " << c->getName() << std::endl;
    transitions[0][0] = Transition(0, c, 0, "start");
}
*/

const std::vector<Transition>& StateMachineObj::getTransitions(){
    //TODO handle replaced children
    return transitions;
}

/*
std::map<unsigned int, std::vector<Transition>> StateMachineObj::getTransitions()
{
    if (auto spec = dynamic_cast<SpecializedComponentObjBase *>(this)) {
        auto res = transitions;
        for (const auto &replacement : spec->replaced_children) {
            auto old = pool->getComponent(replacement.first);
            auto &child = replacement.second;

            for (auto &t_ : res) {
                for (auto &t : t_.second) {
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
        }
        return res;
    } else {
        return transitions;
    }
}
*/

unsigned int StateMachineObj::getCurrentState()
{
    // std::vector<std::pair<std::string, Component *>> res;
    if (auto spec = dynamic_cast<SpecializedComponentObjBase *>(this)) {
        //        std::cout << "Have a specialized StateMachineObj " << getName() << std::endl;
        if (spec->configuration.find("current_state") != spec->configuration.end()) {
            auto current_state = (size_t)atoi(spec->configuration["current_state"].c_str());
            auto states = getStates();
            //            std::cout << "Current State should be: " << current_state << " and we have " << states.size() << std::endl;
            assert(0 <= current_state);
            assert(current_state < getStates().size());

            // auto s = getTransitions()[current_state].target;
            // assert(s);
            return current_state;
            //            std::cout << "New child is: " << s->getName() << std::endl;
            // res.push_back({"main", s});
        } else {
            if (!getStates()[0]) {
                // std::cout << "Orginal is: " << spec->getOrginal() << " their transition " << ((StateMachineObj*)spec->getOrginal().get())->getTransitions()[0].event << std::endl;
                throw std::runtime_error("State machine " + getName() + " has a transition but no target for event ");
                //                std::cerr << "Warn it seems statemachine " << getName() << "HAs no states, cannot return children" << std::endl;
            } else {
                return 0;
            }
        }
    } else {
        if ((getStates().size() != 0) && (!getStates()[0])) {
            throw std::runtime_error("State machine has no transitions");
        } else {
            return 0;
        }
    }
    throw std::runtime_error("Unknown error in getCurrentTransition()");
}


std::vector<std::pair<std::string, Component>> StateMachineObj::getChildren()
{
    std::vector<std::pair<std::string, Component>> res;
    if(getStates().size() == 0){
        throw std::runtime_error("ERROR: StateMachine " + getName() + " has no children");
    }
    res.push_back({"main", getStates()[getCurrentState()]});  // We can return Any one here all
    //res.push_back({"main", getTransitions()[getCurrentTransition()][0].target});  // We can return Any one here all

    // std::cout << "Returning child " << res[0].second->getName() << " for " << getName() << " current transition is: " << getCurrentTransition() << std::endl;
    // std::cout << "Transition size is: " << transitions.size() << std::endl;
    // for(auto t: transitions){
    //    std::cout << "\t- " << t.target->getName() << std::endl;
    //}
    return res;
}

Component StateMachineObj::searchCorresponding(Component c, Pool *pool)
{
    assert(c);

    auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c);
    if (spec.get()) {
        // search for object in DB
        for (auto pci : pool->getItems<ComponentObj>()) {
            auto pc = std::dynamic_pointer_cast<SpecializedComponentObjBase>(pci);
            if (pc.get()) {
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

void StateMachineObj::updateInternals(Pool *pool)
{
    // Update the specialized child replacement
    if (auto spec = dynamic_cast<SpecializedComponentObjBase *>(this)) {
        for (auto &replacement : spec->replaced_children) {
            replacement.second = searchCorresponding(replacement.second, pool);
        }
    }

    //    std::cout << "Update Internals for SM: " << this->getName() << std::endl;
    for (auto &t : states) {
        if (t) t = searchCorresponding(t, pool);  // Can be nil for e.G. the starting state
    }
#if 1  // Testing
    for (auto t : states) {
        if (t) pool->getComponent(t->getName());
        pool->getComponent(t->getName());
    }
#endif
}
/*
void StateMachineObj::addTransition(std::string s, std::string t, std::string event_s, std::string event_name)
{
    auto source = pool->getComponent(s);
    auto target = pool->getComponent(t);
    auto ev_s = pool->getComponent(event_s);
    addTransition(source, target, ev_s, event_name);
}

void StateMachineObj::addTransition(Component source, Component target, Component event_source, std::string ev)
{
    transitions.push_back(Transition(source, target, event_source, ev));
}
*/

SpecializedComponentBase StateMachineObj::getSpecialized(std::shared_ptr<ComponentObj> _obj, std::string name)
{
    assert(this == _obj.get());
    if (auto spec = dynamic_cast<SpecializedComponentObjBase *>(this)) {
        auto obj = std::dynamic_pointer_cast<StateMachineObj>(spec->getOrginal());
        assert(obj);

        auto s = SpecializedComponentObj<StateMachineObj>::make(obj, pool, name);
        s->configuration = spec->configuration;
        s->replaced_children = spec->replaced_children;
        s->setActive(spec->isActive());
        return s;
    }

    auto obj = std::dynamic_pointer_cast<StateMachineObj>(_obj);
    assert(obj);
    auto res = SpecializedComponentObj<StateMachineObj>::make(obj, pool, name);
    return res;
}

/*
int StateMachineObj::getNewState(Component child, std::string event, unsigned int current_state)
{
    //for (size_t i = 0; i < getTransitions().size(); i++) {
    assert(getTransitions()[current_state].event_source == child);
        auto t = getTransitions()[current_state];
        if (t.event_source == child && t.event == event) {
            return i++;
        }
    }
    return -1;
}
*/

Component StateMachineObj::clone(Pool *p) const
{
    throw std::runtime_error("Implement me");
    return 0;
    /*
    auto *c = new StateMachineObj(name, p);
    return c;
    */
};

Forwards StateMachineObj::getEventForwards(Component child, std::string name)
{
    Forwards forwards;
    //    auto forwards = Composition::getEventForwards(child, name);
    forwards["failed"] = "failed";
    forwards["success"] = "failed";
    forwards["aborted"] = "failed";
    forwards["internal_error"] = "failed";
    forwards["fatal_error"] = "failed";

    if (name != "main") {
        throw std::runtime_error("We can only handle event forwards for our main-child currently");
        // Could be extended but is not nessescary
    }

    auto current_transition_id = getCurrentState();
    //    auto current_state = getTransitions()[current_transition_id].target;
    //auto current_state = child;

    //We can currently only figure out event forwrds of our current state
    assert(getStates()[current_transition_id] == child);

    for(auto &s : getTransitions()){
        if(s.source == current_transition_id){
            std::stringstream str;
            str << "transition-" << s.target;
            forwards[s.event] = str.str();
        }
    }
#if 0
    for (size_t i = 0; i < getStates().size(); i++) {
        // auto t = getTransitions()[i];
        auto t = states[i];
        // We only support this so far:
        if (t == current_state) {
            std::stringstream s;
            s << "transition-" << i;
            // std::cerr << "success" << t.event_source->getName() << " -> " << s.str() << std::endl;
            forwards[t.event] = s.str();  // This overloads the normal propagation
        } else {
            // std::cerr << "Failed " << t.event_source->getName() << "vs. " << current_state->getName() << std::endl;
        }
    }
#endif
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

const std::vector<Component> &StateMachineObj::getStates()
{
    return states;
    /*
    std::vector<Component> erg;
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
    */
}

void StateMachineObj::replaceChild(Component child, Component old)
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

void StateMachineObj::replaceChild(Component child, std::string name)
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
/*
void StateMachineObj::addTransition(SpecializedComponentBase source, SpecializedComponentBase target, SpecializedComponentBase event_source, std::string ev)
{
    addTransition(source->getComponent(), target->getComponent(), event_source->getComponent(), ev);
}
void StateMachineObj::addTransition(Component source, SpecializedComponentBase target, SpecializedComponentBase event_source, std::string ev)
{
    addTransition(source, target->getComponent(), event_source->getComponent(), ev);
}
void StateMachineObj::addTransition(SpecializedComponentBase source, Component target, SpecializedComponentBase event_source, std::string ev)
{
    addTransition(source->getComponent(), target, event_source->getComponent(), ev);
}
void StateMachineObj::addTransition(SpecializedComponentBase source, SpecializedComponentBase target, Component event_source, std::string ev)
{
    addTransition(source->getComponent(), target->getComponent(), event_source, ev);
}
void StateMachineObj::addTransition(Component source, SpecializedComponentBase target, Component event_source, std::string ev)
{
    addTransition(source, target->getComponent(), event_source, ev);
}
void StateMachineObj::addTransition(SpecializedComponentBase source, Component target, Component event_source, std::string ev)
{
    addTransition(source->getComponent(), target, event_source, ev);
}
void StateMachineObj::setStart(SpecializedComponentBase c)
{
    setStart(c->getComponent());
}
*/



void StateMachineObj::addTransition(unsigned int source, unsigned int target, std::string causing_event){
    transitions.push_back({source,target,causing_event});
}

void StateMachineObj::addState(Component c, unsigned int id){
    assert(states.size() == id);
    states.push_back(c);
}
