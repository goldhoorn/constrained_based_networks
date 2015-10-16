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
    std::string (*v)(constrained_based_networks::Pool*);
    std::string name;
};


std::string test_cmp_recursion_w_unused_CS(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    //auto a2 = new Composition("A2");
    auto a = new Composition("A",pool);
    auto b = new Composition("B",pool);
    //auto b2 = new Composition("B2");
    a->addChild(b,"b_child");
    auto t = new Task("T",pool);
    b->addChild(t,"t_child");
    return "A";
}

std::string test_ds(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = new Composition("A",pool);
    auto b = new Task("B",pool);
    //auto ds = new DataService("DS");
    b->addFullfillment("DS");
    a->addChild(b,"b_child_is_ds");
    return "A";

}

std::string test_ambigious_ds(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = new Composition("A",pool);
    auto b = new Task("B",pool);
    auto b2 = new Task("B2",pool);
    auto ds = new DataService("DS",pool);

    b->addFullfillment("DS");
    b2->addFullfillment("DS");

    a->addChild(ds,"b_child_is_ds");
    return "A";

}

std::string test_cmp_recursion_w_more_used(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a2 = new Composition("A2",pool);
    auto a = new Composition("A",pool);
    auto b = new Composition("B",pool);
    auto b2 = new Composition("B2",pool);
    auto ds = new DataService("DS",pool);
    //auto ds2 = new DataService("DS2");


    b->addFullfillment("DS");
    b2->addFullfillment("DS");
    b->addFullfillment("DS2");
    b2->addFullfillment("DS2");

    a->addChild(ds,"b_child_is_ds");
    a2->addChild(ds,"b_child");
    auto t = new Task("T",pool);
    b->addChild(t,"t_child");
    return "A";

}

std::string test_cmp_recursion_w_used_DS(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    //auto a2 = new Composition("A2");
    auto a = new Composition("A",pool);
    auto b = new Composition("B",pool);
    //auto b2 = new Composition("B2");
    auto ds = new DataService("DS",pool);
    //auto ds2 = new DataService("DS2");


    b->addFullfillment("DS");
    //b2->addFullfillment("DS");
    //b->addFullfillment("DS2");
    //b2->addFullfillment("DS2");

    a->addChild(ds,"b_child_is_ds");
    //a2->addChild(ds,"b_child");
    auto t = new Task("T",pool);
    b->addChild(t,"t_child");
    return "A";

}

std::string test_cmp_recursion_w_unused_DS(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A",pool);
    auto b = new Composition("B",pool);
    a->addChild(b,"b_child");
    auto t = new Task("T",pool);
    b->addChild(t,"t_child");
    //auto ds = new DataService("DS");
    return "A";
}

std::string test_cmp_recursion_w_unused_DS2(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A",pool);
    auto b = new Composition("B",pool);
    //auto ds = new DataService("DS");
    a->addChild(b,"b_child");
    auto t = new Task("T",pool);
    b->addChild(t,"t_child");
    return "A";
}

std::string test_cmp_recursion(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A",pool);
    auto b = new Composition("B",pool);
    a->addChild(b,"b_child");
    auto t = new Task("T",pool);
    b->addChild(t,"t_child");
    return "A";
}

std::string test_cmp_recursion_child2_time(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A",pool);
    auto b = new Composition("B",pool);
    auto t = new Task("T",pool);
    a->addChild(t,"t_child");
    a->addChild(b,"b_child");
    b->addChild(t,"t_child");
    return "A";
}

std::string test_cmp_recursion_child2_time_abstract(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto ds2 = new DataService("DS2-task",pool);
    auto a = new Composition("A",pool);
    auto b = new Composition("B",pool);
    auto t = new Task("T",pool);
    a->addChild(ds2,"t_abstract_child");
    a->addChild(b,"b_child");
    b->addChild(ds2,"t_abstract_child");
    //auto a2 = new Composition("A2");
    //auto a3 = new Composition("A3");
    auto b3 = new Composition("B2",pool);
    auto b2 = new Composition("B2",pool);
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

std::string test_possible_loop_unused(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A",pool);
    auto a2 = new Composition("A2",pool);
    auto b = new Composition("B",pool);
    auto c = new Composition("C",pool);
    auto t = new Task("T-not",pool);
    auto t2 = new Task("T-yes",pool);

    a->addChild(a2,"a2_child");
    a2->addChild(t2,"t_child");

    b->addChild(c,"c_child");
    c->addChild(t,"task_child");
    c->addChild(b,"b_child");

    return "A";
}

std::string test_possible_loop_unused2(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A",pool);
    auto a2 = new Composition("A2",pool);
    auto b = new Composition("B",pool);
    auto c = new Composition("C",pool);
    auto d = new Composition("D",pool);
    auto t = new Task("T",pool);

    a->addChild(a2,"a2_child");

    b->addChild(c,"c_child");
    c->addChild(t,"task_child");
    c->addChild(d,"d_child");
    d->addChild(b,"b_child");

    return "A";
}

std::string test_state_machine(constrained_based_networks::Pool* pool){
    auto a1 = new Composition("pipe_detector",pool);
    auto t1 = new Task("pipe-cam-task",pool);
    a1->addChild(t1,"cam-detekt");
    t1->addEvent("pipe-end");
    a1->addEvent("fertig");
    a1->addEventForwards("cam-detekt","pipe-end","fertig");

    auto a2 = new Composition("wall-servoing",pool);
    auto t2 = new Task("wall-task",pool);
    a2->addChild(t2,"wall-detector-child");


    auto sm = new StateMachine("SM1",pool);

    sm->addTransition(a1,a2,a1,"fertig");
    sm->setStart(a1);

    return "SM1";
}


std::string test_ambigious_configs(constrained_based_networks::Pool* pool){
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a1 = new Composition("A1",pool);
    auto a2 = new Composition("A2",pool);
    auto t1 = new Task("T1",pool);
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
        {test_state_machine,""},
        {create_model,"AuvControl::DepthFusionCmp"},
        {create_model,"Pipeline::Follower"},
        {create_model,"Buoy::DetectorNewCmp"},
        {create_model,"Wall::Follower"},
        {create_model,"Main::LawnMoverOverPipe"},
        {create_model,"AuvControl::SimpleMove"},
        {0,""}
    };

std::string load_test(int nr=-1){
    pool = new Pool();

    std::string name = "";
    if(nr==-1){
        printf("Creating model from export\n");
        create_model(pool);
        pool->dupFunction = create_model;
    }else{
        //printf("Running test: %i\n",nr);
        if(tests[nr].name == ""){
            name = tests[nr].v(pool);
            pool->dupFunction = tests[nr].v;
        }else{
            tests[nr].v(pool);
            pool->dupFunction = tests[nr].v;
            name = tests[nr].name;
        }
        printf("Running test with name: %s\n",name.c_str());
    }

    pool->mergeDoubles();

    return name;

}

