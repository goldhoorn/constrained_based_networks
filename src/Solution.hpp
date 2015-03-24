#ifndef GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H
#define GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H

#include <gecode/int.hh>
#include <gecode/search.hh>
#include "Pool.hpp"

namespace constrained_based_networks {

/**
 * A solution inherits GECODE's space. the initial situation as well as any real solutions are of type Solution.
 */
class Solution : public Gecode::Space
{
protected:
    /**
     * The query to compute solutions for.
     */
    //Query query;
    /**
     * The actual situation, components from the pool.
     */
    //Query componentPool;
    /**
     * Assignments of query components to pool components. This is what has to be solved.
     */
    std::vector<Gecode::IntVarArray> ir_assignments;
    Gecode::BoolVarArray active;
public:
    /**
     * Construct a solution with an initial situation to search.
     */
    Solution(Pool *pool = Pool::getInstance());
    
    /*
     * constrain function for best solution search. the
     * currently best solution _b is passed and we have to constraint that this solution can only
     * be better than b, for it to be excluded if it isn't
     */
    virtual void constrain(const Gecode::Space& _b);
    
    /**
     * search support. There must be a copy constructor like this for the search engine.
     * Everything must be copied into the new Space
     */
    Solution(bool share, Solution& s);
    /**
     * This method is called be the search engine
     */
    virtual Gecode::Space* copy(bool share);
    
    /**
     * print support
     */
    void print() const;
    /**
     * print support for given outputstream
     */
    void printToStream(std::ostream& os) const;
    
    /**
     * Static method to find a best solution with BAB search.
     * 
     * \throw Exception if there is no solution.
     */
    static Solution* babSearch2();
    static Solution* babSearch(Pool *pool);
};

} // end namespace constrained_based_networks

#endif // GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H
