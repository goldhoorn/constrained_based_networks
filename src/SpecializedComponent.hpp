#pragma once

#include "Pool.hpp"
#include <string>
#include <vector>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/noncopyable.hpp>
#include <iostream>

namespace constrained_based_networks {


struct Configuration : public std::map<std::string,std::string>{
//    std::vector<std::string> name;
//    std::vector<std::string> value;

    void add(std::string _name, std::string _value){
        operator[](_name) = _value;
    }
};


class SpecializedComponentBase : private boost::noncopyable{
    public:
    SpecializedComponentBase(){
    }
    virtual ~SpecializedComponentBase(){
    }

    Configuration configuration;

    void addConfig(std::string name, std::string value){
        configuration.add(name,value);
    };

    virtual constrained_based_networks::Component* getComponent() = 0;
    virtual unsigned int getID() const = 0;

    unsigned int id;

    /*
    operator const Component*() const{
        std::cerr << "FUSEL" << std::endl;
        return 0;
    };
    */

    friend class Pool;

};

template<class T>
class SpecializedComponent : public T, public SpecializedComponentBase{
    public:
    SpecializedComponent(T *t, Pool *pool): T(*t){
        classScope=true;
        pool->addComponent(this);
    }


    virtual ~SpecializedComponent(){
    };

    virtual constrained_based_networks::Component* getComponent(){
        return this;
    };

    virtual unsigned int getID() const{
        if(classScope){
            return T::getID();
        }else{
            return SpecializedComponentBase::id;
        }
    };

    bool classScope;


};

};
