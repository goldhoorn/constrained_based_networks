#include <gecode/int.hh>
#include <gecode/search.hh>
#include <gecode/gist.hh>

#include<vector>

#include "query.h"

using namespace Gecode;

namespace composition_management {

class Solution : public Space {
protected:
    Query query;
    std::vector<std::string> componentPool;
    
public:
    Solution(Query query, std::vector<std::string> componentPool) 
        : query(query)
        , componentPool(componentPool)
    {
  }
};

} // end namespace composition_management

// main function
int main(int argc, char* argv[]) {
    std::cout << "Hello world!" << std::endl;    
    return 0;
}
