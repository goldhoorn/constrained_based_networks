#include "InstanceSolution.hpp"

#include <gecode/gist.hh>
#include <gecode/minimodel.hh>

#include <memory>
#include <vector>
#include <map>
#include <stdexcept>
#include "Pool.hpp"
#include "Task.hpp"
#include "Composition.hpp"
#include "DataService.hpp"
#include "SpecializedComponent.hpp"
#include <stdlib.h>
#include <fstream>
#include <graph_analysis/GraphIO.hpp>
#include <gecode/gist.hh>

using namespace Gecode;

namespace constrained_based_networks
{

void InstanceSolution::gatherAllStringConfigs(){
    std::set<std::string> strings;
    strings.insert("");
    for(const auto &v : graph->getAllVertices()){
        auto helper_node = boost::static_pointer_cast<ComponentInstanceHelper>(v);
        auto component = dynamic_cast<Component*>(helper_node->component.get());
        if (!component) throw std::runtime_error("Cannot get component from graph");
        auto spec_component = dynamic_cast<SpecializedComponentBase*>(component);

        for(const auto &prop : component->getProperties()){
            if(prop.t == ConfigurationModel::STRING){

                //Save all possible options from the config
                for(const auto& section : component->getSections()){
                    try{
                    strings.insert(component->getConfFileProperty(section, prop.name));
                    }catch(std::out_of_range e){
                        //Do nothign because it is notmal that not all propertys exist in all sections
                    }
                }

                //If this is a specialized component it might have configuration request on it's own
                if(spec_component){
                    auto it = spec_component->configuration.find(prop.name);

                    //If this specalized component has a requirement for a string property
                    //Otherwise it does not
                    if( it != spec_component->configuration.end() ){
                        strings.insert(it->second);
                    }
                }
            }
        }
    }
    string_helper = std::shared_ptr< std::map<std::string, unsigned int> >(new std::map<std::string, unsigned int> );
    unsigned int cnt=0;
    for(auto s : strings){
        string_helper->insert( {s,cnt++} );
    }
}


void InstanceSolution::buildInstanceGraph(graph_analysis::Vertex::Ptr parent_orig, graph_analysis::DirectedGraphInterface &orig, graph_analysis::Vertex::Ptr parent){
    //Only for testing
    if(!orig.getOutEdgeIterator(parent_orig)->next()){
        return;
    }
    static int i=0;
    ++i;
    std::cout << "------------ " << i << " OUT EDGES of " << parent_orig->toString() << "-----------" << std::endl;
    for(auto v : orig.outEdges(parent_orig)){
        auto target = ComponentInstanceHelper::make(v->getTargetVertex());
        graph_analysis::Edge::Ptr e(new graph_analysis::Edge());
        e->setSourceVertex(parent);
        e->setTargetVertex(target);
        graph->addEdge(e);
        std::cout << i << " Jeha from " << parent_orig->toString() << " -> " << v->getTargetVertex()->toString() << std::endl;
        buildInstanceGraph(v->getTargetVertex(),orig,target);
    }

    std::cout << "------------ " << i << " OUT EDGES END -------------" << std::endl;
    --i;
}

void InstanceSolution::buildStructure(graph_analysis::Vertex::Ptr parent){
//    for(auto v : orig.outEdges(parent)){
//    }
}

InstanceSolution::InstanceSolution(graph_analysis::BaseGraph::Ptr _graph)// : graph(_graph)
{
    graph_analysis::Vertex::Ptr root;
    std::vector<graph_analysis::BaseGraph::Ptr> erg;
    for(auto v:_graph->getAllVertices()){
        if(v->getLabel() == "root-knot"){
            root = v;
            break;
        }
    }
    if(!root){
        throw std::runtime_error("Could not get the root knot of the graph");
    }

    graph = boost::static_pointer_cast<graph_analysis::DirectedGraphInterface>(graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH));
    auto orig = dynamic_cast<graph_analysis::DirectedGraphInterface*>(_graph.get());
    assert(orig);

    buildInstanceGraph(root,*orig, ComponentInstanceHelper::make(root));

    gatherAllStringConfigs();
    for(auto c : *string_helper){
        std::cout <<c.first  << std::endl;
    }

    // Setup the configuration arrays
    float_config.resize(graph->getAllVertices().size());
    bool_config.resize(graph->getAllVertices().size());
    int_config.resize(graph->getAllVertices().size());
    string_config.resize(graph->getAllVertices().size());

    // Setup all propagations and actual requirements for all configs we have
    // In case we have a specialized component, this is the only case
    // (currently) where a configuiration is actually filled
    // with values. Later a extension based on configurations is needed
    for (auto helper_node : graph->getAllVertices()) {
        auto helper_node2 = boost::static_pointer_cast<ComponentInstanceHelper>(helper_node);
        auto component = dynamic_cast<Component*>(helper_node2->component.get());
        if (!component) throw std::runtime_error("Cannot get component from graph");

        // We need only to separate between this types, al other types should not occur anymore in the graph
        // Like DataServices are replaced with actual task-contexts
        auto cmp = dynamic_cast<Composition*>(component);
        auto task = dynamic_cast<Task*>(component);
        auto spec_cmp = dynamic_cast<SpecializedComponentBase*>(component);
        if (!task && !cmp) throw std::runtime_error("We have some unresolved or unknown elements within the graph");

        if (cmp) {
            setupProperties(cmp, helper_node2, graph);

            // So we need to propagate all entries here
            auto edges = graph->getEdgeIterator(helper_node2);
            while (edges->next()) {

                // If our task is a child and we are NOT a specialized component
                // (which would assume that we are not support this?, maybe we we would set a specific configuration then?!
                auto task = dynamic_cast<Task*>(edges->current()->getTargetVertex().get());
                auto specialized_task = dynamic_cast<SpecializedComponentBase*>(edges->current()->getTargetVertex().get());

                if (task && specialized_task) {
                    throw std::runtime_error("We got a specialized task as child, this is not (Yet) supported");
                }

                if (task) {
                    // Okay we start to use a default config for not
                    // TODO this must be extended later to load instead of the default-configs somthing which
                    // can be set by somehting like a with_conf call within the compositions

                    // Make sure the properties exist
                    setupProperties(task, edges->current()->getTargetVertex(), graph);


                    for (auto prop : task->getProperties()) {
                        switch (prop.t) {
                            case(constrained_based_networks::ConfigurationModel::INT) : {
                                try
                                {
                                    rel(*this, int_config[graph->getVertexId(edges->current()->getTargetVertex())][prop.name], IRT_EQ, atoi(task->getConfFileProperty("default", prop.name).c_str()));
                                }
                                catch (std::out_of_range e)
                                {
                                    int val =0;
                                    if(spec_cmp){
                                        auto it = spec_cmp->configuration.find(prop.name);
                                        if( it != spec_cmp->configuration.end() ){
                                            val = atoi(it->second.c_str());
                                        }else{
                                            std::cerr << "Cannot get configuration " << prop.name << " for Task " << task->getName() << "Assuming 0" << std::endl;
                                        }
                                    }else{
                                        std::cerr << "Cannot get configuration " << prop.name << " for Task " << task->getName() << "Assuming 0" << std::endl;
                                    }
                                    rel(*this, int_config[graph->getVertexId(edges->current()->getTargetVertex())][prop.name], IRT_EQ, val);
                                }
                                break;
                            }
                            case(constrained_based_networks::ConfigurationModel::STRING) : {
                                try
                                {
                                    //This can fail
                                    std::string str = task->getConfFileProperty("default", prop.name);
                                    unsigned int id = 0;
                                    if(string_helper->find(str) == string_helper->end()){
                                        throw std::runtime_error("String helper does not contain the needed string this should not happen");
                                    }else{
                                        id = string_helper->at(str);
                                    }
                                    rel(*this, string_config[graph->getVertexId(edges->current()->getTargetVertex())][prop.name], IRT_EQ, id);
                                }
                                catch (std::out_of_range e)
                                {
                                    int id =0;
                                    if(spec_cmp){
                                        auto it = spec_cmp->configuration.find(prop.name);
                                        if( it != spec_cmp->configuration.end() ){
                                            std::string str;
                                            if(string_helper->find(str) == string_helper->end()){
                                                throw std::runtime_error("String helper does not contain the needed string this should not happen");
                                            }else{
                                                id = string_helper->at(str);
                                            }
                                        }else{
                                            std::cerr << "Cannot get configuration " << prop.name << " for Task " << task->getName() << "Assuming empty string" << std::endl;
                                        }
                                    }else{
                                        std::cerr << "Cannot get configuration " << prop.name << " for Task " << task->getName() << "Assuming empty string" << std::endl;
                                    }
                                    rel(*this, string_config[graph->getVertexId(edges->current()->getTargetVertex())][prop.name], IRT_EQ, id);
                                }
                                break;
                            }
                            case(constrained_based_networks::ConfigurationModel::DOUBLE) : {
                                try
                                {
                                    const float val = atof(task->getConfFileProperty("default", prop.name).c_str());
                                    rel(*this, float_config[graph->getVertexId(edges->current()->getTargetVertex())][prop.name], FRT_EQ, val);
                                }
                                catch (std::out_of_range e)
                                {
                                    double val =0;
                                    if(spec_cmp){
                                        auto it = spec_cmp->configuration.find(prop.name);
                                        if( it != spec_cmp->configuration.end() ){
                                            val = atof(it->second.c_str());
                                        }else{
                                            std::cerr << "Cannot get configuration " << prop.name << " for Task " << task->getName() << "Assuming 0.0" << std::endl;
                                        }
                                    }else{
                                        std::cerr << "Cannot get configuration " << prop.name << " for Task " << task->getName() << "Assuming 0.0" << std::endl;
                                    }
                                    rel(*this, float_config[graph->getVertexId(edges->current()->getTargetVertex())][prop.name], FRT_EQ, val);
                                }
                                break;
                            }
                            case(constrained_based_networks::ConfigurationModel::BOOL) : {
                                try
                                {

                                    rel(*this, bool_config[graph->getVertexId(edges->current()->getTargetVertex())][prop.name], IRT_EQ, atoi(task->getConfFileProperty("default", prop.name).c_str()));
                                }
                                catch (std::out_of_range e)
                                {
                                    bool val = false;
                                    if(spec_cmp){
                                        auto it = spec_cmp->configuration.find(prop.name);
                                        if( it != spec_cmp->configuration.end() ){
                                            val = atoi(it->second.c_str());
                                        }else{
                                            std::cerr << "Cannot get configuration " << prop.name << " for Task " << task->getName() << "Assuming false" << std::endl;
                                        }
                                    }else{
                                        std::cerr << "Cannot get configuration " << prop.name << " for Task " << task->getName() << "Assuming false" << std::endl;
                                    }
                                    rel(*this, bool_config[graph->getVertexId(edges->current()->getTargetVertex())][prop.name], IRT_EQ, val);
                                }
                                break;
                            }
                        };
                    }
                }

                auto child = dynamic_cast<Component*>(edges->current()->getTargetVertex().get());
                for (auto forward : cmp->getArgumentForwards(child)) {

                    if (child->getProperty(forward.second) != cmp->getProperty(forward.first)) {
                        throw std::runtime_error("The properties of child and parend differ in type");
                    }
                    // Dont care which type we choose check is done before
                    switch (child->getProperty(forward.second)) {
                        case(constrained_based_networks::ConfigurationModel::INT) : {
                            auto var = Gecode::IntVar(*this, 0, Gecode::Int::Limits::max);
                            int_config[graph->getVertexId(edges->current()->getTargetVertex())][forward.second] = var;
                            auto var_source = int_config[graph->getVertexId(helper_node2)][forward.first];
                            rel(*this, var, IRT_EQ, var_source);
                            break;
                        }
                        case(constrained_based_networks::ConfigurationModel::STRING) : {
                            auto var = Gecode::IntVar(*this, 0, Gecode::Int::Limits::max);
                            string_config[graph->getVertexId(edges->current()->getTargetVertex())][forward.second] = var;
                            auto var_source = string_config[graph->getVertexId(helper_node2)][forward.first];
                            rel(*this, var, IRT_EQ, var_source);
                            break;
                        }
                        case(constrained_based_networks::ConfigurationModel::DOUBLE) : {
                            auto var = Gecode::FloatVar(*this, 0, Gecode::Float::Limits::max);
                            float_config[graph->getVertexId(edges->current()->getTargetVertex())][forward.second] = var;
                            auto var_source = float_config[graph->getVertexId(helper_node2)][forward.first];
                            rel(*this, var, FRT_EQ, var_source);
                            break;
                        }
                        case(constrained_based_networks::ConfigurationModel::BOOL) : {
                            auto var = Gecode::BoolVar(*this, 0, 1);
                            bool_config[graph->getVertexId(edges->current()->getTargetVertex())][forward.second] = var;
                            auto var_source = bool_config[graph->getVertexId(helper_node2)][forward.first];
                            rel(*this, var, IRT_EQ, var_source);
                            break;
                        }
                    };
                }
            }
        }
        if (task) {
            setupProperties(task, helper_node2, graph);
        }
    }

    // Setup the branching for ALL task-context here, this could
    // be done within the previous loop to save effienfy, but to have the code
    // more strutured do this explicitly and separeated here
    for (auto node : graph->getAllVertices()) {
        if (dynamic_cast<Task*>(node.get())) {
            auto id = graph->getVertexId(node);
            for (auto c : bool_config[id]) {
                branch(*this, c.second, INT_VAL_MIN());
            }
            for (auto c : float_config[id]) {
                branch(*this, c.second, FLOAT_VAL_SPLIT_MIN());
            }
            for (auto c : int_config[id]) {
                branch(*this, c.second, INT_VAL_MIN());
            }
            for (auto c : string_config[id]) {
                branch(*this, c.second, INT_VAL_MIN());
            }
        }
    }
}

InstanceSolution::InstanceSolution(bool share, InstanceSolution& s) : Space(share, s)
{
    graph = s.graph;
    string_helper = s.string_helper;

    float_config.resize(s.float_config.size());
    for (size_t i = 0; i < s.float_config.size(); i++) {
        for (auto c : s.float_config[i]) {
            float_config[i][c.first] = FloatVar();
            float_config[i][c.first].update(*this, share, s.float_config[i][c.first]);
        }
    }

    bool_config.resize(s.bool_config.size());
    for (size_t i = 0; i < s.bool_config.size(); i++) {
        for (auto c : s.bool_config[i]) {
            bool_config[i][c.first] = BoolVar();
            bool_config[i][c.first].update(*this, share, s.bool_config[i][c.first]);
        }
    }

    int_config.resize(s.int_config.size());
    for (size_t i = 0; i < s.int_config.size(); i++) {
        for (auto c : s.int_config[i]) {
            int_config[i][c.first] = IntVar();
            int_config[i][c.first].update(*this, share, s.int_config[i][c.first]);
        }
    }
    string_config.resize(s.string_config.size());
    for (size_t i = 0; i < s.string_config.size(); i++) {
        for (auto c : s.string_config[i]) {
            string_config[i][c.first] = IntVar();
            string_config[i][c.first].update(*this, share, s.string_config[i][c.first]);
        }
    }
}

Space* InstanceSolution::copy(bool share)
{
    return new InstanceSolution(share, *this);
}

void InstanceSolution::compare(const Space& _s, std::ostream& os) const
{
#if 0
    auto s = static_cast<const InstanceSolution&>(_s);
    char buff[512];
    sprintf(buff,"/tmp/dep-%i.dot", print_count);
    std::ofstream file(buff);
    file << "digraph G {" << std::endl;
    size_t cmp_id=0;
    for(auto composition : pool->getItems<Composition*>()){
        size_t child_id=0;
        for(auto child : composition->getChildren()){
            if(ir_assignments[cmp_id][child_id].assigned() && s.ir_assignments[cmp_id][child_id].assigned()){
                if(s.ir_assignments[cmp_id][child_id].val() > ID_START){
                    if(ir_assignments[cmp_id][child_id].val() == s.ir_assignments[cmp_id][child_id].val()){
                        file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[label=\"" << child.first << "\"];" << std::endl;
                    }else{
                        file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
                        file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[s.ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=red,label=\"" << child.first << "\"];" << std::endl;
                    }
                }
            }else if(ir_assignments[cmp_id][child_id].assigned()){
                if(ir_assignments[cmp_id][child_id].val() > ID_START){
                file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
                file << "\t\"" << composition->getName() << "\" -> \"" << "N/A (unresolved)" << "\"[color=red,label=\"" << child.first << "\"];" << std::endl;
                }
//                file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
//                file << "\t\"" << composition->getName() << "\" -> \"" << "N/A (unresolved)" << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
//                 if(ir_assignments[cmp_id][child_id].val() > ID_START){ //Only print assigned solutions
//                    if(!s.ir_assignments[cmp_id][child_id].assigned()){
 //                       file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
//                        file << "\t\"" << composition->getName() << "\" -> \"" << "N/A (unresolved)" << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
  //                  }else{
//                    }
//                 }
            }else if(s.ir_assignments[cmp_id][child_id].assigned()){
                if(s.ir_assignments[cmp_id][child_id].val() > ID_START){
                    file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[s.ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=red,label=\"" << child.first << "\"];" << std::endl;
                    file << "\t\"" << composition->getName() << "\" -> \"" << "N/A (unresolved)" << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
                }
            }else{
                std::cerr << "somethign is really strange here" << std::endl;
            }
            child_id++;
        }
        cmp_id++;
    }


    file << "}" << std::endl;
    sprintf(buff,"dot -Tsvg /tmp/dep-%i.dot -o /tmp/dep-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Child Selection: </h1><br/><img src=\"/tmp/dep-" << print_count << ".svg\"\\>" << std::endl;

    const_cast<InstanceSolution*>(this)->print_count++;
#endif
}

void InstanceSolution::printToDot(std::ostream& os) const
{
    for (auto node : graph->getAllVertices()) {
        auto component = dynamic_cast<Component*>(node.get());
        if (auto composition = dynamic_cast<Composition*>(component)) {
            os << "Cmp::" << composition->getName() << std::endl;
            for (auto c : float_config[graph->getVertexId(node)]) {
                std::cout << c.first.c_str() << std::endl;
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : bool_config[graph->getVertexId(node)]) {
                std::cout << c.first.c_str() << std::endl;
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : int_config[graph->getVertexId(node)]) {
                std::cout << c.first.c_str() << std::endl;
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : string_config[graph->getVertexId(node)]) {
                std::cout << c.first.c_str() << std::endl;
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
        }else if (auto task = dynamic_cast<Task*>(component)) {
            os << task->getName() << std::endl;
            for (auto c : float_config[graph->getVertexId(node)]) {
                std::cout << c.first.c_str() << std::endl;
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : bool_config[graph->getVertexId(node)]) {
                std::cout << c.first.c_str() << std::endl;
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : int_config[graph->getVertexId(node)]) {
                std::cout << c.first.c_str() << std::endl;
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : string_config[graph->getVertexId(node)]) {
                std::cout << c.first.c_str() << std::endl;
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
        }else{
            std::runtime_error("HAve somehting unknown in graphe here");
        }
    }
#if 0
    Pool *pool = Pool::getInstance();
    /*
    os << "Count: " << active.size() << std::endl;

    os << "Running State: { " << std::endl;
    for(int i = full?0:ID_START; i < active.size(); i++)
    {
        //os << "\t" << (*Pool::getInstance())[i]->getName() << "=";
        if(active[i].assigned())
        {
            if(active[i].val() || full){
                os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << std::endl;
            }
        }
        else
        {
            os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << std::endl;
        }
//        os << ", "<< std::endl;
    }
    os << "}" << std::endl;


    os << "Child Selection: { " << std::endl;
    */

    //TODO ugly
    char buff[512];
    sprintf(buff,"/tmp/dep-%i.dot", print_count);
    std::ofstream file(buff);
    file << "digraph G {" << std::endl;
    size_t cmp_id=0;
    for(auto composition : pool->getItems<Composition*>()){
        size_t child_id=0;
        for(auto child : composition->getChildren()){
            if(ir_assignments[cmp_id][child_id].assigned()){
                 if(ir_assignments[cmp_id][child_id].val() > ID_START){ //Only print assigned solutions
                    file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[label=\"" << child.first << "\"];" << std::endl;
                    //os <<  " (" << ir_assignments[cmp_id][child_id] << ")" << std::endl;
                 }
            }else{
                for (IntVarValues j(ir_assignments[cmp_id][child_id]); j(); ++j){
                    //file << "\t\"" << composition->getName() << "\" -> \"!" << ir_assignments[cmp_id][child_id] << "\"[color=red];" << std::endl;
                    //file << "\t\"!" << ir_assignments[cmp_id][child_id] << "\"[color=red];" << std::endl;
                    file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[j.val()]->getName() << "\"[color=red,label=\"" << child.first << "\"];" << std::endl;
                    file << "\t\"" << (*pool)[j.val()]->getName() << "\"[color=red];" << std::endl;
                    file << "\t\"" << composition->getName() << "\"[color=red];" << std::endl;
                    if(active[j.val()].assigned()){
                        if(active[j.val()].val()){
                            file << "\t\"" << (*pool)[j.val()]->getName() << "\"[style=filled,fillcolor=green];" << std::endl;
                        }else{
                            file << "\t\"" << (*pool)[j.val()]->getName() << "\"[style=filled,fillcolor=red];" << std::endl;
                        }
                    }else{
                        file << "\t\"" << (*pool)[j.val()]->getName() << "\"[style=filled,fillcolor=yellow];" << std::endl;
                    }
                }
//                os << "\t" << composition->getName() << "." << child.first << "=" << ir_assignments[cmp_id][child_id] << std::endl;
            }
            child_id++;
        }
        cmp_id++;
    }
    file << "}" << std::endl;
    sprintf(buff,"dot -Tsvg /tmp/dep-%i.dot -o /tmp/dep-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Child Selection: </h1><br/><img src=\"/tmp/dep-" << print_count << ".svg\"\\>" << std::endl;

#if 1
    sprintf(buff,"/tmp/dep-graph-%i.dot", print_count);
    std::ofstream file2(buff);
    file2 << "digraph G {" << std::endl;
    for(size_t i = 1; i< depends.size();i++){
        auto o = (*pool)[i];

        for (SetVarGlbValues j(depends[i]); j(); ++j){
            //if(j.val() < ID_START && !full) continue;
            file2 <<  "\t\"" << o->getName() << "\" -> \""  <<  (*pool)[j.val()]->getName() << "\";" << std::endl;
        }
        for (SetVarUnknownValues j(depends[i]); j(); ++j){
            file2 <<  "\t\"" << o->getName() << "\" -> \""  <<  (*pool)[j.val()]->getName() << "\"[color=yellow];" << std::endl;
        }
    }
    file2 << "}" << std::endl;
    file2.close();
    sprintf(buff,"dot -Tsvg /tmp/dep-graph-%i.dot -o /tmp/dep-graph-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Dependancy Selection: </h1><br/><img src=\"/tmp/dep-graph-" << print_count << ".svg\"\\>" << std::endl;
#endif
#if 1
    sprintf(buff,"/tmp/dep-rgraph-%i.dot", print_count);
    std::ofstream file3(buff);
    file3 << "digraph G {" << std::endl;
    for(size_t i = 1; i< depends_recursive.size();i++){
        auto o = (*pool)[i];
        for (SetVarGlbValues j(depends_recursive[i]); j(); ++j){
            //if(j.val() < ID_START && !full) continue;
            file3 <<  "\t\"" << o->getName() << "\" -> \""  <<  (*pool)[j.val()]->getName() << "\";" << std::endl;
        }
        for (SetVarUnknownValues j(depends_recursive[i]); j(); ++j){
            file3 <<  "\t\"" << o->getName() << "\" -> \""  <<  (*pool)[j.val()]->getName() << "\"[color=yellow];" << std::endl;
        }
    }
    file3 << "}" << std::endl;
    file3.close();
    sprintf(buff,"dot -Tsvg /tmp/dep-rgraph-%i.dot -o /tmp/dep-rgraph-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Recursive dependancy Selection: </h1><br/><img src=\"/tmp/dep-rgraph-" << print_count << ".svg\"\\>" << std::endl;
#endif


    print_count++;

#endif
}

void InstanceSolution::printToStream(std::ostream& os, bool full) const
{

    for (auto node : graph->getAllVertices()) {
        auto component = dynamic_cast<Component*>(node.get());
        if (auto task = dynamic_cast<Task*>(component)) {
            os << task->getName() << std::endl;
            for (auto c : int_config[graph->getVertexId(node)]) {
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : string_config[graph->getVertexId(node)]) {
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : bool_config[graph->getVertexId(node)]) {
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
            for (auto c : float_config[graph->getVertexId(node)]) {
                os << "-- " << c.first << ": " << c.second << std::endl;
            }
        }
    }
    // active[i].assigned()
    // active[i].val()
}

InstanceSolution* InstanceSolution::babSearch(graph_analysis::BaseGraph::Ptr graph)
{
    // Initial situation
    InstanceSolution* so = new InstanceSolution(graph);
    // BAB search engine
    // BAB<InstanceSolution> e(so);
    BAB<InstanceSolution> e(so);
    // DFS<InstanceSolution> e(so);
    // search
    InstanceSolution* best = NULL;
    unsigned int cnt=0;
    while (InstanceSolution* s = e.next()) {
        if (best != NULL) {
            delete best;
            best = 0;
        }
        ++cnt;
        // Save current solution as best
        // s->rprint();
        // std::cout <<
        // "------------------------------------------------------------------------------------------"
        // << std::endl;
        // std::cout <<
        // "------------------------------------------------------------------------------------------"
        // << std::endl;
        best = s;
    }

    // throw exception if there is no solution
    if (best == NULL) {
        delete so;
        throw std::runtime_error("InstanceSolution babSearch: No solutions");
    }
    delete so;
    std::cout << "#####################################################" << std::endl;
    std::cout << "Found " << cnt << " instance solutions" << std::endl;
    std::cout << "#####################################################" << std::endl;
    graph_analysis::io::GraphIO::write("instance-erg.dot", best->graph, graph_analysis::representation::GRAPHVIZ);
    return best;
}

InstanceSolution* InstanceSolution::gistBaBSeach(graph_analysis::BaseGraph::Ptr graph)
{
    InstanceSolution* m = new InstanceSolution(graph);
    Gist::Print<InstanceSolution> printer("Print solution");
    Gist::VarComparator<InstanceSolution> c("Compare nodes");
    Gist::Options o;
    o.c_d = 2;
    o.a_d = 10;
    o.threads = 4;
    o.inspect.click(&printer);
    o.inspect.compare(&c);
    Gist::bab(m, o);
    return m;
}

}  // end namespace constrained_based_networks
