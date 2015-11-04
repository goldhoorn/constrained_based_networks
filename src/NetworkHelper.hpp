#pragma once

#include <string>
#include <vector>
#include <list>

namespace constrained_based_networks
{
    class NetworkHelper{
        public:
            static bool getNextUncalculatedIDs(std::string core_model, std::vector<unsigned int> &ids);
            static void createClassSolution(std::string core_model, std::list<std::string> additionalRequirements, std::vector<unsigned int> ids);
            static void createInstanceSolution(std::string core_model, std::vector<unsigned int> ids);
            static void initializeExporter();
    };

};

