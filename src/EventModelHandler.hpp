#pragma once

#include <graph_analysis/Graph.hpp>
#include <list>
#include "Component.hpp"

namespace constrained_based_networks
{

struct Network
{
    Pool *pool;
};

struct TransitionTrigger
{
    Component *causing_component;
    std::string causing_event;
    Network resulting_requirement;
};

struct EventWithSource
{
    unsigned int component_graph_id;
    std::string event;

    const bool operator <( const EventWithSource&r ) const{
        //TODO this should be done more efficiently
        std::stringstream str1;
        std::stringstream str2;
        str1 << component_graph_id << event;
        str2 << r.component_graph_id << r.event;
        return str1.str() < str2.str();
    }
};

struct EventPropagation
{
    EventWithSource e;
    std::vector<EventWithSource> causes;
};


class EventModelHandler
{
   public:
    EventModelHandler(Pool *initial_pool, graph_analysis::DirectedGraphInterface::Ptr instancitaed_network);
    void generateDBRecursive(graph_analysis::Vertex::Ptr current_node);
    Pool* getFollowRequirements(unsigned int causing_component, std::string causing_event);

    std::list<TransitionTrigger> getTrigger();

   private:
    void getFollowRequirements(Pool *pool, graph_analysis::Vertex::Ptr current, graph_analysis::Vertex::Ptr target);
    std::map<unsigned int, std::map<std::string, std::set<EventWithSource>>> event_propagation_table;
    // std::map<EventWithSource, std::vector<EventWithSource>> event_propagation_table;
    Pool *initial_pool;
    graph_analysis::DirectedGraphInterface::Ptr instancitaed_network;
    graph_analysis::Vertex::Ptr root;
};
};
