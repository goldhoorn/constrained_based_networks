#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/ClassSolution.hpp>
#include <constrained_based_networks/InstanceSolution.hpp>
#include <constrained_based_networks/SpecializedComponent.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include <sys/stat.h>

#include "../tests/tests.hpp"
using namespace constrained_based_networks;

bool debug = false;

void initializeExporter()
{
    graph_analysis::VertexTypeManager *vManager = graph_analysis::VertexTypeManager::getInstance();
    graph_analysis::Vertex::Ptr cc = graph_analysis::Vertex::Ptr(new ConfiguredComponent());
    vManager->registerType(cc);
    vManager->registerAttribute(cc->getClassName(), "config", (graph_analysis::VertexTypeManager::serialize_func_t) & ConfiguredComponent::serializeConfig,
                                (graph_analysis::VertexTypeManager::deserialize_func_t) & ConfiguredComponent::deSerializeConfig,
                                (graph_analysis::VertexTypeManager::print_func_t) & ConfiguredComponent::printConfig);
    // vManager->registerAttribute(cc->getClassName(),"underlaying_name",(graph_analysis::VertexTypeManager::serialize_func_t)&ConfiguredComponent::serializeName,
    // (graph_analysis::VertexTypeManager::deserialize_func_t)&ConfiguredComponent::deserializeName, (graph_analysis::VertexTypeManager::serialize_func_t)&ConfiguredComponent::serializeName);
    vManager->registerAttribute(cc->getClassName(), "underlaying_name", (graph_analysis::VertexTypeManager::serialize_func_t) & ConfiguredComponent::serializeName,
                                (graph_analysis::VertexTypeManager::deserialize_func_t) & ConfiguredComponent::deserializeName,
                                (graph_analysis::VertexTypeManager::serialize_func_t) & ConfiguredComponent::serializeName);
}

void mkdir_p(const char *dir, int mode)
{
    char tmp[256];
    char *p = NULL;
    size_t len;

    snprintf(tmp, sizeof(tmp), "%s", dir);
    len = strlen(tmp);
    if (tmp[len - 1] == '/') tmp[len - 1] = 0;
    for (p = tmp + 1; *p; p++)
        if (*p == '/') {
            *p = 0;
            mkdir(tmp, mode);
            *p = '/';
        }
    mkdir(tmp, mode);
}

std::vector<graph_analysis::BaseGraph::Ptr> resolve(bool debug = false)
{
    pool->checkConsistency();

    try
    {
        std::vector<graph_analysis::BaseGraph::Ptr> erg;
        if (debug) {
            ClassSolution::gistBaBSeach(pool);
        } else {
            erg = ClassSolution::babSearch(pool);
        }
        return erg;
    }
    catch (std::runtime_error e)
    {
        std::cerr << "!!UNSOLVEABLE!! Active components: " << std::endl;
        for (auto component : pool->getItems<Component *>()) {
            if (component->isActive()) {
                std::cerr << "\t- " << component->getName() << std::endl;
            }
        }

        return std::vector<graph_analysis::BaseGraph::Ptr>();
    }
}

Component *setConfig(graph_analysis::Vertex::Ptr v, Component *c)
{
    auto conf = dynamic_cast<ConfiguredComponent *>(v.get());
    assert(conf);
    auto new_active_child = c->getSpecialized();
    for (auto i : conf->int_config) {
        std::stringstream s;
        // TODO rething of limits
        s << i.min;
        new_active_child->addConfig(i.name, s.str());
    }
    for (auto i : conf->bool_config) {
        std::stringstream s;
        // TODO rething of limits
        s << i.min;
        new_active_child->addConfig(i.name, s.str());
    }
    for (auto i : conf->double_config) {
        std::stringstream s;
        // TODO rething of limits
        s << i.min;
        new_active_child->addConfig(i.name, s.str());
    }
    for (auto i : conf->string_config) {
        // TODO rething of limits
        new_active_child->addConfig(i.first, i.second);
    }
    return dynamic_cast<Component*>(new_active_child);
}

void runTest(std::string base_network_file)
{
    std::stringstream folder;
    folder << base_network_file << "-results"
           << "/";

    mkdir_p(folder.str().c_str(), 0755);
    unsigned int cnt = 0;

    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::io::GraphIO::read(base_network_file, graph, graph_analysis::representation::GEXF);

    // Start to create our graph based on the imported graph
    for (auto e : graph->edges()) {
        // We have found a root-knot we need to set all children active in our pool
        if (e->getSourceVertex()->toString() == "root-knot") {
            const auto &active_child = pool->getComponent(e->getTargetVertex()->toString());
            assert(active_child->getPtr());

            auto new_component = setConfig(e->getTargetVertex(), active_child);
            new_component->setActive(true);
        } else {
            std::cout << "From " << e->getSourceVertex()->toString() << " to " << e->getTargetVertex()->toString() << std::endl;
            auto v1 = pool->getComponent(e->getSourceVertex()->toString());
            auto v2 = pool->getComponent(e->getTargetVertex()->toString());
            assert(v1->getPtr());
            assert(v2->getPtr());
            auto cmp = dynamic_cast<Composition *>(v1);
            assert(cmp);  // Otherwise it could not have any children

            auto new_component = setConfig(e->getTargetVertex(), v2);
            assert(new_component);
            cmp->replaceChild(new_component, e->toString());
        }
    }
    pool->mergeDoubles();

    // Change into the folger the GRAPH io does not seems to support the write in subfolders :-/
    chdir(folder.str().c_str());
    for (auto graph : resolve(debug)) {
        std::cout << "Finished calculuation of ClassSolution number" << cnt << std::endl;
        std::stringstream s;
        s << "network-" << std::setw(4) << std::setfill('0') << cnt;
        std::cout << "Write to file: " << s.str() << std::endl;
        graph_analysis::io::GraphIO::write(s.str(), graph, graph_analysis::representation::GEXF);
        ++cnt;
    }
}

// main test function
int main(int argc, char *argv[])
{
    initializeExporter();
    int test_id = -1;
    char c;
    char *base_network_file = 0;

    while ((c = getopt(argc, argv, "hadrt:n:f:")) != -1) {
        switch (c) {
            case 'd':
                debug = true;
                break;
            case 't':
                test_id = atoi(optarg);
                break;
            case 'h':
                printTests();
                return 0;
            case 'f':
                base_network_file = optarg;
                break;
            default:
                printf("On default block\n");
        }
    }
    if (test_id == -1 || base_network_file == 0) {
        std::cerr << "please pass testname with -t <id> and -f <base_network_file>" << std::endl;
        exit(-1);
    }
    load_test(test_id);
    runTest(base_network_file);

    delete pool;

    std::cout << "Finished" << std::endl;
    return 0;
}
