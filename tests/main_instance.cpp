#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/ClassSolution.hpp>
#include <constrained_based_networks/InstanceSolution.hpp>
#include <constrained_based_networks/SpecializedComponent.hpp>
#include <graph_analysis/GraphIO.hpp>
#include "tests.hpp"
#include <constrained_based_networks/EventModelHandler.hpp>

using namespace constrained_based_networks;

// main test function
int main(int argc, char *argv[])
{
    bool debug = false;
    bool resolve_nonresolveable = false;
    int test_id = -1;
    char *file = 0;
    bool follow_reqs = true;

    char c;
    while ((c = getopt(argc, argv, "drt:f:")) != -1) {
        switch (c) {
            case 'd':
                debug = true;
                break;
            case 'r':
                resolve_nonresolveable = true;
                break;
            case 't':
                test_id = atoi(optarg);
                ;
                break;
            case 'f':
                file = optarg;
                ;
                break;
            case 'n':
                follow_reqs = false;
                break;
            default:
                printf("On default block\n");
        }
    }
    if (!file) {
        std::cerr << "Please use a file for testing" << std::endl << "-d  Debug mode" << std::endl << "-r  Resolve unresoloveable" << std::endl << "-t  Test ID for real-generated" << std::endl
                  << "-f  Filename (nonoptional" << std::endl;
    }

    std::string name = load_test(test_id);

    graph_analysis::BaseGraph::Ptr graph_imported = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    //    graph_analysis::io::GraphIO::read("output.yml", graph_imported, graph_analysis::representation::YAML);
    graph_analysis::io::GraphIO::read(file, graph_imported, graph_analysis::representation::GEXF);

    // Start to create our graph based on the imported graph
    for (auto node : graph_imported->getAllEdges()) {
        graph_analysis::Edge::Ptr e(new graph_analysis::Edge(node->getLabel()));
#if 0
        std::cout << "from " << node->getSourceVertex()->getLabel() << std::endl;
        std::cout << "to  " << node->getSourceVertex()->getLabel() << std::endl;
#endif
        const auto &v1 = pool->getComponent(node->getSourceVertex()->getLabel());
        const auto &v2 = pool->getComponent(node->getTargetVertex()->getLabel());
        assert(v1->getPtr());
        assert(v2->getPtr());

        e->setSourceVertex(v1->getPtr());
        e->setTargetVertex(v2->getPtr());
        graph->addEdge(e);
    }

    auto is = InstanceSolution::babSearch(graph);
    std::cout << "All instance solutions are calculated" << std::endl;
    size_t cnt = 0;
    for (auto solution : is) {
        std::stringstream filename;
        filename << file << "-instance-" << cnt << ".dot";
        graph_analysis::io::GraphIO::write(filename.str(), solution, graph_analysis::representation::GRAPHVIZ);

        Pool *p = pool;
        graph_analysis::DirectedGraphInterface::Ptr g = boost::reinterpret_pointer_cast<graph_analysis::DirectedGraphInterface>(solution);
        auto trigger_events = EventModelHandler(p, g);
        if (follow_reqs) {
            auto erg = trigger_events.getTrigger();
            for (auto p : erg) {
                // Restarting search
                size_t cnt2 = 0;
                if (p.resulting_requirement.pool) {
                    auto erg2 = ClassSolution::babSearch(p.resulting_requirement.pool);
                    for (auto graph : erg2) {
                        std::stringstream filename2;
                        filename2 << file << "-instance-" << cnt << "-follow-network-" << cnt2 << "-" << p.causing_component->getName() << "-" << p.causing_event << ".dot";
                        std::cout << "Found a follow-network for a solution, verticies:" << graph->size() << std::endl;
                        if (graph->size() == 0) {
                            auto v = graph_analysis::Vertex::Ptr(new graph_analysis::Vertex("Empty"));
                            graph->addVertex(v);
                        }
                        graph_analysis::io::GraphIO::write(filename2.str(), graph, graph_analysis::representation::GRAPHVIZ);
                        cnt2++;
                    }
                }
            }
            cnt++;
        }
        //Debug TODO
        break;
    }
    std::cout << "Found overall " << cnt << " solutions" << std::endl;
    return 0;
}
