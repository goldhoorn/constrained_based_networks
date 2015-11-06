#include <unistd.h>
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
#include <boost/filesystem.hpp>
#include <openssl/md5.h>
#include <sys/stat.h>
#include <sys/types.h>


using namespace constrained_based_networks;

pthread_mutex_t XML::mutex;


Component* XML::ensureComponentAvailible(Pool* pool, std::string child_name, xmlpp::Node* const root)
{
    Component* child_component;
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
            // Okay maybe a state-machine?
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
        // std::cout << "StateMachine was already imported: " << sm_name << std::endl;
        // Already imported, skipping
        return;
    }

    // Check if the SM itself is specialized
    if (auto specialized = nodeElement->get_attribute("specialized")) {
        if (specialized->get_value() == "true") {
            auto base_component_name = nodeElement->get_attribute("base_class")->get_value();
            auto* spec_cmp = ensureComponentAvailible(pool, base_component_name, root)->getSpecialized(sm_name);
            if (auto active = nodeElement->get_attribute("active")) {
                if (active->get_value() == "true") {
                    spec_cmp->setActive(true);
                }
            }
            for (const auto& conf : child->get_children("config")) {
                const xmlpp::Element* conf_element = dynamic_cast<const xmlpp::Element*>(conf);
                auto config_name = conf_element->get_attribute("name")->get_value();
                auto config_value = conf_element->get_attribute("value")->get_value();
                spec_cmp->addConfig(config_name, config_value);
            }

            auto state_machine = dynamic_cast<StateMachine*>(spec_cmp);
            for (const auto& event_node : child->get_children("replaced_child")) {
                const xmlpp::Element* sub_element = dynamic_cast<const xmlpp::Element*>(event_node);
                assert(sub_element);
                std::string orginal_name = sub_element->get_attribute("orginal_name")->get_value();
                std::string new_name = sub_element->get_attribute("new_name")->get_value();
                auto new_child = ensureComponentAvailible(pool, new_name, root);
                auto old_child = ensureComponentAvailible(pool, orginal_name, root);
                state_machine->replaceChild(new_child,old_child);
            }
        }
        // We do not need to continue, all children are generated based on the parent by getSpecialized
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
        unsigned int child_id = (unsigned int)atoi(sub_element->get_attribute("id")->get_value().c_str());
        auto child_component = ensureComponentAvailible(pool, child_name, root);
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
        // std::cout << "Component was already imported: " << composition_name << std::endl;
        // Already imported, skipping
        return;
    }

    if (auto specialized = nodeElement->get_attribute("specialized")) {
        if (specialized->get_value() == "true") {
            auto base_component_name = nodeElement->get_attribute("base_class")->get_value();
            auto* spec_cmp = ensureComponentAvailible(pool, base_component_name, root)->getSpecialized(composition_name);
            if (auto active = nodeElement->get_attribute("active")) {
                if (active->get_value() == "true") {
                    spec_cmp->setActive(true);
                }
            }
            for (const auto& conf : child->get_children("config")) {
                const xmlpp::Element* conf_element = dynamic_cast<const xmlpp::Element*>(conf);
                auto config_name = conf_element->get_attribute("name")->get_value();
                auto config_value = conf_element->get_attribute("value")->get_value();
                spec_cmp->addConfig(config_name, config_value);
            }
            // We do not need to continue, all children are generated based on the parent by getSpecialized
            return;
        }
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
        Component* child_component = ensureComponentAvailible(pool, child_name, root);
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

        {
            auto mFilename = pNode->get_first_child("initial_model");
            assert(mFilename);
            const xmlpp::Element* nFilename = dynamic_cast<const xmlpp::Element*>(mFilename);
            assert(nFilename);
            p->source_of_model = nFilename->get_attribute("file")->get_value();
        }

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

            if (auto specialized = nodeElement->get_attribute("specialized")) {
                if (specialized->get_value() == "true") {
                    auto base_component_name = nodeElement->get_attribute("base_class")->get_value();
                    auto* spec_cmp = ensureComponentAvailible(p, base_component_name, pNode)->getSpecialized(task_name);
                    if (auto active = nodeElement->get_attribute("active")) {
                        if (active->get_value() == "true") {
                            spec_cmp->setActive(true);
                        }
                    }
                    for (const auto& conf : child->get_children("config")) {
                        const xmlpp::Element* conf_element = dynamic_cast<const xmlpp::Element*>(conf);
                        auto config_name = conf_element->get_attribute("name")->get_value();
                        auto config_value = conf_element->get_attribute("value")->get_value();
                        spec_cmp->addConfig(config_name, config_value);
                    }
                    // We do not need to continue, all children are generated based on the parent by getSpecialized
                    continue;
                }
            }
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

xmlpp::Element* XML::getUnresolvedForFollowNetwork(xmlpp::Element* root, xmlpp::Element* e,std::list<std::vector<unsigned int>> ignored_solutions)
{
    auto chilren = e->get_children("class_solution");
    auto link = e->get_children("link");

    // No children (yet) calculated, returning the parent node
    if (chilren.size() == 0 && link.size() == 0 && !findPath(getPath(root,e),ignored_solutions)) {
        return e;
    }

    for (auto child : chilren) {
        if (auto res = getUnresolvedForClassSolution(root, dynamic_cast<xmlpp::Element*>(child),ignored_solutions)) {
            return res;
        }
    }

    return 0;
}

xmlpp::Element* XML::getUnresolvedForInstanceSolution(xmlpp::Element* root, xmlpp::Element* e,std::list<std::vector<unsigned int>> ignored_solutions)
{

    for (auto child : e->get_children("transition")) {
        auto elem = dynamic_cast<xmlpp::Element*>(child);
        // If this element has a possible follow-network and no children so far, we have to calculate for this a solution
        if (elem->get_attribute("resulting_pool")->get_value() != "") {  // Okay this might be a candidate
            if (elem->get_children().size() == 0 && (!findPath(getPath(root,elem),ignored_solutions))) {
                return elem;
            }
            if (auto res = getUnresolvedForFollowNetwork(root, elem, ignored_solutions)) {
                return res;
            }
        }
    }

    return 0;
}

bool XML::findPath(std::vector<unsigned int> s, std::list<std::vector<unsigned int>> ignored_solutions){
    for(auto so : ignored_solutions){
        if( s == so)
            return true;
    }
    return false;
}

xmlpp::Element* XML::getUnresolvedForClassSolution(xmlpp::Element* root ,xmlpp::Element* e,std::list<std::vector<unsigned int>> ignored_solutions)
{
    auto children = e->get_children("instance_solution");
    if (children.size() == 0 && (!findPath(getPath(root,e),ignored_solutions))) {
        return e;
    }

    for (auto child : children) {
        if (auto res = getUnresolvedForInstanceSolution(root, dynamic_cast<xmlpp::Element*>(child), ignored_solutions)) {
            return res;
        }
    }

    return 0;
}

std::vector<unsigned int> XML::getPath(xmlpp::Element* rootNode, xmlpp::Element* element)
{
    std::vector<unsigned int> res_v;
    std::list<unsigned int> res;

    while (element != rootNode) {
        res.push_front(atoi(element->get_attribute("id")->get_value().c_str()));
        element = element->get_parent();
    }
    for (auto e : res) {
        res_v.push_back(e);
    }
    return res_v;
}

bool XML::findUnresolvedIDs(std::string filename, std::vector<unsigned int>& res_v, std::list<std::vector<unsigned int>> ignored_solutions)
{
    if (!boost::filesystem::is_regular_file(genDBFilename(filename))) {
        createDatabase(filename);
    }

    xmlpp::DomParser parser;
    parser.parse_file(genDBFilename(filename));
    assert(parser);
    auto rootNode = parser.get_document()->get_root_node();
    auto element = getUnresolvedForFollowNetwork(rootNode, rootNode,ignored_solutions);
    if (!element) {
        // Everything is done, cannot find any node which is unresolved
        return false;
    }

    res_v = getPath(rootNode, element);

    return true;
}


xmlpp::Element* XML::findNodeForID(xmlpp::Element* root, const std::vector<unsigned int>& ids)
{
    const char* attribute_name[] = {"class_solution", "instance_solution", "transition"};

    xmlpp::Element* currentElement = root;
    for (size_t i = 0; i < ids.size(); ++i) {
        // We have to determine between class and instance solutions
        // an even indice i is for a class solution, a odd for a instance solution, we are running throught the XML structure
        // by following this loop
        auto nodes = currentElement->find("child::" + std::string(attribute_name[i % 3]) + "[attribute::id='" + std::to_string(ids[i]) + "']");
        if (nodes.size() != 1) {
            throw std::runtime_error("XML file seems broken, expected only one child with id " + std::to_string(ids[i]) + " but got " + std::to_string(nodes.size()) + " while searching index " +
                                     std::to_string(i) + " with value " + std::to_string(ids[i]) + ".");
        }
        currentElement = dynamic_cast<xmlpp::Element*>(nodes[0]);
        assert(currentElement);
    }

    // Sainity check to make sure we don't (yet) have anything calculated for this
    if (currentElement->get_first_child("class_solution")) {
        throw std::runtime_error("Searched next free node to append a Solution, but current node has already class-solutions");
    }
    if (currentElement->get_first_child("instance_solution")) {
        throw std::runtime_error("Searched next free node to append a Solution, but current node has already instance-solutions");
    }
    if (currentElement->get_first_child("transition")) {
        throw std::runtime_error("Searched next free node to append a Solution, but current node has already transitions");
    }

    return currentElement;
}

std::string XML::loadSolution(std::string filename, std::vector<unsigned int> ids)
{
    const char* attribute_names[] = {"resulting_pool", "filename", "filename"};
    xmlpp::DomParser parser;
    parser.parse_file(genDBFilename(filename));
    assert(parser);

    const auto rootNode = parser.get_document()->get_root_node();
    auto parent_node_for_solution = findNodeForID(rootNode, ids);
    // Search the current toplevel node for our class-solution
    std::cout << "DEBUG: " << parent_node_for_solution->get_line() << " attribute: " << attribute_names[ids.size() % 3] << " ids_size: " << ids.size() << std::endl;

    const xmlpp::Element* solutionElement = dynamic_cast<const xmlpp::Element*>(parent_node_for_solution);
    assert(solutionElement);
    auto v = solutionElement->get_attribute(attribute_names[ids.size() % 3]);
    assert(v);
    return boost::filesystem::path(filename).remove_filename().string() + "/" + v->get_value();
}




bool XML::addInstanceSolutions(const std::string filename, std::vector<std::pair<graph_analysis::BaseGraph::Ptr, std::list<TransitionTrigger>>> instance_solutions, std::vector<unsigned int> ids)
{
    pthread_mutex_lock(&mutex);
    if ((ids.size() % 3) != 1) {
        throw std::invalid_argument("The given indexed must have a length%3 == 1, but got " + std::to_string(ids.size()));
    }

    const std::string basePath = boost::filesystem::path(filename).remove_filename().string();

    xmlpp::Document doc;
    xmlpp::DomParser parser;
    parser.parse_file(genDBFilename(filename));
    assert(parser);
    const auto rootNode = parser.get_document()->get_root_node();

    auto toplevelNode = findNodeForID(rootNode, ids);

    std::vector<TransitionHelper> calculationHelper;
    for (size_t i = 0; i < instance_solutions.size(); ++i) {
        const auto& solution = instance_solutions[i];

        // Create the filename for the network file:
        std::string instanceSolutionNetworkFilename = "data/instance-solution-" + boost::filesystem::path(filename).filename().string() + "-";
        for (auto id : ids) {
            instanceSolutionNetworkFilename += std::to_string(id) + "-";
        }
        instanceSolutionNetworkFilename += std::to_string(i) + ".gexf";
        std::cout << "Creating instance solution network file: " << instanceSolutionNetworkFilename << std::endl;
        graph_analysis::io::GraphIO::write(basePath + "/" + instanceSolutionNetworkFilename, solution.first, graph_analysis::representation::GEXF);

        auto solution_node = toplevelNode->add_child("instance_solution");
        solution_node->set_attribute("filename", instanceSolutionNetworkFilename);
        solution_node->set_attribute("id", std::to_string(i));

        // Begin calculation of follow up networks
        int cnt2 = -1;
        for (const auto& trigger : solution.second) {
            cnt2++;
            std::cout << "processing: " << i << "/" << instance_solutions.size() << " " << cnt2 << "/" << solution.second.size() << std::endl;

            auto transition_node = solution_node->add_child("transition");
            transition_node->set_attribute("id", std::to_string(cnt2));
            transition_node->set_attribute("causing_component", trigger.causing_component->getName());
            transition_node->set_attribute("causing_event", trigger.causing_event);
            if (trigger.resulting_requirement.network.size() == 0) {
                std::cout << "Ignoring follow network for : " << trigger.causing_component->getName() << " -> " << trigger.causing_event << std::endl;
                transition_node->set_attribute("resulting_pool", "");
                continue;
            } else {
                bool found = false;
                for (auto e : calculationHelper) {
                    if (e.th == trigger) {
                        std::cout << "!!!!!!!!!-------------------------------  Found a previous generated solution for our problem";
                        transition_node->set_attribute("resulting_pool", e.filename);
                        found = true;
                        if (auto node = getReferenceNodeForNetwork(rootNode, e.filename)) {
                            auto link = transition_node->add_child("link");
                            auto link_id = getPath(rootNode, node);
                            for (auto link_id_elem : link_id) {
                                link->add_child("path")->set_attribute("id",std::to_string(link_id_elem));
                            }
                        }
                        continue;
                    }
                }
                if (found) {
                    continue;
                }
                Pool* pool = XML::load(filename);
                EventModelHandler::createFollowPool(trigger, pool);
                std::string event_follow_network_filename = basePath + "/data/";
                XML::save(pool, event_follow_network_filename, true);

                std::cout << "We have a follow network for: " << trigger.causing_component->getName() << " -> " << trigger.causing_event << " And file " << event_follow_network_filename << std::endl;

                // We have to put only relative PATHs from the original file on in our solution file, we have to twesk the filename here
                event_follow_network_filename = "data/" + boost::filesystem::path(event_follow_network_filename).filename().string();

                if (auto node = getReferenceNodeForNetwork(rootNode, event_follow_network_filename)) {
                    auto link = transition_node->add_child("link");
                    auto link_id = getPath(rootNode, node);
                    for (auto link_id_elem : link_id) {
                        link->add_child("path")->set_attribute("id",std::to_string(link_id_elem));
                    }
                }

                calculationHelper.push_back({trigger, pool, event_follow_network_filename});
                transition_node->set_attribute("resulting_pool", event_follow_network_filename);
                // delete pool;
            }
        }
    }
    for (auto p : calculationHelper) {
        delete p.pool;
    }
    doc.create_root_node_by_import(rootNode, true);
    doc.write_to_file_formatted(genDBFilename(filename));
    sync();
    pthread_mutex_unlock(&mutex);
    return true;
}

xmlpp::Element* XML::getReferenceNodeForNetwork(xmlpp::Element* root, std::string md5)
{
    auto nodes = root->find("//*[@resulting_pool='" + md5 + "']");
    for (auto n : nodes) {
        std::cerr << "Check here" << std::endl;
        if (n->get_children("class_solution").size() != 0) {
            std::cerr << "Allright thats good" << std::endl;
            return dynamic_cast<xmlpp::Element*>(n);
        }
    }
    std::cerr << "Searching pool " << md5 << " not found" << std::endl;
    return 0;
}

std::string XML::loadInstanceSolution(std::string filename, std::vector<unsigned int> ids)
{
    // Trivial check search the pool for a initial requirement
    if (ids.empty()) {
        return filename;
    }

    if ((ids.size() % 3) != 0) {
        throw std::invalid_argument(std::string(__FUNCTION__) + " The given indexed must have a length%3 == 0, but got " + std::to_string(ids.size()));
    }
    return loadSolution(filename, ids);
}

std::string XML::loadClassSolution(std::string filename, std::vector<unsigned int> ids)
{
    if ((ids.size() % 3) != 1) {
        throw std::invalid_argument(std::string(__FUNCTION__) + " The given indexed must have a length%3 == 1, but got " + std::to_string(ids.size()));
    }
    return loadSolution(filename, ids);
}

std::string XML::toMD5(const std::string input)
{
    unsigned char result[MD5_DIGEST_LENGTH];
    MD5((unsigned char*)input.c_str(), input.size(), result);
    std::stringstream str;
    str << std::hex << std::setfill('0');
    for (size_t i = 0; i < MD5_DIGEST_LENGTH; ++i) {
        str << std::setw(2) << static_cast<unsigned>(result[i]);
    }
    return str.str();
}

std::string XML::genDBFilename(const std::string modelFilename)
{
    boost::filesystem::path dir(modelFilename);
    dir.remove_filename();
    boost::filesystem::path file(modelFilename);
    file = file.filename();
    return dir.string() + "/solutions-" + file.string();
}

void XML::createDatabase(std::string original_file)
{
    xmlpp::Document doc;
    auto rootNode = doc.create_root_node("root");
    auto model_node = rootNode->add_child("initial_model");
    model_node->set_attribute("file", boost::filesystem::path(original_file).filename().string());
    doc.write_to_file_formatted(XML::genDBFilename(original_file));
}

bool XML::saveClassSolutions(std::vector<graph_analysis::BaseGraph::Ptr> class_solutions, std::string original_file, std::list<std::string> additionalRequirements, std::vector<unsigned int> id)
{
    pthread_mutex_lock(&mutex);
    if ((id.size() % 3) != 0) {
        throw std::invalid_argument("The given indexed must have a length%3 == 0, but got " + std::to_string(id.size()));
    }

    // Sainity check if solution already exists
    //

    if (!boost::filesystem::is_regular_file(genDBFilename(original_file))) {
        createDatabase(original_file);
    }

    // Open The Database
    xmlpp::Document doc;
    xmlpp::DomParser parser;
    parser.parse_file(genDBFilename(original_file));
    assert(parser);
    auto orginal_root = parser.get_document()->get_root_node();

    auto parent_node_for_solution = findNodeForID(orginal_root, id);
#if 0
    // Okay now we do have the most-toplevel class solution id, we have no created a new class solution, therefore we need to find the highest non-uses id of the previous solution
    int max_id = 0;
    for (auto n : parent_node_for_solution->get_children("class_solution")) {
        const xmlpp::Element* nodeElement = dynamic_cast<const xmlpp::Element*>(n);
        assert(nodeElement);
        int cur_id = atoi(nodeElement->get_attribute("id")->get_value().c_str());
        if (cur_id > max_id) {
            max_id = cur_id;
        }
    }
    max_id++;
#endif

    mkdir((boost::filesystem::path(original_file).remove_filename().string() + "/data/").c_str(), 0755);

    unsigned int solution_id = 0;
    for (auto solution : class_solutions) {
        // Generate the filename for the file
        std::string class_solution_network_filename = "data/class-network-" + boost::filesystem::path(original_file).filename().string() + "-";
        for (auto i : id) {
            class_solution_network_filename += std::to_string(i) + "-";
        }
        class_solution_network_filename += std::to_string(solution_id) + ".gexf";

        auto solution_node = parent_node_for_solution->add_child("class_solution");
        solution_node->set_attribute("filename", class_solution_network_filename);
        graph_analysis::io::GraphIO::write(boost::filesystem::path(original_file).remove_filename().string() + "/" + class_solution_network_filename, solution, graph_analysis::representation::GEXF);
        solution_node->set_attribute("id", std::to_string(solution_id));
        for (auto req : additionalRequirements) {
            solution_node->add_child("requirement", req);
        }
        ++solution_id;
    }

    doc.create_root_node_by_import(orginal_root, true);  // Create a document which is based on this
    doc.write_to_file_formatted(genDBFilename(original_file));

#if 0
    if (!md5) {
    } else {
        std::string xml_result = doc.write_to_string();
        outfile = toMD5(doc.write_to_string());
        boost::filesystem::path p(original_file);
        p.remove_filename();
        p += "/";
        p += outfile;
        std::cout << "Write to: " << p.string() << std::endl;
        doc.write_to_file_formatted(p.string());
    }
#endif
    sync();
    pthread_mutex_unlock(&mutex);
    return true;
}

void XML::addSpecialization(Component* comp, xmlpp::Element* const root)
{
    if (auto spec = dynamic_cast<SpecializedComponentBase*>(comp)) {
        root->set_attribute("specialized", "true");

        // This is currently unsupported and should be resolved anywhere else before
        assert(!dynamic_cast<SpecializedComponentBase*>(spec->getOrginal()));

        root->set_attribute("base_class", spec->getOrginal()->getName());
        for (auto c : spec->configuration) {
            auto cNode = root->add_child("config");
            cNode->set_attribute("name", c.first);
            cNode->set_attribute("value", c.second);
        }
    }
}

bool XML::save(Pool* pool, std::string& filename, bool md5)
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
    {
        if (pool->source_of_model.empty()) {
            pool->source_of_model = filename;
        }
        rootNode->add_child("initial_model")->set_attribute("file", pool->source_of_model);
    }
    for (auto component : pool->getItems<Component*>()) {
        if (component->getName() == "root-knot" || component->getName() == "NIL-Task") continue;

        if (auto sm = dynamic_cast<StateMachine*>(component)) {
            auto smNode = rootNode->add_child("state_machine");
            smNode->set_attribute("name", sm->getName());
            addSpecialization(component, smNode);

            if (auto spec = dynamic_cast<SpecializedComponentBase*>(component)) {
                //TODO replace here the children and continue with the replace-child implementatipn to the specialized component.
                //remove replace-child from state-machine and from composition
                //
                
            }

            // this should be valid because the transition 0 is by default the starting state, and the 1 (not 0) child is the target state to enter
            // 0 and 2 are the SM itselfe
            assert(sm->getTransitions()[0].event == "start");
            smNode->set_attribute("start_state", "0");

            smNode->set_attribute("active", sm->isActive() ? "true" : "false");
            std::vector<Component*> children;
            unsigned int child_id = 0;
            bool first = true;
            for (auto smStates : sm->getTransitions()) {
                // We don't need to export the start-state transition this is create by setStart
                if (first) {
                    first = false;
                    {
                        auto childNode = smNode->add_child("child");
                        childNode->set_attribute("id", std::to_string(child_id++));
                        childNode->set_attribute("name", smStates.target->getName());
                        // TODO we could here figure out the base class and specialize it on or own (or we simply using the specialized as done here
                        // This somehow changes the model but should not change the behaviour in the end
                        childNode->set_attribute("specialized", "false");  // Spcialized would mean that a specilaization is explicitly done
                    }
                    continue;
                }

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
            addSpecialization(component, cNode);
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
            addSpecialization(component, cNode);
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

    if (!md5) {
        doc.write_to_file_formatted(filename);
    } else {
        filename = filename + "/" + toMD5(doc.write_to_string());
        doc.write_to_file_formatted(filename);
    }
    return true;
}
