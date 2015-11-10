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
#if 1
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
#endif
}

double EventModelHandler::distance(graph_analysis::Vertex::Ptr source, graph_analysis::Vertex::Ptr target, double depth)
{
    for (auto k : instancitaed_network->outEdges(source)) {
        if (k->getTargetVertex() == target) return depth + 1;
    }
    for (auto k : instancitaed_network->outEdges(source)) {
        auto d = distance(k->getTargetVertex(), target, depth + 1);
        if (d) return d;
    }
    return 0;
}

std::vector<graph_analysis::BaseGraph::Ptr> EventModelHandler::getFollowRequirementGraph(unsigned int causing_component, std::string causing_event)
{
    std::vector<graph_analysis::BaseGraph::Ptr> res;
    std::vector<graph_analysis::Vertex::Ptr> affected_vertices;

    // First we have to check if a state-machine is affected by this change.
    // Otherwise... TODO rething is this is sensful which affectec component we search
    for (auto affected : event_propagation_table[causing_component][causing_event]) {
        auto affected_event = affected.event;
        auto affected_component_id = affected.component_graph_id;
        auto affected_component = EventModelHandler::get<StateMachine>(instancitaed_network->getVertex(affected_component_id));

        // Ok we found a state-machine which is affected no we have to search for the followup state of it
        if (affected_component) {
            /*
            Pool *p = new Pool();
            initial_pool->dupFunction(p);
            initial_pool->mergeDoubles();
            */
            if (affected_event == "failed") {
                //                std::cerr << "StateMachine Failed, this can cause a empty solution" << std::endl;
                //                std::cerr << causing_component << " -> " << causing_event << " to " << affected_component->getName() << " -> " << affected_event << std::endl;
                //                graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
                //                res.push_back(graph);
                continue;
            }

            unsigned int current_transition;
            int parsed = sscanf(affected_event.c_str(), "transition-%u", &current_transition);
            if (parsed != 1) {
                std::cerr << "Cannot parse arg: '" << parsed << "' '" << affected_event << "' == '" << current_transition << "'" << std::endl;
                // TODO figuring out the actual transition is a bit ugly here and should be done on basis of the causing evetns
                throw std::runtime_error("Cannot get the current transition of a statemachine");
            }
            // std::cout << "Got a affected statemachine " << affected_component->getName() << " the event is: " << causing_event << " -> " << affected_event << std::endl;
            // std::cout << "DEBUG:::::::::::::::::::: " << instancitaed_network->getVertex(affected_component_id)->toString() << std::endl;
            graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
            generateRequrementsGraphRecusivly(graph, initial_pool, root, instancitaed_network->getVertex(affected_component_id), current_transition);

            //            std::cerr << "THIS IS GOOD" << std::endl;
            std::cerr << causing_component << " -> " << causing_event << " to " << affected_component->getName() << " -> " << affected_event << std::endl;
            affected_vertices.push_back(instancitaed_network->getVertex(affected_component_id));
            res.push_back(graph);
        }
    }

    if (res.size() > 1) {
        double dist = std::numeric_limits<double>::max();
        size_t final_id = 0;
        std::cout << "We have multiple affectd components, reduce them to the component which is the closest one to the original one " << affected_vertices.size() << std::endl;
        auto causing = instancitaed_network->getVertex(causing_component);
        for (size_t i = 0; i < affected_vertices.size(); ++i) {
            auto c = affected_vertices[i];
            auto dist_new = distance(causing, c);
            std::cout << "Current distance is: " << dist << " candidate is: " << dist_new << std::endl;
            if (dist_new < dist) {
                final_id = 0;
                dist = dist_new;
            }
        }
        assert(dist != std::numeric_limits<double>::max());
        assert(dist != 0);
        auto result = res[final_id];
        res.clear();
        res.push_back(result);
    }
    return res;
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

void EventModelHandler::generateRequrementsGraphRecusivly(graph_analysis::BaseGraph::Ptr graph, Pool *pool, graph_analysis::Vertex::Ptr current, graph_analysis::Vertex::Ptr target,
                                                          unsigned int transition)
{
    if (isOnPath(current, target)) {
        std::cout << "Component is on path" << std::endl;
        // Searching each child of the composition, becasue we have to fix this
        // for a sub-solution. This prevents a generation of all requiremetns that could fit.
        // We only want to generate only solutions that are really close to out previous one.
        for (auto outEdgeOfCurrent : instancitaed_network->outEdges(current)) {
            std::cout << "Begin for loop" << std::endl;
            // TODO not sure if we need a orginal child here to read the config from it
            auto parent = dynamic_cast<Composition *>(pool->getComponent(EventModelHandler::get<Component>(outEdgeOfCurrent->getSourceVertex())->getName()));
            auto child = dynamic_cast<Composition *>(pool->getComponent(EventModelHandler::get<Component>(outEdgeOfCurrent->getTargetVertex())->getName()));
            assert(parent);
            assert(child);

            if (outEdgeOfCurrent->getTargetVertex() != target) {
                std::cout << "It is not the target" << std::endl;

                graph->addEdge(outEdgeOfCurrent);

                // We branch here a level deeper, we need only to do this if we are on the path
                // becasue everything else we dont care in general
                generateRequrementsGraphRecusivly(graph, pool, outEdgeOfCurrent->getTargetVertex(), target, transition);
            } else {

                std::cout << "It is the target" << std::endl;

                auto new_child = Vertex::Ptr(outEdgeOfCurrent->getTargetVertex()->clone());
                auto sm = dynamic_cast<ConfiguredComponent *>(new_child.get());
                assert(sm);
                bool success = false;
                for (auto &c : sm->int_config) {
                    if (c.name == "current_state") {
                        c.min = transition;
                        c.max = transition;
                        success = true;
                    }
                }
                assert(success);

                // TODO continue work here, we have specialized components to export into the graph which is not yet supported
                // by the tooling. The specialized component must then later after the orgiginal model is loaded  re-linked to the model in the space
                // The best way would be to re-create the specialized components on import maybe
                {
                    graph_analysis::Edge::Ptr e = graph_analysis::Edge::Ptr(new graph_analysis::Edge(outEdgeOfCurrent->getLabel()));
                    e->setSourceVertex(current);
                    e->setTargetVertex(new_child);
                    std::cout << "Adding edge from " << current->toString() << " to " << new_child->toString() << std::endl;
                    graph->addEdge(e);
                }
            }
        }
    } else {
        // We don't care for everything else, expect we have to start it because we are on the root-level
        // which means the component must be started.
        if (current == root) {
            std::cout << "current is root" << std::endl;
            graph_analysis::Edge::Ptr e = graph_analysis::Edge::Ptr(new graph_analysis::Edge("root-requirement"));
            e->setSourceVertex(current);
            e->setTargetVertex(target);
            graph->addEdge(e);
        }
    }
    /*
        if(graph->size() == 1){
            //Somethign goes wonrg here we have no requirements?
            throw std::runtime_error("Cannot create graph with no dependancies this is wrong");
        }
    */
}

void EventModelHandler::generateDBRecursive(graph_analysis::Vertex::Ptr current_node)
{
    auto cc = dynamic_cast<ConfiguredComponent *>(current_node.get());
    assert(cc);
    for (auto _child : instancitaed_network->outEdges(current_node)) {
        auto child = dynamic_cast<Component *>(_child->getTargetVertex().get());
        if (auto spec = dynamic_cast<ConfiguredComponent *>(_child->getTargetVertex().get())) {
            child = spec->component;
        }
        assert(child);
        std::string role = _child->getLabel();

        // This should be a composition because it has children
        auto composition = dynamic_cast<Composition *>(cc->component);
        assert(composition);

        for (auto forwards : composition->getEventForwards(child, role)) {
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
            bool valid = dynamic_cast<Task *>(causing_component);
            if (auto c = dynamic_cast<Composition *>(causing_component)) {
                // TODO maybe be more precise here
                valid = valid || (c->getEmitations().size() != 0);
            };

            if (valid || true) {
                auto p = getFollowRequirementGraph(component.first, event.first);
                // std::cout << "Graph size: " << p.size() << std::endl;
                res.push_back({causing_component, event.first, {p}});
            }
        }
    }
    return res;
}
#if 0
void EventModelHandler::replaceSMinGraph(graph_analysis::DirectedGraphInterface::Ptr graph, graph_analysis::Vertex::Ptr current, Pool *pool, Component *old){

    if(auto parent_sm = dynamic_cast<StateMachine*>(pool->getComponent(current->toString()))){
        auto new_sm = setConfig(current, old);
         
        throw std::runtime_error("Implement me");
        (void)graph;
        (void)parent_sm;
        /*
        for(auto parent : graph->inEdges(current)){
            if(auto cmp = dynamic_cast<Composition*>(pool->getComponent(parent->toString()))){
                replaceSMinGraph(graph,,parent,pool);
            }
        }
        */
    }else if(auto composition = dynamic_cast<Composition*>(pool->getComponent(current->toString()))){
        bool found=false;
        for(auto c : composition->getChildren()){
            if(c.second == old){
                composition->replaceChild(sm,c.first);
                found=true;
                break;
            }
        }
        if(!found){
            throw std::invalid_argument("Cannot replace child");
        }
    }else{
        throw std::runtime_error("Unsupported");
    }
}
#endif

void EventModelHandler::createFollowPoolRecursive(graph_analysis::DirectedGraphInterface::Ptr graph, Pool *pool, graph_analysis::Vertex::Ptr current_vertex, Component *parent)
{
    for (auto child : graph->outEdges(current_vertex)) {
        auto c = pool->getComponent(child->getTargetVertex()->toString());
        auto new_component = setConfig(child->getTargetVertex(), c);

        // We have to start this component IF parent is a root
        if (current_vertex->toString() == "root-knot") {
            new_component->setActive(true);
            std::cout << "Set active on: " << new_component->toString() << std::endl;
        } else {
            if (auto sm = dynamic_cast<StateMachine *>(parent)) {
                if (auto spec = dynamic_cast<SpecializedComponentBase *>(sm)) {
                    spec->replaced_children[c->getName()] = new_component;

                    std::cout << "Warn we should have a state-machine here" << sm->getName() << std::endl;
                    for (auto conf : spec->configuration) {
                        std::cout << "\t- " << conf.first << ": " << conf.second << std::endl;
                    }
                } else {
                    throw std::runtime_error("All state-machines needs to be specialized here");
                }
                // Should not needed, solved by the spec
            } else if (auto composition = dynamic_cast<Composition *>(parent)) {
                composition->replaceChild(new_component, child->toString());
            } else {
                // Maybe a task we dont care here
                // throw std::runtime_error("Unsupported");
            }
        }
        createFollowPoolRecursive(graph, pool, child->getTargetVertex(), new_component);
    }
}

void EventModelHandler::createFollowPool(const TransitionTrigger &trigger, Pool *pool)
{
    // We onl support one following state to a event
    if (trigger.resulting_requirement.network.size() != 1) {
        throw std::invalid_argument("Cannot create pool for network size of " + std::to_string(trigger.resulting_requirement.network.size()));
    }
    graph_analysis::BaseGraph::Ptr graph = trigger.resulting_requirement.network[0];

    auto d_graph = boost::reinterpret_pointer_cast<graph_analysis::DirectedGraphInterface>(graph);
    assert(d_graph);
    graph_analysis::Vertex::Ptr root;
    for (auto k : d_graph->vertices()) {
        if (k->toString() == "root-knot") {
            root = k;
            break;
        }
    }
    assert(root.get());
    /*
    if(graph->size() == 1){
        throw std::runtime_error("Got a empty requrement graph, this is not allowed");
    }else{
        std::cout << "Graph size is: " << graph->size() << std::endl;
    }
    */

    // Disable all current components in the network
    /*
    for (auto e : graph->edges()) {
        if (e->getSourceVertex()->toString() == "root-knot") {
            const auto &active_child = pool->getComponent(e->getTargetVertex()->toString());
            active_child->setActive(false);
        }
    }
    */

    for(auto c : pool->getItems<Component*>()){
        if(c->getName() != "root-knot"){
            c->setActive(false);
        }
    }

    createFollowPoolRecursive(d_graph, pool, root, 0);

    bool valid= false;
    for(auto c : pool->getItems<Component*>()){
        if(c->isActive() && c->getName() != "root-knot"){
            std::cout << "Fond a active component: " << c->getName() << std::endl;
            valid = true;
        }
    }
    if(!valid){
        throw std::runtime_error("Generated follow requiremtn without any component active");
    }

    pool->mergeDoubles();

    valid= false;
    for(auto c : pool->getItems<Component*>()){
        if(c->isActive() && c->getName() != "root-knot"){
            std::cout << "Fond a active component: " << c->getName() << std::endl;
            valid = true;
        }
    }
    if(!valid){
        throw std::runtime_error("Merge doubles is again really bad");
    }

    /*
        // Start to create our graph based on the imported graph
        for (auto e : graph->edges()) {
            // We have found a root-knot we need to set all children active in our pool
            if (e->getSourceVertex()->toString() == "root-knot") {
                const auto &active_child = pool->getComponent(e->getTargetVertex()->toString());
                assert(active_child->getPtr());

                auto new_component = setConfig(e->getTargetVertex(), active_child);
                std::cout << "Setting active: " << new_component->getName() << " for pool " << pool << std::endl;
                new_component->setActive(true);
                // We have to disable the old component
                active_child->setActive(false);

                assert(new_component->isInPool(pool));
            } else {
                // std::cout << "From " << e->getSourceVertex()->toString() << " to " << e->getTargetVertex()->toString() << std::endl;
                auto v1 = pool->getComponent(e->getSourceVertex()->toString());
                auto v2 = pool->getComponent(e->getTargetVertex()->toString());
                assert(v1->getPtr());
                assert(v2->getPtr());
                auto cmp = dynamic_cast<Composition *>(v1);
                assert(cmp);  // Otherwise it could not have any children

                auto new_component = setConfig(e->getTargetVertex(), v2);
                assert(new_component);

                if(auto sm =  dynamic_cast<StateMachine*>(cmp)){
    //                auto new_sm = setConfig(e->getSourceVertex(), sm);
    //                replaceSMinGraph(boost::reinterpret_pointer_cast<graph_analysis::DirectedGraphInterface>(graph) ,e->getSourceVertex(), pool,sm);
                }else{
                    cmp->replaceChild(new_component, e->toString());
                }
            }
        }
    */
}

Component *EventModelHandler::setConfig(graph_analysis::Vertex::Ptr v, Component *c)
{
    auto conf = dynamic_cast<ConfiguredComponent *>(v.get());
    assert(conf);
    auto new_active_child = c->getSpecialized();
    for (auto i : conf->int_config) {
        std::stringstream s;
        // TODO rething of limits
        s << i.min;
        std::cout << "Setting a config value " << i.name << " to " << s.str() << std::endl;
        new_active_child->addConfig(i.name, s.str());
    }
    for (auto i : conf->bool_config) {
        std::stringstream s;
        // TODO rething of limits
        s << i.min;
        new_active_child->addConfig(i.name, s.str());
    }
    for (auto i : conf->double_config) {
        std::stringstream s;
        // TODO rething of limits
        s << i.min;
        new_active_child->addConfig(i.name, s.str());
    }
    for (auto i : conf->string_config) {
        // TODO rething of limits
        new_active_child->addConfig(i.first, i.second);
    }
    return dynamic_cast<Component *>(new_active_child);
}
