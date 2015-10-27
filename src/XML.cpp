#include <libxml++/libxml++.h>
#include "XML.hpp"
#include "Pool.hpp"
#include "DataService.hpp"
#include "Task.hpp"
#include "Composition.hpp"
#include "StateMachine.hpp"
#include "SpecializedComponent.hpp"

using namespace constrained_based_networks;

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
        Component* child_component = 0;
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
            if (!found) {
                throw std::runtime_error("This should not happen it seems the child " + child_name + " does not exist in the pool");
            }
            // Now we got (hopefully the needed child)
            child_component = pool->getComponent(child_name);
        }
        assert(child_component);
        composition->addChild(child_component, child_role);
    }
}

Pool* XML::load(std::string filename)
{
    Pool* p = new Pool();
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
            const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(child);
            assert(nodeElement);
            std::string sm_name = nodeElement->get_attribute("name")->get_value();
            int start_state = atoi(nodeElement->get_attribute("start_state")->get_value().c_str());
            auto sm = new StateMachine(sm_name, p);
            std::vector<Component*> children;
            for (const auto& event_node : child->get_children("child")) {
                const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(event_node);
                assert(sub_element);
                std::string child_name = sub_element->get_attribute("name")->get_value();
                bool specialized = sub_element->get_attribute("specialized")->get_value() == "true";
                int child_id = atoi(sub_element->get_attribute("id")->get_value().c_str());
                auto child_component = p->getComponent(child_name);
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

bool XML::save(Pool* pool, std::string filename)
{
    return false;
}
