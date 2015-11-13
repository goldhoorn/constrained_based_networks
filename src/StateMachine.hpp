#pragma once

#include "Composition.hpp"
#include <vector>

namespace constrained_based_networks
{

class Pool;

struct Transition
{
    Transition(Component _source, Component _target, Component _event_source, std::string _event)
        : target(_target)
        , source(_source)
        , event_source(_event_source)
        , event(_event)
    {
    }

    Component target;
    Component source;
    Component event_source;
    std::string event;
};

class StateMachineObj : public CompositionObj
{
   protected:
    StateMachineObj(Pool *pool, std::string name ="");
    public:
    virtual ~StateMachineObj();
    static std::shared_ptr<StateMachineObj> make(Pool *pool, std::string name);
    void addTransition(std::string s, std::string t, std::string event_s, std::string event_name);
    void addTransition(Component source, Component target, Component event_source, std::string ev);
    void addTransition(std::shared_ptr<SpecializedComponentObjBase> source, std::shared_ptr<SpecializedComponentObjBase> target, std::shared_ptr<SpecializedComponentObjBase> event_source, std::string ev);
    void addTransition(Component source, std::shared_ptr<SpecializedComponentObjBase> target, std::shared_ptr<SpecializedComponentObjBase> event_source, std::string ev);
    void addTransition(std::shared_ptr<SpecializedComponentObjBase> source, Component target, std::shared_ptr<SpecializedComponentObjBase> event_source, std::string ev);
    void addTransition(std::shared_ptr<SpecializedComponentObjBase> source, std::shared_ptr<SpecializedComponentObjBase> target, Component event_source, std::string ev);
    void addTransition(Component source, std::shared_ptr<SpecializedComponentObjBase> target, Component event_source, std::string ev);
    void addTransition(std::shared_ptr<SpecializedComponentObjBase> source, Component target, Component event_source, std::string ev);
    void setStart(std::string name);
    void setStart(Component c);
    void setStart(std::shared_ptr<SpecializedComponentObjBase> c);
    bool abstract() const
    {
        return false;
    }
    virtual std::string getClassName() const
    {
        return "constrained_based_networks::StateMachine";
    }
    virtual Forwards getEventForwards(Component child, std::string name);

    /*
     * This is a ugly part again, we need to update out internal components
     * and searching for the correspinding one in a pool
     * This is used when the pool get altered by e.G. mergeDoubles
     */
    void updateInternals(Pool *pool);
    unsigned int getCurrentTransition();
    virtual void replaceChild(Component child, std::string name);
    virtual void replaceChild(Component child, Component old);
    std::shared_ptr<SpecializedComponentObjBase> getSpecialized(std::shared_ptr<ComponentObj> obj, std::string name="");
    virtual Component clone(Pool *p) const;
    /*
                SpecializedComponent<StateMachineObj> *getSpecialized()
                {
                    return new SpecializedComponent<StateMachineObj>(this, pool);
                }
                */

    /*
     void addConfig(std::string name, std::string value){
         if(auto c = dynamic_cast<SpecializedComponent<StateMachineObj> *>(this)){
             c->addConfig(name,value);
         }else{
             throw std::runtime_error("Called addConfig on invalid class");
         }
     }
     */

    std::vector<Component> getStates();
    int getNewState(Component child, std::string event);

    // Inherited from Composition
    virtual std::vector<std::pair<std::string, Component >> getChildren();


    std::vector<Transition> getTransitions();

   protected:
    std::vector<Transition> transitions;
    Component searchCorresponding(Component c, Pool *pool);
};

typedef std::shared_ptr<StateMachineObj> StateMachine;

}
