#pragma once

#include "Component.hpp"
#include <vector>

namespace constrained_based_networks {

    class Pool;

    struct Transition{
        Transition(Component *target, Component *source, Component *event_source, std::string event):
            target(target),
            source(source),
            event_source(event_source),
            event(event)
        {
        }

        Component *target;
        Component *source;
        Component *event_source;
        std::string event;
    };

    class StateMachine : public Component{
        public:
            StateMachine(std::string name, Pool *pool);
            ~StateMachine();
            void addTransition(std::string s, std::string t, std::string event_s, std::string event_name);
            void addTransition(Component *source,  Component *target, Component *event_source, std::string ev); 
            void setStart(std::string name);
            void setStart(Component *c);
            bool abstract() const{return false;}
            Component* getSpecialized()
            {
                return new SpecializedComponent<StateMachine>(this, pool);
            }
            
            void addConfig(std::string name, std::string value){
                if(auto c = dynamic_cast<StateMachine*>(this)){
                    c->addConfig(name,value);
                }else{
                    throw std::runtime_error("Called addConfig on invalid class");
                }
            }

            std::vector<Component*> getStates();

        protected:
            std::vector<Transition> transitions;
            Component *start;

    };
}
