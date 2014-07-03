#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>
#include <gecode/minimodel.hh>

#include<vector>
#include<map>

#include "query.h"

using namespace Gecode;

namespace composition_management {

// TODO consider VarArray instead of vector
class Solution : public Space {
protected:
    Query query;
    Query componentPool;
    
    
    // array of assignments (as rows x cols)
    //BoolVarArray assignments; 
    IntVarArray assignments_int;
public:
    Solution(Query query, Query componentPool) 
        : query(query)
        , componentPool(componentPool)
        //, assignments(*this, componentPool.getComponents().size() * query.getComponents().size())
        , assignments_int(*this, query.getComponents().size(), 0, componentPool.getComponents().size())
    {
        // matrix version of assigenments
        //Matrix<BoolVarArray> assignments_matrix(assignments, componentPool.getComponents().size(), query.getComponents().size());
        
        // Each row must have exactly one one. 
        //for(int r = 0; r < assignments_matrix.height(); r++) {
        //    linear(*this, assignments_matrix.row(r), IRT_EQ, 1);
        //}
        // This one must be at a spot where the col and row type are equal
        //rel(*this, north, IRT_EQ, east);
        //element(*this, assignments_matrix, );
        
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
            dom(*this, assignments_int[i], typeIndicesPool[type], upperLimit);
        }
        
        // For all queried components
        for(int i = 0; i < assignments_int.size(); i++)
        {
            // For all possible pool components they can be assigned
            for(int j = assignments_int[i].min(); j <= assignments_int[i].max(); j++)
            {
                // For all configurations
                for(int k = 0; k < query.getComponents()[i].getConfiguration().size(); k++)
                {
                    int requiredConfiguration = query.getComponents()[i].getConfiguration()[k];
                    int actualConfiguration = componentPool.getComponents()[j].getConfiguration()[k];
                    // If the configurations are not compatible, post != constraint on this assignment
                    if(requiredConfiguration != 0 && actualConfiguration != 0 && requiredConfiguration != actualConfiguration)
                    {
                        rel(*this, assignments_int[i], IRT_NQ, j);
                        break;
                    }
                }
            }
        }
        
        // branching
        branch(*this, assignments_int, INT_VAR_SIZE_MIN(), INT_VAL_MIN());
    }
    
    // search support
    Solution(bool share, Solution& s) 
        : Space(share, s)
        , query(s.query)
        , componentPool(s.componentPool)
    {
        assignments_int.update(*this, share, s.assignments_int);
    }
    virtual Space* copy(bool share) {
        return new Solution(share,*this);
    }
    // print support
    void print(void) const {
        std::cout << "Solution: " << assignments_int << std::endl;
    }
    void print(std::ostream& os) const {
        os << assignments_int << std::endl;
    }
};

} // end namespace composition_management

// main function
int main(int argc, char* argv[]) {
    using namespace composition_management;
    // General components
    Component compA (0, 2, std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component compB (1, 1, std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component compC (2, 1, std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    
    // Query components
    Component queryCompA = compA;
    Component queryCompB = compB;
    Component queryCompC = compC;
    // Configure query components
    queryCompA.getConfiguration()[0] = 3;
    queryCompA.getConfiguration()[1] = 1;
    // No constraint on B's config
    queryCompC.getConfiguration()[0] = 2;
    // Push into vector
    std::vector<Component> queryComps;
    queryComps.push_back(queryCompA);
    queryComps.push_back(queryCompB);
    queryComps.push_back(queryCompC);
    
    // Pool components
    Component poolCompA0 = compA;
    Component poolCompA1 = compA;
    Component poolCompB0 = compB;
    Component poolCompB1 = compB;
    Component poolCompC0 = compC;
    Component poolCompC1 = compC;
    // Configure pool components
    poolCompA0.getConfiguration()[0] = 3;
    poolCompA0.getConfiguration()[1] = 3;
    poolCompB0.getConfiguration()[0] = 2;
    poolCompC1.getConfiguration()[0] = 2;
    // Push into vector
    std::vector<Component> poolComps;
    poolComps.push_back(poolCompA0);
    poolComps.push_back(poolCompA1);
    poolComps.push_back(poolCompB0);
    poolComps.push_back(poolCompB1);
    poolComps.push_back(poolCompC0);
    poolComps.push_back(poolCompC1);
    
    Query query (queryComps, std::map<IncomingPort, OutgoingPort>());
    Query pool (poolComps, std::map<IncomingPort, OutgoingPort>());
    
    Solution* s = new Solution(query, pool);
    DFS<Solution> e(s);
    delete s;
    // search and print all solutions
    while (Solution* s = e.next()) {
        s->print(); delete s;
    } 
    return 0;
}
