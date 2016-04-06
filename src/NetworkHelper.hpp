#pragma once

#include <string>
#include <vector>
#include <list>

namespace constrained_based_networks
{
    class NetworkHelper{
        public:
            static bool getNextUncalculatedIDs(std::string core_model, std::vector<unsigned int> &ids, std::list<std::vector<unsigned int>> id);
            static void createClassSolution(std::string core_model, std::list<std::string> additionalRequirements, std::vector<unsigned int> ids);
            static void createInstanceSolution(std::string core_model, std::vector<unsigned int> ids);
            static std::string normalizeName(std::string);
            static void initializeExporter();
            static bool isIgnoredState(std::string name);
            static void createModelExportForLatex(std::string core_model, std::string filename);
    };

};

