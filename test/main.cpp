#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/Solution.hpp>



// main test function
int main(int argc, char* argv[]) {
    using namespace constrained_based_networks;
    
    Composition compA ("a");
    Task B("b");
    Task C("c");
    Task C2("c2");
    Task C3("c3");
    /*
    for(int i=0;i<10;i++){
        char buff[500];
        sprintf(buff,"Unused-%i",i);
        Task *unused = new Task(std::string(buff));
        sprintf(buff,"Unused-%i_child",i);
        compA.addChild(unused,std::string(buff));
    }
    */
    DataService D("data-service_1");
    C.addFullfillment("data-service_1");
    C3.addFullfillment("data-service_1");
    compA.addChild(&B,"b_child");
    //compA.addChild(&C,"c_child");
    compA.addChild(&D,"c_abstract_child");


    compA.setActive();





    /*
    Composition compB ("b");
    Composition compC ("c");
    // And their ports
    compA.pushBackOutPort(OutgoingPort("std::string","out0"));
    compA.pushBackOutPort(OutgoingPort("int","out1"));
    compA.pushBackInPort(IncomingPort("std::string","in0"));
    compB.pushBackInPort(IncomingPort("std::string","in0"));
    compB.pushBackInPort(IncomingPort("float","in1"));
    compC.pushBackOutPort(OutgoingPort("std::string","out0"));
    compC.pushBackOutPort(OutgoingPort("float","out1"));
    compC.pushBackInPort(IncomingPort("int","in0"));
    
    // Query components
    Composition queryCompA = compA;
    Composition queryCompB = compB;
    Composition queryCompC = compC;
    Composition queryCompAnotherC = compC;
    queryCompAnotherC.setName("anotherC");
    // Configure query components
    std::vector<std::string> compAConf;
    compAConf.push_back("3");
    compAConf.push_back("1");
    queryCompA.setConfiguration(compAConf);
    // No constraint on B's config
    std::vector<std::string> compCConf;
    compCConf.push_back("2");
    queryCompC.setConfiguration(compCConf);
    // Construct query and subquery
    Query query ("query"), subquery ("subquery");
    query.addComposition(queryCompA);
    query.addComposition(queryCompAnotherC);
    subquery.addComposition(queryCompB);
    subquery.addComposition(queryCompC);
    
    
    // Configure connections
    subquery.addConnection(queryCompC.getName(), "out1", 
                           queryCompB.getName(), "in1");
    query.integrateSubQuery(subquery);
    
    query.addConnection(queryCompA.getName(), "out0", 
                        queryCompB.getName(), "in0");
    query.addConnection(queryCompA.getName(), "out1", 
                        queryCompAnotherC.getName(), "in0");
    query.addConnection(queryCompAnotherC.getName(), "out0", 
                        queryCompA.getName(), "in0");
    
    // Pool components
    Composition poolCompA0 = compA;
    poolCompA0.setName("a0");
    Composition poolCompA1 = compA;
    poolCompA1.setName("a1");
    Composition poolCompB0 = compB;
    poolCompB0.setName("b0");
    Composition poolCompB1 = compB;
    poolCompB1.setName("b1");
    Composition poolCompC0 = compC;
    poolCompC0.setName("c0");
    Composition poolCompC1 = compC;
    poolCompC1.setName("c1");
    // Configure pool components
    //std::vector<std::string> compA0Conf;
    //compA0Conf.push_back("3");
    //compA0Conf.push_back("3");
    //poolCompA0.setConfiguration(compA0Conf);
    //std::vector<std::string> compB0Conf;
    //compB0Conf.push_back("2");
    //poolCompB0.setConfiguration(compB0Conf);
    //std::vector<std::string> compC1Conf;
    //compC1Conf.push_back("2");
    //poolCompC1.setConfiguration(compC1Conf);
    // Construct pool
    Query pool2 ("pool");
    pool2.addComposition(poolCompA0);
    pool2.addComposition(poolCompA1);
    pool2.addComposition(poolCompB0);
    pool2.addComposition(poolCompB1);
    pool2.addComposition(poolCompC0);
    pool2.addComposition(poolCompC1);
    
    // Configure connections
    pool2.addConnection(poolCompA0.getName(), "out0", 
                       poolCompB0.getName(), "in0");
    pool2.addConnection(poolCompA0.getName(), "out1", 
                       poolCompC1.getName(), "in0");
    pool2.addConnection(poolCompC0.getName(), "out0", 
                       poolCompA0.getName(), "in0");
    pool2.addConnection(poolCompC1.getName(), "out1", 
                       poolCompB0.getName(), "in1");
    // Optional:
    pool2.addConnection(poolCompA0.getName(), "out0", 
                       poolCompB1.getName(), "in0");
    
    // Print pool and query
    std::cout << "Pool: " << pool2.toString();
    std::cout << "Query: " << query.toString();
    */

    Solution* s = Solution::babSearch(Pool::getInstance());
    s->print();
    return 0;
}
