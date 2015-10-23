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

// void resolve(std::string name, bool res, bool debug = false){
std::vector<graph_analysis::BaseGraph::Ptr> resolve(Component *c, bool res, bool debug = false)
{
    //Check pool consitency
    for (auto co : pool->getItems<Component *>()) {
        if (co != pool->operator[](co->getID())) {
            std::cout << "Check in resolve for the following component failed: " << c->getName() << std::endl;
            std::cout << "Failig id: " << co->getID() << " for component with name: " << co->getName() << std::endl;
            if (auto spec = dynamic_cast<SpecializedComponentBase *>(co)) {
                std::cout << "It is a specialized one: " << spec->getID() << " and base id: " << spec->getComponent()->getID() << std::endl;
            }
            throw std::runtime_error("Pool is inconsistent to IDs");
        }
    }

    if(c){
        c->setActive(true);
    }

    try
    {
        std::vector<graph_analysis::BaseGraph::Ptr> erg;
        if (debug) {
            // TODO
            ClassSolution::gistBaBSeach(pool);
        } else {
            erg = ClassSolution::babSearch(pool);
        }

#if 0
        std::cout << "+++++ Is solveable: " << c->getName() << std::endl;
        s->rprint();
        std::cout << "End ClassSolution " << c->getName() << std::endl;
#endif
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

    // try{
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
        std::stringstream s2;
        s2 << folder.str() << "graph-" << std::setw(4) << std::setfill('0') << cnt;
        graph_analysis::io::GraphIO::write(s2.str(), graph, graph_analysis::representation::GRAPHVIZ);
        ++cnt;
    }
#if 0
//        InstanceSolution::gistBaBSeach(s);
        std::cout << "Finished calculuation of Instance InstanceSolution, Solution is:" << std::endl;
        std::cout << "################################################################################"<< std::endl;
        std::cout << "################################################################################"<< std::endl;
        std::cout << "################################################################################"<< std::endl;
        auto is = InstanceSolution::babSearch(graph);
        is->rprint();
    }else{
        std::cerr << "Cannot create instance solution, class resolution does not return" << std::endl;
    }
#endif

    //}catch(...){
    //    std::cerr << " Got maybe a out of mem error" << std::endl;
    //}
    //    ClassSolution* s = ClassSolution::babSearch(pool);
    //    s->rprint();
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

    while ((c = getopt(argc, argv, "hadrt:n:")) != -1) {
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

