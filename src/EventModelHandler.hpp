#pragma once

#include <graph_analysis/Graph.hpp>
#include <list>
#include "Component.hpp"
#include "InstanceSolution.hpp"

namespace constrained_based_networks
{

struct Network
{
    std::vector<graph_analysis::BaseGraph::Ptr> network;
};

struct TransitionTrigger
{
    std::weak_ptr<ComponentObj> causing_component;
    std::string causing_event;
    Network resulting_requirement;

    bool operator==(const TransitionTrigger &t)
    {
        //assert(false);
        // TODO we should check more than the component name, this might not be enougth
        if (causing_event != t.causing_event || causing_component.lock()->getName() != causing_component.lock()->getName()) {
            return false;
        }
        if (resulting_requirement.network.size() != t.resulting_requirement.network.size()) {
            return false;
        }
        for (size_t i = 0; i < resulting_requirement.network.size(); ++i) {
            for (auto v : resulting_requirement.network[i]->edges()) {
                bool found = false;
                for (auto v2 : t.resulting_requirement.network[i]->edges()) {
                    if (v->getSourceVertex() == v2->getSourceVertex() && v->getTargetVertex() == v2->getTargetVertex()) {
                        found = true;
                        break;
                    }
                }
                if (!found) return false;
            }
        }
        return true;
    }
};

struct EventWithSource
{
    unsigned int component_graph_id;
    std::string event;

    bool operator<(const EventWithSource &r) const
    {
        // TODO this should be done more efficiently
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
    std::vector<graph_analysis::BaseGraph::Ptr> getFollowRequirementGraph(unsigned int causing_component, std::string causing_event);
    double distance(graph_analysis::Vertex::Ptr source, graph_analysis::Vertex::Ptr target, double depth=0);

    std::list<TransitionTrigger> getTrigger();

    template <typename C>
    static std::shared_ptr<C> get(graph_analysis::Vertex::Ptr v)
    {
        if(0){
            //Compile time check, make sure we request only Components
            ((C*)0)->getName();
        }
        auto component = std::dynamic_pointer_cast<C>(v);
        if (auto c = std::dynamic_pointer_cast<ConfiguredComponent>(v)) component = std::dynamic_pointer_cast<C>(c->component);
        return component;
    }

    /*
     * \param pool the pool that already contains all previous components, the new ones get added
     */
    static void createFollowPool(const TransitionTrigger &trigger, Pool *pool);
    static Component setConfig(graph_analysis::Vertex::Ptr v, Component c);
    static void createFollowPoolRecursive(graph_analysis::DirectedGraphInterface::Ptr graph, Pool *pool, graph_analysis::Vertex::Ptr current_vertex, Component parent);

   private:
    bool isOnPath(graph_analysis::Vertex::Ptr current, graph_analysis::Vertex::Ptr target);
    void generateRequrementsGraphRecusivly(graph_analysis::BaseGraph::Ptr graph, Pool *pool, graph_analysis::Vertex::Ptr current, graph_analysis::Vertex::Ptr target, unsigned int transition);
    std::map<unsigned int, std::map<std::string, std::set<EventWithSource>>> event_propagation_table;
    // std::map<EventWithSource, std::vector<EventWithSource>> event_propagation_table;
    Pool *initial_pool;
    graph_analysis::DirectedGraphInterface::Ptr instancitaed_network;
    graph_analysis::Vertex::Ptr root;
};
};
