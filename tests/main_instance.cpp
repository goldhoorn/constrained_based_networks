#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/ClassSolution.hpp>
#include <constrained_based_networks/InstanceSolution.hpp>
#include <constrained_based_networks/SpecializedComponent.hpp>
#include <graph_analysis/GraphIO.hpp>
#include "tests.hpp"

using namespace constrained_based_networks;


// main test function
int main(int argc, char* argv[]) {
    bool debug = false;
    bool resolve_nonresolveable = false;
    int test_id =-1;
    char *file=0;

    char c;
    while((c=getopt(argc,argv,"drt:f:")) != -1){
        switch(c){
            case 'd':
                debug = true;
                break;
            case 'r':
                resolve_nonresolveable=true;
                break;
            case 't':
                test_id=atoi(optarg);;
                break;
            case 'f':
                file=optarg;;
                break;

            default:
                printf("On default block\n");
        }
    }
    if(!file){
        std::cerr << "Please use a file for testing" << std::endl
            << "-d  Debug mode" << std::endl
            << "-r  Resolve unresoloveable" << std::endl
            << "-t  Test ID for real-generated" << std::endl
            << "-f  Filename (nonoptional" << std::endl;
    }

    std::string name = load_test(test_id);

    graph_analysis::BaseGraph::Ptr graph_imported = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
//    graph_analysis::io::GraphIO::read("output.yml", graph_imported, graph_analysis::representation::YAML);
    graph_analysis::io::GraphIO::read(file, graph_imported, graph_analysis::representation::GEXF);

    //Start to create our graph based on the imported graph
    for (auto node : graph_imported->getAllEdges()) {
        graph_analysis::Edge::Ptr e(new graph_analysis::Edge());
        std::cout << "from " << node->getSourceVertex()->getLabel() << std::endl;
        std::cout << "to  " << node->getSourceVertex()->getLabel() << std::endl;

        const auto &v1 = pool->getComponent(node->getSourceVertex()->getLabel());
        const auto &v2 = pool->getComponent(node->getTargetVertex()->getLabel());
        assert(v1->getPtr());
        assert(v2->getPtr());

        e->setSourceVertex(v1->getPtr());
        e->setTargetVertex(v2->getPtr());
        graph->addEdge(e);
    }


    auto is = InstanceSolution::babSearch(graph);
    (void)is;
//    is->rprint();

    return 0;
}
