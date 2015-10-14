#ifndef DECODE_INSTANCE_SOLUTION_H
#define DECODE_INSTANCE_SOLUTION_H

#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/float.hh>
#include <gecode/search.hh>
#include "Pool.hpp"
#include <graph_analysis/GraphAnalysis.hpp>

namespace constrained_based_networks
{

class ConfiguredComponent : public graph_analysis::Vertex
{
   public:

    template <typename T>
    struct Config
    {
        T min;
        T max;
        std::string name;
    };

    ConfiguredComponent(Component* underlaying_component, std::map<std::string, Gecode::FloatVar> f, std::map<std::string, Gecode::BoolVar> b, std::map<std::string, Gecode::IntVar> i,
                        std::map<std::string, Gecode::IntVar> s, std::shared_ptr<std::map<std::string, unsigned int>> sh)
        : component(underlaying_component)
    {
        //string_name << component->toString() << std::endl;
        for (auto j : i) {
            int_config.push_back(Config<int>{j.second.min(), j.second.max(), j.first});
        }
        for (auto j : f) {
            double_config.push_back(Config<double>{j.second.min(), j.second.max(), j.first});
        }
        for (auto j : b) {
            bool_config.push_back(Config<bool>{(bool)j.second.min(),(bool)j.second.max(), j.first});
        }
        for (auto e : s) {
            std::string config_value = "ERR: N/A";
            //TODO check unassigned values
            if(e.second.assigned()){
                auto id = e.second.val();
                for (auto v : *sh) {
                    if (v.second == id) {
                        config_value = v.first;
                        break;
                    }
                }
            }
            string_config.push_back({e.first, config_value});
        }
    }

    std::string toString() const
    {
        std::stringstream str;
        str << component->toString() << std::endl;
        for(auto i:int_config){
            if(i.min == i.max){
                str << i.name << ": " << i.min << std::endl;
            }else{
                str << i.name << ": " << i.min << "..." << i.max << std::endl;
            }
        }
        for(auto i:bool_config){
            if(i.min == i.max){
                str << i.name << ": " << i.min << std::endl;
            }else{
                str << i.name << ": " << i.min << "..." << i.max << std::endl;
            }
        }
        for(auto i:double_config){
            if(i.min == i.max){
                str << i.name << ": " << i.min << std::endl;
            }else{
                str << i.name << ": " << i.min << "..." << i.max << std::endl;
            }
        }
        for(auto i:string_config){
                str << i.first<< ": " << i.second << std::endl;
        }
        return str.str();
    }

    std::vector<Config<int>> int_config;
    std::vector<Config<double>> double_config;
    std::vector<std::pair<std::string, std::string>> string_config;
    std::vector<Config<bool>> bool_config;
    Component* component;

};

class ComponentInstanceHelper : public graph_analysis::Vertex
{
   public:
    ComponentInstanceHelper(graph_analysis::Vertex::Ptr underlaying) : component(underlaying)
    {
    }

    graph_analysis::Vertex::Ptr getPtr()
    {
        if (self.get() == nullptr) {
            self = graph_analysis::Vertex::Ptr(this);
        }
        return self;
    }
    static graph_analysis::Vertex::Ptr make(graph_analysis::Vertex::Ptr o)
    {
        auto c = new ComponentInstanceHelper(o);
        return c->getPtr();
    }
    std::string toString() const
    {
        return component->toString();
    }

    graph_analysis::Vertex::Ptr component;
    graph_analysis::Vertex::Ptr self;
};

class Composition;

/**
 * A solution inherits GECODE's space. the initial situation as well as any real
 * solutions are of type InstanceSolution.
 */
class InstanceSolution : public Gecode::Space
{
   public:
    // Default user-constructor
    InstanceSolution(graph_analysis::BaseGraph::Ptr graph);

    // Helper method for copying
    InstanceSolution(bool share, InstanceSolution& s);

    // Some helper-static constructors which also rxecures the search
    static std::vector<graph_analysis::BaseGraph::Ptr> babSearch(graph_analysis::BaseGraph::Ptr graph);
#if 0
    static std::vector<graph_analysis::BaseGraph::Ptr> gistBaBSeach(graph_analysis::BaseGraph::Ptr graph);
#endif

    // helper methods for gist
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
    graph_analysis::Vertex::Ptr getConfiguredComponent(graph_analysis::Vertex::Ptr vertex);
    void build_tree(graph_analysis::BaseGraph::Ptr erg, graph_analysis::Vertex::Ptr parent);
    size_t verticies_in_tree;
    graph_analysis::DirectedGraphInterface::Ptr graph;
    static graph_analysis::Vertex::Ptr getRoot(const graph_analysis::BaseGraph::Ptr& graph);

    void buildStructure(graph_analysis::Vertex::Ptr parent);

    void gatherAllStringConfigs();
    void buildInstanceGraph(graph_analysis::Vertex::Ptr orig_parent, graph_analysis::DirectedGraphInterface& orig, graph_analysis::Vertex::Ptr parent);

    // TODO cleanup this should not be part of this class
    std::map<unsigned int, graph_analysis::Vertex::Ptr> configured_component_helper;

    std::vector<std::map<std::string, Gecode::FloatVar>> float_config;
    std::vector<std::map<std::string, Gecode::BoolVar>> bool_config;
    std::vector<std::map<std::string, Gecode::IntVar>> int_config;
    std::vector<std::map<std::string, Gecode::IntVar>> string_config;
    Gecode::BoolVarArray interleaved;

    std::shared_ptr<std::map<std::string, unsigned int>> string_helper;

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
                        auto var = Gecode::IntVar(*this, Gecode::Int::Limits::min, Gecode::Int::Limits::max);
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
                        auto var = Gecode::FloatVar(*this, Gecode::Float::Limits::min, Gecode::Float::Limits::max);
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
                    try
                    {
                        string_config[graph->getVertexId(vertex)].at(cfg.name);
                    }
                    catch (std::out_of_range e)
                    {
                        // Ok, propetry it is not yet created, let's create one
                        auto var = Gecode::IntVar(*this, 0, string_helper->size());
                        string_config[graph->getVertexId(vertex)][cfg.name] = var;
                    }
                    break;
                }
            }
        }
    }
};

}  // end namespace constrained_based_networks

#endif  // DECODE_INSTANCE_SOLUTION_H
