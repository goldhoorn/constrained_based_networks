#include "EventModelHandler.hpp"
#include "InstanceSolution.hpp"
#include "Composition.hpp"
#include "SpecializedComponent.hpp"
#include <stdio.h>

using namespace graph_analysis;
using namespace constrained_based_networks;

EventModelHandler::EventModelHandler(Pool *_initial_pool, graph_analysis::DirectedGraphInterface::Ptr _instanciated_network)
    : initial_pool(_initial_pool)
    , instancitaed_network(_instanciated_network)
{
    root = InstanceSolution::getRoot(instancitaed_network);
    generateDBRecursive(root);

    // Flatten the Databse until everyting is done
    // From the DB createion we know for all direct children how they would introduce our behaviour e.G:
    // event_propagation_table[child][child_event] >> [parent][parent_event]
    // And we need to create somehting like
    // event_propagation_table[child][child_event] >> [ [parent][parent_event] [parent2][parent_event2] ]
    // Which each itertion we stepping one level higher we do this by checking
    // the event_propagation_table[parent][parent_event] to get [paren2][parent_event2]
    bool done = true;
    do {
        done = true;
        for (auto h1 : event_propagation_table) {
            auto causing_component_id = h1.first;

            auto causing_component = dynamic_cast<Component *>(instancitaed_network->getVertex(causing_component_id).get());
            if (auto c = dynamic_cast<ConfiguredComponent *>(instancitaed_network->getVertex(causing_component_id).get())) causing_component = c->component;
            assert(causing_component);
            for (auto h2 : h1.second) {
                auto causing_event = h2.first;

                // Here is the map for target causes
                for (auto cause : h2.second) {
                    auto resulting_event = cause.event;
                    auto resulting_component_id = cause.component_graph_id;
                    // auto resulting_component = dynamic_cast<Composition *>(instancitaed_network->getVertex(resulting_component_id).get());
                    // if(auto c = dynamic_cast<ConfiguredComponent*>(instancitaed_network->getVertex(resulting_component_id).get())) resulting_component= c->component;

                    // If the target is not a composition it cannot forward anything, therefore we can stop here
                    // if (!resulting_component) continue;

                    // Search for all parents and check if they have registerd a forward for the resulting event
                    for (auto parent : instancitaed_network->inEdges(instancitaed_network->getVertex(resulting_component_id))) {

                        // Figure out if this component should actuylla forward the event, if so add a cause to the list, otherwise we are done here
                        for (auto h3 : event_propagation_table[resulting_component_id][resulting_event]) {
                            auto new_target = h3.component_graph_id;
                            auto new_target_event = h3.event;
                            //                            std::cout << "Debug: " << new_target_event << std::endl;
                            if (event_propagation_table[causing_component_id][causing_event].find({new_target, new_target_event}) ==
                                event_propagation_table[causing_component_id][causing_event].end()) {
                                event_propagation_table[causing_component_id][causing_event].insert({new_target, new_target_event});
                                done = false;
                                //                               std::cout << "############# WE ARE NOT DONE ################";
                            }
                        }
                    }
                }
            }
        }
    } while (!done);

    // Print final result:
    for (auto h1 : event_propagation_table) {
        auto causing_component_id = h1.first;
        auto causing_component = dynamic_cast<Component *>(instancitaed_network->getVertex(causing_component_id).get());
        if (auto c = dynamic_cast<ConfiguredComponent *>(instancitaed_network->getVertex(causing_component_id).get())) causing_component = c->component;
        std::cout << "- Component: " << causing_component->getName() << std::endl;
        for (auto h2 : h1.second) {
            auto causing_event = h2.first;
            std::cout << "\t -Event: " << causing_event << std::endl;
            for (auto cause : h2.second) {
                auto resulting_component = dynamic_cast<Component *>(instancitaed_network->getVertex(cause.component_graph_id).get());
                if (auto c = dynamic_cast<ConfiguredComponent *>(instancitaed_network->getVertex(cause.component_graph_id).get())) resulting_component = c->component;
                std::cout << "\t\t -- " << resulting_component->getName() << " event " << cause.event << std::endl;
            }
        }
    }

    std::cout << "Jeha i'm done" << std::endl;
}

Pool *EventModelHandler::getFollowRequirements(unsigned int causing_component, std::string causing_event)
{
    // First we have to check if a state-machine is affected by this change.
    // Otherwise... TODO
    for (auto affected : event_propagation_table[causing_component][causing_event]) {
        auto affected_event = affected.event;
        auto affected_component_id = affected.component_graph_id;
        auto affected_component = dynamic_cast<StateMachine *>(instancitaed_network->getVertex(affected_component_id).get());
        if (auto c = dynamic_cast<ConfiguredComponent *>(instancitaed_network->getVertex(affected_component_id).get())) affected_component = dynamic_cast<StateMachine *>(c->component);

        // Ok we found a state-machine which is affected no we have to search for the followup state of it
        if (affected_component) {
            Pool *p = new Pool();
            initial_pool->dupFunction(p);
            unsigned int current_transition;
            int parsed = sscanf(affected_event.c_str(), "transition-%u", &current_transition);
            if (parsed != 1) {
                std::cerr << "Cannot parse arg: " << parsed << " " << affected_event << " == " << current_transition << std::endl;
                // TODO figuring out the actual transition is a bit ugly here and should be done on basis of the causing evetns
                throw std::runtime_error("Cannot get the current transition of a statemachine");
            }
            std::cout << "Got a affected statemachine " << affected_component->getName() << " the event is: " << causing_event << " -> " << affected_event << std::endl;
            std::cout << "DEBUG:::::::::::::::::::: " << instancitaed_network->getVertex(affected_component_id)->toString() << std::endl;
            getFollowRequirements(p, root, instancitaed_network->getVertex(affected_component_id), current_transition);
            return p;
        }
    }
    return 0;
}

bool EventModelHandler::isOnPath(graph_analysis::Vertex::Ptr current, graph_analysis::Vertex::Ptr target)
{
    for (auto v : instancitaed_network->outEdges(current)) {
        if (v->getTargetVertex() == target) {
            return true;
        } else {
            if (isOnPath(v->getTargetVertex(), target) == true) {
                return true;
            }
        }
    }
    return false;
}

void EventModelHandler::getFollowRequirements(Pool *pool, graph_analysis::Vertex::Ptr current, graph_analysis::Vertex::Ptr target, unsigned int transition)
{
#if 0
    // If we are at level 0 of our new graph
    if (root == current) {
        for (auto root_req : instancitaed_network->outEdges(root)) {
            auto component = dynamic_cast<Component *>(root_req->getTargetVertex().get());
            if (auto c = dynamic_cast<ConfiguredComponent *>(root_req->getTargetVertex().get())) component = dynamic_cast<Component *>(c->component);
            assert(component);
            auto c = pool->getComponent(component->getName());
            assert(c);

            if (isOnPath(root_req->getTargetVertex(), target)) {
                // TODO add only fitting component
                auto spec = c->getSpecialized();
                if (auto cmp = dynamic_cast<Composition *>(spec)) {
                    for (auto child_and_name : cmp->getChildren()) {
                    }
                }
                spec->setActive(true);
            } else {
                c->setActive(true);
            }
        }
    }
#endif
    // for (auto edge : instancitaed_network->outEdges(current)) {

    if (isOnPath(current, target)) {
        /*
        // This should mean we have a Composition here
        auto composition = EventModelHandler::get<Composition>(edge->getTargetVertex());
        assert(composition);
        auto new_composition = dynamic_cast<Composition *>(pool->getComponent(component->getName()));
        assert(new_composition);
        */

        // Searching each child of the composition, becasue we have to fix this
        // for a sub-solution. This prevents a generation of all requiremetns that could fit.
        // We only want to generate only solutions that are really close to out previous one.
        for (auto c : instancitaed_network->outEdges(current)) {
            auto c_ = EventModelHandler::get<Component>(c->getTargetVertex());
            auto new_child = pool->getComponent(c_->getName());
            auto new_composition = dynamic_cast<Composition *>(pool->getComponent(c_->getName()));
            assert(new_composition);
            // std::cout << "FUSEL DUESEL: " << new_child->getName() << " <-> " << c_->getName() << std::endl;
            assert(new_child);
            // if (edge->getTargetVertex() != target) {
            if (c->getTargetVertex() != target) {
                std::cout << "Replace child for: " << new_composition->getName() << "with name: " << c->getLabel() << std::endl;
                new_composition->replaceChild(new_child, c->getLabel());
                // We branch here a level deeper, we need only to do this if we are on the path
                // becasue everything else we dont care in general
                getFollowRequirements(pool, c->getTargetVertex(), target, transition);
            } else {
                auto spec = dynamic_cast<SpecializedComponent<StateMachine> *>(new_child->getSpecialized());
                // auto spec = dynamic_cast<StateMachine*>(new_child->getSpecialized());
                /*
                if(!spec){
                    std::cout << "DEBUG2:::::::::::::::::::: " << target->toString() << " " << c_->getName() << std::endl;
                    std::cout << "Got a invalid specialized child with name: " << new_child->getName() << std::endl;
                }
                */
                assert(spec);
                std::stringstream str;
                str << transition;
                spec->addConfig("current_state", str.str());

                if(current == root){
                    //Special case state-machine itself was a root-requirement
                    //we need to activate it before
                    spec->setActive(true);
                }else{
                    new_composition->replaceChild(spec, c->getLabel());
                }
            }
        }
    } else {
        // We don't care for everything else, expect we have to start it because we are on the root-level
        // which means the component must be started.
        if (current == root) {
            auto component = EventModelHandler::get<Component>(current);
            auto new_component = pool->getComponent(component->getName());
            assert(new_component);
            new_component->setActive(true);
        }
    }

    // TODO we have to somehow build up the graph with new components
    //getFollowRequirements(pool, edge->getTargetVertex(), target, transition);
}
#if 0
    else
    {
        auto component = dynamic_cast<StateMachine *>(edge->getTargetVertex().get());
        if (auto c = dynamic_cast<ConfiguredComponent *>(edge->getTargetVertex().get())) component = dynamic_cast<StateMachine *>(c->component);
        component = dynamic_cast<StateMachine *>(pool->getComponent(component->getName()));
        assert(component);
        // auto spec = dynamic_cast<SpecializedComponent<StateMachine>*> (component->getSpecialized());
        auto spec = (component->getSpecialized());
        assert(spec);
        std::stringstream str;
        str << transition;
        spec->addConfig("current_state", str.str());
        spec->setActive(true);
    }
}
#endif

void EventModelHandler::generateDBRecursive(graph_analysis::Vertex::Ptr current_node)
{
    auto cc = dynamic_cast<ConfiguredComponent *>(current_node.get());
    assert(cc);
    for (auto _child : instancitaed_network->outEdges(current_node)) {
        auto child = dynamic_cast<Component *>(_child->getTargetVertex().get());
        if (auto spec = dynamic_cast<ConfiguredComponent *>(_child->getTargetVertex().get())) {
            std::cout << "Got a configured component" << std::endl;
            child = spec->component;
        }
        assert(child);

        // This should be a composition because it has children
        auto composition = dynamic_cast<Composition *>(cc->component);
        assert(composition);

        for (auto forwards : composition->getEventForwards(child)) {
            event_propagation_table[instancitaed_network->getVertexId(_child->getTargetVertex())][forwards.first].insert({instancitaed_network->getVertexId(current_node), forwards.second});
        }
        generateDBRecursive(_child->getTargetVertex());
    }
}

std::list<TransitionTrigger> EventModelHandler::getTrigger()
{
    std::list<TransitionTrigger> res;
    for (auto component : event_propagation_table) {
        for (auto event : component.second) {
            auto causing_component = dynamic_cast<Component *>(instancitaed_network->getVertex(component.first).get());
            if (auto c = dynamic_cast<ConfiguredComponent *>(instancitaed_network->getVertex(component.first).get())) causing_component = c->component;
            auto p = getFollowRequirements(component.first, event.first);
            if (p) res.push_back({causing_component, event.first, {p}});
        }
    }
    return res;
}
