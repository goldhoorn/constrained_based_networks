#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>

#include<vector>

#include "query.h"

using namespace Gecode;

namespace composition_management {

// TODO consider VarArray instead of vector
class Solution : public Space {
protected:
    Query query;
    std::vector<std::string> componentPool;
    
    /**
     * The variable n represents the nth Component of the Query. Its value, m, indicates
     * the mth position in the componentPool.
     */
    IntVarArray assignments;
    
public:
    Solution(Query query, std::vector<std::string> componentPool) 
        : query(query)
        , componentPool(componentPool)
        // FIXME that every component can initally be assigned to any other seems to be a bad model
        , assignments(*this, query.getComponents().size(), 0, componentPool.size())
    {
    }
};

} // end namespace composition_management

// main function
int main(int argc, char* argv[]) {
    std::cout << "Hello world!" << std::endl;    
    return 0;
}
