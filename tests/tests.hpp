#pragma once

#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/ClassSolution.hpp>
#include <constrained_based_networks/InstanceSolution.hpp>
#include <constrained_based_networks/SpecializedComponent.hpp>
#include <graph_analysis/GraphIO.hpp>
//#include "dump.hpp"
#include <sys/stat.h>
#include <execinfo.h>
#include <cxxabi.h>

using namespace constrained_based_networks;
Pool* pool;

struct Tests
{
    std::string (*v)(constrained_based_networks::Pool*);
    std::string name;
};

std::string test_cmp_recursion_w_unused_CS(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    // auto a2 = CompositionObj::make("A2");
    auto a = CompositionObj::make( pool,"A");
    auto b = CompositionObj::make( pool,"B");
    // auto b2 = CompositionObj::make("B2");
    a->addChild(b, "b_child");
    auto t = TaskObj::make( pool,"T");
    b->addChild(t, "t_child");
    return "A";
}


std::string create_model(constrained_based_networks::Pool *pool){
    throw std::runtime_error("Unsupportd");
}

std::string test_ds(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = CompositionObj::make( pool,"A");
    auto b = TaskObj::make( pool,"B");
    // auto ds = new DataService("DS");
    b->addFullfillment("DS");
    a->addChild(b, "b_child_is_ds");
    return "A";
}

std::string test_ambigious_ds(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = CompositionObj::make( pool,"A");
    auto t = TaskObj::make( pool,"T");
    auto t2 = TaskObj::make( pool,"T2");
    auto ds = DataServiceObj::make( pool,"DS");

    t->addFullfillment("DS");
    t2->addFullfillment("DS");

    a->addChild(ds, "t_child");
    return "A";
}

std::string test_cmp_recursion_w_more_used(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a2 = CompositionObj::make( pool,"A2");
    auto a = CompositionObj::make( pool,"A");
    auto b = CompositionObj::make( pool,"B");
    auto b2 = CompositionObj::make( pool,"B2");
    auto ds = DataServiceObj::make( pool,"DS");
    // auto ds2 = new DataService("DS2");

    b->addFullfillment("DS");
    b2->addFullfillment("DS");
    b->addFullfillment("DS2");
    b2->addFullfillment("DS2");

    a->addChild(ds, "b_child_is_ds");
    a2->addChild(ds, "b_child");
    auto t = TaskObj::make( pool,"T");
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion_w_used_DS(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    // auto a2 = CompositionObj::make("A2");
    auto a = CompositionObj::make( pool,"A");
    auto b = CompositionObj::make( pool,"B");
    // auto b2 = CompositionObj::make("B2");
    auto ds = DataServiceObj::make( pool,"DS");
    // auto ds2 = new DataService("DS2");

    b->addFullfillment("DS");
    // b2->addFullfillment("DS");
    // b->addFullfillment("DS2");
    // b2->addFullfillment("DS2");

    a->addChild(ds, "b_child_is_ds");
    // a2->addChild(ds,"b_child");
    auto t = TaskObj::make( pool,"T");
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion_w_unused_DS(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = CompositionObj::make( pool,"A");
    auto b = CompositionObj::make( pool,"B");
    a->addChild(b, "b_child");
    auto t = TaskObj::make( pool,"T");
    b->addChild(t, "t_child");
    // auto ds = new DataService("DS");
    return "A";
}

std::string test_cmp_recursion_w_unused_DS2(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = CompositionObj::make( pool,"A");
    auto b = CompositionObj::make( pool,"B");
    // auto ds = new DataService("DS");
    a->addChild(b, "b_child");
    auto t = TaskObj::make( pool,"T");
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = CompositionObj::make( pool,"A");
    auto b = CompositionObj::make( pool,"B");
    a->addChild(b, "b_child");
    auto t = TaskObj::make( pool,"T");
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion_child2_time(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = CompositionObj::make( pool,"A");
    auto b = CompositionObj::make( pool,"B");
    auto t = TaskObj::make( pool,"T");
    a->addChild(t, "t_child");
    a->addChild(b, "b_child");
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion_child2_time_abstract(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto ds2 = DataServiceObj::make( pool,"DS2-task");
    auto a = CompositionObj::make( pool,"A");
    auto b = CompositionObj::make( pool,"B");
    auto t = TaskObj::make( pool,"T");
    a->addChild(ds2, "t_abstract_child");
    a->addChild(b, "b_child");
    b->addChild(ds2, "t_abstract_child");
    // auto a2 = CompositionObj::make("A2");
    // auto a3 = CompositionObj::make("A3");
    auto b3 = CompositionObj::make( pool,"B2");
    auto b2 = CompositionObj::make( pool,"B2");
    // auto ds = new DataService("DS");

    b3->addChild(t, "task_child");
    b3->addFullfillment(ds2->getName());

    t->addFullfillment(ds2->getName());
    b3->addFullfillment("DS");
    b2->addFullfillment("DS");
    b3->addFullfillment("DS2");
    b2->addFullfillment("DS2");

    return "A";
}

std::string test_possible_loop_unused(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = CompositionObj::make( pool,"A");
    auto a2 = CompositionObj::make( pool,"A2");
    auto b = CompositionObj::make( pool,"B");
    auto c = CompositionObj::make( pool,"C");
    auto t = TaskObj::make( pool,"T1");
    auto t2 = TaskObj::make( pool,"T2");

    a->addChild(a2, "a2_child");
    a2->addChild(t2, "t_child");

    b->addChild(c, "c_child");
    c->addChild(t, "task_child");
    c->addChild(b, "b_child");

    return "A";
}

std::string test_possible_loop_unused2(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = CompositionObj::make( pool,"A");
    auto a2 = CompositionObj::make( pool,"A2");
    auto b = CompositionObj::make( pool,"B");
    auto c = CompositionObj::make( pool,"C");
    auto d = CompositionObj::make( pool,"D");
    auto t = TaskObj::make( pool,"T");

    a->addChild(a2, "a2_child");

    b->addChild(c, "c_child");
    c->addChild(t, "task_child");
    c->addChild(d, "d_child");
    d->addChild(b, "b_child");

    return "A";
}

std::string test_state_machinemultiple_results(constrained_based_networks::Pool* pool)
{
    auto t1 = TaskObj::make( pool,"task1");
    t1->addEvent("event1");
    t1->addEvent("event2");

    auto t2 = TaskObj::make( pool,"task2");
    t2->addEvent("event1");
    t2->addEvent("event2");

    auto sm = StateMachineObj::make( pool,"SM1");

    sm->addState(t1,0);
    sm->addState(t2,1);
    sm->addTransition(0, 1, "event1");
    sm->addTransition(1, 0, "event1");
    sm->addEventForwards("t1_child", "event2", "failed");
    sm->addEventForwards("t2_child", "event2", "failed");
    //sm->setStart(t1);
    return "SM1";
}


std::string test_state_machinefailed(constrained_based_networks::Pool* pool)
{

    auto start = StateMachineObj::make( pool,"start_sm");

    auto t1 = TaskObj::make( pool,"task1");
    t1->addEvent("event1");
    t1->addEvent("event2");

    auto t2 = TaskObj::make( pool,"task2");
    t2->addEvent("event1");
    t2->addEvent("event2");

    auto c = CompositionObj::make(pool,"CmpInBetween");
    auto s = c->getSpecialized(c);
    s->addConfig("composition_config", "5");
    c = std::dynamic_pointer_cast<CompositionObj>(s);

    auto t3 = TaskObj::make( pool,"revover-task");
    c->addChild(t3,"t3_child");


    auto sm = StateMachineObj::make( pool,"SM1");

    //start->setStart(sm);
    start->addState(sm,0);

    sm->addState(t1,0);
    sm->addState(t2,1);
    sm->addState(c,2);

    sm->addTransition(0, 1, "event1");
    sm->addTransition(0, 2, "event2");
    sm->addTransition(1, 0, "event1");
    sm->addTransition(2, 0, "failed");
    sm->addTransition(2, 1, "success");
//    sm->addEventForwards("t1_child", "event2", "failed");
//    sm->addEventForwards("t2_child", "event2", "failed");

    //sm->addState(t1,0);
    //sm->setStart(t1);
    //return "start_sm";
    return "SM1";
}

std::string test_state_machine(constrained_based_networks::Pool* pool)
{
    auto a1 = CompositionObj::make( pool,"pipe_detector");
    auto t1 = TaskObj::make( pool,"pipe-cam-task");
    a1->addChild(t1, "cam-detekt");
    t1->addEvent("pipe-end");
    a1->addEvent("fertig");
    a1->addEventForwards("cam-detekt", "pipe-end", "fertig");

    auto a2 = CompositionObj::make( pool,"wall-servoing");
    auto t2 = TaskObj::make( pool,"wall-task");
    a2->addChild(t2, "wall-detector-child");


    auto sm = StateMachineObj::make( pool,"SM1");
    sm->addState(a1,0);
    sm->addState(a2,0);
    sm->addTransition(0, 1, "fertig");

    return "SM1";
}

std::string test_state_machine2(constrained_based_networks::Pool* pool)
{
    auto p = CompositionObj::make( pool,"parent_cmp");

    auto a1 = CompositionObj::make( pool,"pipe_detector");
    auto t1 = TaskObj::make( pool,"pipe-cam-task");
    a1->addChild(t1, "cam-detekt");
    t1->addEvent("pipe-end");
    a1->addEvent("fertig");
    a1->addEventForwards("cam-detekt", "pipe-end", "fertig");

    auto a2 = CompositionObj::make( pool,"wall-servoing");
    auto t2 = TaskObj::make( pool,"wall-task");
    a2->addChild(t2, "wall-detector-child");

    auto sm = StateMachineObj::make( pool,"SM1");
    sm->addState(a1,0);
    sm->addState(a2,1);

    sm->addTransition(0, 1, "fertig");

    return "parent_cmp";
}

std::string test_state_machine3(constrained_based_networks::Pool* pool)
{
    auto psm = StateMachineObj::make( pool,"parent_sm");

    auto p = CompositionObj::make( pool,"parent_cmp");

    auto a1 = CompositionObj::make( pool,"pipe_detector");
    auto t1 = TaskObj::make( pool,"pipe-cam-task");
    a1->addChild(t1, "cam-detekt");
    t1->addEvent("pipe-end");
    a1->addEvent("fertig");
    a1->addEventForwards("cam-detekt", "pipe-end", "fertig");

    auto a2 = CompositionObj::make( pool,"wall-servoing");
    auto t2 = TaskObj::make( pool,"wall-task");

    auto f = TaskObj::make( pool,"final-task");

    a2->addChild(t2, "wall-detector-child");

    psm->addState(p,0);
    psm->addState(f,1);
    psm->addTransition(0, 1,"success");

    auto sm = StateMachineObj::make( pool,"SM1");

    sm->addState(a1,0);
    sm->addState(a2,1);
    sm->addTransition(0,1, "fertig");

    p->addChild(sm, "only");
    p->addEventForwards("only", "success", "success");

    return "parent_sm";
}

std::string test_ambigious_configs(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a1 = CompositionObj::make( pool,"A1");
    auto a2 = CompositionObj::make( pool,"A2");
    auto t1 = TaskObj::make( pool,"T1");
    t1->addProperty("int_config", constrained_based_networks::ConfigurationModel::INT);

    a1->addChild(a2, "a2_child");

    a1->addChild(t1, "task_child");
    a2->addChild(t1, "task_child");

    return "A1";
}

std::string test_config(constrained_based_networks::Pool* pool)
{
    auto cmp = CompositionObj::make(pool,"ConfigurableComposition");
    cmp->addProperty("composition-config", constrained_based_networks::ConfigurationModel::INT);
    auto t1 = TaskObj::make( pool,"T1");
    t1->addProperty("task-config", constrained_based_networks::ConfigurationModel::INT);
    cmp->addChild(t1,"task");
    cmp->addArgumentForwards("task","composition-config", "task-config");

    auto cmp2 = cmp->getSpecialized(cmp);
    cmp2->addConfig("composition-config", "5");

    //t1->addProperty("int_config", constrained_based_networks::ConfigurationModel::INT);
    //auto t2 = t1->getSpecialized();
    //t2->addConfig("int_config", "5");
    return cmp2->getName();
}


std::string sonar_sample(constrained_based_networks::Pool* pool)
{
    auto local = CompositionObj::make(pool,"Localization");
    auto wall = CompositionObj::make(pool,"Wall");
    local->addProperty("sonar-angle", constrained_based_networks::ConfigurationModel::INT);
    wall->addProperty("sonar-angle", constrained_based_networks::ConfigurationModel::INT);

    auto sonar = TaskObj::make( pool,"Sonar");
    sonar->addProperty("scan-angle", constrained_based_networks::ConfigurationModel::INT);

    wall->addChild(sonar,"sonar");
    local->addChild(sonar,"sonar");

    local->addArgumentForwards("sonar","sonar-angle", "scan-angle");
    wall->addArgumentForwards("sonar","sonar-angle", "scan-angle");

    auto local_s = local->getSpecialized(local);
    auto wall_s = wall->getSpecialized(wall);


    local_s->addConfig("sonar-angle", "0", "360");
    wall_s->addConfig("sonar-angle", "70", "120");

    return local_s->getName();
}



std::string load_specialied_simple_move(constrained_based_networks::Pool* pool)
{
    create_model(pool);
    pool->mergeDoubles();
    for (auto component : pool->getItems<ComponentObj>()) {
        if (component->getName().find("AuvControl::SimpleMove_") != std::string::npos) {
            std::cout << "Got " << component->getName() << std::endl;
            return component->getName();
        }
    }
    throw std::runtime_error("Cannot get specialized simple move component");
}

std::string test_lawn_mower_child(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing for lawn-mower child" << std::endl;
    create_model(pool);
    pool->mergeDoubles();
    auto c = std::dynamic_pointer_cast<CompositionObj>(pool->getComponent("Main::LawnMoverOverPipe"));
    return c->getChildren()[0].second->getName();
}

Tests tests[] = {{test_cmp_recursion, ""},
                 {test_cmp_recursion_w_unused_CS, ""},
                 {test_cmp_recursion_w_unused_DS, ""},
                 {test_cmp_recursion_w_unused_DS2, ""},
                 {test_cmp_recursion_w_used_DS, ""},
                 {test_ds, ""},
                 {test_ambigious_ds, ""},
                 {test_cmp_recursion_w_more_used, ""},
                 {test_cmp_recursion_child2_time, ""},
                 {test_cmp_recursion_child2_time_abstract, ""},
                 {test_possible_loop_unused, ""},
                 {test_possible_loop_unused2, ""},
                 {test_config, ""},
                 {test_ambigious_configs, ""},
                 {test_state_machine, ""},
                 {test_state_machine2, ""},
                 {test_state_machine3, ""},
                 {test_state_machinemultiple_results, ""},
                 {test_state_machinefailed, ""},
                 {create_model, ""},  // Trivial test empty (VALID!) solution
                 {create_model, "AuvControl::DepthFusionCmp"},
                 {create_model, "Pipeline::Follower"},
                 {create_model, "Buoy::DetectorNewCmp"},
                 {create_model, "Wall::Follower"},
                 {create_model, "Main::LawnMoverOverPipe"},
                 {create_model, "AuvControl::SimpleMove"},
                 {load_specialied_simple_move, ""},
                 {test_lawn_mower_child, ""},
                 {create_model, "Main::Win"},
                 {sonar_sample, ""},
                 {0, ""}};

std::string demangle(std::string input)
{
    char symbollist[512];
    strcpy(symbollist, input.c_str());

    // allocate string which will be filled with the demangled function name
    size_t funcnamesize = 256;
    char* funcname = (char*)malloc(funcnamesize);

    // iterate over the returned symbol lines. skip the first, it is the
    // address of this function.
    char* begin_name = 0, *begin_offset = 0, *end_offset = 0;

    // find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x15c) [0x8048a6d]
    for (char* p = symbollist; *p; ++p) {
        if (*p == '(')
            begin_name = p;
        else if (*p == '+')
            begin_offset = p;
        else if (*p == ')' && begin_offset) {
            end_offset = p;
            break;
        }
    }

    if (begin_name && begin_offset && end_offset && begin_name < begin_offset) {

        *begin_name++ = '\0';
        *begin_offset++ = '\0';
        *end_offset = '\0';

        // mangled name is now in [begin_name, begin_offset) and caller
        // offset in [begin_offset, end_offset). now apply
        // __cxa_demangle():

        int status;
        char* ret = abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
        if (status == 0) {
            funcname = ret;  // use possibly realloc()-ed string
    //        printf("  %s : %s+%s\n", symbollist, funcname, begin_offset);
        } else {
            // demangling failed. Output function name as a C function with
            // no arguments.
  //          printf("  %s : %s()+%s\n", symbollist, begin_name, begin_offset);
        }
    } else {
        // couldn't parse the line? print the whole line.
    //    printf("  %s\n", symbollist);
    }

    std::string erg(funcname);
    free(funcname);
    return erg;
}

void printTests()
{
    for (int i = 0; tests[i].v; i++) {
        std::cout << "Test " << i << ": ";
        std::cout << demangle(backtrace_symbols((void * const*)&tests[i].v, 1)[0]);
        std::cout << "\t\t" << tests[i].name;
        std::cout << std::endl;
    }
}

std::string load_test(int nr = -1)
{
    pool = new Pool();

    std::string name = "";
    if (nr == -1) {
        printf("Creating model from export\n");
        create_model(pool);
        //pool->dupFunction = create_model;
        pool->mergeDoubles();
    } else {
        // printf("Running test: %i\n",nr);
        if (tests[nr].name == "") {
            name = tests[nr].v(pool);
//            pool->dupFunction = tests[nr].v;
        } else {
            tests[nr].v(pool);
//            pool->dupFunction = tests[nr].v;
            name = tests[nr].name;
            pool->mergeDoubles();
        }
        printf("Running test with name: %s\n", name.c_str());
    }

    return name;
}
