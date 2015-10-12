#pragma once

#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/ClassSolution.hpp>
#include <constrained_based_networks/InstanceSolution.hpp>
#include <constrained_based_networks/SpecializedComponent.hpp>
#include <graph_analysis/GraphIO.hpp>
#include "dump.hpp"
#include  <sys/stat.h>

using namespace constrained_based_networks;
Pool *pool;

struct Tests{
    std::string (*v)();
    std::string name;
};


std::string test_cmp_recursion_w_unused_CS(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    //auto a2 = new Composition("A2");
    auto a = new Composition("A");
    auto b = new Composition("B");
    //auto b2 = new Composition("B2");
    a->addChild(b,"b_child");
    auto t = new Task("T");
    b->addChild(t,"t_child");
    return "A";
}

std::string test_ds(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = new Composition("A");
    auto b = new Task("B");
    //auto ds = new DataService("DS");
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
    //auto ds2 = new DataService("DS2");


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
    //auto ds = new DataService("DS");
    return "A";
}

std::string test_cmp_recursion_w_unused_DS2(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A");
    auto b = new Composition("B");
    //auto ds = new DataService("DS");
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

std::string test_cmp_recursion_child2_time(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A");
    auto b = new Composition("B");
    auto t = new Task("T");
    a->addChild(t,"t_child");
    a->addChild(b,"b_child");
    b->addChild(t,"t_child");
    return "A";
}

std::string test_cmp_recursion_child2_time_abstract(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto ds2 = new DataService("DS2-task");
    auto a = new Composition("A");
    auto b = new Composition("B");
    auto t = new Task("T");
    a->addChild(ds2,"t_abstract_child");
    a->addChild(b,"b_child");
    b->addChild(ds2,"t_abstract_child");
    //auto a2 = new Composition("A2");
    //auto a3 = new Composition("A3");
    auto b3 = new Composition("B2");
    auto b2 = new Composition("B2");
    //auto ds = new DataService("DS");

    b3->addChild(t,"task_child");
    b3->addFullfillment(ds2->getName());

    t->addFullfillment(ds2->getName());
    b3->addFullfillment("DS");
    b2->addFullfillment("DS");
    b3->addFullfillment("DS2");
    b2->addFullfillment("DS2");

    return "A";
}

std::string test_possible_loop_unused(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A");
    auto a2 = new Composition("A2");
    auto b = new Composition("B");
    auto c = new Composition("C");
    auto t = new Task("T-not");
    auto t2 = new Task("T-yes");

    a->addChild(a2,"a2_child");
    a2->addChild(t2,"t_child");

    b->addChild(c,"c_child");
    c->addChild(t,"task_child");
    c->addChild(b,"b_child");

    return "A";
}

std::string test_possible_loop_unused2(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A");
    auto a2 = new Composition("A2");
    auto b = new Composition("B");
    auto c = new Composition("C");
    auto d = new Composition("D");
    auto t = new Task("T");

    a->addChild(a2,"a2_child");

    b->addChild(c,"c_child");
    c->addChild(t,"task_child");
    c->addChild(d,"d_child");
    d->addChild(b,"b_child");

    return "A";
}


std::string test_ambigious_configs(){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a1 = new Composition("A1");
    auto a2 = new Composition("A2");
    auto t1 = new Task("T1");
    t1->addProperty("int_config",constrained_based_networks::ConfigurationModel::INT);

    a1->addChild(a2,"a2_child");

    a1->addChild(t1,"task_child");
    a2->addChild(t1,"task_child");

    return "A1";
}

    Tests tests[] = {
        {test_cmp_recursion, ""},
        {test_cmp_recursion_w_unused_CS,""},
        {test_cmp_recursion_w_unused_DS,""},
        {test_cmp_recursion_w_unused_DS2,""},
        {test_cmp_recursion_w_used_DS,""},
        {test_ds,""},
        {test_ambigious_ds,""},
        {test_cmp_recursion_w_more_used,""},
        {test_cmp_recursion_child2_time,""},
        {test_cmp_recursion_child2_time_abstract,""},
        {test_possible_loop_unused,""},
        {test_possible_loop_unused2,""},
        {test_ambigious_configs,""},
        {create_model,"AuvControl::DepthFusionCmp"},
        {create_model,"Pipeline::Follower"},
        {create_model,"Buoy::DetectorNewCmp"},
        {create_model,"Wall::Follower"},
        {create_model,"Main::LawnMoverOverPipe"},
        {create_model,"AuvControl::SimpleMove"},
        {0,""}
    };

std::string load_test(int nr=-1){
    if(nr==-1){
        create_model();
        return "";
    }

    printf("Running test: %i\n",nr);

    std::string name;
    if(tests[nr].name == ""){
        name = tests[nr].v();
    }else{
        tests[nr].v();
        name = tests[nr].name;
    }

    pool = Pool::getInstance();

    pool->mergeDoubles();

    printf("Running test with name: %s\n",name.c_str());
    return name;

}

