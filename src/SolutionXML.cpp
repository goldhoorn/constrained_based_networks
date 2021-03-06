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
    short_failures = true;
}

SolutionXML::~SolutionXML()
{
    delete parser;
}

std::string SolutionXML::getIdentifier(xmlpp::Node *c)
{
    assert(c);
    auto erg = XML::getPathAsElements(rootNode, dynamic_cast<xmlpp::Element *>(c));
    std::stringstream s;
    s << "Network" << std::endl;
    for (size_t i = 0; i < erg.size(); ++i) {
        auto e = erg[i];
        if ((i % 3) == 0) {

        } else if ((i % 3) == 1) {
        } else if ((i % 3) == 2) {
            s << e->get_attribute("causing_component")->get_value() << ":" << e->get_attribute("causing_event")->get_value() << std::endl;  //"<br/>";
        }
    }
    return s.str();
}

void SolutionXML::addLabelIfNotExist(std::stringstream &s, std::string node, std::string label)
{
    if (labelExists.find(node) == labelExists.end()) {
        // s << "\"" << node << "\"[label = \"" << label << "\"];";
        s << "\"" << node << "\"[label = \"" << node << "\n" << label << "\"];";
        labelExists.insert(node);
    }
}

void SolutionXML::addConnectionIfNotExist(std::stringstream &s, std::string source, std::string target, std::string label)
{
    std::string conn = "\"" + source + "\" -> \"" + target + "\"";
    if (connExists.find(conn) == connExists.end()) {
        if (label.empty()) {
            s << conn << ";\n";
        } else {
            s << conn << " [label=\"" << label << "\"] ;\n";
        }
        connExists.insert(conn);
    }
}

/**
 * This function walks throught incance-solution, to the input is a class-solution node
 *  it paints the cluster around each instance and connects each instance with <this>
 */
std::list<std::pair<std::string, std::string>> SolutionXML::parseOnlyStates(xmlpp::Element *c, std::stringstream &s)
{
    std::list<std::pair<std::string, std::string>> class_names;

    for (auto class_child : c->get_children("class_solution")) {
        //        auto class_id = atoi(dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("id")->get_value().c_str());
        // Paint one allone to make sure everything is displayed
        if (dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("unsolveable") && dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("unsolveable")->get_value() == "true") {
            //    addLabelIfNotExist(s, getIdentifier(class_child), "UNSOLVEABLE C-" + std::to_string(class_id));
            //    class_names.push_back(getIdentifier(class_child));
            continue;
        }

        // std::string class_name = dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("filename")->get_value();
        // addLabelIfNotExist(s, class_name, "C-" + std::to_string(class_id));

        // class_names.push_back(class_name);
        for (auto instance_child : class_child->get_children("instance_solution")) {
            //            auto instance_id = atoi(dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("id")->get_value().c_str());
            // addLabelIfNotExist(s, dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value(), "I-" + std::to_string(instance_id));

            // Paint connection
            // s << "\"" << dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("filename")->get_value() << "\" -> ";
            // s << "\"" << dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value() << "\";" << std::endl;
            std::list<std::string> childs;

            std::map<std::string, std::string> connections;

            for (auto transition_child : instance_child->get_children("transition")) {
                auto elem_t = dynamic_cast<xmlpp::Element *>(transition_child);
                if (elem_t->get_attribute("resulting_pool")->get_value() != "") {
                    // Write the connection for ONE resulting pool solution
                    addLabelIfNotExist(s, elem_t->get_attribute("resulting_pool")->get_value(), getIdentifier(transition_child));

                    std::string conn("\"" + dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value() + "\" -> \"" + elem_t->get_attribute("resulting_pool")->get_value() +
                                     "\" ");

                    // Need to initialize first
                    if (short_failures) {
                        std::string event = elem_t->get_attribute("causing_event")->get_value();
                        if (event == "fatal_error" || event == "internal_error" || event == "aborted") {
                            //event = "failed";
                        }else{
                            connections[conn] = connections[conn] + elem_t->get_attribute("causing_component")->get_value() + ":" + event;
                        }
                    } else {
                        connections[conn] = connections[conn] + elem_t->get_attribute("causing_component")->get_value() + ":" + elem_t->get_attribute("causing_event")->get_value();
                    }
                    /*
                                        s << "\"" << dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value() << "\""
                                          << " -> \"" << elem_t->get_attribute("resulting_pool")->get_value() << "\" [label=\"" << elem_t->get_attribute("causing_component")->get_value() << ":"
                                          << elem_t->get_attribute("causing_event")->get_value() << "\"];" << std::endl;
                                          */
                    class_names.push_back(std::make_pair(elem_t->get_attribute("resulting_pool")->get_value(), connections[conn]));
                    auto resultings = parseOnlyStates(elem_t, s);
                    for (auto name : resultings) {
                        addConnectionIfNotExist(s, elem_t->get_attribute("resulting_pool")->get_value(), name.first, name.second);
                        // s << "[label=\"" << elem_t->get_attribute("causing_component")->get_value() << ":" << elem_t->get_attribute("causing_event")->get_value() << "\"];" << std::endl;
                    }
                }
            }
        }
    }
    return class_names;
}

/**
 * This function walks throught incance-solution, to the input is a class-solution node
 *  it paints the cluster around each instance and connects each instance with <this>
 */
std::list<std::string> SolutionXML::parse(xmlpp::Element *c, std::stringstream &s)
{
    std::list<std::string> class_names;

    for (auto class_child : c->get_children("class_solution")) {
        auto class_id = atoi(dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("id")->get_value().c_str());
        // Paint one allone to make sure everything is displayed
        if (dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("unsolveable") && dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("unsolveable")->get_value() == "true") {
            addLabelIfNotExist(s, getIdentifier(class_child), "UNSOLVEABLE C-" + std::to_string(class_id));
            class_names.push_back(getIdentifier(class_child));
            continue;
        }

        std::string class_name = dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("filename")->get_value();
        addLabelIfNotExist(s, class_name, "C-" + std::to_string(class_id));

        class_names.push_back(class_name);
        for (auto instance_child : class_child->get_children("instance_solution")) {
            auto instance_id = atoi(dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("id")->get_value().c_str());
            addLabelIfNotExist(s, dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value(), "I-" + std::to_string(instance_id));

            // Paint connection
            s << "\"" << dynamic_cast<xmlpp::Element *>(class_child)->get_attribute("filename")->get_value() << "\" -> ";
            s << "\"" << dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value() << "\";" << std::endl;
            std::list<std::string> childs;

            std::map<std::string, std::string> connections;

            for (auto transition_child : instance_child->get_children("transition")) {
                auto elem_t = dynamic_cast<xmlpp::Element *>(transition_child);
                if (elem_t->get_attribute("resulting_pool")->get_value() != "") {
                    // Write the connection for ONE resulting pool solution
                    addLabelIfNotExist(s, elem_t->get_attribute("resulting_pool")->get_value(), getIdentifier(transition_child));
                    std::string conn("\"" + dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value() + "\" -> \"" + elem_t->get_attribute("resulting_pool")->get_value() +
                                     "\" ");

                    std::string event = elem_t->get_attribute("causing_event")->get_value();
//                    std::cout << "Resulting pool is" << elem_t->get_attribute("resulting_pool")->get_value() << " for event " << event << std::endl; 
                    // Need to initialize first
                    if (short_failures) {
                        if (event == "fatal_error" || event == "internal_error" || event == "aborted") {
                            event = "failed";
                        }else{
                            connections[conn] = connections[conn] + elem_t->get_attribute("causing_component")->get_value() + ":" + event + "\n";
                        }
                    } else {
                        connections[conn] = connections[conn] + elem_t->get_attribute("causing_component")->get_value() + ":" + elem_t->get_attribute("causing_event")->get_value() + "\n";
                    }
                    /*
                                        s << "\"" << dynamic_cast<xmlpp::Element *>(instance_child)->get_attribute("filename")->get_value() << "\""
                                          << " -> \"" << elem_t->get_attribute("resulting_pool")->get_value() << "\" [label=\"" << elem_t->get_attribute("causing_component")->get_value() << ":"
                                          << elem_t->get_attribute("causing_event")->get_value() << "\"];" << std::endl;
                                          */
                    auto resultings = parse(elem_t, s);
                    for (auto name : resultings) {
                        s << "\"" << elem_t->get_attribute("resulting_pool")->get_value() << "\" -> \"" << name << "\";" << std::endl;
                    }
                }
            }
            for (auto c : connections) {
                s << c.first << " [label=\"" << c.second << "\"];" << std::endl;
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

std::string SolutionXML::getDotGraph(bool onlyStates)
{
    std::stringstream result;

    result << "digraph G{" << std::endl;
    result << "compound=true;" << std::endl;

    auto initial_pool = rootNode->get_attribute("resulting_pool")->get_value();
    if (onlyStates) {
        addLabelIfNotExist(result, rootNode->get_attribute("resulting_pool")->get_value(), "Start");
        //result << "Start;" << std::endl;
        auto r = parseOnlyStates(rootNode, result);
        for (auto c : r) {
            //addConnectionIfNotExist(result, "Start", c.first, c.second);
            addConnectionIfNotExist(result, rootNode->get_attribute("resulting_pool")->get_value(), c.first, c.second);
        }
    } else {
        addLabelIfNotExist(result, rootNode->get_attribute("resulting_pool")->get_value(), getIdentifier(rootNode));
        auto resultings = parse(rootNode, result);
        for (auto name : resultings) {
            result << "\"" << rootNode->get_attribute("resulting_pool")->get_value() << "\" -> \"" << name << "\";" << std::endl;
        }
    }
    result << "}" << std::endl;
    return result.str();
}
