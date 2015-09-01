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

#if 0
struct InstanceComponent_internal {
   public:
    InstanceComponent_internal(unsigned int tree_id, Component* c) : tree_id(tree_id), component(c) {
    };



    void addLimitation(Gecode::Space& space, std::string name,
                       std::string value, size_t cnt);

    void finalize(Gecode::Space& space) {
/*
        float_config.resize(usage_count);
        bool_config.resize(usage_count);
        int_config.resize(usage_count);

       for (size_t i = 0; i < usage_count; i++) {
            for (auto prop : underlaying_component->getProperties()) {
                switch (prop.t) {
                    case(ConfigurationModel::INT) : {
                        int_config[i][prop.name] =
                            Gecode::IntVar(space, 0, Gecode::Int::Limits::max);
                        break;
                    }
                    case(ConfigurationModel::DOUBLE) : {
                        float_config[i][prop.name] = Gecode::FloatVar(
                            space, 0, Gecode::Float::Limits::max);
                        break;
                    }
                    case(ConfigurationModel::BOOL) : {
                        bool_config[i][prop.name] =
                            Gecode::BoolVar(space, 0, 1);
                        break;
                    }
                    case(ConfigurationModel::STRING) : {
                        // TODO handle strings *BLAEH*
                        // int_config[i][j].push_back(IntVar(*this,0,Int::Limits::max));
                        break;
                    }
                };
            }
        }
        */

    }

    void addLimitation(Gecode::Space& space) {}

    void add_flattend_use_id(unsigned int id) {
        used_ids_in_flattend_graph.push_back(id);
    }

    static std::shared_ptr<InstanceComponent_internal> NewInstanceComponent(unsigned int tree_id, Component *c){
        return std::shared_ptr<InstanceComponent_internal>(new InstanceComponent_internal(tree_id,c));
    }

   protected:
    std::vector<std::map<std::string, Gecode::FloatVar>> float_config;
    std::vector<std::map<std::string, Gecode::BoolVar>> bool_config;
    std::vector<std::map<std::string, Gecode::IntVar>> int_config;
    std::vector<unsigned int> used_ids_in_flattend_graph;

    unsigned int tree_id;
    Component* component;
};

typedef std::shared_ptr<InstanceComponent_internal> InstanceComponent;
#endif



/**
 * A solution inherits GECODE's space. the initial situation as well as any real
 * solutions are of type InstanceSolution.
 */
class InstanceSolution : public Gecode::Space {
   protected:


    std::vector<std::map <std::string, Gecode::FloatVar> > float_config;
    std::vector<std::map <std::string, Gecode::BoolVar > > bool_config;
    std::vector<std::map <std::string, Gecode::IntVar  > > int_config;

    template <typename C>
    void setupProperties(C component, graph_analysis::Vertex::Ptr vertex, graph_analysis::BaseGraph::Ptr graph){
            for(auto cfg : component->getProperties()){
                switch(cfg.t){
                    case constrained_based_networks::ConfigurationModel::INT:
                        {
                            try{
                                int_config[graph->getVertexId(vertex)].at(cfg.name);
                            }catch(std::out_of_range e){
                                //Ok, propetry it is not yet created, let's create one
                                auto var = Gecode::IntVar(*this, 0, Gecode::Int::Limits::max);
                                int_config[graph->getVertexId(vertex)][cfg.name] = var;
                            }
                            break;
                        }
                    case constrained_based_networks::ConfigurationModel::DOUBLE:
                        {
                            try{
                                float_config[graph->getVertexId(vertex)].at(cfg.name);
                            }catch(std::out_of_range e){
                                //Ok, propetry it is not yet created, let's create one
                                auto var = Gecode::FloatVar(*this, 0, Gecode::Float::Limits::max);
                                float_config[graph->getVertexId(vertex)][cfg.name] = var;
                            }
                            break;
                        }
                    case constrained_based_networks::ConfigurationModel::BOOL:
                        {
                            try{
                                bool_config[graph->getVertexId(vertex)].at(cfg.name);
                            }catch(std::out_of_range e){
                                //Ok, propetry it is not yet created, let's create one
                                auto var = Gecode::BoolVar(*this, 0, 1);
                                bool_config[graph->getVertexId(vertex)][cfg.name] = var;
                            }
                            break;
                        }
                    case constrained_based_networks::ConfigurationModel::STRING:
                        {
                            throw std::runtime_error("Not yet implemented");
                            break;
                        }
                }
            }
    }
    /**
     * The query to compute solutions for.
     */
    // Query query;
    /**
     * The actual situation, components from the pool.
     */
    // Query componentPool;
    /**
     * Assignments of query components to pool components. This is what has to
     * be solved.
     */
//    Pool* pool;
    ClassSolution* cs;
 //   std::vector<Gecode::IntVar> tasks;
 //   std::vector<Gecode::IntVarArray> task_assignments;
 //   std::vector<InstanceComponent> instance_components;
    //    void limitComponents(unsigned int cmp_id);
 //   void createFlattendIDs(Composition* cmp, unsigned int next_free_id);
 //   void limitConfigs(Composition* cmp, unsigned int next_free_id);

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
    bool markCompositionAsChildless(Composition *composition, size_t
    composition_id);
    //Ids are the composition ids that are used within the core-root
    bool allDepsResolved(unsigned int cmp_id, std::vector<size_t> &ids);

    bool findNextBrancher(unsigned int id);

    //pass all USED compotions, all other ones will be removed
    void removeAllUnsedCmps(std::vector<size_t> ids);
    void doMissingBranching();
    */

   public:
    void compare(const Space& s, std::ostream& os) const;
   // static int print_count;
   // static void postBranching(Space& space);
  //  static void postBranching2(Space& space);
    /**
     * Construct a solution with an initial situation to search.
     */
    InstanceSolution(ClassSolution* cs);

    /**
     * search support. There must be a copy constructor like this for the search
     * engine.
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
    void rprint() const {
        printToStream(std::cout);
    };
    // void print(std::ostream& os = std::cout) const{printToStream(os,true);};
    void print(std::ostream& os = std::cout) const {
        printToDot(os);
    };
    /**
     * print support for given outputstream
     */
    void printToStream(std::ostream& os, bool full = false) const;
    void printToDot(std::ostream& os) const;

    static void print(const Space& home, const Gecode::BrancherHandle& bh,
                      unsigned int a, Gecode::IntVar x, int i, const int& n,
                      std::ostream& o);

    static void print(const Space& home, const Gecode::BrancherHandle& bh,
                      unsigned int a, Gecode::BoolVar x, int i, const int& n,
                      std::ostream& o);

    /**
     * Static method to find a best solution with BAB search.
     *
     * \throw Exception if there is no solution.
     */
    static InstanceSolution* babSearch2(ClassSolution* cs);
    static InstanceSolution* babSearch(ClassSolution* cs);
    static InstanceSolution* gistBaBSeach(ClassSolution* cs);
};

}  // end namespace constrained_based_networks

#endif  // DECODE_INSTANCE_SOLUTION_H
