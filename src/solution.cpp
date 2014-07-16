#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <gecode/minimodel.hh>

#include<vector>
#include<map>

#include "query.h"

using namespace Gecode;

namespace composition_management {

/**
 * A solution inherits GECODE's space. the initial situation as well as any real solutions are type Solution.
 */
class Solution : public Space {
protected:
    /**
     * The query to compute solutions for.
     */
    Query query;
    /**
     * The actual situation, components from the pool.
     */
    Query componentPool;
    /**
     * Assignments of query components to pool components. This is what has to be solved.
     */
    IntVarArray assignments_int;
public:
    /**
     * Construct a solution with an initial situation to search.
     */
    Solution(Query query, Query componentPool) 
        : query(query)
        , componentPool(componentPool)
        , assignments_int(*this, query.getComponents().size(), 0, componentPool.getComponents().size())
    {        
        // The first indices of a type, in the componentPool
        std::vector<int> typeIndicesPool;
        for(int i = 0; i < componentPool.getComponents().size(); i++) {
            if(i == 0 || componentPool.getComponents().at(i - 1).getType() != componentPool.getComponents().at(i).getType()) {
                typeIndicesPool.push_back(i);
            }
        }
        // The first indices of a type, in the query
        std::vector<int> typeIndicesQuery;
        for(int i = 0; i < query.getComponents().size(); i++) {
            if(i == 0 || query.getComponents().at(i - 1).getType() != query.getComponents().at(i).getType()) {
                typeIndicesQuery.push_back(i);
            }
        }
        
        for(int i = 0, type = -1; i < assignments_int.size(); i++)
        {
            // Increment type if necessary
            if(std::find(typeIndicesQuery.begin(), typeIndicesQuery.end(), i) != typeIndicesQuery.end())
            {
                type++;
            }
            int upperLimit = type + 1 < typeIndicesPool.size() ? typeIndicesPool[type + 1] - 1 : componentPool.getComponents().size() - 1;
            // Constraint domain such that the types equal
            // The initial domain of each IntVar is the number of pool components. This constraints it so be just
            // the pool components of the same type. This is simplified with lower ans upper bound as components
            // are ordered in ascending type.
            dom(*this, assignments_int[i], typeIndicesPool[type], upperLimit);
        }
        
        // For all queried components
        for(int i = 0; i < assignments_int.size(); i++)
        {
            std::cout << "Constraints for query " << query.getComponents()[i].toString() << std::endl;
            
            // For all possible pool components they can be assigned
            for(int j = assignments_int[i].min(); j <= assignments_int[i].max(); j++)
            {
                std::cout << " Against pool " << componentPool.getComponents()[j].toString() << std::endl;
                
                // TODO extern method testing if two components are compitable regarding their configurations
                // as vector<string>
                // At the moment, the conf vector just has to be equal
                const std::vector<std::string>& requiredConfiguration = query.getComponents().at(i).getConfiguration();
                const std::vector<std::string>& actualConfiguration = componentPool.getComponents().at(j).getConfiguration();
                // If the configurations are not compatible, post != constraint on this assignment
                if(!requiredConfiguration.empty()&& !actualConfiguration.empty() && requiredConfiguration != actualConfiguration)
                {
                    std::cout << "Adding configuration constraint. Component " << query.getComponents()[i].getName() << "!=" << componentPool.getComponents()[j].getName() << std::endl;
                    // TODO best solution: already assigned conf is better, for reusing components
                    
                    // This means that the ith query component cannot be assigned the jth pool component,
                    // since the configurations are incompatible.
                    rel(*this, assignments_int[i], IRT_NQ, j);
                }
                
                
                // For all incoming ports of the query component
                const std::map<IncomingPort, const Component*> map = query.getComponents().at(i).getIncomingConnections();
                for(std::map<IncomingPort, const Component*>::const_iterator it = map.begin(); it != map.end(); ++it)
                {
                    std::cout << "Checking connection constraint. Component " << query.getComponents()[i].getName() << "=" << componentPool.getComponents()[j].getName() 
                                  << " on in port " << it->first.name << std::endl;
                    
                    // If the ith query component gets assigned to the jth pool component
                    // and they both are connected on the same input port,
                    // the connection origin must also be assigned equally.
                    std::map<IncomingPort, const Component*> poolComponentMap = componentPool.getComponents().at(j).getIncomingConnections();
                    if(poolComponentMap.count(it->first) != 0)
                    {
                        // Get number of the origins
                        int originQueryNum = std::find(query.getComponents().begin(), query.getComponents().end(), *(it->second)) - query.getComponents().begin();
                        int originPoolNum = std::find(componentPool.getComponents().begin(), componentPool.getComponents().end(), *(poolComponentMap.at(it->first))) 
                                            - componentPool.getComponents().begin();
                                            
                        std::cout << "Adding connection constraint. Component " << query.getComponents()[i].getName() << "=" << componentPool.getComponents()[j].getName() 
                                  << " => " << it->second->getName() << "=" << poolComponentMap.at(it->first)->getName() << std::endl;
                        
                        // Both connected on same port
                        BoolVar ithAssignedToJth(*this, 0, 1);
                        rel(*this, assignments_int[i], IRT_EQ, j, ithAssignedToJth);
                        // Origins connected on same port
                        BoolVar originsConnectedEqually(*this, 0, 1);
                        rel(*this, assignments_int[originQueryNum], IRT_EQ, originPoolNum, originsConnectedEqually);
                        // Implication as a constraint e.g. A=a0 => B=b0
                        BoolVar implication(*this, 0, 1);
                        rel(*this, ithAssignedToJth, BOT_IMP, originsConnectedEqually, 1);
                    }
                }
            }
        }
        
        // branching TODO test and improve
        
        // this tells the search engine to branch first on the variable with the smallest domain,
        // that is the variables where there are less components of that type in the pool
        // The engine will then try the possible values (assignments) in ascending order.
        branch(*this, assignments_int, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }
    
    // search support. There must be a copy constructor like this for the search engine.
    // Everything must be copied into the new Space
    Solution(bool share, Solution& s) 
        : Space(share, s)
        , query(s.query)
        , componentPool(s.componentPool)
    {
        assignments_int.update(*this, share, s.assignments_int);
    }
    // This method is called be the search engine
    virtual Space* copy(bool share) {
        return new Solution(share,*this);
    }
    // print support
    void print(void) const {
        print(std::cout);
    }
    // print support for given outputstream
    void print(std::ostream& os) const {
        os << "Solution: { ";
        for(int i = 0; i < assignments_int.size(); i++)
        {
            os << query.getComponents()[i].getName() << "=" << componentPool.getComponents()[assignments_int[i].val()].getName() << ", ";
        }
        os << "}" << std::endl;
    }
};

} // end namespace composition_management

// main test function
int main(int argc, char* argv[]) {
    using namespace composition_management;
    // General components
    Component compA (0, "a");
    Component compB (1, "b");
    Component compC (2, "c");
    // And their ports
    compA.pushBackOutPort(OutgoingPort("std::string","a_out_1"));
    compA.pushBackOutPort(OutgoingPort("int","a_out_2"));
    compA.pushBackInPort(IncomingPort("std::string","a_in_1"));
    compB.pushBackInPort(IncomingPort("std::string","b_in_1"));
    compB.pushBackInPort(IncomingPort("float","b_in_2"));
    compC.pushBackOutPort(OutgoingPort("std::string","c_out_1"));
    compC.pushBackOutPort(OutgoingPort("float","c_out_2"));
    compC.pushBackInPort(IncomingPort("int","c_in_1"));
    
    // Query components
    Component queryCompA = compA;
    Component queryCompB = compB;
    Component queryCompC = compC;
    // Configure query components
    std::vector<std::string> compAConf;
    compAConf.push_back("3");
    compAConf.push_back("1");
    queryCompA.setConfiguration(compAConf);
    // No constraint on B's config
    std::vector<std::string> compCConf;
    compCConf.push_back("2");
    queryCompC.setConfiguration(compCConf);
    // Construct query
    Query query;
    query.addComponent(queryCompA);
    query.addComponent(queryCompB);
    query.addComponent(queryCompC);
    
    
    // Configure connections
    query.addConnection("a", query.getComponent("a").getOutPorts().at(0), "b", query.getComponent("b").getInPorts().at(0));
    query.addConnection("a", query.getComponent("a").getOutPorts().at(1), "c", query.getComponent("c").getInPorts().at(0));
    query.addConnection("c", query.getComponent("c").getOutPorts().at(0), "a", query.getComponent("a").getInPorts().at(0));
    query.addConnection("c", query.getComponent("c").getOutPorts().at(1), "b", query.getComponent("b").getInPorts().at(1));
    
    // Pool components
    Component poolCompA0 = compA;
    poolCompA0.setName("a0");
    Component poolCompA1 = compA;
    poolCompA1.setName("a1");
    Component poolCompB0 = compB;
    poolCompB0.setName("b0");
    Component poolCompB1 = compB;
    poolCompB1.setName("b1");
    Component poolCompC0 = compC;
    poolCompC0.setName("c0");
    Component poolCompC1 = compC;
    poolCompC1.setName("c1");
    // Configure pool components
    std::vector<std::string> compA0Conf;
    compA0Conf.push_back("3");
    compA0Conf.push_back("3");
    poolCompA0.setConfiguration(compA0Conf);
    std::vector<std::string> compB0Conf;
    compB0Conf.push_back("2");
    poolCompB0.setConfiguration(compB0Conf);
    std::vector<std::string> compC1Conf;
    compC1Conf.push_back("2");
    poolCompC1.setConfiguration(compC1Conf);
    // Construct pool
    Query pool;
    pool.addComponent(poolCompA0);
    pool.addComponent(poolCompA1);
    pool.addComponent(poolCompB0);
    pool.addComponent(poolCompB1);
    pool.addComponent(poolCompC0);
    pool.addComponent(poolCompC1);
    
    // Configure connections
    pool.addConnection("a0", pool.getComponent("a0").getOutPorts().at(0), "b0", pool.getComponent("b0").getInPorts().at(0));
    pool.addConnection("a0", pool.getComponent("a0").getOutPorts().at(1), "c1", pool.getComponent("c1").getInPorts().at(0));
    pool.addConnection("c0", pool.getComponent("c0").getOutPorts().at(0), "a0", pool.getComponent("a0").getInPorts().at(0));
    pool.addConnection("c1", pool.getComponent("c1").getOutPorts().at(1), "b0", pool.getComponent("b0").getInPorts().at(1));
    // Optional:
    //pool.addConnection("a0", pool.getComponent("a0").getOutPorts().at(0), "ba", pool.getComponent("b1").getInPorts().at(0));
    
    // Print pool and query
    std::cout << "Pool: " << pool.toString();
    std::cout << "Query: " << query.toString();
    
    Solution* s = new Solution(query, pool);
    DFS<Solution> e(s);
    delete s;
    // search and print all solutions
    while (Solution* s = e.next()) {
        s->print();
        delete s;
    } 
    return 0;
}
