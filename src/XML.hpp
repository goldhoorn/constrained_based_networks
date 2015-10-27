#pragma once
#include <string>

namespace xmlpp{
    class Node;
}

namespace constrained_based_networks
{
    class Pool;

    class XML{
        public:
        static void importComposition(Pool* pool, xmlpp::Node *const child, xmlpp::Node *const root);
        static Pool* load(std::string filename);
        static bool save(Pool *pool, std::string filename);
    };
};

