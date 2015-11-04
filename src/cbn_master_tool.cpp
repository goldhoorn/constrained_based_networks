#include <unistd.h>
#include <iostream>
#include "NetworkHelper.hpp"

using namespace constrained_based_networks;

void printHelp()
{
    std::cout << "Please use -f <filename> -c <components_to_start>" << std::endl;
    exit(-1);
}

// main test function
int main(int argc, char *argv[])
{
    char c;
    char *base_network_file = 0;
    bool list_unresolved = false;

    while ((c = getopt(argc, argv, "lf:")) != -1) {
        switch (c) {
            case 'l':
                list_unresolved = true;
                break;
            case 'f':
                base_network_file = optarg;
                break;
            default:
                printHelp();
        }
    }
    if (!base_network_file) {
        printHelp();
    }

    std::vector<unsigned int> ids;
    if (list_unresolved) {
        if (NetworkHelper::getNextUncalculatedIDs(base_network_file, ids)) {
            std::cout << "Unresolved ids: " << std::endl;
            for (auto id : ids) {
                std::cout << " " << id;
            }
            std::cout << std::endl;
        } else {
            std::cerr << "Everything is resolved" << std::endl;
        }
        return 0;
    }

    // Okay we don't should list only, then we do a full resolution
    while (NetworkHelper::getNextUncalculatedIDs(base_network_file, ids)) {
        std::cout << "Resolving: " << std::endl;
        for (auto id : ids) {
            std::cout << " " << id;
        }
        std::cout << std::endl;
        if (ids.size() % 3 == 0) {
            NetworkHelper::createClassSolution(base_network_file, std::list<std::string>(), ids);
        } else if (ids.size() % 3 == 1) {
            NetworkHelper::createInstanceSolution(base_network_file, ids);
        }
        ids.clear();
    }
    return 0;
}
