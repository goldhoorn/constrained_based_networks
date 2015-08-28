#pragma once

#include <string>
#include <vector>
#include <map>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/noncopyable.hpp>
#include <graph_analysis/GraphAnalysis.hpp>
#include <graph_analysis/lemon/Graph.hpp>

namespace constrained_based_networks {

class Pool;
class SpecializedComponentBase;
class Component;

class ConfigurationModel {
 public:
  enum Type {
    INT = 0,
    DOUBLE,
    BOOL,
    STRING,
  };

  ConfigurationModel(std::string _name, Type _t) : name(_name), t(_t) {}

  std::string name;

  Type t;
};

class Component : public graph_analysis::Vertex {
 public:
  Component(Pool *pool);
  virtual ~Component();
  virtual unsigned int getID() const;
  virtual bool abstract() const = 0;

  void addFullfillment(std::string name);
  bool isFullfilling(std::string name);
  virtual bool isActive();
  virtual void setActive(bool active = true);
  std::string getName() const;
  std::string toString() const { return getName(); }
  std::string getClassName() const { return "MyVertex"; }

  // Made for testing purposes
  bool isIgnored();

  void addEvent(const std::string &name);

  Component *getComponent(std::string s);

  void addProperty(const std::string &_name, ConfigurationModel::Type t) {
    properties.push_back(ConfigurationModel(_name, t));
  }

  // virtual void addConfig(std::string name, std::string value)=0;
  virtual SpecializedComponentBase *getSpecialized() = 0;

  const ConfigurationModel::Type getProperty(std::string _name) const {
    for (auto p : properties) {
      if (p.name == _name) {
        return p.t;
      }
    }
    throw std::runtime_error("Could not find property on underlaying task");
  }
  const std::vector<ConfigurationModel> &getProperties() const {
    return properties;
  }

  graph_analysis::Vertex::Ptr getPtr() {
    if (self.get() == nullptr) {
      self = graph_analysis::Vertex::Ptr(this);
    }
    return self;
  }

 protected:
  /*
   * DataServices that this component fullfills
   */
  std::vector<std::string> fullfillments;
  std::vector<std::string> events;
  std::vector<ConfigurationModel> properties;
  graph_analysis::Vertex::Ptr self;

  Pool *pool;
  bool active;
  std::string name;

 private:
  unsigned int id;

  friend class Pool;
};
}
