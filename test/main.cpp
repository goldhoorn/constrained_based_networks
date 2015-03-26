#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/Solution.hpp>

#include "dump.hpp"

using namespace constrained_based_networks;
Pool *pool;

void resolve(std::string name){
    
    Component *c = pool->getComponent(name);
    pool->getComponent(name);
    c->setActive(true);
    try{
        Solution* s = Solution::babSearch(pool);
        std::cout << "+++++ Is solveable: " << name << std::endl;
        s->rprint();
    }catch(...){
        std::cout << "!!!!! UN-Solveable: " << name << std::endl;
        c->setActive(false);
        Composition *comp = dynamic_cast<Composition*>(c);
        if(!comp){
            std::cerr << name << " is No compositon FATAL" << std::endl;
            return;
        }
        for(auto child : comp->getChildren()){
            if(child.second->abstract()){
                if(name == (child.second->getName() + "_cmp")){
                    std::cerr << "Cannot finally solve " <<  name << std::endl;
                    return;
                }
                resolve(child.second->getName() + "_cmp");
            }else{
                resolve(child.second->getName());
            }
        }
    }
}


void test_cmp_recursion(){
    auto a = new Composition("A");
    auto b = new Composition("B");
    a->addChild(b,"b_child");
    auto t = new Task("T");
    b->addChild(t,"t_child");

}

// main test function
int main(int argc, char* argv[]) {
    using namespace constrained_based_networks;
    //test_cmp_recursion();
    //std::string name("A");
    
    create_model();
    pool = Pool::getInstance();







    //std::string name("Base::WorldXYPositionControllerSrv");
    //std::string name("AuvCont::WorldXYPositionCmp");
//    std::string name("Pipeline::Detector_new"); //Nicht ok!!!
//    std::string name("AuvControl::DepthFusionCmp"); //ok!!!
    std::string name("Base::OrientationWithZSrv_cmp"); //Nicht ok!!!

    //std::string name("Base::WorldXYPositionControllerSrv_cmp"); //NICHT ok
    //std::string name("Wall::DetectorNew"); //NICHT ok //Velocity Service
    //std::string name("Localization::ParticleDetector"); //NICHT ok //Hough service
    //std::string name("Localization::HoughDetector"); //NICHT ok //Hough service



    //pool->getComponent("Pipeline::Detector_new")->setActive(true);
    //pool->getComponent("Pipeline::Follower")->setActive(true);
    
  
    resolve(name);
//    Solution* s = Solution::babSearch(pool);
//    s->rprint();
    return 0;
}
