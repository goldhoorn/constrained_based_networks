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
    std::list<std::string> start_components;
    std::vector<unsigned int> ids;

    while ((c = getopt(argc, argv, "c:f:i:")) != -1) {
        switch (c) {
            case 'c':
                start_components.push_back(optarg);
                break;
            case 'f':
                base_network_file = optarg;
                break;
            case 'i':
                ids.push_back(atoi(optarg));
                break;
            default:
                printHelp();
        }
    }
    if (!base_network_file) {
        printHelp();
    }

    NetworkHelper::createClassSolution(base_network_file,start_components,ids);
    return 0;
}

