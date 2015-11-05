#pragma once

#include <string>

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
    void  parseTransitionSolution(xmlpp::Element *c, std::stringstream &s);
    std::string parseInstanceSolution(xmlpp::Element *c, std::stringstream &s);
    std::string parseClassSolution(xmlpp::Element *c, std::stringstream &s);
    std::string getIdentifier(xmlpp::Element *c);
    std::string filename;
    xmlpp::Element *rootNode;
    xmlpp::DomParser *parser;
};
};
