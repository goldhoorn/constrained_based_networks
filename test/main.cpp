#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/Solution.hpp>

#include "dump.hpp"

using namespace constrained_based_networks;
Pool *pool;

void resolve(std::string name, bool res){
    
    Component *c = pool->getComponent(name);
    pool->getComponent(name);
    c->setActive(true);
    try{
        Solution* s = Solution::babSearch(pool);
//        Solution* s = Solution::gistBaBSeach();
        std::cout << "+++++ Is solveable: " << name << std::endl;
        s->rprint();
    }catch(std::runtime_error e){
        std::cout << "!!!!! UN-Solveable: " << name << " " << e.what() << " " << std::endl;
        if(res){ 
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
                    resolve(child.second->getName() + "_cmp",res);
                }else{
                    resolve(child.second->getName(),res);
                }
            }
        }
    }
}

std::string test_cmp_recursion2(){
//    auto a2 = new Composition("AuvControl::DepthFusionCmp");
    return "A";
}

std::string test_cmp_recursion_w_unused_CS(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    //auto a2 = new Composition("A2");
    auto a = new Composition("A");
    auto b = new Composition("B");
    auto b2 = new Composition("B2");
    a->addChild(b,"b_child");
    auto t = new Task("T");
    b->addChild(t,"t_child");
    return "A";
}

std::string test_ds(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = new Composition("A");
    auto b = new Task("B");
    auto ds = new DataService("DS");
    b->addFullfillment("DS");
    a->addChild(b,"b_child_is_ds");
    return "A";

}

std::string test_ambigious_ds(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = new Composition("A");
    auto b = new Task("B");
    auto b2 = new Task("B2");
    auto ds = new DataService("DS");

    b->addFullfillment("DS");
    b2->addFullfillment("DS");

    a->addChild(ds,"b_child_is_ds");
    return "A";

}

std::string test_cmp_recursion_w_more_used(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a2 = new Composition("A2");
    auto a = new Composition("A");
    auto b = new Composition("B");
    auto b2 = new Composition("B2");
    auto ds = new DataService("DS");
    auto ds2 = new DataService("DS2");


    b->addFullfillment("DS");
    b2->addFullfillment("DS");
    b->addFullfillment("DS2");
    b2->addFullfillment("DS2");

    a->addChild(ds,"b_child_is_ds");
    a2->addChild(ds,"b_child");
    auto t = new Task("T");
    b->addChild(t,"t_child");
    return "A";

}

std::string test_cmp_recursion_w_used_DS(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    //auto a2 = new Composition("A2");
    auto a = new Composition("A");
    auto b = new Composition("B");
    //auto b2 = new Composition("B2");
    auto ds = new DataService("DS");
    //auto ds2 = new DataService("DS2");


    b->addFullfillment("DS");
    //b2->addFullfillment("DS");
    //b->addFullfillment("DS2");
    //b2->addFullfillment("DS2");

    a->addChild(ds,"b_child_is_ds");
    //a2->addChild(ds,"b_child");
    auto t = new Task("T");
    b->addChild(t,"t_child");
    return "A";

}

std::string test_cmp_recursion_w_unused_DS(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A");
    auto b = new Composition("B");
    a->addChild(b,"b_child");
    auto t = new Task("T");
    b->addChild(t,"t_child");
    auto ds = new DataService("DS");
    return "A";
}

std::string test_cmp_recursion_w_unused_DS2(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A");
    auto b = new Composition("B");
    auto ds = new DataService("DS");
    a->addChild(b,"b_child");
    auto t = new Task("T");
    b->addChild(t,"t_child");
    return "A";
}

std::string test_cmp_recursion(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A");
    auto b = new Composition("B");
    a->addChild(b,"b_child");
    auto t = new Task("T");
    b->addChild(t,"t_child");
    return "A";
}

// main test function
int main(int argc, char* argv[]) {
    using namespace constrained_based_networks;
    int test_id =0;
    if(argc == 2 || argc ==3){
        test_id = atoi(argv[1]);
    }
    bool res = false;
    res = argc==3;

    std::string (*v[])() = {
        test_cmp_recursion,
        test_cmp_recursion_w_unused_CS,
        test_cmp_recursion_w_unused_DS,
        test_cmp_recursion_w_unused_DS2,
        test_cmp_recursion_w_used_DS,
        test_ds,
        test_ambigious_ds,
        test_cmp_recursion_w_more_used,
        create_model
    };


    std::string name = v[test_id]();
    if(name==""){
        name = "AuvControl::DepthFusionCmp";
    }
    
    
    pool = Pool::getInstance();
    //std::string name("Base::WorldXYPositionControllerSrv");
    //std::string name("AuvCont::WorldXYPositionCmp");
//    std::string name("Pipeline::Detector_new"); //Nicht ok!!!
    //std::string name("AuvControl::DepthFusionCmp"); //ok!!!
//    std::string name("Base::OrientationWithZSrv_cmp"); //Nicht ok!!!
//    std::string name("PoseAuv::PoseEstimatorBlindCmp"); //Nicht ok!!!

    //std::string name("Base::WorldXYPositionControllerSrv_cmp"); //NICHT ok
    //std::string name("Wall::DetectorNew"); //NICHT ok //Velocity Service
    //std::string name("Localization::ParticleDetector"); //NICHT ok //Hough service
    //std::string name("Localization::HoughDetector"); //NICHT ok //Hough service
    
    //std::string name("A"); //NICHT ok //Hough service

//    auto a = new Composition("A");
//    a->addChild(pool->getComponent("AuvControl::DepthFusionCmp"),"main_child");


    //pool->getComponent("Pipeline::Detector_new")->setActive(true);
    //pool->getComponent("Pipeline::Follower")->setActive(true);
    //pool->getComponent("AuvControl::DepthFusionCmp")->setActive(true);
    
    //try{ 
    resolve(name,res);
    //}catch(...){
    //    std::cerr << " Got maybe a out of mem error" << std::endl;
    //}
//    Solution* s = Solution::babSearch(pool);
//    s->rprint();
    return 0;
}
