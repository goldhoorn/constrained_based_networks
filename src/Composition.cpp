#include "Composition.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"
#include "SpecializedComponent.hpp"

namespace constrained_based_networks
{

Composition::Composition(Pool *pool) : Component(pool)
{
}

Composition::Composition(std::string name, Pool *pool) : Component(pool)
{
    this->name = name;
    // cmp_id = pool->getItems<Composition *>().size() - 1;
    //    std::cout << "Creating composition: " << name << std::endl;
}

Forwards Composition::getArgumentForwards(Component *child)
{
    return argument_forwards[child];
}

bool Composition::hasChild(Component *child)
{
    std::cout << this->getName() << "<->" << child->getName() << std::endl;
    Component *c = child;
    while (auto spec = dynamic_cast<SpecializedComponentBase *>(c)) {
        c = spec->getOrginal();
    }

    for (auto ch : getChildren()) {
        std::cout << "\t Child: " << ch.second->getName() << std::endl;
        if (ch.second == c || (ch.second == child)) {
            return true;
        }
    }
    return false;
}

Forwards Composition::getEventForwards(Component *child, std::string name)
{
    if (children.find(name) == children.end()) {
        if (this->getName() != "root-knot") {
            std::cerr << "This is really bad !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! could not find child " << name << std::endl;
        }
        return Forwards();
    }
    child = children[name];

    /*
    std::cout << "Forwards for " << this->getName() << " and child " << child->getName() << std::endl;
    for (auto e : event_forwards[child]) {
        std::cout << "-\t" << e.first << " to " << e.second << std::endl;
    }
    if (!hasChild(child)) {
        std::cout << "THIS IS BADDDDDDDDDDDDDDDDDDDDDDDD !!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << std::endl;
    } else {
        std::cout << "THIS IS GOOOOOOOOOOOOOOOOODDDDDDDDD" << std::endl;
    }
    */

    return event_forwards[child];
}

bool Composition::operator==(const Composition &comp) const
{
    return name == comp.name;
}

void Composition::addArgumentForwards(std::string child, std::string source, std::string target)
{
    try
    {
        auto &c = children.at(child);
        argument_forwards[c][source] = target;
    }
    catch (std::out_of_range e)
    {
        std::cerr << "Could not find child \"" << child << "\"" << std::endl;
        std::cerr << "Possible children are: " << std::endl;
        for (auto child : children) {
            std::cerr << "- " << child.first << std::endl;
        }
        std::cerr << std::endl;
        throw e;
    }
}

Component *Composition::searchCorresponding(Component *c, Pool *pool)
{
    if (auto spec = dynamic_cast<SpecializedComponentBase *>(c)) {
        // search for object in DB
        for (auto pci : pool->getItems<Component *>()) {
            if (auto pc = dynamic_cast<SpecializedComponentBase *>(pci)) {
                if (pc->getName(true) == spec->getName(true) && pc->configuration == spec->configuration) {
                    //                            std::cout << "pc: " << pc << " pci: " << pci << std::endl;
                    //                            std::cout << "pc: " << sizeof(*pc) << " pci: " << sizeof(*pci)  << " " << std::abs((long int)pc-(long int)pci) << std::endl;
                    //                            throw std::runtime_error("bla");

                    // This check should not be needed
                    assert(pool->getComponent(pci->getName()) == pci);
                    return pci;
                }
            }
        }
        throw std::runtime_error("Could not find compatible component in pool");
    } else {

        // Sainity check would rase a error otherwise
        assert(pool->getComponent(c->getName()) == c);
        return c;
    }
}

void Composition::updateInternals(Pool *pool)
{
    for (auto &t : children) {
        t.second = searchCorresponding(t.second, pool);
    }
}

void Composition::addEventForwards(std::string child, std::string source, std::string target)
{
    event_forwards[children[child]][source] = target;
}

#if 0
std::string Composition::toString() const
{
    std::ostringstream ss;
    ss << name;
    /*
    if (configurations.size() != 0){
        ss << "  Configuration: [";
        for(int i = 0; i < configurations.size(); i++)
        {
            ss << configurations[i] << " ";
        }
        ss << "]" << std::endl;
    }
    */
    return ss.str();
}
#endif

/*
size_t Composition::getCmpID() const
{
    return cmp_id;
}
*/

int Composition::getType() const
{
    return 0;
}

#if 0
const std::vector<std::string>& Composition::getConfiguration() const
{
    return configurations;
}

void Composition::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}
#endif

void Composition::replaceChild(Component *c, std::string name)
{
    if (isIgnored()) return;
    if (children.find(name) == children.end()) {
        std::cout << "Availilble children: " << std::endl;
        for (auto c : children) {
            std::cout << "\t-" << c.first << std::endl;
        }
        throw std::runtime_error("Cannot replace child, the requested child does not exist so far: " + name);
    }
    children[name] = c;
}

void Composition::addChild(Component *c, std::string name)
{
    if (isIgnored()) return;
    children[name] = c;

    // Creating default forwards, this can be overridden by addEventForwards
    addEventForwards(name, "failed", "failed");
    addEventForwards(name, "success", "failed");
    addEventForwards(name, "aborted", "failed");
    addEventForwards(name, "internal_error", "failed");
    addEventForwards(name, "fatal_error", "failed");
}

void Composition::addConstraint(std::string child, std::string target)
{

    if (children.find(child) == children.end()) {
        throw std::invalid_argument(child + " cannot be found on " + getName());
    }
    auto c = pool->getComponent(target);
    if (!c) {
        throw std::invalid_argument(getName() + " cannot redefine child " + name);
    }
    children.erase(child);

    // addChild(pool->getComponent("DepthReader::Task"),"z");
    // addChild(pool->getComponent("DepthReader::Task"),"z");

    // TODO better do this later on CSP level instead here...
    addChild(c, child);
    //   std::cout << child << "----" << children[child]->getName() <<std::endl;
    //    children[child] = pool->getComponent(name);
}

std::vector<std::string> Composition::unsolveableChildren()
{
    std::vector<std::string> res;
    // Pool *pool = Pool::getInstance();

    for (auto child : children) {
        bool valid = false;
        for (auto provider : pool->getItems<Component *>()) {
            if (provider->abstract()) {
                continue;
            }

            if (provider->isFullfilling(child.second->getName())) {
                //       std::cout << std::string("FULLFILLING: " + getName() +
                // "." + child.first + " = " + child.second->getName() + "==" +
                // provider->getName()) << std::endl;
                //                printf("GOT A FULLILLING! %s for
                // %s\n",getName().c_str(), provider->getName().c_str());
                valid = true;
                break;
            }
        }
        if (!valid) {
            // std::cout << std::string("NOT FULLFILLING:" + getName() + "." +
            // child.first + " = " + child.second->getName()) << std::endl;

            res.push_back(getName() + "." + child.first + " = " + child.second->getName());
        }
    }
    return res;
}

Gecode::IntVarArray Composition::getPossibleTaskAssignments(Gecode::Space *space)
{
    // auto arr =
    // Gecode::IntVarArray(*space,children.size(),0,pool->getNonAbstractCount()-1);
    auto arr = Gecode::IntVarArray(*space, getChildren().size(), 0, pool->size() - 1);
    return arr;
}

std::vector<std::pair<std::string, Component *>> Composition::getChildren()
{
    std::vector<std::pair<std::string, Component *>> erg;
    for (auto i : children) {
        erg.push_back(std::pair<std::string, Component *>(i.first, i.second));
    }
    return erg;
};

SpecializedComponentBase *Composition::getSpecialized()
{
    return new SpecializedComponent<Composition>(this, pool);
}

}  // end namespace constrained_based_networks
