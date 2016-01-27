#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Pool.hpp>
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/ClassSolution.hpp>
#include <constrained_based_networks/InstanceSolution.hpp>
#include <constrained_based_networks/SpecializedComponent.hpp>
#include <graph_analysis/GraphIO.hpp>
#include <constrained_based_networks/EventModelHandler.hpp>
#include <graph_analysis/VertexTypeManager.hpp>
#include "SolutionXML.hpp"
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

char buff[1024];

using namespace constrained_based_networks;

void initializeExporter(){
    graph_analysis::VertexTypeManager *vManager = graph_analysis::VertexTypeManager::getInstance();
    graph_analysis::Vertex::Ptr cc = graph_analysis::Vertex::Ptr(new ConfiguredComponent());
    vManager->registerType(cc);
    vManager->registerAttribute(cc->getClassName(),"max_use",(graph_analysis::VertexTypeManager::serialize_func_t)&ConfiguredComponent::serializeUc, (graph_analysis::VertexTypeManager::deserialize_func_t)&ConfiguredComponent::deserializeUc, (graph_analysis::VertexTypeManager::print_func_t)&ConfiguredComponent::printUc);
    vManager->registerAttribute(cc->getClassName(),"config",(graph_analysis::VertexTypeManager::serialize_func_t)&ConfiguredComponent::serializeConfig, (graph_analysis::VertexTypeManager::deserialize_func_t)&ConfiguredComponent::deSerializeConfig, (graph_analysis::VertexTypeManager::print_func_t)&ConfiguredComponent::printConfig);
   // vManager->registerAttribute(cc->getClassName(),"underlaying_name",(graph_analysis::VertexTypeManager::serialize_func_t)&ConfiguredComponent::serializeName, (graph_analysis::VertexTypeManager::deserialize_func_t)&ConfiguredComponent::deserializeName, (graph_analysis::VertexTypeManager::serialize_func_t)&ConfiguredComponent::serializeName);
    vManager->registerAttribute(cc->getClassName(),"underlaying_name",(graph_analysis::VertexTypeManager::serialize_func_t)&ConfiguredComponent::serializeName, (graph_analysis::VertexTypeManager::deserialize_func_t)&ConfiguredComponent::deserializeName, (graph_analysis::VertexTypeManager::serialize_func_t)&ConfiguredComponent::serializeName);
}


// main test function
int main(int argc, char *argv[])
{
    initializeExporter();
    char *infile=0;
    char c;
    bool onlySolutions = false;
    while ((c = getopt(argc, argv, "i:s")) != -1){
        switch (c) {
            case 's':
                onlySolutions = true;
                break;
            case 'i':
                infile= optarg;
                break;
        }
    }

    if(!infile){
        std::cerr << "ERR" << std::endl;
        return -1;
    }

    SolutionXML xml(infile);
    std::cout << xml.getDotGraph(onlySolutions) << std::endl;
    return 0;
}


#if 0
    char tmpfile[512];
    int tmp_file_fd=0;
    if(!outfile){
        sprintf(tmpfile,"/tmp/gexf_dot_converter_XXXXXX.dot");
        tmp_file_fd = mkstemps(tmpfile,4);
    }


    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    graph_analysis::io::GraphIO::read(infile, graph, graph_analysis::representation::GEXF);
    if(tmp_file_fd){
        graph_analysis::io::GraphIO::write(tmpfile, graph, graph_analysis::representation::GRAPHVIZ);
        size_t buflen;
        lseek(tmp_file_fd,0,SEEK_SET);
        while((buflen = read(tmp_file_fd, buff, 1024)) > 0){
            write(1, buff, buflen);
        }
        close(tmp_file_fd);
    }else{
        graph_analysis::io::GraphIO::write(outfile, graph, graph_analysis::representation::GRAPHVIZ);
    }
#endif
