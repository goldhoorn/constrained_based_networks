#ifndef GECODE_CLASS_SOLUTION_H
#define GECODE_CLASS_SOLUTION_H

#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/search.hh>
#include "Pool.hpp"

namespace constrained_based_networks {
    class Composition;

#define CONSTRAIN

/**
 * A solution inherits GECODE's space. the initial situation as well as any real solutions are of type ClassSolution.
 */
class ClassSolution : public Gecode::Space
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
    Gecode::BoolVarArray active;
    Gecode::SetVarArray depends;
    Gecode::SetVarArray depends_recursive;
    std::vector<Gecode::BrancherHandle> ir_assignments_brancher;
    Gecode::BrancherHandle active_brancher;
    Gecode::BrancherHandle depends_brancher;
    Gecode::BrancherHandle depends_recursive_brancher;
    void markInactiveAsInactive();
    void depsOnlyOnCmp();
    void markAbstractAsInactive();
    void markActiveAsActive();
    void removeSelfCompositonsFromDepends();
    bool markCompositionAsChildless(Composition *composition, size_t composition_id);
    //Ids are the composition ids that are used within the core-root
    bool allDepsResolved(unsigned int cmp_id, std::vector<size_t> &ids);

    bool findNextBrancher(unsigned int id);

    //pass all USED compotions, all other ones will be removed
    void removeAllUnsedCmps(std::vector<size_t> ids);
    void doMissingBranching();
    void createConstraintsForComposition(Composition *cmp);
    std::map<Composition*, unsigned int> cmp_ids;
    std::vector<bool> cmp_constraints_done;
    void prepareCompositionConstraints(Composition *composition);

public:
    std::vector<Gecode::IntVarArray> ir_assignments;
    bool build_tree(graph_analysis::BaseGraph::Ptr g, unsigned int cmp_id);

    void compare(const Space& s, std::ostream& os) const;
    static int print_count;
    static void postBranching(Space &space);
    static void postBranching2(Space &space);
    /**
     * Construct a solution with an initial situation to search.
     */
    ClassSolution(Pool *pool = Pool::getInstance());

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
    ClassSolution(bool share, ClassSolution& s);
    /**
     * This method is called be the search engine
     */
    virtual Gecode::Space* copy(bool share);

    /**
     * print support
     */
    void rprint() const{printToStream(std::cout);};
    //void print(std::ostream& os = std::cout) const{printToStream(os,true);};
    void print(std::ostream& os = std::cout) const{printToDot(os);};
    /**
     * print support for given outputstream
     */
    void printToStream(std::ostream& os, bool full=false) const;
    void printToDot(std::ostream& os) const;

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
    static ClassSolution* babSearch2();
    static ClassSolution* babSearch(Pool *pool);
    static ClassSolution* gistBaBSeach();
};

} // end namespace constrained_based_networks

#endif // GECODE_CLASS_SOLUTION_H
