#include "EventModelHandler.hpp"
#include "InstanceSolution.hpp"
#include "Composition.hpp"

using namespace graph_analysis;
using namespace constrained_based_networks;

EventModelHandler::EventModelHandler(Pool *_initial_pool, graph_analysis::DirectedGraphInterface::Ptr _instanciated_network)
    : initial_pool(_initial_pool)
    , instancitaed_network(_instanciated_network)
{
    auto root = InstanceSolution::getRoot(instancitaed_network);
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
    std::cout << __LINE__ << std::endl;
    for (auto affected : event_propagation_table[causing_component][causing_event]) {
        auto affected_event = affected.event;
        auto affected_component_id = affected.component_graph_id;
        auto affected_component = dynamic_cast<StateMachine *>(instancitaed_network->getVertex(affected_component_id).get());
        
        std::cout << __LINE__ << std::endl;
        if (auto c = dynamic_cast<ConfiguredComponent *>(instancitaed_network->getVertex(affected_component_id).get())) affected_component = dynamic_cast<StateMachine *>(c->component);
        std::cout << __LINE__ << std::endl;

        // Ok we found a state-machine which is affected no we have to search for the followup state of it
        if (affected_component) {
            std::cout << "Got a affected statemachine, the event is: " << causing_event << " -> " << affected_event << std::endl;
        }
    }

    return 0;
#if 0
    Pool *p = new Pool();
    initial_pool->dupFunction(p);
    auto conf = dynamic_cast<ConfiguredComponent*>(instancitaed_network->getVertex(causing_component).get());
    assert(conf);


    if(auto state_machine  = dynamic_cast<StateMachine*>(conf->component)){
        if(state_machine->getNewState(
    }
#endif
}

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
