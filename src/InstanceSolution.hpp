#ifndef DECODE_INSTANCE_SOLUTION_H
#define DECODE_INSTANCE_SOLUTION_H

#include "ClassSolution.hpp"
#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/float.hh>
#include <gecode/search.hh>
#include "Pool.hpp"

namespace constrained_based_networks
{

class Composition;

/**
 * A solution inherits GECODE's space. the initial situation as well as any real
 * solutions are of type InstanceSolution.
 */
class InstanceSolution : public Gecode::Space
{
   public:
    // Default user-constructor
    InstanceSolution(ClassSolution* cs);

    // Helper method for copying
    InstanceSolution(bool share, InstanceSolution& s);

    // Some helper-static constructors which also rxecures the search
    static InstanceSolution* babSearch2(ClassSolution* cs);
    static InstanceSolution* babSearch(ClassSolution* cs);
    static InstanceSolution* gistBaBSeach(ClassSolution* cs);

    // helper methods for gist
    void compare(const Space& s, std::ostream& os) const;

    virtual Gecode::Space* copy(bool share);

    void rprint() const
    {
        printToStream(std::cout);
    };

    // void print(std::ostream& os = std::cout) const{printToStream(os,true);};
    void print(std::ostream& os = std::cout) const
    {
        printToDot(os);
    };

    /**
     * print support for given outputstream
     */
    void printToStream(std::ostream& os, bool full = false) const;
    void printToDot(std::ostream& os) const;

    static void print(const Space& home, const Gecode::BrancherHandle& bh, unsigned int a, Gecode::IntVar x, int i, const int& n, std::ostream& o);

    static void print(const Space& home, const Gecode::BrancherHandle& bh, unsigned int a, Gecode::BoolVar x, int i, const int& n, std::ostream& o);

   protected:
    ClassSolution* cs;
    graph_analysis::BaseGraph::Ptr graph;

    std::vector<std::map<std::string, Gecode::FloatVar>> float_config;
    std::vector<std::map<std::string, Gecode::BoolVar>> bool_config;
    std::vector<std::map<std::string, Gecode::IntVar>> int_config;

    template <typename C>
    void setupProperties(C component, graph_analysis::Vertex::Ptr vertex, graph_analysis::BaseGraph::Ptr graph)
    {
        for (auto cfg : component->getProperties()) {
            switch (cfg.t) {
                case constrained_based_networks::ConfigurationModel::INT: {
                    try
                    {
                        int_config[graph->getVertexId(vertex)].at(cfg.name);
                    }
                    catch (std::out_of_range e)
                    {
                        // Ok, propetry it is not yet created, let's create one
                        auto var = Gecode::IntVar(*this, 0, Gecode::Int::Limits::max);
                        std::cout << "int test: " << cfg.name << std::endl;
                        int_config[graph->getVertexId(vertex)][cfg.name] = var;
                    }
                    break;
                }
                case constrained_based_networks::ConfigurationModel::DOUBLE: {
                    try
                    {
                        float_config[graph->getVertexId(vertex)].at(cfg.name);
                    }
                    catch (std::out_of_range e)
                    {
                        // Ok, propetry it is not yet created, let's create one
                        auto var = Gecode::FloatVar(*this, 0, Gecode::Float::Limits::max);
                        float_config[graph->getVertexId(vertex)][cfg.name] = var;
                    }
                    break;
                }
                case constrained_based_networks::ConfigurationModel::BOOL: {
                    try
                    {
                        bool_config[graph->getVertexId(vertex)].at(cfg.name);
                    }
                    catch (std::out_of_range e)
                    {
                        // Ok, propetry it is not yet created, let's create one
                        auto var = Gecode::BoolVar(*this, 0, 1);
                        bool_config[graph->getVertexId(vertex)][cfg.name] = var;
                    }
                    break;
                }
                case constrained_based_networks::ConfigurationModel::STRING: {
                    std::cout << "String hanlding not yet implemented in " << __FILE__ << " +" << __LINE__ << std::endl;
//                    throw std::runtime_error("Not yet implemented");
                    break;
                }
            }
        }
    }
};

}  // end namespace constrained_based_networks

#endif  // DECODE_INSTANCE_SOLUTION_H
