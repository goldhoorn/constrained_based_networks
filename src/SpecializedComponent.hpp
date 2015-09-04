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

struct Configuration : public std::map<std::string, std::string> {
  //    std::vector<std::string> name;
  //    std::vector<std::string> value;

  void add(std::string _name, std::string _value) {
    operator[](_name) = _value;
  }
};

class SpecializedComponentBase : private boost::noncopyable {
 public:
  SpecializedComponentBase() {}
  virtual ~SpecializedComponentBase() {}

  Configuration configuration;

  void addConfig(std::string name, std::string value) {
    configuration.add(name, value);
  };

  virtual constrained_based_networks::Component* getComponent() = 0;
  virtual constrained_based_networks::Component* getOrginal() = 0;
  virtual bool isActive() = 0;
  virtual void setActive(bool active = true)=0;
  virtual unsigned int getID() const = 0;
  virtual std::string getName(bool b) const=0;
  virtual std::string getName() const=0;

  unsigned int id;

  /*
  operator const Component*() const{
      std::cerr << "FUSEL" << std::endl;
      return 0;
  };
  */

  friend class Pool;
};

template <class T>
class SpecializedComponent :  public SpecializedComponentBase,  public T{
 public:
  SpecializedComponent(T* t, Pool* pool) : T(*t) {
    //classScope = false;
    pool->addComponent(this);
    org = t;
  }

  virtual bool isActive(){return active;};
  virtual void setActive(bool active = true){this->active = active;};

  virtual ~SpecializedComponent() {};

  virtual constrained_based_networks::Component* getComponent() {
    return this;
  };
  
  virtual std::string getName() const{
    return getName(false);
  }

  virtual std::string getName(bool base) const{
      if(base) return T::getName();

      std::stringstream s;
      s << T::getName() << "_" << getID();
    return s.str();
  }

  // This is ugly helper function to recover the IDs from mergeComponent
  // should not used somewhere else, a cast should be sufficent
  virtual constrained_based_networks::Component* getOrginal() {
    return org;
  };

  virtual unsigned int getID() const {
      return SpecializedComponentBase::id;
      /*
    if (classScope) {
      return T::getID();
    } else {
      return SpecializedComponentBase::id;
    }
*/

  };
  private:
    T *org;
    bool active;
//  bool classScope;
};
};
