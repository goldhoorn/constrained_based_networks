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


#include <gecode/gist.hh>

using namespace Gecode;

namespace constrained_based_networks {

Solution::Solution(Pool *pool)
    : active(*this, pool->getComponentCount(), 0, 1)
    , inactive(*this, pool->getComponentCount(), 0, 1)
    , depends(*this,pool->getComponentCount(), IntSet::empty, IntSet(0,pool->getCount<Composition*>()-1))
//    : query(query)
//    , componentPool(componentPool)
//    , assignments_int(*this, query.getCompositions().size(), 0, componentPool.getCompositions().size() - 1)
{
/*
    std::cout << "Got " << pool->getItems<DataService*>().size() << "DataServices" << std::endl;
    std::cout << "Got " << pool->getItems<Composition*>().size() << "Compositions" << std::endl;
    std::cout << "Got " << pool->getItems<Task*>().size() << "Tasks" << std::endl;
*/
    for(size_t i = 0;i<active.size();i++)
    {
        rel(*this,active[i],IRT_EQ, 0, inactive[i]);
    }
    
    
    for(auto provider : pool->getItems<DataService*>()){
        if(provider->abstract()){
            rel(*this,active[pool->getId(provider)],IRT_EQ, 0);
    //        std::cout << "Got some abstract" << std::endl;
        }else{
            std::cout << "Got non abstract data-service?\n";
        }
    }

    for(auto c: pool->getItems<Component*>()){
        unsigned int cmp_id = pool->getId(c);
        if(c->isActive()){
            rel(*this,active[cmp_id],IRT_EQ, 1);
        }
    }

    bool used_components[pool->getComponentCount()];
    for(size_t i=0;i<pool->getComponentCount();i++){
        used_components[i] = false;
    }


    for(auto component: pool->getItems<Component*>()){
        unsigned int cnt=0;
        for(auto composition : pool->getItems<Composition*>()){
            if(composition->getName() == component->getName()){
//                std::cout << "Ignore " << component->getName() << " " << cnt << std::endl;              
                dom(*this,depends[pool->getId(component)], SRT_DISJ, cnt);
            }
            cnt++;
        }
    }

    size_t cmp_counter=0;
    for(auto composition : pool->getItems<Composition*>()){
        unsigned int cmp_id = pool->getId(composition);

        auto composition_child_constraints = composition->getPossibleTaskAssignments(this);



        unsigned int child_id=0;
        if(composition->getChildren().size() == 0){
            for(auto provider : pool->getItems<Component*>()){
                dom(*this,depends[pool->getId(provider)], SRT_DISJ, cmp_counter);
            }

        }//else
        for(auto child : composition->getChildren()){
            for(auto provider : pool->getItems<Component*>()){
                //If any child is assigned to the DUMMY comonent, this composition cannot run
                rel(*this,composition_child_constraints[child_id],IRT_EQ, 0, imp(inactive[cmp_id]));



                if(provider->abstract()){
                    //Prevent selection of data-services for children
                    rel(*this,composition_child_constraints[child_id],IRT_NQ, pool->getId(provider));
                    dom(*this,depends[pool->getId(provider)], SRT_DISJ, cmp_counter);
                    /*
                    if(composition->isActive()){
                        std::cout << "!!!!!!! forbidding DS for " << child.first << " -- " << (*pool)[pool->getId(provider)]->getName() << std::endl;
                    }
                    */
                    continue;
                    
                }

                used_components[pool->getId(provider)] = true;

                if(provider->isFullfilling(child.second->getName())){
//                    std::cout << "+++++++ allowing for " << child.first << " -- " << (*pool)[pool->getId(provider)]->getName() << std::endl;
//
                    //This provider is able fo fullfill the requested DS from the child
                    //rel(*this,active[cmp_id], 
                    //Do nothing later add weights
           //         rel(*this,composition_child_constraints[child_id],IRT_EQ, 0, active[cmp_id]);

                    //define that if the child is used, then it needs to be active

                     rel(*this, composition_child_constraints[child_id],IRT_EQ, pool->getId(provider), pmi(active[pool->getId(provider)]));
                     dom(*this,depends[pool->getId(provider)], SRT_SUB, cmp_counter, active[cmp_id]);

//                    rel(*this, composition_child_constraints[child_id],IRT_EQ, pool->getId(provider), pmi(active[cmp_id]));

                }else{
//                    std::cout << "####### forbidding for " << child.first << " -- " << (*pool)[pool->getId(provider)]->getName() << std::endl;
                    //if(pool->getId(provider) != 0){
                        rel(*this,composition_child_constraints[child_id],IRT_NQ, pool->getId(provider), imp(active[cmp_id]));
                        dom(*this,depends[pool->getId(provider)], SRT_DISJ, cmp_counter);
                    //}
                    //rel(*this,composition_child_constraints[child_id],IRT_NQ, pool->getId(provider), imp(active[cmp_id]));
//                    rel(*this,composition_child_constraints[child_id],IRT_NQ, pool->getId(provider), (active[cmp_id]));

                }
            }
        child_id++;
        }
    
        for(size_t i=0;i<pool->getComponentCount();i++){
            if(!used_components[i]){
                rel(*this,active[i],IRT_EQ,0);   
            }
        }

        branch(*this, composition_child_constraints, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
        //branch(*this, composition_child_constraints, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
        //branch(*this, composition_child_constraints, INT_VAR_NONE(), INT_VAL_MIN());//, INT_VAL_MIN(),NULL,&print);
        ir_assignments.push_back(composition_child_constraints);
//        cmp_id++;
        cmp_counter++;
    }
    branch(*this, active, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
    branch(*this, inactive, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
    branch(*this, depends, SET_VAR_NONE(), SET_VAL_MIN_INC());

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
    
    //branch(*this, active, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);

#ifdef CONSTRAN
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
    //nvalues(*this, active, IRT_GE, valuesCount);
    
    // If we have an equal amount of values used, the number of reconfigured components must be less
    BoolVar equalAmountOfValues;
    //nvalues(*this, assignments_int, IRT_LQ, valuesCount, equalAmountOfValues);
    
    //std::cout << " Adding best search constraint. This ";
    /*
    std::cerr << "##########################################################################" << std::endl;
    rprint();
    std::cerr << "##########################################################################" << std::endl;
    */
    //std::cout << " must be better than so far best ";
    //b.print();
    
}
#endif

Solution::Solution(bool share, Solution& s) 
    : Space(share, s)
    //, query(s.query)
    //, componentPool(s.componentPool)
{
    active.update(*this, share, s.active);
    inactive.update(*this, share, s.inactive);
    depends.update(*this, share, s.depends);
    ir_assignments.resize(s.ir_assignments.size());

    for(size_t i = 0; i < s.ir_assignments.size();i++){
        ir_assignments[i].update(*this,share,s.ir_assignments[i]);
    }
}

Space* Solution::copy(bool share) 
{
    return new Solution(share,*this);
}

//void Solution::print(std::ostream& os) const 
//{
//    printToStream(os);
//}

void Solution::printToStream(std::ostream& os, bool full) const 
{
    os << "Count: " << active.size() << std::endl;

    os << "Solution: { " << std::endl;
    for(int i = 0; i < active.size(); i++)
    {
        //os << "\t" << (*Pool::getInstance())[i]->getName() << "=";
        if(active[i].assigned())
        {
            if(active[i].val() || full){
                os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << "(" << inactive[i] << ")" << std::endl;
            }
        }
        else
        {
            os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << std::endl;
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
                 if(ir_assignments[cmp_id][child_id].val() != 0 || full){ //Only print assigned solutions
                    os << composition->getName() << "." << child.first << "=";
                    os << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName();
                    os <<  " (" << ir_assignments[cmp_id][child_id] << ")" << std::endl;
                 }
            }else{
                os << composition->getName() << "." << child.first << "=" << ir_assignments[cmp_id][child_id] << std::endl;
            }
            child_id++;
        }
        cmp_id++;
    }
#if 1
    for(size_t i = 0; i< depends.size();i++){
        //auto o = pool->getItems<Composition*>()[i];//(*pool)[i];
        auto o = (*pool)[i];
        os << "Deps for " << o->getName() << ": " <<std::endl;//<< depends << std::endl; 
        for (SetVarGlbValues j(depends[i]); j(); ++j){
            os << "- " << j.val() << " " <<  pool->getItems<Composition*>()[j.val()]->getName() << std::endl;
        }
    }
#endif
}

void Solution::print(const Space& home, const BrancherHandle& bh, unsigned int a, BoolVar x, int i, const int& n, std::ostream& o) {
    //const Solution& c = static_cast<const Solution&>(home);
    o << "foo" << a << " " << x << " " << i << " " << n << std::endl;
}

void Solution::print(const Space& home, const BrancherHandle& bh, unsigned int a, IntVar x, int i, const int& n, std::ostream& o) {
    //const Solution& c = static_cast<const Solution&>(home);
    o << "foo" << a << " " << x << " " << i << " " << n << std::endl;
    /*
    int x = i % c.w, y = i / c.w;
    o << "letters[" << x << "," << y << "] "
        << ((a == 0) ? "=" : "!=") << " "
        << static_cast<char>(n);
    */
}

Solution* Solution::babSearch2()
{
    return Solution::babSearch(Pool::getInstance());
}

Solution* Solution::babSearch(Pool *pool)
{
    // Initial situation
    Solution* so = new Solution(pool);
    // BAB search engine
    //BAB<Solution> e(so);
    BAB<Solution> e(so);
    // search
    Solution* best = NULL;
    
    while (Solution* s = e.next()){
        if(best != NULL)
        {
            delete best;
            best=0;
        }
        // Save current solution as best
        s->rprint();
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        best = s;

    } 
    
    // throw exception if there is no solution
    if(best == NULL)
    {
        delete so;
        throw std::runtime_error("Solution babSearch: No solutions");
    }
    delete so;
    return best;
}

Solution *Solution::gistBaBSeach(){
    Solution* m = new Solution(Pool::getInstance());
    //Solution* m = Solution::babSearch(Pool::getInstance());
    Gist::Print<Solution> printer("Print solution");
//    Gist::VarComparator<Solution> c("Compare nodes");
   Gist::Options o;
    o.inspect.click(&printer);
//    o.inspect.compare(&c);
    Gist::bab(m,o);
    return m;
}

} // end namespace constrained_based_networks

