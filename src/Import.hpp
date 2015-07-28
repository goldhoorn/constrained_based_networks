#pragma once

#include <owlapi/model/OWLOntology.hpp>
#include "Pool.hpp"

namespace constrained_based_networks {

class Pool;

class Import{
    public:
        Import(Pool *pool);
        ~Import();
};

};
