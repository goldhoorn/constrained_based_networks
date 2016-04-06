
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/BaseGraph.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include "NetworkHelper.hpp"
#include "Pool.hpp"
#include "ClassSolution.hpp"
#include "XML.hpp"
#include "DataService.hpp"
#include <fstream>

using namespace constrained_based_networks;

static graph_analysis::Vertex::Ptr cc;

void NetworkHelper::initializeExporter()
{
    graph_analysis::VertexTypeManager *vManager = graph_analysis::VertexTypeManager::getInstance();
    auto confComp = new ConfiguredComponent();
    cc = graph_analysis::Vertex::Ptr(confComp);
    vManager->registerType(cc);
    vManager->registerAttribute(cc->getClassName(), "max_use", (graph_analysis::VertexTypeManager::serialize_func_t) & ConfiguredComponent::serializeUc,
                                (graph_analysis::VertexTypeManager::deserialize_func_t) & ConfiguredComponent::deserializeUc,
                                (graph_analysis::VertexTypeManager::print_func_t) & ConfiguredComponent::printUc);
    vManager->registerAttribute(cc->getClassName(), "config", (graph_analysis::VertexTypeManager::serialize_func_t) & ConfiguredComponent::serializeConfig,
                                (graph_analysis::VertexTypeManager::deserialize_func_t) & ConfiguredComponent::deSerializeConfig,
                                (graph_analysis::VertexTypeManager::print_func_t) & ConfiguredComponent::printConfig);
    vManager->registerAttribute(cc->getClassName(), "underlaying_name", (graph_analysis::VertexTypeManager::serialize_func_t) & ConfiguredComponent::serializeName,
                                (graph_analysis::VertexTypeManager::deserialize_func_t) & ConfiguredComponent::deserializeName,
                                (graph_analysis::VertexTypeManager::print_func_t) & ConfiguredComponent::serializeName);
}

std::string NetworkHelper::normalizeName(std::string in)
{
    std::replace(in.begin(), in.end(), '_', ' ');
    std::replace(in.begin(), in.end(), ':', ' ');
    return in;
}

bool NetworkHelper::isIgnoredState(std::string name)
{
    std::string ignored[] = {"failed", "running", "stopped", "exception",      "runtime_error", "pre_operational", "init",        "start",
                             "stop",   "success", "aborted", "internal_error", "updated_data",  "poll_transition", "fatal_error", "interrupt"};
    for (auto s : ignored) {
        if (name == s) {
            return true;
        }
    }
    return false;
}

void NetworkHelper::createModelExportForLatex(std::string core_model, std::string filename)
{
    std::ofstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open given file " + filename);
    }
    /*    std::vector<StateMachineObj> C;
        std::vector<CompositionObj> C;
        std::vector<DataServiceObj> D;
        std::map<CompositionObj, ComponentObj> phi;
        std::map<CompositionObj,std::pair<std::string,std::string> > props;
        std::map<ComponentObj, int> max_usages;
    */

    Pool *pool = XML::load(core_model);
    bool only_shortnames = true;
    file << "\\noindent" << std::endl;
    file << "A = \\{ ";
    auto l = pool->getItems<ComponentObj>();
    for (auto c : l) {
        if (c->isActive()) {
            file << normalizeName(c->getName()) << " ";
            // if (l.back() != c) file << " ,";
        }
    }
    file << "\\}" << std::endl;
    /*
    for (auto c : pool->getItems<TaskObj>()) {
        std::string events;
        for (auto e : c->getEvents()) {
            if (!NetworkHelper::isIgnoredState(e)) {
                only_shortnames = false;
            }
        }
        if (c->useCount() != 10000) {
            only_shortnames = false;
        }
        if (!only_shortnames) break;
    }
    */
    if (!only_shortnames) {
        file << "\\noindent T = \\{\\\\" << std::endl;
        file << "\\begin{longtable}{|p{8cm}|l|p{5cm}|}" << std::endl;
        file << "\\hline" << std::endl;
        for (auto c : pool->getItems<TaskObj>()) {
            std::string events;
            for (auto e : c->getEvents()) {
                if (!NetworkHelper::isIgnoredState(e)) {
                    events = events + e + ", ";
                }
            }
            if (c->useCount() == 10000) {
                file << normalizeName(c->getName()) << " & - & " << normalizeName(events) << "\\\\" << std::endl;
            } else {
                file << normalizeName(c->getName()) << " & " << c->useCount() << " & " << normalizeName(events) << "\\\\" << std::endl;
            }
        }
        file << "\\hline" << std::endl;
        file << "\\end{longtable}" << std::endl;
    } else {
        // file << "T = \\{ ";
        auto l = pool->getItems<TaskObj>();
        file << "\\begin{itemize}[nosep,noitemsep,topsep=0pt,partopsep=0pt]" << std::endl;
        for (auto c : l) {
            bool has_different_state = false;
            file << "\\item[T:] " << normalizeName(c->getName()) << "" << std::endl;
            for (auto e : c->getEvents()) {
                if (!NetworkHelper::isIgnoredState(e)) {
                    has_different_state = true;
                }
            }

            if (c->useCount() != 10000 || has_different_state) {
                file << "\\begin{itemize}[nosep,noitemsep,topsep=0pt,partopsep=0pt]" << std::endl;
                for (auto e : c->getEvents()) {
                    if (!NetworkHelper::isIgnoredState(e)) {
                        file << "\\item[S] " << normalizeName(e) << std::endl;
                    }
                }
                if (c->useCount() != 1000) {
                    file << "\\item[$\\Xi$] " << c->useCount() << std::endl;
                }
                file << "\\end{itemize}" << std::endl;
            }

            // file << normalizeName(c->getName());
            //            if (l.back() != c) file << " ,";
        }
        file << "\\end{itemize}" << std::endl;
        // file << "\\}\\\\" << std::endl;
    }
    if (pool->getItems<DataServiceObj>().size() != 0) {
        if (pool->getItems<DataServiceObj>().size() < 9999999) {
            file << "D = \\{";
            auto l = pool->getItems<DataServiceObj>();
            for (auto c : l) {
                file << normalizeName(c->getName()) << ", ";
                if (l.back() != c) file << " ,";
            }
            file << "\\}\\\\" << std::endl;
        } else {
            file << "\\begin{itemize}[nosep,noitemsep,topsep=0pt,partopsep=0pt]" << std::endl;
            for (auto c : pool->getItems<DataServiceObj>()) {
                file << "\\item[D] " << normalizeName(c->getName()) << std::endl;
            }
            file << "\\end{itemize}" << std::endl;
            /*
            file << "D = \\{\\\\" << std::endl;
            file << "\\begin{longtable}{|p{5cm}|}" << std::endl;
            file << "\\hline" << std::endl;
            for (auto c : pool->getItems<DataServiceObj>()) {
                file << normalizeName(c->getName()) << "\\\\" << std::endl;
            }
            file << "\\hline" << std::endl;
            file << "\\end{longtable}" << std::endl;
            */
        }
    }

    // file << "C = \\{\\\\" << std::endl;
    // file << "\\begin{longtable}{|p{5cm}|l|l|l|}" << std::endl;
    file << "\\begin{itemize}[nosep,noitemsep,topsep=0pt,partopsep=0pt]" << std::endl;

    // file << "\\hline" << std::endl;
    for (auto c : pool->getItems<CompositionObj>()) {
        if (dynamic_cast<StateMachineObj *>(c.get())) {
            file << "\\item[S:] " << normalizeName(c->getName()) << "" << std::endl;
        } else {
            file << "\\item[C:] " << normalizeName(c->getName()) << "" << std::endl;
        }
        // std::string events;
        std::vector<std::string> states;
        for (auto e : c->getEvents()) {
            if (!NetworkHelper::isIgnoredState(e)) {
                states.push_back(e);
                // events = events +  e  + ", ";
            }
        }

        file << "\\begin{itemize}[nosep,noitemsep,topsep=0pt,partopsep=0pt]" << std::endl;
        if (!states.empty()) {
            for (auto e : states) {
                file << "\\item event: " << normalizeName(e) << "" << std::endl;
            }
        }
        // std::string children;
        // file << "\\begin{itemize}" << std::endl;
        for (auto child : c->getChildren()) {
            file << "\\item[$\\phi$] " << normalizeName(child.first) << ": " << normalizeName(child.second->getName()) << std::endl;
            // children = children + child.first + ": " + child.second->getName() + ", ";
        }
        // file << "\\end{itemize}" << std::endl;
        if (c->useCount() != 10000) {
            // file << "\\begin{itemize}" << std::endl;
            file << "\\item[$\\Xi$] " << c->useCount() << std::endl;
            // file << "\\end{itemize}" << std::endl;
        }

        // file << "\\begin{itemize}" << std::endl;
        for (auto fw : c->getArgumentForwards()) {
            for (auto fw_child : fw.second) {
                file << "\\item[$\\Gamma$] " << normalizeName(fw_child.first) << "$\\rightarrow$" << normalizeName(fw.first) << "." << normalizeName(fw_child.second) << std::endl;
            }
        }
        // file << "\\end{itemize}" << std::endl;

        // file << "\\begin{itemize}" << std::endl;
        for (auto config : c->getProperties()) {
            file << "\\item[$\\Sigma$] " << normalizeName(config.name) << std::endl;
            if (auto s = dynamic_cast<SpecializedComponentObjBase *>(c.get())) {
                if (s->configuration.find(config.name) != s->configuration.end()) {
                    if (s->configuration[config.name.c_str()].first == s->configuration[config.name.c_str()].second) {
                        file << ": " << s->configuration[config.name.c_str()].first << std::endl;
                    } else {
                        file << ": " << s->configuration[config.name.c_str()].first << " to " << s->configuration[config.name.c_str()].second << std::endl;
                    }
                }
            }
        }

        if (auto sm = dynamic_cast<StateMachineObj *>(c.get())) {
            auto states = sm->getStates();
            for (int i = 0; i < states.size(); i++) {
                file << "\\item[$s_" << i << "$]" << normalizeName(states[i]->getName()) << std::endl;
            }
            for (auto tr : sm->getTransitions()) {
                file << "\\item[$\\delta$] $s_" << tr.source << "$." << normalizeName(tr.event) << " $\\rightarrow$ $s_" << tr.target << "$ " << std::endl;
            }
        }

        for (auto fw : c->getEventForwards()) {
            for (auto fw_child : fw.second) {
                if ((fw_child.first == "failed" || fw_child.first == "aborted" || fw_child.first == "fatal_error" || fw_child.first == "internal_error" || fw_child.first == "success") &&
                    fw_child.second == "failed")
                    continue;

                file << "\\item[$\\zeta$] " << normalizeName(fw.first) << "." << normalizeName(fw_child.first) << "$\\rightarrow$ " << normalizeName(fw_child.second) << std::endl;

                file << "\\item[$\\zeta$] " << normalizeName(fw.first) << "." << normalizeName(fw_child.first) << "$\\rightarrow$ " << normalizeName(fw_child.second) << std::endl;
            }
        }

        file << "\\end{itemize}" << std::endl;
        // file << name << " & " << events << "\\\\" << std::endl;
        // file << normalizeName(c->getName()) << " & " << c->useCount() << " & " << normalizeName(events)  << " & " <<  normalizeName(children) << "\\\\" << std::endl;
    }
    file << "\\end{itemize}" << std::endl;
    // file << "\\hline" << std::endl;
    // file << "\\end{longtable}" << std::endl;

    /*
        file << "\\}" << std::endl;
        file << "C = \\{\\";
        for(auto c : pool->getItems<CompositionObj>()){
            file << c->getName() << "\\" << std::endl;
        }
        file << "\\}" << std::endl;
    */

    /*
    for(auto c : pool->getItems<ComponentObj>()){
        if(auto sm = std::dynamic_pointer_cast<StateMachineObj>(c)){
            file << sm->getName() << std::endl;
        }else if(auto cmp = std::dynamic_pointer_cast<CompositionObj>(c)){
            file << "CMP: " << cmp->getName() << std::endl;
            for(auto child : cmp->getChildren()){
            }
        }else if(auto ds = std::dynamic_pointer_cast<DataServiceObj>(c)){
        }

        if (c->isActive()) {
        }

    }
    */
}

void NetworkHelper::createClassSolution(std::string core_model, std::list<std::string> additionalRequirements, std::vector<unsigned int> ids)
{
    Pool *pool = XML::load(XML::loadInstanceSolution(core_model, ids));

    for (auto s : additionalRequirements) {
        if (!pool->hasComponent(s)) {
            throw std::invalid_argument("Cannot start component: " + s + ", it does not exist in the pool");
        }
        pool->getComponent(s)->setActive(true);
    }

    for (auto c : pool->getItems<ComponentObj>()) {
        if (c->isActive()) {
            std::cout << "Should start: " << c->getName() << std::endl;
            auto cmp = std::dynamic_pointer_cast<CompositionObj>(c);
            if (cmp.get()) {
                for (auto child : cmp->getChildren()) {
                    std::cout << "\t -child: " << child.second->getName() << std::endl;
                }
            }
        }
    }
    auto solutions = ClassSolution::babSearch(pool);
    XML::saveClassSolutions(solutions, core_model, additionalRequirements, ids);
    delete pool;
}

void NetworkHelper::createInstanceSolution(std::string core_model, std::vector<unsigned int> ids)
{
    std::vector<unsigned int> ids_for_loading = ids;
    ids_for_loading.pop_back();
    std::string mode_to_load = XML::loadInstanceSolution(core_model, ids_for_loading);
    std::cout << "Loading model: " << mode_to_load << std::endl;
    Pool *pool = XML::load(mode_to_load);
    auto instance_graph_filename = XML::loadClassSolution(core_model, ids);

    graph_analysis::BaseGraph::Ptr graph_imported = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::io::GraphIO::read(instance_graph_filename, graph_imported, graph_analysis::representation::GEXF);

    // Start to create our graph based on the imported graph
    for (auto node : graph_imported->getAllEdges()) {
        graph_analysis::Edge::Ptr e(new graph_analysis::Edge(node->getLabel()));
        const auto &v1 = pool->getComponent(node->getSourceVertex()->getLabel());
        const auto &v2 = pool->getComponent(node->getTargetVertex()->getLabel());
        assert(v1.get());
        assert(v2.get());
        e->setSourceVertex(v1);
        e->setTargetVertex(v2);
        graph->addEdge(e);
    }

    std::cout << "Start to create instance-solutions" << std::endl;
    unsigned int solution_id = 0;
    InstanceSolution::babSearch(graph, [&](graph_analysis::BaseGraph::Ptr solution) {
        graph_analysis::DirectedGraphInterface::Ptr g = std::dynamic_pointer_cast<graph_analysis::DirectedGraphInterface>(solution);
        auto trigger_events = EventModelHandler(pool, g);
        auto events = trigger_events.getTrigger();
        XML::addInstanceSolutions(core_model, {solution, events}, ids, solution_id++);
    });
    XML::addInstanceSolutionCount(core_model, ids, solution_id);

    std::cout << "Calculated all follow solutions " << solution_id << std::endl;
    std::cout << "Finish" << std::endl;
    delete pool;
}

bool NetworkHelper::getNextUncalculatedIDs(std::string core_model, std::vector<unsigned int> &ids, std::list<std::vector<unsigned int>> id)
{
    return XML::findUnresolvedIDs(core_model, ids, id);
}
