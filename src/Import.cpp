
#include "Import.hpp"
#include "Pool.hpp"
#include <owlapi/model/OWLOntologyTell.hpp>

using namespace constrained_based_networks;
using namespace owlapi::model;


Import::Import(Pool *pool){
    OWLOntology::Ptr ontology;
    OWLOntologyTell tell(ontology);
    OWLClass::Ptr robotKlass = tell.klass("http:://www.rock-robotics.org/2014/01/om-schema#Sherpa");


}

Import::~Import(){
}
