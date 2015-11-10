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

    // auto a2 = new Composition("A2");
    auto a = new Composition("A", pool);
    auto b = new Composition("B", pool);
    // auto b2 = new Composition("B2");
    a->addChild(b, "b_child");
    auto t = new Task("T", pool);
    b->addChild(t, "t_child");
    return "A";
}

std::string test_ds(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = new Composition("A", pool);
    auto b = new Task("B", pool);
    // auto ds = new DataService("DS");
    b->addFullfillment("DS");
    a->addChild(b, "b_child_is_ds");
    return "A";
}

std::string test_ambigious_ds(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a = new Composition("A", pool);
    auto b = new Task("B", pool);
    auto b2 = new Task("B2", pool);
    auto ds = new DataService("DS", pool);

    b->addFullfillment("DS");
    b2->addFullfillment("DS");

    a->addChild(ds, "b_child_is_ds");
    return "A";
}

std::string test_cmp_recursion_w_more_used(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    auto a2 = new Composition("A2", pool);
    auto a = new Composition("A", pool);
    auto b = new Composition("B", pool);
    auto b2 = new Composition("B2", pool);
    auto ds = new DataService("DS", pool);
    // auto ds2 = new DataService("DS2");

    b->addFullfillment("DS");
    b2->addFullfillment("DS");
    b->addFullfillment("DS2");
    b2->addFullfillment("DS2");

    a->addChild(ds, "b_child_is_ds");
    a2->addChild(ds, "b_child");
    auto t = new Task("T", pool);
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion_w_used_DS(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;

    // auto a2 = new Composition("A2");
    auto a = new Composition("A", pool);
    auto b = new Composition("B", pool);
    // auto b2 = new Composition("B2");
    auto ds = new DataService("DS", pool);
    // auto ds2 = new DataService("DS2");

    b->addFullfillment("DS");
    // b2->addFullfillment("DS");
    // b->addFullfillment("DS2");
    // b2->addFullfillment("DS2");

    a->addChild(ds, "b_child_is_ds");
    // a2->addChild(ds,"b_child");
    auto t = new Task("T", pool);
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion_w_unused_DS(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A", pool);
    auto b = new Composition("B", pool);
    a->addChild(b, "b_child");
    auto t = new Task("T", pool);
    b->addChild(t, "t_child");
    // auto ds = new DataService("DS");
    return "A";
}

std::string test_cmp_recursion_w_unused_DS2(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A", pool);
    auto b = new Composition("B", pool);
    // auto ds = new DataService("DS");
    a->addChild(b, "b_child");
    auto t = new Task("T", pool);
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A", pool);
    auto b = new Composition("B", pool);
    a->addChild(b, "b_child");
    auto t = new Task("T", pool);
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion_child2_time(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a = new Composition("A", pool);
    auto b = new Composition("B", pool);
    auto t = new Task("T", pool);
    a->addChild(t, "t_child");
    a->addChild(b, "b_child");
    b->addChild(t, "t_child");
    return "A";
}

std::string test_cmp_recursion_child2_time_abstract(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto ds2 = new DataService("DS2-task", pool);
    auto a = new Composition("A", pool);
    auto b = new Composition("B", pool);
    auto t = new Task("T", pool);
    a->addChild(ds2, "t_abstract_child");
    a->addChild(b, "b_child");
    b->addChild(ds2, "t_abstract_child");
    // auto a2 = new Composition("A2");
    // auto a3 = new Composition("A3");
    auto b3 = new Composition("B2", pool);
    auto b2 = new Composition("B2", pool);
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
    auto a = new Composition("A", pool);
    auto a2 = new Composition("A2", pool);
    auto b = new Composition("B", pool);
    auto c = new Composition("C", pool);
    auto t = new Task("T-not", pool);
    auto t2 = new Task("T-yes", pool);

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
    auto a = new Composition("A", pool);
    auto a2 = new Composition("A2", pool);
    auto b = new Composition("B", pool);
    auto c = new Composition("C", pool);
    auto d = new Composition("D", pool);
    auto t = new Task("T", pool);

    a->addChild(a2, "a2_child");

    b->addChild(c, "c_child");
    c->addChild(t, "task_child");
    c->addChild(d, "d_child");
    d->addChild(b, "b_child");

    return "A";
}

std::string test_state_machinemultiple_results(constrained_based_networks::Pool* pool)
{
    auto t1 = new Task("task1", pool);
    t1->addEvent("event1");
    t1->addEvent("event2");

    auto t2 = new Task("task2", pool);
    t2->addEvent("event1");
    t2->addEvent("event2");

    auto sm = new StateMachine("SM1", pool);

    sm->addTransition(t1, t2, t1, "event1");
    sm->addTransition(t2, t1, t2, "event1");
    sm->addEventForwards("t1_child", "event2", "failed");
    sm->addEventForwards("t2_child", "event2", "failed");
    sm->setStart(t1);
    return "SM1";
}


std::string test_state_machinefailed(constrained_based_networks::Pool* pool)
{

    auto start = new StateMachine("start_sm", pool);

    auto t1 = new Task("task1", pool);
    t1->addEvent("event1");
    t1->addEvent("event2");

    auto t2 = new Task("task2", pool);
    t2->addEvent("event1");
    t2->addEvent("event2");

    auto c = new Composition("CmpInBetween",pool);
    auto s = c->getSpecialized();
    s->addConfig("composition_config", "5");
    c = dynamic_cast<Composition*>(s);

    auto t3 = new Task("revover-task", pool);
    c->addChild(t3,"t3_child");


    auto sm = new StateMachine("SM1", pool);

    start->setStart(sm);
    
    sm->addTransition(t1, t2, t1, "event1");
    sm->addTransition(t1, c, t1, "event2");
    sm->addTransition(t2, t1, t2, "event1");
    sm->addTransition(c, t1, c, "failed");
    sm->addTransition(c, t2, c, "success");
//    sm->addEventForwards("t1_child", "event2", "failed");
//    sm->addEventForwards("t2_child", "event2", "failed");
    
    sm->setStart(t1);
    //return "start_sm";
    return "SM1";
}

std::string test_state_machine(constrained_based_networks::Pool* pool)
{
    auto a1 = new Composition("pipe_detector", pool);
    auto t1 = new Task("pipe-cam-task", pool);
    a1->addChild(t1, "cam-detekt");
    t1->addEvent("pipe-end");
    a1->addEvent("fertig");
    a1->addEventForwards("cam-detekt", "pipe-end", "fertig");

    auto a2 = new Composition("wall-servoing", pool);
    auto t2 = new Task("wall-task", pool);
    a2->addChild(t2, "wall-detector-child");

    auto sm = new StateMachine("SM1", pool);

    sm->addTransition(a1, a2, a1, "fertig");
    sm->setStart(a1);

    return "SM1";
}

std::string test_state_machine2(constrained_based_networks::Pool* pool)
{
    auto p = new Composition("parent_cmp", pool);

    auto a1 = new Composition("pipe_detector", pool);
    auto t1 = new Task("pipe-cam-task", pool);
    a1->addChild(t1, "cam-detekt");
    t1->addEvent("pipe-end");
    a1->addEvent("fertig");
    a1->addEventForwards("cam-detekt", "pipe-end", "fertig");

    auto a2 = new Composition("wall-servoing", pool);
    auto t2 = new Task("wall-task", pool);
    a2->addChild(t2, "wall-detector-child");

    auto sm = new StateMachine("SM1", pool);

    sm->addTransition(a1, a2, a1, "fertig");
    sm->setStart(a1);

    p->addChild(sm, "only");

    return "parent_cmp";
}

std::string test_state_machine3(constrained_based_networks::Pool* pool)
{
    auto psm = new StateMachine("parent_sm", pool);

    auto p = new Composition("parent_cmp", pool);

    auto a1 = new Composition("pipe_detector", pool);
    auto t1 = new Task("pipe-cam-task", pool);
    a1->addChild(t1, "cam-detekt");
    t1->addEvent("pipe-end");
    a1->addEvent("fertig");
    a1->addEventForwards("cam-detekt", "pipe-end", "fertig");

    auto a2 = new Composition("wall-servoing", pool);
    auto t2 = new Task("wall-task", pool);

    auto f = new Task("final-task", pool);

    a2->addChild(t2, "wall-detector-child");

    psm->setStart(p);
    psm->addTransition(p, f, p, "success");

    auto sm = new StateMachine("SM1", pool);

    sm->addTransition(a1, a2, a1, "fertig");
    sm->setStart(a1);

    p->addChild(sm, "only");
    p->addEventForwards("only", "success", "success");

    return "parent_sm";
}

std::string test_ambigious_configs(constrained_based_networks::Pool* pool)
{
    std::cout << "Testing " << __FUNCTION__ << std::endl;
    auto a1 = new Composition("A1", pool);
    auto a2 = new Composition("A2", pool);
    auto t1 = new Task("T1", pool);
    t1->addProperty("int_config", constrained_based_networks::ConfigurationModel::INT);

    a1->addChild(a2, "a2_child");

    a1->addChild(t1, "task_child");
    a2->addChild(t1, "task_child");

    return "A1";
}

std::string test_config(constrained_based_networks::Pool* pool)
{
    auto cmp = new Composition("ConfigurableComposition",pool);
    cmp->addProperty("composition_config", constrained_based_networks::ConfigurationModel::INT);
    auto t1 = new Task("T1", pool);
    t1->addProperty("task_config", constrained_based_networks::ConfigurationModel::INT);
    cmp->addChild(t1,"task");
    cmp->addArgumentForwards("task","composition_config", "task_config");

    auto cmp2 = cmp->getSpecialized();
    cmp2->addConfig("composition_config", "5");



    //t1->addProperty("int_config", constrained_based_networks::ConfigurationModel::INT);
    //auto t2 = t1->getSpecialized();
    //t2->addConfig("int_config", "5");
    return cmp2->getName();
}

std::string load_specialied_simple_move(constrained_based_networks::Pool* pool)
{
    create_model(pool);
    pool->mergeDoubles();
    for (auto component : pool->getItems<Component*>()) {
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
    auto c = dynamic_cast<Composition*>(pool->getComponent("Main::LawnMoverOverPipe"));
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
