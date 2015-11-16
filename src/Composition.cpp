#include "Composition.hpp"
#include <sstream>
#include <stdexcept>
#include "Pool.hpp"
#include "SpecializedComponent.hpp"

namespace constrained_based_networks
{

CompositionObj::CompositionObj(Pool *pool) : ComponentObj(pool)
{
}

CompositionObj::CompositionObj(std::string name, Pool *pool) : ComponentObj(pool)
{
    this->name = name;
    // cmp_id = pool->getItems<CompositionObj *>().size() - 1;
    //    std::cout << "Creating composition: " << name << std::endl;
}

Component CompositionObj::clone(Pool *p) const
{
    throw std::runtime_error("Implement me");
    return 0;
    /*
    CompositionObj *c = new CompositionObj(name,p);
    return c;
    */
};

Forwards CompositionObj::getArgumentForwards(Component child, std::string name)
{
    if (children.find(name) == children.end()) {
        if (this->getName() != "root-knot") {
            if (!dynamic_cast<StateMachineObj *>(this)) {
                std::cerr << "This is really bad !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! could not find child " << name << std::endl;
                assert(false);
            } else {
                std::cerr << "WARN implement argumetn forwards for state-machines" << std::endl;
            }
        }
        return Forwards();
    }
    if (children[name] != child) {
        if (!child->isFullfilling(children[name]->getName())) {
            throw std::invalid_argument("Cannot get argument forwards, the child " + children[name]->getName() + " does not equal the given child " + child->getName() + " for role " + name);
        }
    }

    return argument_forwards[name];
}

bool CompositionObj::hasChild(Component child)
{
    std::cout << this->getName() << "<->" << child->getName() << std::endl;
    Component c = child;
//    auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c);
    //while ((std::shared_ptr<SpecializedComponentObjBase> spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c)) && spec.get()) {
    SpecializedComponentBase spec;

    //TODO try this
    //for(auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c); spec.get(); c=spec->getOrginal());
    while ((spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c)) && spec.get()) {
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

Forwards CompositionObj::getEventForwards(Component child, std::string name)
{
    if (children.find(name) == children.end()) {
        if (this->getName() != "root-knot") {
            std::cerr << "This is really bad !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!! could not find child " << name << std::endl;
            assert(false);
        }
        return Forwards();
    }
    if (children[name] != child) {
        if (!child->isFullfilling(children[name]->getName())) {
            throw std::invalid_argument("Cannot get argument forwards, the child " + children[name]->getName() + " does not equal the given child " + child->getName() + " for role " + name);
        }
    }

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

    return event_forwards[name];
}

bool CompositionObj::operator==(const CompositionObj &comp) const
{
    return name == comp.name;
}

void CompositionObj::addArgumentForwards(std::string child, std::string source, std::string target)
{
    try
    {
        children.at(child);
        argument_forwards[child][source] = target;
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

Component CompositionObj::searchCorresponding(Component c, Pool *pool)
{
    auto spec = std::dynamic_pointer_cast<SpecializedComponentObjBase>(c);
    if (spec.get()) {
        // search for object in DB
        for (auto pci : pool->getItems<ComponentObj>()) {
            auto pc = std::dynamic_pointer_cast<SpecializedComponentObjBase>(pci);
            if (pc.get()) {
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

void CompositionObj::updateInternals(Pool *pool)
{
    for (auto &t : children) {
        t.second = searchCorresponding(t.second, pool);
    }
}

void CompositionObj::addEventForwards(std::string child, std::string source, std::string target)
{
    try
    {
        children.at(child);
        event_forwards[child][source] = target;
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

#if 0
std::string CompositionObj::toString() const
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
size_t CompositionObj::getCmpID() const
{
    return cmp_id;
}
*/

int CompositionObj::getType() const
{
    return 0;
}

Composition CompositionObj::make(Pool *pool, std::string name)
{
    auto res = Composition((new CompositionObj(name,pool)));
    pool->addComponent(res);
    return res;
}

#if 0
const std::vector<std::string>& CompositionObj::getConfiguration() const
{
    return configurations;
}

void CompositionObj::setConfiguration(const std::vector<std::string>& configurations)
{
    this->configurations = configurations;
}
#endif

void CompositionObj::replaceChild(Component c, std::string name)
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

const std::map<std::string, Forwards> &CompositionObj::getArgumentForwards() const
{
    return argument_forwards;
}

const std::map<std::string, Forwards> &CompositionObj::getEventForwards() const
{
    return event_forwards;
}

void CompositionObj::addChild(Component c, std::string name)
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

void CompositionObj::addConstraint(std::string child, std::string target)
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

std::vector<std::string> CompositionObj::unsolveableChildren()
{
    std::vector<std::string> res;
    // Pool *pool = Pool::getInstance();

    for (auto child : children) {
        bool valid = false;
        for (auto provider : pool->getItems<ComponentObj>()) {
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

Gecode::IntVarArray CompositionObj::getPossibleTaskAssignments(Gecode::Space *space)
{
    // auto arr =
    // Gecode::IntVarArray(*space,children.size(),0,pool->getNonAbstractCount()-1);
    auto arr = Gecode::IntVarArray(*space, getChildren().size(), 0, pool->size() - 1);
    return arr;
}

std::vector<std::pair<std::string, Component>> CompositionObj::getChildren()
{
    std::vector<std::pair<std::string, Component>> erg;
    for (auto i : children) {
        erg.push_back(std::pair<std::string, Component>(i.first, i.second));
    }
    return erg;
};

SpecializedComponentBase CompositionObj::getSpecialized(std::shared_ptr<ComponentObj> _obj, std::string name){
    Composition obj = std::dynamic_pointer_cast<CompositionObj>(_obj);
    assert(obj.get());
    return SpecializedComponentObj<CompositionObj>::make(obj, pool,name);
}

}  // end namespace constrained_based_networks
