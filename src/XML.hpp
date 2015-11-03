#pragma once
#include <string>
#include <graph_analysis/BaseGraph.hpp>
#include "EventModelHandler.hpp"

namespace xmlpp{
    class Node;
    class Element;
}

namespace constrained_based_networks
{
    class Pool;

    struct ClassSolutionImport{
//        std::vector<std::string> additionalRequirements;
        std::string graph_filename;
        int solution_id;
    };


    class XML{
        public:
        static void importSM(Pool* pool, xmlpp::Node *const child, xmlpp::Node *const root);
        static void importComposition(Pool* pool, xmlpp::Node *const child, xmlpp::Node *const root);
        static std::vector<ClassSolutionImport> loadClassSolutions(std::string filename);
        static Pool* load(std::string filename);
        static bool save(Pool *pool, std::string &filename, bool md5=false);
        static bool saveClassSolutions(std::vector<graph_analysis::BaseGraph::Ptr> class_solutions, std::string original_file, std::string &outfile, bool md5=false);
        static bool addInstanceSolutions(int classSolutionID, std::vector<std::pair<graph_analysis::BaseGraph::Ptr, std::list<TransitionTrigger> > > instance_solutions, std::string original_file);
        static Component* ensureComponentAvailible(Pool *pool, std::string component_name, xmlpp::Node* const root);
        static void addSpecialization(Component *comp, xmlpp::Element* const root);
        static std::string toMD5(const std::string input);

        private:
        struct TransitionHelper{
            TransitionTrigger th;
            Pool *pool;
            std::string filename;
        };
    };
};

