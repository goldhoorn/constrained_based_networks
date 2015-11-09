#pragma once

#include <string>
#include <list>
#include <set>

namespace xmlpp
{
class Node;
class Element;
class DomParser;
}

namespace constrained_based_networks
{

class SolutionXML
{
   public:
    SolutionXML(std::string filename);
    ~SolutionXML();

    std::string getDotGraph();

   private:
    //    std::list<std::string>parseTransitionSolution(xmlpp::Element *c, std::stringstream &s);
    //    std::list<std::string> parseInstanceSolution(xmlpp::Element *c, std::stringstream &s);
    //    std::list<std::string> parseClassSolution(xmlpp::Element *c, std::stringstream &s);

    void addLabelIfNotExist(std::stringstream &s, std::string node, std::string label);
    std::set<std::string> labelExists;
    std::list<std::string> parse(xmlpp::Element *c, std::stringstream &s);
    std::string getIdentifier(xmlpp::Node *c);
    std::string filename;
    xmlpp::Element *rootNode;
    xmlpp::DomParser *parser;
    bool cluster;
};
};
