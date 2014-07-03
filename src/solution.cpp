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
    // Query components
    Component queryCompA = Component(0, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component queryCompB = Component(1, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component queryCompC = Component(2, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    std::vector<Component> queryComps;// = boost::assign::list_of(queryCompA)(queryCompB)(queryCompC);
    queryComps.push_back(queryCompA);
    queryComps.push_back(queryCompB);
    queryComps.push_back(queryCompC);
    
    // Pool components
    Component poolCompA0 = Component(0, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component poolCompA1 = Component(0, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component poolCompB0 = Component(1, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component poolCompB1 = Component(1, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component poolCompC0 = Component(2, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    Component poolCompC1 = Component(2, std::vector<IntVar>(), std::vector<IncomingPort>(), std::vector<OutgoingPort>());
    std::vector<Component> poolComps;//= boost::assign::list_of(poolCompA0)(poolCompA1)(poolCompB0)(poolCompB1)(poolCompC0)(poolCompC1);
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
