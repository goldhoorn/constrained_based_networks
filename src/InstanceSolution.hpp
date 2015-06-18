#ifndef DECODE_INSTANCE_SOLUTION_H
#define DECODE_INSTANCE_SOLUTION_H

#include "ClassSolution.hpp"
#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/float.hh>
#include <gecode/search.hh>
#include "Pool.hpp"


namespace constrained_based_networks {

    class Composition;

#define CONSTRAIN 


struct InstanceComponent{
public:
    InstanceComponent(){
        usage_count=0;
        finalized = false;
    };
    
    void increse_usage(){
        if(finalized) throw std::runtime_error("InstanceComponent is already finalized");
        usage_count++;
    };

    void initialize(Component *c){
        if(finalized) throw std::runtime_error("InstanceComponent is already finalized");
        underlaying_component = c;
    }

    void addLimitation(Gecode::Space &space, std::string name, std::string value, size_t cnt);


    void finalize(Gecode::Space &space){
        if(finalized) throw std::runtime_error("InstanceComponent is already finalized");
            
        float_config.resize(usage_count);       
        bool_config.resize(usage_count);       
        int_config.resize(usage_count);       
        
        for(size_t i = 0; i < usage_count; i++){
            for(auto prop : underlaying_component->getProperties()){
                switch(prop.t){
                    case(ConfigurationModel::INT):
                        {
                            int_config[i][prop.name] = Gecode::IntVar(space,0,Gecode::Int::Limits::max);
                            break;
                        }
                    case(ConfigurationModel::DOUBLE):
                        {
                            float_config[i][prop.name] = Gecode::FloatVar(space,0,Gecode::Float::Limits::max);
                            break;
                        }
                    case(ConfigurationModel::BOOL):
                        {
                            bool_config[i][prop.name] = Gecode::BoolVar(space,0,1);
                            break;
                        }
                    case(ConfigurationModel::STRING):
                        {
                            //TODO handle strings *BLAEH*
                            //int_config[i][j].push_back(IntVar(*this,0,Int::Limits::max));
                            break;
                        }
                };
            }
        }

        finalized=true;
    }

    void addLimitation(Gecode::Space &space){
    }

    void add_flattend_use_id(unsigned int id){
        used_ids_in_flattend_graph.push_back(id);
    }


protected:
    std::vector< std::map< std::string, Gecode::FloatVar> > float_config;
    std::vector< std::map< std::string, Gecode::BoolVar> > bool_config;
    std::vector< std::map< std::string, Gecode::IntVar> > int_config;
    std::vector<unsigned int> used_ids_in_flattend_graph;
    Component *underlaying_component;
    size_t usage_count;
    bool finalized;

};

/**
 * A solution inherits GECODE's space. the initial situation as well as any real solutions are of type InstanceSolution.
 */
class InstanceSolution : public Gecode::Space
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
    ClassSolution *cs;
    std::vector<Gecode::IntVar> tasks;
    std::vector<Gecode::IntVarArray> task_assignments;
    std::vector<InstanceComponent> instance_components;
//    void limitComponents(unsigned int cmp_id);
    void createFlattendIDs(Composition* cmp, unsigned int next_free_id);
    void limitConfigs(Composition* cmp, unsigned int next_free_id);



    /*
    std::vector<Gecode::IntVarArray> ir_assignments;
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
    */

public:
    void compare(const Space& s, std::ostream& os) const;
    static int print_count;
    static void postBranching(Space &space);
    static void postBranching2(Space &space);
    /**
     * Construct a solution with an initial situation to search.
     */
    InstanceSolution(ClassSolution *cs, Pool *pool = Pool::getInstance());
   
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
    InstanceSolution(bool share, InstanceSolution& s);
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
    static InstanceSolution* babSearch2(ClassSolution *cs, Pool *pool);
    static InstanceSolution* babSearch(ClassSolution *cs, Pool *pool);
    static InstanceSolution* gistBaBSeach(ClassSolution *cs, Pool *pool);
};

} // end namespace constrained_based_networks

#endif // DECODE_INSTANCE_SOLUTION_H
