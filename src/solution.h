#ifndef GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H
#define GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H

#include <gecode/search.hh>

#include "query.h"

using namespace Gecode;

namespace composition_management {

/**
 * A solution inherits GECODE's space. the initial situation as well as any real solutions are of type Solution.
 */
class Solution : public Space
{
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
    Solution(Query query, Query componentPool);
    
    /*
     * constrain function for best solution search. the
     * currently best solution _b is passed and we have to constraint that this solution can only
     * be better than b, for it to be excluded if it isn't
     */
    virtual void constrain(const Space& _b);
    
    // search support. There must be a copy constructor like this for the search engine.
    // Everything must be copied into the new Space
    Solution(bool share, Solution& s);
    // This method is called be the search engine
    virtual Space* copy(bool share);
    
    // print support
    void print() const;
    // print support for given outputstream
    void print(std::ostream& os) const;
};

} // end namespace composition_management

#endif // GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H