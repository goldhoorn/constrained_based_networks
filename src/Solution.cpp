#include "Solution.hpp"

#include <gecode/gist.hh>
#include <gecode/minimodel.hh>

#include <vector>
#include <map>
#include <stdexcept>
#include "Pool.hpp"
#include "Task.hpp"
#include "Composition.hpp"
#include "DataService.hpp"

using namespace Gecode;

namespace constrained_based_networks {

Solution::Solution(Pool *pool)
    : active(*this, pool->getComponentCount(), 0, 1)
//    : query(query)
//    , componentPool(componentPool)
//    , assignments_int(*this, query.getCompositions().size(), 0, componentPool.getCompositions().size() - 1)
{
//    Pool *pool = Pool::getInstance();
//    unsigned int cmp_id=0;
    std::cout << "Got " << pool->getItems<DataService*>().size() << "DataServices" << std::endl;
    std::cout << "Got " << pool->getItems<Composition*>().size() << "Compositions" << std::endl;
    std::cout << "Got " << pool->getItems<Task*>().size() << "Tasks" << std::endl;
    /*
    for(auto provider : pool->getItems<Component*>()){
        if(provider->getName() == "Base::ZProviderSrv"){
            std::cout << std::endl;
            std::cout << "I got a Base::ZProviderSrv" << std::endl;

            if(dynamic_cast<DataService*>(provider)){
                std::cout << "It is a data-service" << std::endl;
            }
            if(dynamic_cast<Composition*>(provider)){
                std::cout << "It is a Composition" << std::endl;
            }
            if(dynamic_cast<Component*>(provider)){
                std::cout << "It is a Component" << std::endl;
            }
            if(dynamic_cast<Task*>(provider)){
                std::cout << "It is a Task" << std::endl;
            }
            std::cout << std::endl;
        }
    }
    */
    
    for(auto provider : pool->getItems<DataService*>()){
        if(provider->abstract()){
    //        rel(*this,active[pool->getId(provider)],IRT_EQ, 0);
    //        std::cout << "Got some abstract" << std::endl;
        }else{
    //        std::cout << "Got non abstract data-service?\n";
        }
    }

    for(auto composition : pool->getItems<Composition*>()){
        unsigned int cmp_id = pool->getId(composition);

        auto composition_child_constraints = composition->getPossibleTaskAssignments(this);

        branch(*this, composition_child_constraints, INT_VAR_SIZE_MIN(), INT_VAL_MIN());

        if(composition->isActive()){
            std::cout << "Composition " << composition->getName() << " is active" << std::endl;
            rel(*this,active[cmp_id],IRT_EQ, 1);
        }else{
            rel(*this,active[cmp_id],IRT_EQ, 0);
        }

        unsigned int child_id=0;
        for(auto child : composition->getChildren()){
            //unsigned int child_id = pool->getId(child.second);
            for(auto provider : pool->getItems<Component*>()){
                if(provider->abstract()){
                    rel(*this,composition_child_constraints[child_id],IRT_NQ, pool->getId(provider));
                    if(composition->isActive()){
                        //std::cout << "!!!!!!! forbidding DS for " << child.first << " -- " << (*pool)[pool->getId(provider)]->getName() << std::endl;
                    }
                    continue;
                    
                }

                if(provider->isFullfilling(child.second->getName())){
//                    std::cout << "+++++++ allowing for " << child.first << " -- " << (*pool)[pool->getId(provider)]->getName() << std::endl;
                    //printf("+++++++ Allowing %s (%u) for %s\n",provider->getName().c_str(),pool->getId(provider), child.second->getName().c_str());
                    //This provider is able fo fullfill the requested DS from the child
                    //rel(*this,active[cmp_id], 
                    //Do nothing later add weights
            //        rel(*this,composition_child_constraints[child_id],IRT_EQ, 0, active[cmp_id]);

                    //define that if the child is used, then it needs to be active
                    rel(*this, composition_child_constraints[child_id],IRT_EQ, pool->getId(provider), pmi(active[pool->getId(provider)]));
                }else{
//                    std::cout << "####### forbidding for " << child.first << " -- " << (*pool)[pool->getId(provider)]->getName() << std::endl;
                    rel(*this,composition_child_constraints[child_id],IRT_NQ, pool->getId(provider), imp(active[cmp_id]));

                }
            }
        child_id++;
        }
        ir_assignments.push_back(composition_child_constraints);
//        cmp_id++;
    }

    //assignment_cmps 
    
    /*
    // Check types of all query components against all pool components and only allow identical types to be assigned
    for(int i = 0; i < assignments_int.size(); i++)
    {
        for(int j = 0; j < componentPool.getCompositions().size(); j++)
        {
            if(query.getCompositions().at(i).getType() != componentPool.getCompositions().at(j).getType())
            {
                // This constrains to that the ith query component cannot be assigned the jth pool component,
                // as their types differ.
                rel(*this, assignments_int[i], IRT_NQ, j);
            }
        }
    }
    
    // For all queried components
    for(int i = 0; i < assignments_int.size(); i++)
    {
        std::cout << "Constraints for query " << query.getCompositions()[i].toString() << std::endl;
        std::cout << "With domian " << assignments_int[i] << std::endl;
        
        // Differently configured query components may not be assigned the same unconfigured pool component
        for(int j = 0; j < assignments_int.size(); j++)
        {
            if(i == j || query.getCompositions().at(i).getType() != query.getCompositions().at(j).getType())
            {
                continue;
            }
            std::cout << " Against query " << query.getCompositions()[j].toString() << std::endl;
            
            const std::vector<std::string>& requiredConfiguration0 = query.getCompositions().at(i).getConfiguration();
            const std::vector<std::string>& requiredConfiguration1 = query.getCompositions().at(j).getConfiguration();
            // If the configurations are not compatible, post != constraint on this assignment
            if(!requiredConfiguration0.empty()&& !requiredConfiguration1.empty() && requiredConfiguration0 != requiredConfiguration1)
            {
                std::cout << "Adding configuration constraint. Composition " << query.getCompositions()[i].getName() << "!=" <<  query.getCompositions()[j].getName() << std::endl;
                // This means that the ith query component and the jth query component cannot be assigned the same pool component,
                // since the configurations are incompatible.
                rel(*this, assignments_int[i], IRT_NQ, assignments_int[j]);
            }
        }
        
        // For all possible pool components they can be assigned
        for(int j = assignments_int[i].min(); j <= assignments_int[i].max(); j++)
        {
            std::cout << " Against pool " << componentPool.getCompositions()[j].toString() << std::endl;
            
            // TODO extern method testing if two components are compitable regarding their configurations
            // as vector<string>
            // At the moment, the conf vector just has to be equal
            const std::vector<std::string>& requiredConfiguration = query.getCompositions().at(i).getConfiguration();
            const std::vector<std::string>& actualConfiguration = componentPool.getCompositions().at(j).getConfiguration();
            // If the configurations are not compatible, post != constraint on this assignment
            if(!requiredConfiguration.empty()&& !actualConfiguration.empty() && requiredConfiguration != actualConfiguration)
            {
                std::cout << "Adding configuration constraint. Composition " << query.getCompositions()[i].getName() << "!=" << componentPool.getCompositions()[j].getName() << std::endl;
                // This means that the ith query component cannot be assigned the jth pool component,
                // since the configurations are incompatible.
                rel(*this, assignments_int[i], IRT_NQ, j);
            }
            
            
            // For all incoming ports of the query component
            const std::map<IncomingPort, std::string> map = query.getCompositions().at(i).getIncomingConnections();
            for(std::map<IncomingPort, std::string>::const_iterator it = map.begin(); it != map.end(); ++it)
            {
                std::cout << "Checking connection constraint. Composition " << query.getCompositions()[i].getName() << "=" << componentPool.getCompositions()[j].getName() 
                                << " on in port " << it->first.name << std::endl;
                
                // If the ith query component gets assigned to the jth pool component
                // and they both are connected on the same input port,
                // the connection origin must also be assigned equally.
                std::map<IncomingPort, std::string> poolCompositionMap = componentPool.getCompositions().at(j).getIncomingConnections();
                if(poolCompositionMap.count(it->first) != 0)
                {
                    // Get number of the origins
                    int originQueryNum = query.getCompositionIndex(it->second);
                    int originPoolNum = componentPool.getCompositionIndex(poolCompositionMap.at(it->first));
                                        
                    std::cout << "Adding connection constraint. Composition " << query.getCompositions()[i].getName() << "=" << componentPool.getCompositions()[j].getName() 
                                << " => " << it->second << "=" << poolCompositionMap.at(it->first) << std::endl;
                    
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
    */
    
    // branching TODO test and improve
    
    // this tells the search engine to branch first on the variable with the smallest domain,
    // that is the variables where there are less components of that type in the pool
    // The engine will then try the possible values (assignments) in ascending order.
    
    branch(*this, active, INT_VAR_SIZE_MIN(), INT_VAL_MIN());

}

void Solution::constrain(const Space& _b) 
{
    const Solution& b = static_cast<const Solution&>(_b);
    
    // Number of used components
    // Determine number of used values in b
    int valuesCount = 0;
    std::vector<int> values;
    for(BoolVarArray::const_iterator it = b.active.begin(); it != b.active.end(); ++it)
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
    nvalues(*this, active, IRT_LE, valuesCount);
    //nvalues(*this, active, IRT_GT, valuesCount);
    
    // If we have an equal amount of values used, the number of reconfigured components must be less
    BoolVar equalAmountOfValues;
    //nvalues(*this, assignments_int, IRT_LQ, valuesCount, equalAmountOfValues);
    
    //std::cout << " Adding best search constraint. This ";
    //print();
    //std::cout << " must be better than so far best ";
    //b.print();
    
}

Solution::Solution(bool share, Solution& s) 
    : Space(share, s)
    //, query(s.query)
    //, componentPool(s.componentPool)
{
    active.update(*this, share, s.active);
    ir_assignments.resize(s.ir_assignments.size());

    for(size_t i = 0; i < s.ir_assignments.size();i++){
        ir_assignments[i].update(*this,share,s.ir_assignments[i]);
    }
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
    os << "Count: " << active.size() << std::endl;

    os << "Solution: { " << std::endl;
    for(int i = 0; i < active.size(); i++)
    {
        //os << "\t" << (*Pool::getInstance())[i]->getName() << "=";
        if(active[i].assigned())
        {
            if(active[i].val()){
                os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << std::endl;
            }
        }
        else
        {
            os << "\t" << (*Pool::getInstance())[i]->getName() << "=?" << std::endl;
        }
//        os << ", "<< std::endl;
    }
    os << "}" << std::endl;
    
   
    Pool *pool = Pool::getInstance();

    size_t cmp_id=0;
    for(auto composition : pool->getItems<Composition*>()){
        size_t child_id=0;
        for(auto child : composition->getChildren()){
            if(ir_assignments[cmp_id][child_id].assigned()){
                 if(ir_assignments[cmp_id][child_id].val() != 0){ //Only print assigned solutions
                    os << composition->getName() << "." << child.first << "=";
                    os << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName();
                    os <<  " (" << ir_assignments[cmp_id][child_id] << ")" << std::endl;
                 }
            }else{
                os << composition->getName() << "." << child.first << "=?" << std::endl;
            }
            child_id++;
        }
        cmp_id++;
    }

}

Solution* Solution::babSearch2()
{
    return Solution::babSearch(Pool::getInstance());
}

Solution* Solution::babSearch(Pool *pool)
{
    // Initial situation
    Solution* s = new Solution(pool);
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

} // end namespace constrained_based_networks

