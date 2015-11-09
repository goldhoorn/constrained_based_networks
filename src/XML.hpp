#pragma once
#include <string>
#include <graph_analysis/BaseGraph.hpp>
#include "EventModelHandler.hpp"
#include <boost/thread/recursive_mutex.hpp>


namespace xmlpp
{
class Node;
class Element;
}

namespace constrained_based_networks
{
class Pool;

class XML
{
   public:
    static void importSM(Pool* pool, xmlpp::Node* const child, xmlpp::Node* const root);
    static void importComposition(Pool* pool, xmlpp::Node* const child, xmlpp::Node* const root);
    static std::string loadInstanceSolution(std::string filename, std::vector<unsigned int> ids);
    static std::string loadClassSolution(std::string filename, std::vector<unsigned int> ids);
    static Pool* load(std::string filename);
    static bool save(Pool* pool, std::string& filename, bool md5 = false);
    static bool saveClassSolutions(std::vector<graph_analysis::BaseGraph::Ptr> class_solutions, std::string original_file, std::list<std::string> additionalRequirements, std::vector<unsigned int> id);
    static bool addInstanceSolutions(const std::string filename, std::vector<std::pair<graph_analysis::BaseGraph::Ptr, std::list<TransitionTrigger>>> instance_solutions,
                                     std::vector<unsigned int> ids);
    static Component* ensureComponentAvailible(Pool* pool, std::string component_name, xmlpp::Node* const root);
    static void addSpecialization(Component* comp, xmlpp::Element* const root);
    static std::string toMD5(const std::string input);
    static void createDatabase(std::string original_file);
    static bool findUnresolvedIDs(std::string filename, std::vector<unsigned int> &res_v, std::list<std::vector<unsigned int>> ignored_solutions);
    static std::vector<unsigned int> findUncalculatedID(std::string filename, std::vector<unsigned int> ids);
    static std::string genDBFilename(const std::string modelFilename);
    static std::vector<unsigned int> getPath(xmlpp::Element *rootNode, xmlpp::Element *element);
    static std::vector<xmlpp::Element*> getPathAsElements(xmlpp::Element* rootNode, xmlpp::Element* element);

   private:
    static std::string loadSolution(std::string filename, std::vector<unsigned int> ids);
    static xmlpp::Element* findNodeForID(xmlpp::Element* root, const std::vector<unsigned int>& ids);
    static xmlpp::Element* getUnresolvedForFollowNetwork(xmlpp::Element* root,xmlpp::Element* e,std::list<std::vector<unsigned int>> ignored_solutions);
    static xmlpp::Element* getUnresolvedForInstanceSolution(xmlpp::Element* root,xmlpp::Element* e,std::list<std::vector<unsigned int>> ignored_solutions);
    static xmlpp::Element* getUnresolvedForClassSolution(xmlpp::Element* root,xmlpp::Element* e,std::list<std::vector<unsigned int>> ignored_solutions);
    static xmlpp::Element* getReferenceNodeForNetwork(xmlpp::Element *root, std::string md5);
    static bool findPath(std::vector<unsigned int> s, std::list<std::vector<unsigned int>> ignored_solutions);

    static boost::recursive_mutex mutex;

    struct TransitionHelper
    {
        TransitionTrigger th;
        Pool* pool;
        std::string filename;
    };
};
};
