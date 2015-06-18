#pragma once

#include "Component.hpp"
#include <vector>

namespace constrained_based_networks {

    class Pool;

    struct Transition{
        Transition(Component *_target, Component *_source, Component *_event_source, std::string _event):
            target(_target),
            source(_source),
            event_source(_event_source),
            event(_event)
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
            void setStart(const Component *c);
            bool abstract() const{return false;}
    
            SpecializedComponentBase *getSpecialized(){
                return new SpecializedComponent<StateMachine>(this, pool);
            }
/*
            SpecializedComponent<StateMachine> *getSpecialized()
            {
                return new SpecializedComponent<StateMachine>(this, pool);
            }
            */
            
           /* 
            void addConfig(std::string name, std::string value){
                if(auto c = dynamic_cast<SpecializedComponent<StateMachine> *>(this)){
                    c->addConfig(name,value);
                }else{
                    throw std::runtime_error("Called addConfig on invalid class");
                }
            }
            */

            std::vector<Component*> getStates();

        protected:
            std::vector<Transition> transitions;
            const Component *start;

    };
}
