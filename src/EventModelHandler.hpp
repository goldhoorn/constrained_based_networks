#pragma once

#include <graph_analysis/Graph.hpp>
#include <list>
#include "Component.hpp"

namespace constrained_based_networks
{
    struct TransitionTrigger{
        Component* causing_component;
        std::string causing_event;
        std::string resulting_requirement;
    };

class EventModelHandler
{
   public:
       static std::list<TransitionTrigger> calculateTrigger(graph_analysis::BaseGraph::Ptr graph);
};
};
