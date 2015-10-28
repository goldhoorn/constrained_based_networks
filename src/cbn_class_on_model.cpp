#include "XML.hpp"
#include <unistd.h>
#include <iostream>
#include "Pool.hpp"
#include "ClassSolution.hpp"
#include "DataService.hpp"
#include <graph_analysis/GraphIO.hpp>

using namespace constrained_based_networks;

void printHelp()
{
    std::cout << "Please use -f <filename>" << std::endl;
    exit(-1);
}

// main test function
int main(int argc, char *argv[])
{
    char c;
    char *base_network_file = 0;
    std::vector<std::string> start_components;

    while ((c = getopt(argc, argv, "c:f:")) != -1) {
        switch (c) {
            case 'c':
                start_components.push_back(optarg);
                break;
            case 'f':
                base_network_file = optarg;
                break;
            default:
                printf("On default block\n");
        }
    }
    if (!base_network_file) {
        printHelp();
    }

    Pool *pool = XML::load(base_network_file);
    for (auto s : start_components) {
        if (!pool->hasComponent(s)) {
            std::cerr << "Cannot start component: " << s << ", it does not exist in the pool" << std::endl;
            return -2;
        }
        pool->getComponent(s)->setActive(true);
    }
    std::cout << "Pool size is: " << pool->size() << std::endl;
    std::cout << "Compositions: " << pool->getCount<Composition*>() << std::endl;
    std::cout << "Task: " << pool->getCount<Task*>() << std::endl;
    std::cout << "StateMachine: " << pool->getCount<StateMachine*>() << std::endl;
    std::cout << "DataService: " << pool->getCount<DataService*>() << std::endl;
    for(auto c : pool->getItems<Component*>()){
        if(c->isActive()){
            std::cout << "Should start: " << c->getName() << std::endl;
            if(auto cmp = dynamic_cast<Composition*>(c)){
                for(auto child : cmp->getChildren()){
                    std::cout << "\t -child: " << child.second->getName() << std::endl;
                }
            }
        }
    }
    auto solutions = ClassSolution::babSearch(pool);
    std::stringstream s;
    s << base_network_file;
    for(auto c : start_components){
     s << "-" << c ;
    }
    s << "-class-result.xml";
    XML::save(solutions,base_network_file,s.str());
    /*
    for (auto solution : ClassSolution::babSearch(pool)) {
        std::stringstream s;
        s << "network-" << std::setw(4) << std::setfill('0') << i;
        graph_analysis::io::GraphIO::write(s.str(), solution, graph_analysis::representation::GEXF);
        i++;
    }
    std::cout << "Finished class solution, found " << i << " Networks" << std::endl;
    */
    std::cout << "Ende" << std::endl;
    return 0;
}

#if 0
#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/ClassSolution.hpp>
#include <constrained_based_networks/InstanceSolution.hpp>
#include <constrained_based_networks/SpecializedComponent.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <sys/stat.h>

#include "tests.hpp"
using namespace constrained_based_networks;

bool debug = false;
bool resolve_nonresolveable = false;

std::vector<graph_analysis::BaseGraph::Ptr> resolve(char *base_network_file, bool res, bool debug = false){
    std::vector<graph_analysis::BaseGraph::Ptr> erg;

    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::io::GraphIO::read(base_network_file, graph, graph_analysis::representation::GEXF);

    // Start to create our graph based on the imported graph
    for (auto node : graph->getAllEdges()) {
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

    return erg;
}

std::vector<graph_analysis::BaseGraph::Ptr> resolve(Component *c, bool res, bool debug = false)
{
    pool->checkConsistency();

    if(c){
        c->setActive(true);
    }

    try
    {
        std::vector<graph_analysis::BaseGraph::Ptr> erg;
        if (debug) {
            ClassSolution::gistBaBSeach(pool);
        } else {
            erg = ClassSolution::babSearch(pool);
        }
        return erg;
    }
    catch (std::runtime_error e)
    {
        std::cout << "!!!!! UN-Solveable: " << c->getName() << " " << e.what() << " " << std::endl;
        std::cerr << "Active components: " << std::endl;
        for(auto component : pool->getItems<Component*>()){
            if(component->isActive()){
                std::cerr << "\t- " << component->getName() << std::endl;
            }
        }
        std::cerr << "Children are: " << std::endl;
        if(Composition *comp = dynamic_cast<Composition *>(c)){
            for(auto child : comp->getChildren()){
            std::cerr << "\t- " << child.second->getName() << " (" << child.first << ")" << std::endl;
            }
        }else{
            std::cerr << "\t IS NO COMPOSITION" << std::endl;
        }

        if (res) {
            c->setActive(false);
            Composition *comp = dynamic_cast<Composition *>(c);
            if (!comp) {
                std::cerr << c->getName() << " is No compositon FATAL" << std::endl;
                return std::vector<graph_analysis::BaseGraph::Ptr>();
            }
            for (auto child : comp->getChildren()) {
                if (child.second->abstract()) {
                    if (c->getName() == (child.second->getName() + "_cmp")) {
                        std::cerr << "Cannot finally solve " << c->getName() << std::endl;
                        return std::vector<graph_analysis::BaseGraph::Ptr>();
                    }
                    // TODO fix here specializations got lost
                    return resolve(pool->getComponent(child.second->getName() + "_cmp"), res, debug);
                } else {
                    return resolve(child.second, res, debug);
                }
            }
        }
        return std::vector<graph_analysis::BaseGraph::Ptr>();
    }
}


void runTest(std::string name){
    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);


    std::stringstream folder;
    folder << "result-" << name << "/";

    mkdir(folder.str().c_str(), 0755);
    unsigned int cnt = 0;
    Component *c=0;
    if(!name.empty()){
        c = pool->getComponent(name);
    }
    for (auto graph : resolve(c, resolve_nonresolveable, debug)) {
        std::cout << "Finished calculuation of ClassSolution number" << cnt << std::endl;
        std::stringstream s;
        s << folder.str() << "output-" << std::setw(4) << std::setfill('0') << cnt;
        graph_analysis::io::GraphIO::write(s.str(), graph, graph_analysis::representation::GEXF);
        ++cnt;
    }
}

void runTest(int test_id){
    std::string name = load_test(test_id);
    runTest(name);
}

// main test function
int main(int argc, char *argv[])
{
    int test_id = -1;
    bool all = false;
    char c;
    char *testname = 0;
    char *base_network_file= 0;

    while ((c = getopt(argc, argv, "hadrt:n:f:")) != -1) {
        switch (c) {
            case 'n':
                testname = optarg;
                break;
            case 'a':
                all = true;
                break;
            case 'd':
                debug = true;
                break;
            case 't':
                test_id = atoi(optarg);
                ;
                break;
            case 'r':
                resolve_nonresolveable = true;
                break;
            case 'h':
                printTests();
                return 0;
            case 'f':
                base_network_file = optarg;
                return 0;
            default:
                printf("On default block\n");
        }
    }
    if (test_id == -1 && !all && !testname) {
        std::cerr << "please pass testname with -t <id>" << std::endl;
        exit(-1);
    }


    if(all){
        size_t test_count = (sizeof(tests)/sizeof(Tests))-1;
        std::cout << "Have " << sizeof(tests)/sizeof(Tests) << std::endl;
        for(size_t i=0;i<test_count-1;i++){
            runTest(i);
        }
        delete pool;
    }else{
        if(testname){
            //Load the default test
            load_test(-1);
            runTest(testname);
        }else{
            runTest(test_id);
        }
        delete pool;
    }

    std::cout << "Finished" << std::endl;
    return 0;
}

#endif
