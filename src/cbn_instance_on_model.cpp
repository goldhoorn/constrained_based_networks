#include "XML.hpp"
#include <unistd.h>
#include <iostream>
#include "Pool.hpp"
#include "ClassSolution.hpp"
#include "EventModelHandler.hpp"
#include "DataService.hpp"
#include "InstanceSolution.hpp"
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/VertexTypeManager.hpp>

using namespace constrained_based_networks;

void initializeExporter()
{
    graph_analysis::VertexTypeManager *vManager = graph_analysis::VertexTypeManager::getInstance();
    graph_analysis::Vertex::Ptr cc = graph_analysis::Vertex::Ptr(new ConfiguredComponent());
    vManager->registerType(cc);
    vManager->registerAttribute(cc->getClassName(), "config", (graph_analysis::VertexTypeManager::serialize_func_t) & ConfiguredComponent::serializeConfig,
                                (graph_analysis::VertexTypeManager::deserialize_func_t) & ConfiguredComponent::deSerializeConfig,
                                (graph_analysis::VertexTypeManager::print_func_t) & ConfiguredComponent::printConfig);
    vManager->registerAttribute(cc->getClassName(), "underlaying_name", (graph_analysis::VertexTypeManager::serialize_func_t) & ConfiguredComponent::serializeName,
                                (graph_analysis::VertexTypeManager::deserialize_func_t) & ConfiguredComponent::deserializeName,
                                (graph_analysis::VertexTypeManager::print_func_t) & ConfiguredComponent::serializeName);
}

void printHelp()
{
    std::cout << "Please use -f <filename> -i <instance-id>" << std::endl;
    exit(-1);
}

// main test function
int main(int argc, char *argv[])
{
    initializeExporter();
    char c;
    char *base_network_file = 0;
    std::vector<std::string> start_components;
    int instance_id = -1;

    while ((c = getopt(argc, argv, "f:i:")) != -1) {
        switch (c) {
            case 'i':
                instance_id = atoi(optarg);
                break;
            case 'f':
                base_network_file = optarg;
                break;
            default:
                printf("On default block\n");
        }
    }
    if (!base_network_file || instance_id == -1) {
        printHelp();
    }

    Pool *pool = XML::load(base_network_file);
    auto instances = XML::loadClassSolutions(base_network_file);

    std::string instance_graph_filename;
    for (auto i : instances) {
        if (i.solution_id == instance_id) {
            instance_graph_filename = i.graph_filename + std::string(".gexf");
            break;
        }
    }
    if (instance_graph_filename.empty()) {
        std::cout << "Cannot find ClassSolution id " << instance_id << std::endl;
        printHelp();
    }

    std::cout << "Loading graph: " << instance_graph_filename << std::endl;
    graph_analysis::BaseGraph::Ptr graph_imported = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::io::GraphIO::read(instance_graph_filename, graph_imported, graph_analysis::representation::GEXF);

    // Start to create our graph based on the imported graph
    for (auto node : graph_imported->getAllEdges()) {
        graph_analysis::Edge::Ptr e(new graph_analysis::Edge(node->getLabel()));
        const auto &v1 = pool->getComponent(node->getSourceVertex()->getLabel());
        const auto &v2 = pool->getComponent(node->getTargetVertex()->getLabel());
        assert(v1->getPtr());
        assert(v2->getPtr());
        e->setSourceVertex(v1->getPtr());
        e->setTargetVertex(v2->getPtr());
        graph->addEdge(e);
    }

    std::cout << "Start to create instance-solutions" << std::endl;
    auto is = InstanceSolution::babSearch(graph);
    std::cout << "All instance solutions are calculated" << std::endl;

    std::vector<std::pair<graph_analysis::BaseGraph::Ptr, std::list<TransitionTrigger>>> results;
    for (auto solution : is) {
        graph_analysis::DirectedGraphInterface::Ptr g = boost::reinterpret_pointer_cast<graph_analysis::DirectedGraphInterface>(solution);
        auto trigger_events = EventModelHandler(pool, g);
        auto events = trigger_events.getTrigger();
        /*
        std::cout << "Event size is: " <<events.size() << std::endl;
        for(auto e : events){
            std::cout << "\t- " << e.resulting_requirement.network.size() << std::endl;
        }
        */
        results.push_back({solution,events});
    }
    std::cout << "Calculated all follow solutions " << results.size() << std::endl;
    XML::addInstanceSolutions(instance_id,results,base_network_file);
    std::cout << "Finish" << std::endl;
return 0;
}
