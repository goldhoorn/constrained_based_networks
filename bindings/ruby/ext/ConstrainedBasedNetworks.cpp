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
#include <constrained_based_networks/DataService.hpp>
#include <constrained_based_networks/Composition.hpp>
#include <constrained_based_networks/ClassSolution.hpp>


using namespace Rice;
using namespace constrained_based_networks;

typedef std::vector< std::string > StringVector;
typedef std::vector< InputPort* > InputPortVector;
typedef std::vector< OutputPort* > OutputPortVector;

Data_Type<StringVector> stringVector;
Data_Type<InputPortVector*> incomingPortVector;
Data_Type<OutputPortVector*> outgoingPortVector;

Data_Type<Pool> rb_cPool;
Data_Type<Component> rb_cComponent;
Data_Type<InputPort> rb_cInputPort;
Data_Type<OutputPort> rb_cOutputPort;
Data_Type<Composition> rb_cComposition;
Data_Type<Task> rb_cTask;
Data_Type<DataService> rb_cDataService;
Data_Type<ClassSolution> rb_cClassSolution;

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
Object to_ruby< InputPortVector >(const InputPortVector& v)
{
    Array ports;
    InputPortVector::const_iterator it;
    for( it = v.begin(); it != v.end(); it++)
    {
            ports.push(Data_Object<InputPort>( new InputPort(**it)));
    }

    return ports;
}

template<>
Object to_ruby< OutputPortVector >(const OutputPortVector& v)
{
    Array ports;
    OutputPortVector::const_iterator it;
    for( it = v.begin(); it != v.end(); it++)
    {
            ports.push(Data_Object<OutputPort>(new OutputPort(**it) ));
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
    InputPortVector ports = component->getInPorts();
    return to_ruby<InputPortVector>(ports);
}

Array wrap_task_getOutPorts(Object self)
{
    Data_Object<Task> component(self, rb_cTask);
    OutputPortVector ports = component->getOutPorts();
    return to_ruby<OutputPortVector>(ports);
}

Object wrap_task_copy(Object self)
{
    // Use C++ Copy Constructor
    Data_Object<Task> component(self, rb_cTask);
    Task* copy = new Task(*component);
    return Data_Object<Task>(copy);
}

// Wrappers DataService 
Array wrap_ds_getConfiguration(Object self)
{
    Data_Object<DataService> component(self, rb_cDataService);
    StringVector conf = component->getConfiguration();
    return to_ruby<StringVector>(conf);
}

Object wrap_ds_setConfiguration(Object self, Array configuration)
{
    Data_Object<DataService> component(self, rb_cDataService);
    StringVector conf = from_ruby_string_vector(configuration);
    component->setConfiguration(conf);
    
    return self;
}
/*
Array wrap_ds_getInPorts(Object self)
{
    Data_Object<DataService> component(self, rb_cDataService);
    InputPortVector ports = component->getInPorts();
    return to_ruby<InputPortVector>(ports);
}

Array wrap_ds_getOutPorts(Object self)
{
    Data_Object<DataService> component(self, rb_cDataService);
    OutputPortVector ports = component->getOutPorts();
    return to_ruby<OutputPortVector>(ports);
}
*/

Object wrap_ds_copy(Object self)
{
    // Use C++ Copy Constructor
    Data_Object<DataService> component(self, rb_cDataService);
    DataService* copy = new DataService(*component);
    return Data_Object<DataService>(copy);
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
    InputPortVector ports = component->getInPorts();
    return to_ruby<InputPortVector>(ports);
}

Array wrap_composition_getOutPorts(Object self)
{
    Data_Object<Composition> component(self, rb_cComposition);
    OutputPortVector ports = component->getOutPorts();
    return to_ruby<OutputPortVector>(ports);
}

Object wrap_composition_copy(Object self)
{
    // Use C++ Copy Constructor
    Data_Object<Composition> component(self, rb_cComposition);
    Composition* copy = new Composition(*component);
    return Data_Object<Composition>(copy);
}

Object wrap_composition_add_child(Object self, Object type, String name){
    Data_Object<Composition> component(self, rb_cComposition);
    Data_Object<Component> type_o(type, rb_cComponent);
    component->addChild(type_o.get(), name.str());
    return self;
}

Array wrap_task_get_unresolveable(Object self)
{
    Data_Object<Composition> component(self, rb_cComposition);
    StringVector conf = component->unsolveableChildren();
    return to_ruby<StringVector>(conf);
}

//Component Wrapper
Object wrap_component_add_fullfillment(Object self, String name){
    Data_Object<Component> component(self, rb_cComponent);
    component->addFullfillment(name.str());
    return self;
}

/**
 *
 */
extern "C"
void Init_constrained_based_networks_ruby()
{

    // Define module FIPA
    rb_mGecodeCompMgmt = define_module("CONSTRAINED_BASED_NETWORKS");
    
    rb_cInputPort = define_class_under<InputPort>(rb_mGecodeCompMgmt, "InputPort")
        .define_constructor(Constructor<InputPort, const std::string&, const std::string&>(), (Arg("datatype"), Arg("name")))
        .define_method("toString", &InputPort::toString)
    ;
    
    rb_cPool = define_class_under<Pool>(rb_mGecodeCompMgmt, "Pool")
        .define_constructor(Constructor<Pool, std::string>(), Arg("file-to-load"))
        .define_singleton_method("instance", &Pool::getInstance) // static method
        .define_method("save", &Pool::save, Arg("filename"))
    ;
    
    rb_cOutputPort = define_class_under<OutputPort>(rb_mGecodeCompMgmt, "OutputPort")
        .define_constructor(Constructor<OutputPort, const std::string&, const std::string&>(), (Arg("datatype"), Arg("name")))
        .define_method("toString", &OutputPort::toString)
    ;
   
    rb_cComponent = define_class_under<Component>(rb_mGecodeCompMgmt,"Component")
        .define_method("active", &Composition::setActive, Arg("active"))
        .define_method("add_fullfillment", &wrap_component_add_fullfillment, Arg("name"))
        .define_method("getName", &Composition::getName)
    ;

    rb_cComposition = define_class_under<Composition, Component>(rb_mGecodeCompMgmt, "Composition")
        .define_constructor(Constructor<Composition, const std::string&>(), (Arg("name")))
        .define_method("copy", &wrap_composition_copy)
        .define_method("toString", &Composition::toString)
        .define_method("getType", &Composition::getType)
        .define_method("setName", &Composition::setName, Arg("name"))
        .define_method("add_child", &wrap_composition_add_child, (Arg("child"),Arg("name")))
        .define_method("createInput", &Composition::createInput, (Arg("name"),Arg("type")))
        .define_method("createOutput", &Composition::createOutput, (Arg("name"), Arg("type")))
        .define_method("unsolveable_childs", &wrap_task_get_unresolveable)
        //.define_method("putIncomingConnection", &Composition::putIncomingConnection, (Arg("inPort"), Arg("compositionName")))
        //.define_method("putOutgoingConnection", &Composition::putOutgoingConnection, (Arg("outPort"), Arg("compositionName")))
        //.define_method("getConfiguration", &wrap_composition_getConfiguration)
        //.define_method("setConfiguration", &wrap_composition_setConfiguration, Arg("confguration"))
        //.define_method("getInPorts", &wrap_composition_getInPorts)
        //.define_method("getOutPorts", &wrap_composition_getOutPorts)
    ;
    
    rb_cTask= define_class_under<Task, Component>(rb_mGecodeCompMgmt, "Task")
        .define_constructor(Constructor<Task, const std::string&>(), (Arg("name")))
        .define_method("copy", &wrap_task_copy)
        .define_method("toString", &Task::toString)
        .define_method("setName", &Task::setName, Arg("name"))
        .define_method("createInput", &Task::createInput, (Arg("name"),Arg("type")))
        .define_method("createOutput", &Task::createOutput, (Arg("name"), Arg("type")))
        .define_method("getConfiguration", &wrap_task_getConfiguration)
        .define_method("setConfiguration", &wrap_task_setConfiguration, Arg("confguration"))
        .define_method("getInPorts", &wrap_task_getInPorts)
        .define_method("getOutPorts", &wrap_task_getOutPorts)
    ;
    
    rb_cDataService = define_class_under<DataService, Component>(rb_mGecodeCompMgmt, "DataService")
        .define_constructor(Constructor<DataService, const std::string&>(), (Arg("name")))
        .define_method("copy", &wrap_ds_copy)
        .define_method("toString", &DataService::toString)
        .define_method("setName", &DataService::setName, Arg("name"))
//        .define_method("createInput", &DataService::createInput, (Arg("name"),Arg("type")))
//        .define_method("createOutput", &DataService::createOutput, (Arg("name"), Arg("type")))
        .define_method("getConfiguration", &wrap_ds_getConfiguration)
        .define_method("setConfiguration", &wrap_ds_setConfiguration, Arg("confguration"))
//        .define_method("getInPorts", &wrap_ds_getInPorts)
//        .define_method("getOutPorts", &wrap_ds_getOutPorts)
    ;
    
    rb_cClassSolution = define_class_under<ClassSolution>(rb_mGecodeCompMgmt, "ClassSolution")
        .define_singleton_method("bab_search", &ClassSolution::babSearch2 ) // static method
        .define_singleton_method("gist_search", &ClassSolution::gistBaBSeach ) // static method
        .define_method("print", &ClassSolution::rprint)
    ;
}
