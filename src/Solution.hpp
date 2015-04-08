#ifndef GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H
#define GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H

#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/search.hh>
#include "Pool.hpp"

namespace constrained_based_networks {

    class Composition;
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
    Pool *pool;
    std::vector<Gecode::IntVarArray> ir_assignments;
    std::vector<Gecode::BoolVar> workaround;
    Gecode::BoolVarArray active;
    Gecode::SetVarArray depends;
    Gecode::SetVarArray depends_recursive;
    void markInactiveAsInactive();
    void depsOnlyOnCmp();
    void markAbstractAsInactive();
    void markActiveAsActive();
    void removeSelfCompositonsFromDepends();
    bool markCompositionAsChildless(Composition *composition, size_t composition_id);

public:
    /**
     * Construct a solution with an initial situation to search.
     */
    Solution(Pool *pool = Pool::getInstance());
   
#ifdef CONSTRAIN
    /*
     * constrain function for best solution search. the
     * currently best solution _b is passed and we have to constraint that this solution can only
     * be better than b, for it to be excluded if it isn't
     */
    virtual void constrain(const Gecode::Space& _b);
#endif
    
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
    void rprint() const{printToStream(std::cout);};
    void print(std::ostream& os = std::cout) const{printToStream(os,true);};
    /**
     * print support for given outputstream
     */
    void printToStream(std::ostream& os, bool full=false) const;

    static void print(const Space& home,
           const Gecode::BrancherHandle& bh,
           unsigned int a,
           Gecode::IntVar x, int i, const int& n,
           std::ostream& o);
    
    static void print(const Space& home,
           const Gecode::BrancherHandle& bh,
           unsigned int a,
           Gecode::BoolVar x, int i, const int& n,
           std::ostream& o);

    /**
     * Static method to find a best solution with BAB search.
     * 
     * \throw Exception if there is no solution.
     */
    static Solution* babSearch2();
    static Solution* babSearch(Pool *pool);
    static Solution* gistBaBSeach();
};

} // end namespace constrained_based_networks

#endif // GECODE_COMPOSITION_MANAGEMENT_SOLUTION_H
