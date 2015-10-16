#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/ClassSolution.hpp>
#include <constrained_based_networks/InstanceSolution.hpp>
#include <constrained_based_networks/SpecializedComponent.hpp>
#include <graph_analysis/GraphIO.hpp>
#include  <sys/stat.h>

#include "tests.hpp"
using namespace constrained_based_networks;



//void resolve(std::string name, bool res, bool debug = false){
std::vector<graph_analysis::BaseGraph::Ptr> resolve(Component *c, bool res, bool debug = false){
    /*
        std::cout << "Resolving for component: " <<  c->getName() << " (" << c->getID() << ")" << std::endl;
        //Checking check check
        if(c != pool->operator[](c->getID())){
            std::cout << "Failig id in step 1: " << c->getID() << " for component with name: " << c->getName() << std::endl;
            if(auto spec = dynamic_cast<SpecializedComponentBase *>(c)){
                std::cout << "It is a specialized one, bsae id: " << spec->getOrginal()->getID() << std::endl;
            }
            throw std::runtime_error("1 WHAAAAAAAAARRRRRRRRRRRRRRRRRRRR            Pool is inconsistent to IDs");
        }
*/
    //Check again
    for(auto co: pool->getItems<Component*>()){
        if(co != pool->operator[](co->getID())){
            std::cout << "Check in resolve for the following component failed: " << c->getName() << std::endl;
            std::cout << "Failig id: " << co->getID() << " for component with name: " << co->getName() << std::endl;
            if(auto spec = dynamic_cast<SpecializedComponentBase*>(co)){
                std::cout << "It is a specialized one: " << spec->getID() << " and base id: " << spec->getComponent()->getID() << std::endl;
            }
            throw std::runtime_error("Pool is inconsistent to IDs");
        }
    }

    if(auto sm =  dynamic_cast<StateMachine*>(c)){
        for(auto state : sm->getStates()){
            printf("??????????????      ClassSolution for statemachien %s, for task %s ??????????????????????????\n",sm->getName().c_str(),state->getName().c_str());
            if(dynamic_cast<SpecializedComponentBase *>(state)){
                std::cout << "Habe specialized component" << std::endl;
            }
            return resolve(state,res,debug);
            printf("??????????????      !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! ??????????????????????????\n");
        }
    }

    c->setActive(true);
/*
    if(!dynamic_cast<SpecializedComponentBase *>(c)){
        throw std::runtime_error("I currently want a specialized\n");
    }else{
        std::cout << "starte: \n";
        std::cout << dynamic_cast<SpecializedComponentBase *>(c)->getComponent()->getName() << ": " << dynamic_cast<SpecializedComponentBase *>(c)->getComponent()->getID() << std::endl;
        std::cout << "pointer in main: " << c  << " | " << dynamic_cast<SpecializedComponentBase *>(c)->getComponent() << std::endl;
        std::cout << "Pool in main: " << pool << std::endl;
//        std::cout << "pointer in vector in main: " << pool->operator[](360)  << std::endl;

    }
*/
/**
        if(c != pool->operator[](c->getID())){
            std::cout << "Failig id: " << c->getID() << " for component with name: " << c->getName() << std::endl;
            if(auto spec = dynamic_cast<SpecializedComponentBase *>(c)){
                std::cout << "It is a specialized one, bsae id: " << spec->getOrginal()->getID() << std::endl;
            }
            throw std::runtime_error("2 WHAAAAAAAAARRRRRRRRRRRRRRRRRRRR            Pool is inconsistent to IDs");
        }
        */

/*
    for(auto c : pool->getItems<Component*>()){
        auto d = dynamic_cast<SpecializedComponentBase*>(c);
        if(d){
            std::cout << "test 2: \n";
            std::cout << d->getComponent()->getName() << ": " << d->getComponent()->getID() << std::endl;
            std::cout << "pointer in main: " << d  << " | " << dynamic_cast<SpecializedComponentBase *>(d)->getComponent() << std::endl;
        }
    }
*/
    try{
        std::vector<graph_analysis::BaseGraph::Ptr> erg;
        if(debug){
            //TODO
            ClassSolution::gistBaBSeach(pool);
        }else{
            erg = ClassSolution::babSearch(pool);
        }

#if 0
        std::cout << "+++++ Is solveable: " << c->getName() << std::endl;
        s->rprint();
        std::cout << "End ClassSolution " << c->getName() << std::endl;
#endif
        return erg;

    }catch(std::runtime_error e){
        std::cout << "!!!!! UN-Solveable: " << c->getName() << " " << e.what() << " " << std::endl;
        if(res){
            c->setActive(false);
            Composition *comp = dynamic_cast<Composition*>(c);
            if(!comp){
                std::cerr << c->getName() << " is No compositon FATAL" << std::endl;
                return std::vector<graph_analysis::BaseGraph::Ptr>();
            }
            for(auto child : comp->getChildren()){
                if(child.second->abstract()){
                    if(c->getName() == (child.second->getName() + "_cmp")){
                        std::cerr << "Cannot finally solve " <<  c->getName() << std::endl;
                        return std::vector<graph_analysis::BaseGraph::Ptr>();
                    }
                    //TODO fix here specializations got lost
                    return resolve(pool->getComponent(child.second->getName() + "_cmp"),res,debug);
                }else{
                    return resolve(child.second,res, debug);
                }
            }
        }
        return std::vector<graph_analysis::BaseGraph::Ptr>();
    }
}

// main test function
int main(int argc, char* argv[]) {
    bool debug = false;
    bool resolve_nonresolveable = false;
    int test_id =0;

    char c;
    while((c=getopt(argc,argv,"dr")) != -1){
        switch(c){
            case 'd':
                debug = true;
                break;
            case 'r':
                resolve_nonresolveable=true;
                break;
            default:
                printf("On default block\n");
        }
    }

    if(optind == argc){
        std::cerr << "please pass testname" << std::endl;
        exit(-1);
    }
    test_id = atoi(argv[optind]);
    std::string name = load_test(test_id);

    //try{
    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);

    for(auto c: pool->getItems<Component*>()){
        if(c != pool->operator[](c->getID())){
            std::cout << "Failig id: " << c->getID() << " for component with name: " << c->getName() << std::endl;
            if(auto spec = dynamic_cast<SpecializedComponentBase*>(c)){
                std::cout << "It is a specialized one: " << spec->getID() << " and base id: " << spec->getComponent()->getID() << std::endl;
            }
            throw std::runtime_error("Pool is inconsistent to IDs");
        }
    }

    std::stringstream folder;
    folder << "result-" << test_id << "/";

    mkdir(folder.str().c_str(),0755);
    unsigned int cnt=0;
    for(auto graph : resolve(pool->getComponent(name),resolve_nonresolveable,debug)){
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
    return 0;
}
