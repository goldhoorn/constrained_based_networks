#include <unistd.h>
#include <iostream>
#include "NetworkHelper.hpp"

using namespace constrained_based_networks;


void printHelp()
{
    std::cout << "Please use -f <filename> -i <instance-id>" << std::endl;
    exit(-1);
}

// main test function
int main(int argc, char *argv[])
{
    NetworkHelper::initializeExporter();
    char c;
    char *base_network_file = 0;
    std::vector<std::string> start_components;
    std::vector<unsigned int> ids;

    while ((c = getopt(argc, argv, "f:i:")) != -1) {
        switch (c) {
            case 'i':
                ids.push_back(atoi(optarg));
                break;
            case 'f':
                base_network_file = optarg;
                break;
            default:
                printf("On default block\n");
        }
    }
    if (!base_network_file || ids.size() == 0) {
        printHelp();
    }

    NetworkHelper::createInstanceSolution(base_network_file,ids);
    std::cout << "Finish" << std::endl;
    return 0;
}
