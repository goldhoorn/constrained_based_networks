#include "EventModelHandler.hpp"
#include "InstanceSolution.hpp"

using namespace graph_analysis;
using namespace constrained_based_networks;

std::list<TransitionTrigger> EventModelHandler::calculateTrigger(graph_analysis::BaseGraph::Ptr graph)
{
    std::list<TransitionTrigger> res;
    for(auto vertex : graph->vertices()){
        auto cc = dynamic_cast<ConfiguredComponent*>(vertex.get());
        assert(cc);
        auto component = cc->component;
        std::cout << "Component events for: " << component->getName() << std::endl;
        for(auto event : component->getEvents()){
            std::cout << "- " << event << std::endl;
        }
    }
    return res;
}
