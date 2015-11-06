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
    cluster = false;
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

/**
 * This function walks throught incance-solution, to the input is a class-solution node
 *  it paints the cluster around each instance and connects each instance with <this>
 */
std::list<std::string> SolutionXML::parse(xmlpp::Element *c, std::stringstream &s)
{
    std::list<std::string> class_names;

    for (auto class_child : c->get_children("class_solution")) {
        //Paint one allone to make sure everything is displayed
        std::string class_name = "\"" + dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("filename")->get_value() + "\";";
        s << class_name << std::endl;
        class_names.push_back(class_name);
        for (auto instance_child : class_child->get_children("instance_solution")) {
            //Paint connection
            s << "\"" << dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("filename")->get_value() << "\" -> ";
            s << "\"" << dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value() << "\";" << std::endl;
            std::list<std::string> childs;
            for (auto transition_child : instance_child->get_children("transition")) {
                auto elem_t = dynamic_cast<xmlpp::Element *>(transition_child);
                if (elem_t->get_attribute("resulting_pool")->get_value() != "") {
                    //Write the connection for ONE resulting pool solution
                    s << "\"" << dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value() << "\""
                    << " -> \"" << elem_t->get_attribute("resulting_pool")->get_value() << "\" [label=\"" << elem_t->get_attribute("causing_component")->get_value() << ":"
                    << elem_t->get_attribute("causing_event")->get_value() << "\"];" << std::endl;
                    auto resultings = parse(elem_t,s);
                    for(auto name : resultings){
                        s << "\"" << elem_t->get_attribute("resulting_pool")->get_value() << "\" -> " << name << std::endl;
                    }
                }
            }

        }
    }
    return class_names;
}
#if 0
std::list<std::string> SolutionXML::parseTransitionSolution(xmlpp::Element *c, std::stringstream &s)
{
    if (cluster) {
        s << "subgraph \"cluster-T" << getIdentifier(c) << "\" {" << std::endl;
        s << "label = \"Transitions " << getIdentifier(c) << "\";" << std::endl;
    }
    for (auto child : c->get_children("transition")) {
        auto elem = dynamic_cast<xmlpp::Element *>(child);
        if (elem->get_attribute("resulting_pool")->get_value() != "") {
            auto name = parseInstanceSolution(dynamic_cast<xmlpp::Element *>(child), s);

            // Print the name
            s << "\"" << getIdentifier(dynamic_cast<xmlpp::Element *>(child)) << "\";" << std::endl;

            //            s << "\"" << elem->get_attribute("causing_component")->get_value() << "\"" //This is correct but we have not resolved the network names
            s << "\"" << getIdentifier(dynamic_cast<xmlpp::Element *>(child)) << "\""
                //<< " -> " << name << "[label=\"" << elem->get_attribute("causing_event")->get_value() << "\"];";
              << " -> \"" << elem->get_attribute("resulting_pool")->get_value() << "\" [label=\"" << elem->get_attribute("causing_component")->get_value() << ":"
              << elem->get_attribute("causing_event")->get_value() << "\"];";
        }
    }
    if (cluster) {
        s << "}" << std::endl;
    }
}

std::list<std::string> SolutionXML::parseInstanceSolution(xmlpp::Element *c, std::stringstream &s)
{
    assert(c);
    if (c->get_children("class_solution").size() == 0) {
        return "";
    }

    if (cluster) {
        s << "subgraph \"cluster-I" << getIdentifier(c) << "\" {" << std::endl;
        s << "label = \"Class Solution " << getIdentifier(c) << "\";" << std::endl;
    }
    for (auto child : c->get_children("class_solution")) {
        assert(dynamic_cast<xmlpp::Element *>(child));
        s << "\"" << dynamic_cast<xmlpp::Element *>(child)->get_attribute("filename")->get_value() << "\";" << std::endl;
        ;
        parseClassSolution(dynamic_cast<xmlpp::Element *>(child), s);
    }
    if (cluster) {
        s << "}" << std::endl;
    }
    return "NIL";
}
#endif

std::string SolutionXML::getDotGraph()
{
    std::stringstream result;

    result << "digraph G{" << std::endl;
    result << "compound=true;" << std::endl;
    parse(rootNode, result);
    result << "}" << std::endl;
    return result.str();
}
