#ifndef DECODE_INSTANCE_SOLUTION_H
#define DECODE_INSTANCE_SOLUTION_H

#include <gecode/set.hh>
#include <gecode/int.hh>
#include <gecode/float.hh>
#include <gecode/search.hh>
#include "Pool.hpp"
#include <graph_analysis/GraphAnalysis.hpp>
#include <graph_analysis/DirectedGraphInterface.hpp>
#include "SpecializedComponent.hpp"

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

    /*Empty constructor should not be used, but needed for GraphClass */
    ConfiguredComponent()
    {
        component = 0;
        underlaying_name = "UNDEF";
    }

    ConfiguredComponent(const ConfiguredComponent* c)
    {
        int_config = c->int_config;
        double_config = c->double_config;
        bool_config = c->bool_config;
        string_config = c->string_config;
        underlaying_name = c->underlaying_name;
    }

    virtual Vertex* getClone() const
    {
        auto cc = new ConfiguredComponent(this);
        return cc;
    }

    std::string serializeName()
    {
        return underlaying_name;
    }
    void deserializeName(std::string n)
    {
        underlaying_name = n;
    }

    std::string serializeConfig()
    {

        std::stringstream str;
        //        std::cout << "Serialize int_config with size: " << int_config.size() << " for " << underlaying_name << std::endl;
        str << int_config.size() << " ";
        for (auto j : int_config) {
            str << j.name << " " << j.min << " " << j.max << " ";
        }
        str << double_config.size() << " ";
        for (auto j : double_config) {
            str << j.name << " " << j.min << " " << j.max << " ";
            assert(j.max <= std::numeric_limits<double>::max());
            // assert(j.max != std::numeric_limits<double>::max());
        }
        str << bool_config.size() << " ";
        for (auto j : bool_config) {
            str << j.name << " " << j.min << " " << j.max << " ";
        }
        str << string_config.size() << " ";
        for (auto j : string_config) {
            str << j.first << " " << j.second.size() << " " << j.second << " ";
        }
        return str.str();
    }

    std::string printConfig()
    {
        std::stringstream str;
        std::set<std::string> names;

        for (auto j : int_config) {
            if (names.find(j.name) == names.end()) {
                names.insert(j.name);
                if (j.min != j.max) {
                    if (j.min == Gecode::Int::Limits::min && j.max == Gecode::Int::Limits::max) {
                        str << j.name << ": <undef>" << std::endl;
                    } else {
                        str << j.name << ": " << j.min << "..." << j.max << " " << std::endl;
                    }
                } else {
                    str << j.name << ": " << j.min << std::endl;
                }
            }
        }
        for (auto j : double_config) {
            if (names.find(j.name) == names.end()) {
                names.insert(j.name);
                if (j.min != j.max) {
                    if ((j.min == Gecode::Float::Limits::min || j.min == std::numeric_limits<double>::min()) && (j.max == Gecode::Float::Limits::max || j.max == std::numeric_limits<double>::max())) {
                        str << j.name << ": <undef>" << std::endl;
                    } else {
                        str << j.name << ": " << j.min << "..." << j.max << " " << std::endl;
                    }
                } else {
                    str << j.name << ": " << j.min << std::endl;
                }
            }
        }
        for (auto j : bool_config) {
            if (names.find(j.name) == names.end()) {
                names.insert(j.name);
                if (j.min != j.max) {
                    if (j.min == 0 && j.max == 1) {
                        str << j.name << ": "
                            << ": <undef>" << std::endl;
                    } else {
                        str << j.name << ": " << j.min << "..." << j.max << " " << std::endl;
                    }
                } else {
                    str << j.name << ": " << j.min << std::endl;
                }
            }
        }
        for (auto j : string_config) {
            if (names.find(j.first) == names.end()) {
                names.insert(j.first);
                str << j.first << ": " << j.second << " " << std::endl;
            }
        }
        return str.str();
    }

    void deSerializeConfig(std::string input)
    {
        std::stringstream ifs(input);
        int min, max;
        size_t count;
        std::string name;
        ifs >> count;
        for (size_t i = 0; i < count; ++i) {
            ifs >> name;
            ifs >> min;
            ifs >> max;
            int_config.push_back({min, max, name});
        }

        double dmin = 0, dmax = 0;
        name = "";

        ifs >> count;

        for (size_t i = 0; i < count; ++i) {
            name = "";
            dmin = 0;
            dmax = 0;
            ifs >> name;
            assert(ifs.good());
            std::string sMin;
            ifs >> sMin;
            if (sMin == "-1.79769e+308") {  // workaround import problem
                dmin = Gecode::Float::Limits::min;
            } else {
                dmin = atof(sMin.c_str());
            }

            assert(ifs.good());
            std::string sMax;
            ifs >> sMax;

            if (sMax == "1.7977e+308") {  // workaround import problem
                dmax = Gecode::Float::Limits::max;
            } else {
                dmax = atof(sMax.c_str());
            }

            assert(!ifs.eof());
            assert(!ifs.fail());
            assert(!ifs.bad());
            assert(ifs.good());
            double_config.push_back({dmin, dmax, name});
        }

        bool bMin, bMax;
        ifs >> count;
        for (size_t i = 0; i < count; ++i) {
            ifs >> name;
            ifs >> bMin;
            ifs >> bMax;
            bool_config.push_back({bMin, bMax, name});
        }
        size_t s_count;
        ifs >> count;
        for (size_t i = 0; i < count; ++i) {
            ifs >> name;
            ifs >> s_count;
            char res[s_count];
            for (size_t j = 0; j < s_count; ++j) {
                ifs >> res[j];
            }
            string_config.push_back({name, res});
        }
    }

    ConfiguredComponent(Component underlaying_component) : underlaying_name(underlaying_component->getName())
    {
        component = underlaying_component;
        auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(underlaying_component);
        if (spec.get()) {
            (void)spec;
            throw std::runtime_error("Cannot create configuredComponent from a specialized component yet - Not implemented");
        }
        auto sm = std::dynamic_pointer_cast<StateMachineObj>(underlaying_component);
        if (sm.get()) {
            int_config.push_back(Config<int>{(int)sm->getCurrentTransition(), (int)sm->getCurrentTransition(), "current_state"});
        }
        /*
                // std::cout << "Debug for " << underlaying_component->getName() << std::endl;
                // string_name << component->toString() << std::endl;
                for (auto j : i) {
                    // std::cout << "\t-" << j.first << j.second << std::endl;
                    int_config.push_back(Config<int>{j.second.min(), j.second.max(), j.first});
                }
                for (auto j : f) {
                    double_config.push_back(Config<double>{j.second.min(), j.second.max(), j.first});
                }
                for (auto j : b) {
                    bool_config.push_back(Config<bool>{(bool)j.second.min(), (bool)j.second.max(), j.first});
                }
                for (auto e : s) {
                    std::string config_value = "ERR: N/A";
                    // TODO check unassigned values
                    if (e.second.assigned()) {
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
                */
    }

    ConfiguredComponent(Component underlaying_component, std::map<std::string, Gecode::FloatVar> f, std::map<std::string, Gecode::BoolVar> b, std::map<std::string, Gecode::IntVar> i,
                        std::map<std::string, Gecode::IntVar> s, std::shared_ptr<std::map<std::string, int>> sh)
        : underlaying_name(underlaying_component->getName())
    {
        component = underlaying_component;
        // std::cout << "Debug for " << underlaying_component->getName() << std::endl;
        // string_name << component->toString() << std::endl;
        for (auto j : i) {
            // std::cout << "\t-" << j.first << j.second << std::endl;
            int_config.push_back(Config<int>{j.second.min(), j.second.max(), j.first});
        }
        for (auto j : f) {
            double_config.push_back(Config<double>{j.second.min(), j.second.max(), j.first});
        }
        for (auto j : b) {
            bool_config.push_back(Config<bool>{(bool)j.second.min(), (bool)j.second.max(), j.first});
        }
        for (auto e : s) {
            std::string config_value = "ERR (FATAL)";
            bool valid = false;
            if (e.second.assigned()) {
                auto id = e.second.val();
                for (auto v : *sh) {
                    if (v.second == id) {
                        config_value = v.first;
                        valid = true;
                        break;
                    }
                }
            } else {
                valid = true;
                config_value = "<unassigned>";
            }
            assert(valid);
            string_config.push_back({e.first, config_value});
        }
    }

    Component component;
    virtual std::string getClassName() const
    {
        return "constrained_based_networks::ConfiguredComponent";
    }

    std::string toString() const
    {
        return underlaying_name;
    }

    std::vector<Config<int>> int_config;
    std::vector<Config<double>> double_config;
    std::vector<std::pair<std::string, std::string>> string_config;
    std::vector<Config<bool>> bool_config;
    std::string underlaying_name;
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
        // std::cout << o->getClassName() << std::endl;
        auto c = new ComponentInstanceHelper(o);
        auto p = c->getPtr();
        assert(p.get());
        return p;
    }
    std::string toString() const
    {
        return component->toString();
    }

    graph_analysis::Vertex::Ptr component;
    graph_analysis::Vertex::Ptr self;
};

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
    void printToStream(std::ostream& os) const;
    void printToDot(std::ostream& os) const;

    static void print(const Space& home, const Gecode::BrancherHandle& bh, unsigned int a, Gecode::IntVar x, int i, const int& n, std::ostream& o);

    template <typename C>
    static std::shared_ptr<C> get(graph_analysis::Vertex::Ptr v)
    {
        if(0){
            //Compile time check, make sure we request only Components
            ((C*)0)->getName();
        }
        auto component = std::dynamic_pointer_cast<C>(v);
        if (auto c = std::dynamic_pointer_cast<ConfiguredComponent>(v)) component = std::dynamic_pointer_cast<C>(c->component);
        return component;
    }

    static void print(const Space& home, const Gecode::BrancherHandle& bh, unsigned int a, Gecode::BoolVar x, int i, const int& n, std::ostream& o);
    static graph_analysis::Vertex::Ptr getRoot(const graph_analysis::BaseGraph::Ptr& graph);

   protected:
    graph_analysis::Vertex::Ptr getConfiguredComponent(graph_analysis::Vertex::Ptr vertex);
    void build_tree(graph_analysis::BaseGraph::Ptr erg, graph_analysis::Vertex::Ptr parent);
    size_t verticies_in_tree;
    graph_analysis::DirectedGraphInterface::Ptr graph;

    void gatherAllStringConfigs();
    void buildInstanceGraph(graph_analysis::Vertex::Ptr orig_parent, graph_analysis::DirectedGraphInterface& orig, graph_analysis::Vertex::Ptr parent);

    // TODO cleanup this should not be part of this class
    std::map<unsigned int, graph_analysis::Vertex::Ptr> configured_component_helper;

    std::vector<std::map<std::string, Gecode::FloatVar>> float_config;
    std::vector<std::map<std::string, Gecode::BoolVar>> bool_config;
    std::vector<std::map<std::string, Gecode::IntVar>> int_config;
    std::vector<std::map<std::string, Gecode::IntVar>> string_config;
    Gecode::BoolVarArray interleaved;

    std::shared_ptr<std::map<std::string, int>> string_helper;

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
