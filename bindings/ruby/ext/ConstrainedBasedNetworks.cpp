#include "rice/Class.hpp"
#include "rice/Constructor.hpp"
#include "rice/String.hpp"
#include "rice/Array.hpp"
#include "rice/Enum.hpp"

#include <stdint.h>
#include <vector>
#include <deque>
#include <iostream>

#include <constrained_based_networks/Port.hpp>
#include <constrained_based_networks/Task.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/Query.hpp>
#include <constrained_based_networks/Solution.hpp>


using namespace Rice;
using namespace constrained_based_networks;

typedef std::vector< std::string > StringVector;
typedef std::vector< IncomingPort > IncomingPortVector;
typedef std::vector< OutgoingPort > OutgoingPortVector;

Data_Type<StringVector> stringVector;
Data_Type<IncomingPortVector> incomingPortVector;
Data_Type<OutgoingPortVector> outgoingPortVector;

Data_Type<IncomingPort> rb_cIncomingPort;
Data_Type<OutgoingPort> rb_cOutgoingPort;
Data_Type<Composition> rb_cComposition;
Data_Type<Task> rb_cTask;
Data_Type<Query> rb_cQuery;
Data_Type<Solution> rb_cSolution;

static Module rb_mGecodeCompMgmt;

// Conversion templates
//template<>
StringVector from_ruby_string_vector(Array a)
{
    //Array a(obj);
    StringVector v;
    for(Array::iterator aI = a.begin(); aI != a.end(); ++aI)
        v.push_back(((String)*aI).str());
    return v;
}

template<>
Object to_ruby<StringVector>(const StringVector& v)
{
    Array strings;
    StringVector::const_iterator it;
    for(it = v.begin(); it != v.end(); it++)
    {
            strings.push(String(*it));
    }

    return strings;
}

template<>
Object to_ruby< IncomingPortVector >(const IncomingPortVector& v)
{
    Array ports;
    IncomingPortVector::const_iterator it;
    for( it = v.begin(); it != v.end(); it++)
    {
            ports.push(Data_Object<IncomingPort>( new IncomingPort(*it)));
    }

    return ports;
}

template<>
Object to_ruby< OutgoingPortVector >(const OutgoingPortVector& v)
{
    Array ports;
    OutgoingPortVector::const_iterator it;
    for( it = v.begin(); it != v.end(); it++)
    {
            ports.push(Data_Object<OutgoingPort>(new OutgoingPort(*it) ));
    }

    return ports;
}


// Wrappers Task
Array wrap_task_getConfiguration(Object self)
{
    Data_Object<Task> component(self, rb_cTask);
    StringVector conf = component->getConfiguration();
    return to_ruby<StringVector>(conf);
}

Object wrap_task_setConfiguration(Object self, Array configuration)
{
    Data_Object<Task> component(self, rb_cTask);
    StringVector conf = from_ruby_string_vector(configuration);
    component->setConfiguration(conf);
    
    return self;
}

Array wrap_task_getInPorts(Object self)
{
    Data_Object<Task> component(self, rb_cTask);
    IncomingPortVector ports = component->getInPorts();
    return to_ruby<IncomingPortVector>(ports);
}

Array wrap_task_getOutPorts(Object self)
{
    Data_Object<Task> component(self, rb_cTask);
    OutgoingPortVector ports = component->getOutPorts();
    return to_ruby<OutgoingPortVector>(ports);
}

Object wrap_task_copy(Object self)
{
    // Use C++ Copy Constructor
    Data_Object<Task> component(self, rb_cTask);
    Task* copy = new Task(*component);
    return Data_Object<Task>(copy);
}

// Wrappers
Array wrap_composition_getConfiguration(Object self)
{
    Data_Object<Composition> component(self, rb_cComposition);
    StringVector conf = component->getConfiguration();
    return to_ruby<StringVector>(conf);
}

Object wrap_composition_setConfiguration(Object self, Array configuration)
{
    Data_Object<Composition> component(self, rb_cComposition);
    StringVector conf = from_ruby_string_vector(configuration);
    component->setConfiguration(conf);
    
    return self;
}

Array wrap_composition_getInPorts(Object self)
{
    Data_Object<Composition> component(self, rb_cComposition);
    IncomingPortVector ports = component->getInPorts();
    return to_ruby<IncomingPortVector>(ports);
}

Array wrap_composition_getOutPorts(Object self)
{
    Data_Object<Composition> component(self, rb_cComposition);
    OutgoingPortVector ports = component->getOutPorts();
    return to_ruby<OutgoingPortVector>(ports);
}

Object wrap_composition_copy(Object self)
{
    // Use C++ Copy Constructor
    Data_Object<Composition> component(self, rb_cComposition);
    Composition* copy = new Composition(*component);
    return Data_Object<Composition>(copy);
}

/**
 *
 */
extern "C"
void Init_constrained_based_networks_ruby()
{

    // Define module FIPA
    rb_mGecodeCompMgmt = define_module("CONSTRAINED_BASED_NETWORKS");
    
    rb_cIncomingPort = define_class_under<IncomingPort>(rb_mGecodeCompMgmt, "IncomingPort")
        .define_constructor(Constructor<IncomingPort, const std::string&, const std::string&>(), (Arg("datatype"), Arg("name")))
        .define_method("toString", &IncomingPort::toString)
    ;
    
    rb_cOutgoingPort = define_class_under<OutgoingPort>(rb_mGecodeCompMgmt, "OutgoingPort")
        .define_constructor(Constructor<OutgoingPort, const std::string&, const std::string&>(), (Arg("datatype"), Arg("name")))
        .define_method("toString", &OutgoingPort::toString)
    ;
    
    rb_cComposition = define_class_under<Composition>(rb_mGecodeCompMgmt, "Composition")
        .define_constructor(Constructor<Composition, const std::string&>(), (Arg("name")))
        .define_method("copy", &wrap_composition_copy)
        .define_method("toString", &Composition::toString)
        .define_method("getType", &Composition::getType)
        .define_method("getName", &Composition::getName)
        .define_method("setName", &Composition::setName, Arg("name"))
        .define_method("pushBackInPort", &Composition::pushBackInPort, Arg("inPort"))
        .define_method("pushBackOutPort", &Composition::pushBackOutPort, Arg("outPort"))
        //.define_method("putIncomingConnection", &Composition::putIncomingConnection, (Arg("inPort"), Arg("compositionName")))
        //.define_method("putOutgoingConnection", &Composition::putOutgoingConnection, (Arg("outPort"), Arg("compositionName")))
        //.define_method("getConfiguration", &wrap_composition_getConfiguration)
        //.define_method("setConfiguration", &wrap_composition_setConfiguration, Arg("confguration"))
        //.define_method("getInPorts", &wrap_composition_getInPorts)
        //.define_method("getOutPorts", &wrap_composition_getOutPorts)
    ;
    
    rb_cTask= define_class_under<Task>(rb_mGecodeCompMgmt, "Task")
        .define_constructor(Constructor<Task, const std::string&>(), (Arg("name")))
        .define_method("copy", &wrap_task_copy)
        .define_method("toString", &Task::toString)
        .define_method("getName", &Task::getName)
        .define_method("setName", &Task::setName, Arg("name"))
        .define_method("pushBackInPort", &Task::pushBackInPort, Arg("inPort"))
        .define_method("pushBackOutPort", &Task::pushBackOutPort, Arg("outPort"))
        .define_method("getConfiguration", &wrap_task_getConfiguration)
        .define_method("setConfiguration", &wrap_task_setConfiguration, Arg("confguration"))
        .define_method("getInPorts", &wrap_task_getInPorts)
        .define_method("getOutPorts", &wrap_task_getOutPorts)
    ;
    
    rb_cQuery = define_class_under<Query>(rb_mGecodeCompMgmt, "Query")
        .define_constructor(Constructor<Query, const std::string&>(), Arg("name"))
        .define_method("toString", &Query::toString)
        .define_method("getComposition", &Query::getComposition, Arg("name"))
        //.define_method("getCompositionIndex", &Query::getCompositionIndex, Arg("name"))
        .define_method("getName", &Query::getName)
        .define_method("addComposition", &Query::addComposition, Arg("component"))
        .define_method("integrateSubQuery", &Query::integrateSubQuery, Arg("subQuery"))
        .define_method("addConnection", &Query::addConnection, (Arg("outCompName"), Arg("outPortName"), Arg("inCompName"), Arg("inPortName")))
    ;
    
    rb_cSolution = define_class_under<Solution>(rb_mGecodeCompMgmt, "Solution")
        .define_singleton_method("babSearch", &Solution::babSearch, (Arg("query"), Arg("pool"))) // static method
        .define_method("print", &Solution::print)
    ;
}
