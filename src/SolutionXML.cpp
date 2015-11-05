#include <libxml++/libxml++.h>
#include "SolutionXML.hpp"
#include "XML.hpp"
#include <sstream>

using namespace constrained_based_networks;

SolutionXML::SolutionXML(std::string filename) : filename(filename)
{
    parser = new xmlpp::DomParser();
    parser->parse_file(XML::genDBFilename(filename));
    rootNode = parser->get_document()->get_root_node();
}

SolutionXML::~SolutionXML()
{
    delete parser;
}

std::string SolutionXML::getIdentifier(xmlpp::Element *c)
{
    assert(c);
    auto erg = XML::getPath(rootNode, c);
    std::stringstream s;
    s << "graph";
    for (auto e : erg) {
        s << "-" << e;
    }
    return s.str();
}

std::string SolutionXML::parseClassSolution(xmlpp::Element *c, std::stringstream &s)
{
    assert(c);
    s << "subgraph \"cluster-C" << getIdentifier(c) << "\" {" << std::endl;
    s << "label = \"C-" << getIdentifier(c) << "\";" << std::endl;
    std::string any_node;
    // std::string any_node = "\"DEBUG-" + getIdentifier(c) + "\"";

    for (auto child : c->get_children("instance_solution")) {
        s << "\"" << dynamic_cast<xmlpp::Element *>(child)->get_attribute("filename")->get_value() << "\";" << std::endl;
        any_node = dynamic_cast<xmlpp::Element *>(child)->get_attribute("filename")->get_value();
        parseTransitionSolution(dynamic_cast<xmlpp::Element *>(child), s);
    }

    if (any_node.empty()) {
        any_node = "\"DEBUG-" + getIdentifier(c) + "\"";
        s << any_node << ";";
    }
    s << "}" << std::endl;
    return any_node + " [lhead=\"cluster-C" + getIdentifier(c) + "\"]";
}

void SolutionXML::parseTransitionSolution(xmlpp::Element *c, std::stringstream &s)
{
    s << "subgraph \"cluster-T" << getIdentifier(c) << "\" {" << std::endl;
    s << "label = \"T-" << getIdentifier(c) << "\";" << std::endl;
    for (auto child : c->get_children("transition")) {
        auto elem = dynamic_cast<xmlpp::Element *>(child);
        if (elem->get_attribute("resulting_pool")->get_value() != "") {
            auto name = parseInstanceSolution(dynamic_cast<xmlpp::Element *>(child), s);

            // Print the name
            s << "\"" << getIdentifier(dynamic_cast<xmlpp::Element *>(child)) << "\";" << std::endl;

            //            s << "\"" << elem->get_attribute("causing_component")->get_value() << "\"" //This is correct but we have not resolved the network names
            s << "\"" << getIdentifier(dynamic_cast<xmlpp::Element *>(child)) << "\""
                //<< " -> " << name << "[label=\"" << elem->get_attribute("causing_event")->get_value() << "\"];";
              << " -> \"" << elem->get_attribute("resulting_pool")->get_value() << "\" [label=\"" << elem->get_attribute("causing_component")->get_value() << ":" << elem->get_attribute("causing_event")->get_value() << "\"];";
        }
    }
    s << "}" << std::endl;
}

std::string SolutionXML::parseInstanceSolution(xmlpp::Element *c, std::stringstream &s)
{
    assert(c);
    if(c->get_children("class_solution").size() == 0){
        return "";
    }

    s << "subgraph \"cluster-I" << getIdentifier(c) << "\" {" << std::endl;
    s << "label = \"I-" << getIdentifier(c) << "\";" << std::endl;
    for (auto child : c->get_children("class_solution")) {
        assert(dynamic_cast<xmlpp::Element *>(child));
        s << "\"" << dynamic_cast<xmlpp::Element *>(child)->get_attribute("filename")->get_value() << "\";" << std::endl;
        ;
        parseClassSolution(dynamic_cast<xmlpp::Element *>(child), s);
    }
    s << "}" << std::endl;

    return "NIL";
}

std::string SolutionXML::getDotGraph()
{
    std::stringstream result;

    result << "digraph G{" << std::endl;
    result << "compound=true;" << std::endl;
    parseInstanceSolution(rootNode, result);
    result << "}" << std::endl;
    return result.str();
}
