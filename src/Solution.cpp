#include "Solution.hpp"

#include <gecode/gist.hh>
#include <gecode/minimodel.hh>

#include<vector>
#include<map>
#include<stdexcept>

using namespace Gecode;

namespace composition_management {

Solution::Solution(Query query, Query componentPool) 
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
                // This means that the ith query component cannot be assigned the jth pool component,
                // since the configurations are incompatible.
                rel(*this, assignments_int[i], IRT_NQ, j);
            }
            
            
            // For all incoming ports of the query component
            const std::map<IncomingPort, std::string> map = query.getComponents().at(i).getIncomingConnections();
            for(std::map<IncomingPort, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
            {
                std::cout << "Checking connection constraint. Component " << query.getComponents()[i].getName() << "=" << componentPool.getComponents()[j].getName() 
                                << " on in port " << it->first.name << std::endl;
                
                // If the ith query component gets assigned to the jth pool component
                // and they both are connected on the same input port,
                // the connection origin must also be assigned equally.
                std::map<IncomingPort, std::string> poolComponentMap = componentPool.getComponents().at(j).getIncomingConnections();
                if(poolComponentMap.count(it->first) != 0)
                {
                    // Get number of the origins
                    int originQueryNum = query.getComponentIndex(it->second);
                    int originPoolNum = componentPool.getComponentIndex(poolComponentMap.at(it->first));
                                        
                    std::cout << "Adding connection constraint. Component " << query.getComponents()[i].getName() << "=" << componentPool.getComponents()[j].getName() 
                                << " => " << it->second << "=" << poolComponentMap.at(it->first) << std::endl;
                    
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

void Solution::constrain(const Space& _b) 
{
    const Solution& b = static_cast<const Solution&>(_b);
    
    // Number of used components
    // Determine number of used values in b
    int valuesCount = 0;
    std::vector<int> values;
    for(IntVarArray::const_iterator it = b.assignments_int.begin(); it != b.assignments_int.end(); ++it)
    {
        // if not contains...
        if(std::find(values.begin(), values.end(), it->val()) == values.end())
        {
            values.push_back(it->val());
            valuesCount++;
        }
    }
    
    // We must have at most that many components used as the so far best solution
    // FIXME LQ, and stuff below
    nvalues(*this, assignments_int, IRT_LE, valuesCount);
    
    // If we have an equal amount of values used, the number of reconfigured components must be less
    BoolVar equalAmountOfValues;
    //nvalues(*this, assignments_int, IRT_LQ, valuesCount, equalAmountOfValues);
    
    std::cout << " Adding best search constraint. This ";
    print();
    std::cout << " must be better than so far best ";
    b.print();
}

Solution::Solution(bool share, Solution& s) 
    : Space(share, s)
    , query(s.query)
    , componentPool(s.componentPool)
{
    assignments_int.update(*this, share, s.assignments_int);
}

Space* Solution::copy(bool share) 
{
    return new Solution(share,*this);
}

void Solution::print() const 
{
    printToStream(std::cout);
}

void Solution::printToStream(std::ostream& os) const 
{
    os << "Solution: { ";
    for(int i = 0; i < assignments_int.size(); i++)
    {
        os << query.getComponents()[i].getName() << "=";
        if(assignments_int[i].assigned())
        {
                os << componentPool.getComponents()[assignments_int[i].val()].getName();
        }
        else
        {
            os << assignments_int[i];
        }
        os << ", ";
    }
    os << "}" << std::endl;
}

Solution* Solution::babSearch(Query query, Query pool)
{
    // Initial situation
    Solution* s = new Solution(query, pool);
    // BAB search engine
    BAB<Solution> e(s);
    delete s;
    // search
    Solution* best = NULL;
    while (Solution* s = e.next()) {
        if(best != NULL)
        {
            delete best;
        }
        // Save current solution as best
        best = s;
    } 
    
    // throw exception if there is no solution
    if(best == NULL)
    {
        throw std::runtime_error("Solution babSearch: No solutions");
    }
    
    return best;
}

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
    Component queryCompAnotherC = compC;
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
    query.addComponent(queryCompA);
    query.addComponent(queryCompAnotherC);
    subquery.addComponent(queryCompB);
    subquery.addComponent(queryCompC);
    
    
    // Configure connections
    subquery.addConnection(queryCompC.getName(), subquery.getComponent(queryCompC.getName()).getOutPorts().at(1), 
                           queryCompB.getName(), subquery.getComponent(queryCompB.getName()).getInPorts().at(1));
    query.integrateSubQuery(subquery);
    
    query.addConnection(queryCompA.getName(), query.getComponent(queryCompA.getName()).getOutPorts().at(0), 
                        queryCompB.getName(), query.getComponent(queryCompB.getName()).getInPorts().at(0));
    query.addConnection(queryCompA.getName(), query.getComponent(queryCompA.getName()).getOutPorts().at(1), 
                        queryCompAnotherC.getName(), query.getComponent(queryCompAnotherC.getName()).getInPorts().at(0));
    query.addConnection(queryCompAnotherC.getName(), query.getComponent(queryCompAnotherC.getName()).getOutPorts().at(0), 
                        queryCompA.getName(), query.getComponent(queryCompA.getName()).getInPorts().at(0));
    
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
    Query pool ("pool");
    pool.addComponent(poolCompA0);
    pool.addComponent(poolCompA1);
    pool.addComponent(poolCompB0);
    pool.addComponent(poolCompB1);
    pool.addComponent(poolCompC0);
    pool.addComponent(poolCompC1);
    
    // Configure connections
    pool.addConnection(poolCompA0.getName(), pool.getComponent(poolCompA0.getName()).getOutPorts().at(0), 
                       poolCompB0.getName(), pool.getComponent(poolCompB0.getName()).getInPorts().at(0));
    pool.addConnection(poolCompA0.getName(), pool.getComponent(poolCompA0.getName()).getOutPorts().at(1), 
                       poolCompC1.getName(), pool.getComponent(poolCompC1.getName()).getInPorts().at(0));
    pool.addConnection(poolCompC0.getName(), pool.getComponent(poolCompC0.getName()).getOutPorts().at(0), 
                       poolCompA0.getName(), pool.getComponent(poolCompA0.getName()).getInPorts().at(0));
    pool.addConnection(poolCompC1.getName(), pool.getComponent(poolCompC1.getName()).getOutPorts().at(1), 
                       poolCompB0.getName(), pool.getComponent(poolCompB0.getName()).getInPorts().at(1));
    // Optional:
    pool.addConnection(poolCompA0.getName(), pool.getComponent(poolCompA0.getName()).getOutPorts().at(0), 
                       poolCompB1.getName(), pool.getComponent(poolCompB1.getName()).getInPorts().at(0));
    
    // Print pool and query
    std::cout << "Pool: " << pool.toString();
    std::cout << "Query: " << query.toString();
    
    Solution* s = Solution::babSearch(query, pool);
    s->print();
    return 0;
}
