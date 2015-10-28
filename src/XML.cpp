#include <libxml++/libxml++.h>
#include "XML.hpp"
#include "Pool.hpp"
#include "DataService.hpp"
#include "Task.hpp"
#include "Composition.hpp"
#include "StateMachine.hpp"
#include "SpecializedComponent.hpp"
#include "EventModelHandler.hpp"
#include <graph_analysis/GraphIO.hpp>

using namespace constrained_based_networks;

Component* XML::ensureComponentAvailible(Pool *pool, std::string child_name, xmlpp::Node* const root){
        Component *child_component;
        try
        {
            child_component = pool->getComponent(child_name);
        }
        catch (std::invalid_argument c)
        {
            // We need to include the other one before, so we are searching the element in the XML graph and call outself
            bool found = false;
            for (const auto& c : root->get_children("composition")) {
                const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(c);
                assert(nodeElement);
                std::string c_name = nodeElement->get_attribute("name")->get_value();
                DEBUG_XML << "Compare: " << c_name << " with " << child_name << std::endl;
                if (child_name == c_name) {
                    DEBUG_XML << "Now import first another component" << c_name << std::endl;
                    importComposition(pool, c, root);
                    found = true;
                    break;
                }
            }
            if(!found){
                //Okay maybe a state-machine?
                for (const auto& c : root->get_children("state_machine")) {
                    const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(c);
                    assert(nodeElement);
                    std::string c_name = nodeElement->get_attribute("name")->get_value();
                    DEBUG_XML << "Compare: " << c_name << " with " << child_name << std::endl;
                    if (child_name == c_name) {
                        DEBUG_XML << "Now import first another StateMachine" << c_name << std::endl;
                        importSM(pool, c, root);
                        found = true;
                        break;
                    }
                }
            }
            if (!found) {
                throw std::runtime_error("This should not happen it seems the child " + child_name + " does not exist in the pool");
            }

            // Now we got (hopefully the needed child)
            child_component = pool->getComponent(child_name);
        }
        assert(child_component);
        return child_component;
}


void XML::importSM(Pool* pool, xmlpp::Node* const child, xmlpp::Node* const root)
{
    const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(child);
    assert(nodeElement);
    std::string sm_name = nodeElement->get_attribute("name")->get_value();
    if (pool->hasComponent(sm_name)) {
        std::cout << "StateMachine was already imported: " << sm_name << std::endl;
        // Already imported, skipping
        return;
    }
    int start_state = atoi(nodeElement->get_attribute("start_state")->get_value().c_str());
    auto sm = new StateMachine(sm_name, pool);
    if (auto active = nodeElement->get_attribute("active")) {
        if (active->get_value() == "true") {
            sm->setActive(true);
        }
    }
    std::vector<Component*> children;
    for (const auto& event_node : child->get_children("child")) {
        const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(event_node);
        assert(sub_element);
        std::string child_name = sub_element->get_attribute("name")->get_value();
        bool specialized = sub_element->get_attribute("specialized")->get_value() == "true";
        int child_id = atoi(sub_element->get_attribute("id")->get_value().c_str());
        auto child_component = ensureComponentAvailible(pool,child_name,root);
        assert(child_component);
        if (specialized) {
            DEBUG_XML << "Creating a specialized component" << std::endl;
            auto spec = child_component->getSpecialized();
            for (const auto& c_node : sub_element->get_children("config")) {
                const xmlpp::Element* c_element = dynamic_cast<const xmlpp::Element*>(c_node);
                assert(c_element);
                std::string config_name = c_element->get_attribute("name")->get_value();
                std::string config_value = c_element->get_attribute("value")->get_value();
                spec->addConfig(config_name, config_value);
            }
            children.push_back(dynamic_cast<Component*>(spec));
        } else {
            children.push_back(child_component);
        }
        assert((children.size() - 1) == child_id);
    }
    for (const auto& sub_node : child->get_children("transition")) {
        const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(sub_node);
        assert(sub_element);
        int source = atoi(sub_element->get_attribute("source")->get_value().c_str());
        int target = atoi(sub_element->get_attribute("target")->get_value().c_str());
        int trigger = atoi(sub_element->get_attribute("trigger")->get_value().c_str());
        std::string event = sub_element->get_attribute("event")->get_value();
        sm->addTransition(children[source], children[target], children[trigger], event);
    }
    sm->setStart(children[start_state]);
}

void XML::importComposition(Pool* pool, xmlpp::Node* const child, xmlpp::Node* const root)
{
    const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(child);
    assert(nodeElement);
    std::string composition_name = nodeElement->get_attribute("name")->get_value();
    if (pool->hasComponent(composition_name)) {
        std::cout << "Component was already imported: " << composition_name << std::endl;
        // Already imported, skipping
        return;
    }

    DEBUG_XML << "Importing Composition: " << composition_name << std::endl;
    // This adds the DS automatically to the pool
    auto composition = new Composition(composition_name, pool);
    if (auto active = nodeElement->get_attribute("active")) {
        if (active->get_value() == "true") {
            composition->setActive(true);
        }
    }
    for (const auto& event_node : child->get_children("event")) {
        const xmlpp::Element* event_node_element = dynamic_cast<const xmlpp::Element*>(event_node);
        assert(event_node_element);
        std::string event_name = event_node_element->get_attribute("name")->get_value();
        composition->addEvent(event_name);
    }
    for (const auto& sub_node : child->get_children("fullfills")) {
        const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(sub_node);
        assert(sub_element);
        std::string service = sub_element->get_attribute("service")->get_value();
        composition->addFullfillment(service);
    }
    for (const auto& sub_node : child->get_children("child")) {
        const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(sub_node);
        assert(sub_element);
        std::string child_name = sub_element->get_attribute("name")->get_value();
        std::string child_role = sub_element->get_attribute("role")->get_value();
        DEBUG_XML << "Adding child " << child_name << " as " << child_role << " to " << composition_name << std::endl;
        Component* child_component = ensureComponentAvailible(pool,child_name,root);
        composition->addChild(child_component, child_role);
    }
}

Pool* XML::load(std::string filename)
{
    Pool* p = new Pool;
    xmlpp::DomParser parser;
    parser.parse_file(filename);
    if (parser) {
        const auto pNode = parser.get_document()->get_root_node();

        // Import all DataServices
        for (const auto& child : pNode->get_children("data_service")) {
            const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(child);
            assert(nodeElement);
            std::string ds_name = nodeElement->get_attribute("name")->get_value();
            DEBUG_XML << "Importing DS: " << ds_name << std::endl;
            // This adds the DS automatically to the pool
            new DataService(ds_name, p);
        }

        // Import all Tasks
        for (const auto& child : pNode->get_children("task")) {
            const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(child);
            assert(nodeElement);
            std::string task_name = nodeElement->get_attribute("name")->get_value();
            // This adds the DS automatically to the pool
            auto task = new Task(task_name, p);
            if (auto active = nodeElement->get_attribute("active")) {
                if (active->get_value() == "true") {
                    task->setActive(true);
                }
            }
            DEBUG_XML << "Importing task: " << task_name << std::endl;
            for (const auto& event_node : child->get_children("event")) {
                const xmlpp::Element* event_node_element = dynamic_cast<const xmlpp::Element*>(event_node);
                assert(event_node_element);
                std::string event_name = event_node_element->get_attribute("name")->get_value();
                task->addEvent(event_name);
            }
            for (const auto& sub_node : child->get_children("config_file")) {
                const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(sub_node);
                assert(sub_element);
                std::string section = sub_element->get_attribute("section")->get_value();
                std::string property = sub_element->get_attribute("property")->get_value();
                std::string value = sub_element->get_attribute("value")->get_value();
                task->addConfFileProperty(section, property, value);
            }
            for (const auto& sub_node : child->get_children("fullfills")) {
                const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(sub_node);
                assert(sub_element);
                std::string service = sub_element->get_attribute("service")->get_value();
                task->addFullfillment(service);
            }
            for (const auto& sub_node : child->get_children("property")) {
                const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(sub_node);
                assert(sub_element);
                std::string name = sub_element->get_attribute("name")->get_value();
                std::string type = sub_element->get_attribute("type")->get_value();
                task->addProperty(name, type);
            }
        }

        // Import all Compositions
        for (const auto& child : pNode->get_children("composition")) {
            XML::importComposition(p, child, pNode);
        }

        // Import all StateMachines
        for (const auto& child : pNode->get_children("state_machine")) {
            XML::importSM(p, child, pNode);
        }
        // Import all Constraints
        for (const auto& child : pNode->get_children("constraint")) {
            const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(child);
            assert(nodeElement);
            std::string component = nodeElement->get_attribute("component")->get_value();
            std::string child_c = nodeElement->get_attribute("child")->get_value();
            std::string target = nodeElement->get_attribute("target")->get_value();
            auto c = p->getComponent(component);
            assert(c);
            auto cmp = dynamic_cast<Composition*>(c);
            assert(cmp);
            cmp->addConstraint(child_c, target);
        }
    } else {
        throw std::runtime_error("Have no parser?");
    }
    p->mergeDoubles();
    return p;
}

void iterate(const xmlpp::Node* n, const std::string name, std::function<void(const xmlpp::Element* data)> f)
{
    for (const auto& data : n->get_children(name)) {
        const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(data);
        assert(nodeElement);
        f(nodeElement);
    }
}

std::vector<ClassSolutionImport> XML::loadClassSolutions(std::string filename)
{
    std::vector<ClassSolutionImport> res;
    xmlpp::DomParser parser;
    parser.parse_file(filename);
    assert(parser);

    const auto pNode = parser.get_document()->get_root_node();

    // Import all DataServices
    iterate(pNode, "class_instance", [&](const xmlpp::Element* data) {
        int instance_id = atoi(data->get_attribute("id")->get_value().c_str());
        std::string instance_filename = data->get_attribute("filename")->get_value();
        std::cout << "imported " << instance_filename << " id: " << instance_id << std::endl;
        res.push_back({instance_filename, instance_id});
    });
    return res;
}

bool XML::addInstanceSolutions(int classSolutionID, std::vector<std::pair<graph_analysis::BaseGraph::Ptr, std::list<TransitionTrigger>>> instance_solutions, std::string original_file,
                               std::string outfile)
{
    xmlpp::Document doc;
    xmlpp::DomParser parser;
    parser.parse_file(original_file);
    assert(parser);
    const auto orginal_root = parser.get_document()->get_root_node();
    auto elem = doc.create_root_node_by_import(orginal_root, true);  // Create a document which is based on this
    unsigned int i = 0;
    for (const auto& solution : instance_solutions) {
        auto solution_node = elem->add_child("solution");
        std::stringstream s;
        s << outfile << "-instance_network-" << i++;
        solution_node->set_attribute("filename", s.str());
        solution_node->set_attribute("class_solution_id", std::to_string(classSolutionID));
        graph_analysis::io::GraphIO::write(s.str(), solution.first, graph_analysis::representation::GEXF);
        solution_node->set_attribute("id", std::to_string(i));
        int cnt2 = 0;
        for (const auto& trigger : solution.second) {
            if (trigger.causing_event == "aborted" || trigger.causing_event == "internal_error" || trigger.causing_event == "fatal_error") continue;

            cnt2++;
            std::cout << "processing: " << i << "/" << instance_solutions.size() << " " << cnt2 << "/" << solution.second.size() << std::endl;
            auto transition_node = solution_node->add_child("transition");
            transition_node->set_attribute("causing_component", trigger.causing_component->getName());
            transition_node->set_attribute("causing_event", trigger.causing_event);
            if (trigger.resulting_requirement.network.size() == 0) {
                std::cout << "Ignoring follow network for : " << trigger.causing_component->getName() << " -> " << trigger.causing_event << std::endl;
                transition_node->set_attribute("resulting-pool", "");
                continue;
            } else {
                std::stringstream event_follow_network_filename;
                event_follow_network_filename << s.str() << "-" << trigger.causing_component->getName() << "-" << trigger.causing_event;
                std::cout << "We have a follow network for: " << trigger.causing_component->getName() << " -> " << trigger.causing_event << " And file " << event_follow_network_filename.str()
                          << std::endl;
                // First we load the orginal pool, then we add our components we have to create in order to re-run our network resolution
                Pool* pool = XML::load(original_file);
                EventModelHandler::createFollowPool(trigger, pool);
                XML::save(pool, event_follow_network_filename.str());
                delete pool;
            }
        }
    }
    doc.write_to_file_formatted(outfile);
    return true;
}

bool XML::save(std::vector<graph_analysis::BaseGraph::Ptr> class_solutions, std::string original_file, std::string outfile)
{
    xmlpp::Document doc;
    xmlpp::DomParser parser;
    parser.parse_file(original_file);
    assert(parser);
    const auto orginal_root = parser.get_document()->get_root_node();
    auto elem = doc.create_root_node_by_import(orginal_root, true);  // Create a document which is based on this
    unsigned int i = 0;
    for (auto solution : class_solutions) {
        auto solution_node = elem->add_child("class_instance");
        std::stringstream s;
        s << outfile << "-class_network-" << i;
        solution_node->set_attribute("filename", s.str());
        graph_analysis::io::GraphIO::write(s.str(), solution, graph_analysis::representation::GEXF);
        solution_node->set_attribute("id", std::to_string(i));
    }
    doc.write_to_file_formatted(outfile);
    return true;
}

bool XML::save(Pool* pool, const std::string filename)
{
    /*
    for (auto component : pool->getItems<Component*>()) {
        if(component->isActive()){
            assert(component->isInPool(pool));
            std::cout << "Component: " << component->getName() << " / " << component->toString() << "is active" << std::endl;
            std::cout << "Pool pointer: " << pool << std::endl;
        }
    };
    */

    xmlpp::Document doc;
    auto rootNode = doc.create_root_node("root");
    for (auto component : pool->getItems<Component*>()) {
        if (component->getName() == "root-knot" || component->getName() == "NIL-Task") continue;

        if (auto sm = dynamic_cast<StateMachine*>(component)) {
            auto smNode = rootNode->add_child("state_machine");
            smNode->set_attribute("name", sm->getName());

            // this should be valid because the transition 0 is by default the starting state, and the 1 (not 0) child is the target state to enter
            // 0 and 2 are the SM itselfe
            assert(sm->getTransitions()[0].event == "start");
            smNode->set_attribute("start_state", "1");

            smNode->set_attribute("active", sm->isActive() ? "true" : "false");
            std::vector<Component*> children;
            unsigned int child_id = 0;
            for (auto smStates : sm->getTransitions()) {
                {
                    auto childNode = smNode->add_child("child");
                    childNode->set_attribute("id", std::to_string(child_id++));
                    childNode->set_attribute("name", smStates.source->getName());
                    // TODO we could here figure out the base class and specialize it on or own (or we simply using the specialized as done here
                    // This somehow changes the model but should not change the behaviour in the end
                    childNode->set_attribute("specialized", "false");  // Spcialized would mean that a specilaization is explicitly done
                }
                {
                    auto childNode = smNode->add_child("child");
                    childNode->set_attribute("id", std::to_string(child_id++));
                    childNode->set_attribute("name", smStates.target->getName());
                    // TODO we could here figure out the base class and specialize it on or own (or we simply using the specialized as done here
                    // This somehow changes the model but should not change the behaviour in the end
                    childNode->set_attribute("specialized", "false");  // Spcialized would mean that a specilaization is explicitly done
                }
                {
                    auto childNode = smNode->add_child("child");
                    childNode->set_attribute("id", std::to_string(child_id++));
                    childNode->set_attribute("name", smStates.event_source->getName());
                    // TODO we could here figure out the base class and specialize it on or own (or we simply using the specialized as done here
                    // This somehow changes the model but should not change the behaviour in the end
                    childNode->set_attribute("specialized", "false");  // Spcialized would mean that a specilaization is explicitly done
                }
                auto childNode = smNode->add_child("transition");
                childNode->set_attribute("source", std::to_string(child_id - 3));
                childNode->set_attribute("target", std::to_string(child_id - 2));
                childNode->set_attribute("trigger", std::to_string(child_id - 1));
                childNode->set_attribute("event", smStates.event);
            }
        } else if (auto composition = dynamic_cast<Composition*>(component)) {
            auto cNode = rootNode->add_child("composition");
            cNode->set_attribute("name", composition->getName());
            cNode->set_attribute("active", composition->isActive() ? "true" : "false");
            for (auto e : composition->getEvents()) {
                cNode->add_child("event")->set_attribute("name", e);
            }
            for (auto e : composition->getProperties()) {
                auto p = cNode->add_child("property");
                p->set_attribute("name", e.name);
                p->set_attribute("type", e.typeAsString());
            }
            for (auto e : composition->getEvents()) {
                cNode->add_child("emits")->set_attribute("name", e);
            }
            for (auto e : composition->getFullfillments()) {
                cNode->add_child("fullfills")->set_attribute("service", e);
            }
            for (auto c : composition->getChildren()) {
                auto childNode = cNode->add_child("child");
                childNode->set_attribute("name", c.second->getName());
                childNode->set_attribute("role", c.first);
            }
        } else if (auto task = dynamic_cast<Task*>(component)) {
            auto cNode = rootNode->add_child("task");
            cNode->set_attribute("active", task->isActive() ? "true" : "false");
            cNode->set_attribute("name", task->getName());
            for (auto e : task->getEvents()) {
                cNode->add_child("event")->set_attribute("name", e);
            }
            for (auto e : task->getProperties()) {
                auto p = cNode->add_child("property");
                p->set_attribute("name", e.name);
                p->set_attribute("type", e.typeAsString());
            }
            for (auto e : task->getEvents()) {
                cNode->add_child("emits")->set_attribute("name", e);
            }
            for (auto e : task->getFullfillments()) {
                cNode->add_child("fullfills")->set_attribute("service", e);
            }
        } else if (auto ds = dynamic_cast<DataService*>(component)) {
            rootNode->add_child("data_service")->set_attribute("name", ds->getName());
        } else {
            throw std::runtime_error("Got unknown component " + component->getName());
        }
    }
    doc.write_to_file_formatted(filename);
    return true;
}
