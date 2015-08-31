#include "InstanceSolution.hpp"

#include <gecode/gist.hh>
#include <gecode/minimodel.hh>

#include <vector>
#include <map>
#include <stdexcept>
#include "Pool.hpp"
#include "Task.hpp"
#include "Composition.hpp"
#include "DataService.hpp"
#include "SpecializedComponent.hpp"
#include <stdlib.h>
#include <fstream>

#include <gecode/gist.hh>

using namespace Gecode;

#define REMOVE_REC

namespace constrained_based_networks {

void InstanceComponent::addLimitation(Gecode::Space &space, std::string name, std::string value, size_t cnt)
{
    auto i = int_config[cnt].find(name);
    auto b = bool_config[cnt].find(name);
    auto d = float_config[cnt].find(name);
    if(i != int_config[cnt].end()){
        std::cout << "hallo int" << std::endl;
        rel(space,i->second, IRT_EQ, atoi(value.c_str()));
    }else if(b != bool_config[cnt].end()){
        std::cout << "hallo bool" << std::endl;
        rel(space,b->second, IRT_EQ, atoi(value.c_str()) != 0);
    }else if(d != float_config[cnt].end()){
        std::cout << "hallo float" << std::endl;
        rel(space,b->second, IRT_EQ, atof(value.c_str()));
    }else{
        throw std::invalid_argument("Configuration attribute with " + name + " is unknown");
    }
}


void InstanceSolution::createFlattendIDs(Composition* cmp, unsigned int next_free_id){
    auto cmp_id = cmp->getCmpID();
    instance_components[cmp->getID()].add_flattend_use_id(next_free_id++);

    for(size_t i =0;i< cs->ir_assignments[cmp_id].size();i++){
        if(cs->ir_assignments[cmp_id][i].assigned()){
            unsigned int id = cs->ir_assignments[cmp_id][i].val();
            auto c = (*pool)[id];

            if(auto child_cmp = dynamic_cast<Composition*>(c)){
                createFlattendIDs(child_cmp, next_free_id);
                //Increase is down in recursion
            }else{
                //leaf
                instance_components[c->getID()].add_flattend_use_id(next_free_id++);
            }
        }else{
            throw std::runtime_error("This shoult not happen");
        }
    }
}


void InstanceSolution::limitConfigs(Composition* cmp, unsigned int next_free_id){
    if(cmp->isActive()){
        std::cout << cmp->getName();
        if(auto child_cmp = dynamic_cast<SpecializedComponentBase*>(cmp)){
            std::cout << "Is specialized" << std::endl;
        }else{
            std::cout << "Is NOT specialized" << std::endl;
        }
    }

    auto cmp_id = cmp->getCmpID();
    instance_components[cmp->getID()].add_flattend_use_id(next_free_id++);

    if(auto sc = dynamic_cast<SpecializedComponentBase*>(cmp)){
            printf("huhu\n");
//        auto c = (Component*)sc;
        for(auto c : sc->configuration){
            std::string name = c.first;
            std::string value = c.second;
            instance_components[cmp->getID()].addLimitation(*this, name,value,0);
        }
    }




    for(size_t i =0;i< cs->ir_assignments[cmp_id].size();i++){
        if(cs->ir_assignments[cmp_id][i].assigned()){
            unsigned int id = cs->ir_assignments[cmp_id][i].val();
            auto c = (*pool)[id];

            if(auto child_cmp = dynamic_cast<Composition*>(c)){



                limitConfigs(child_cmp, next_free_id);
                //Increase is down in recursion
            }else{
                //leaf
                instance_components[c->getID()].add_flattend_use_id(next_free_id++);
            }
        }else{
            throw std::runtime_error("This shoult not happen");
        }
    }
}
/*
void InstanceSolution::limitComponents(unsigned int cmp_id){
    for(size_t i =0;i< cs->ir_assignments[cmp_id].size();i++){
        if(cs->ir_assignments[cmp_id][i].assigned()){
            unsigned int id = cs->ir_assignments[cmp_id][i].val();
            auto c = (*pool)[id];
            if(auto cmp = dynamic_cast<Composition*>(c)){
                limitComponents(cmp->getCmpID());
            }else{
                //Setting limits for this we are in a leaf
            }
        }else{
            throw std::runtime_error("This shoult not happen");
        }
    }
}
*/


InstanceSolution::InstanceSolution(ClassSolution *_cs, Pool *_pool)
    :pool(_pool),
    cs(_cs)
#if 0
    , active(*this, pool->getComponentCount(), 0, 1)
    , depends(*this,pool->getComponentCount(), IntSet::empty, IntSet(0,pool->getComponentCount()-1)) //pool->getCount<Composition*>()-1))
#ifdef REMOVE_REC
    , depends_recursive(*this,pool->getComponentCount(), IntSet::empty, IntSet(0,pool->getComponentCount()-1)) //pool->getCount<Composition*>()-1))
#endif
#endif
{


    unsigned int usage_count[_pool->getCount<Component*>()];
    memset(usage_count,0,sizeof(usage_count));


    graph_analysis::BaseGraph::Ptr graph = graph_analysis::BaseGraph::getInstance(graph_analysis::BaseGraph::LEMON_DIRECTED_GRAPH);
    _cs->build_tree(graph,0);

    //Search for the root_knot first
    graph_analysis::Vertex::Ptr root = 0;
    for(auto node : graph->getAllVertices()){
        auto component = dynamic_cast<Component*>(node.get());
        if(component->getID() == 0){ //This is the root_know we need this later
            root = node;
        }
    }

    for(auto node : graph->getAllVertices()){
        auto component = dynamic_cast<Component*>(node.get());
        if(!component){
            throw std::runtime_error("Cannot get component from graph");
        }
        auto cmp = dynamic_cast<Composition*>(component);
        auto task = dynamic_cast<Task*>(component);
        auto spec_cmp = dynamic_cast<SpecializedComponentBase*>(component);
        if(cmp){
            std::cout << "I'm having a composition here"  <<  cmp->getName() << std::endl;
        }
        if(spec_cmp){
            std::cout << "I'm having a specialized here"  <<  spec_cmp->getComponent()->getName() << std::endl;
        }
        if(task){
            std::cout << "I'm having a task here"  <<  task->getName() << " id: " << graph->getVertexId(node) << std::endl;
            usage_count[task->getID()]++;
        }
    }

    //Save all possible configurations a component can have
    //We save all possible occurances of a component a different set of configurations
    //The index refers to the index of the tree, furthermore we need a array to combine existing components into one component

    // Wen need somthieng:
    // a) to link two components (same config) based on tree-id's
    // b) to save the configs of components (tree-id vs. config)
    // b.1) needs to linked against one 'real' component (link against tree-id)
    



    return;

    //Initialize internal structure
    instance_components.resize(pool->size());
    for(size_t i =0;i< pool->size();i++){
        auto c = pool->operator[](i);
        instance_components[c->getID()].initialize(c);
    };

    //We need to run throught the tree of all nodes and figure out all needed configs, we create here
    for(size_t p = 0; p < cs->ir_assignments.size();p++){
        //Composition *parent = pool->getItems<Composition*>()[p];

        for(auto t : cs->ir_assignments[p]){
            if(!t.assigned()) throw std::runtime_error("This shold never happen here that we have unassigned solutions");
            //std::cout << "HAve val: " << t.val() << std::endl;
            Component *child= pool->getItems<Component*>()[t.val()];
            instance_components[child->getID()].increse_usage();

            /* TODO fix this
            if(auto spez = dynamic_cast<SpecializedComponent*>(child)){
                std::cerr << "Jeha we have a specalized component" << std::endl;
            }
            */
            //usage_count[t.val()]++;
            continue;
        }
    }

    for(auto c : instance_components){
        //Finalize components means create internal structures for the constraints
        c.finalize(*this);
    }

    //First we need to flatten the graph to have a unique it from our helper components back to the graph-components
    createFlattendIDs(dynamic_cast<Composition*>(pool->operator[](ID_ROOT_KNOT)), 0); //First if is the root compositon, and we start at number 0

    //Now it is the time to put the component constraints from the previously generated graph to the components
    //We need to run througth the graph and post the constraints onto the components itself
    limitConfigs(dynamic_cast<Composition*>(pool->operator[](ID_ROOT_KNOT)), 0); //First if is the root compositon, and we start at number 0



    //Now we have defined the upper limits time to limit the actual configurations while we running throught the tree of components
    //limitComponents(0);

}


#ifdef CONSTRAIN
void InstanceSolution::constrain(const Space& _b)
{
    /*
    printf("In constrain block %i\n",print_count);
    printf("active is: %s\n", active_brancher(*this)?"active":"inactive");
    for(size_t i=0;i<ir_assignments_brancher.size();i++){
        printf("ir_brancher[%lu] is: %s\n",i ,ir_assignments_brancher[i](*this)?"active":"inactive");
    }

//    this->brancher();
    //printf("Current Brancher: %i\n",this->brancher(2));

    InstanceSolution::postBranching(*this);
    */
    return;

}
#endif

InstanceSolution::InstanceSolution(bool share, InstanceSolution& s)
    : Space(share, s)
    //, query(s.query)
    //, componentPool(s.componentPool)
{
    pool = s.pool;
    cs = s.cs;
#if 0
    active.update(*this, share, s.active);
    depends.update(*this, share, s.depends);
#ifdef REMOVE_REC
    depends_recursive.update(*this, share, s.depends_recursive);
#endif
    ir_assignments.resize(s.ir_assignments.size());
    for(size_t i = 0; i < s.ir_assignments.size();i++){
        ir_assignments[i].update(*this,share,s.ir_assignments[i]);
    }
#endif
}

Space* InstanceSolution::copy(bool share)
{
    return new InstanceSolution(share,*this);
}


void InstanceSolution::compare(const Space& _s, std::ostream& os) const{
#if 0
    auto s = static_cast<const InstanceSolution&>(_s);
    char buff[512];
    sprintf(buff,"/tmp/dep-%i.dot", print_count);
    std::ofstream file(buff);
    file << "digraph G {" << std::endl;
    size_t cmp_id=0;
    for(auto composition : pool->getItems<Composition*>()){
        size_t child_id=0;
        for(auto child : composition->getChildren()){
            if(ir_assignments[cmp_id][child_id].assigned() && s.ir_assignments[cmp_id][child_id].assigned()){
                if(s.ir_assignments[cmp_id][child_id].val() > ID_START){
                    if(ir_assignments[cmp_id][child_id].val() == s.ir_assignments[cmp_id][child_id].val()){
                        file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[label=\"" << child.first << "\"];" << std::endl;
                    }else{
                        file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
                        file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[s.ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=red,label=\"" << child.first << "\"];" << std::endl;
                    }
                }
            }else if(ir_assignments[cmp_id][child_id].assigned()){
                if(ir_assignments[cmp_id][child_id].val() > ID_START){
                file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
                file << "\t\"" << composition->getName() << "\" -> \"" << "N/A (unresolved)" << "\"[color=red,label=\"" << child.first << "\"];" << std::endl;
                }
//                file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
//                file << "\t\"" << composition->getName() << "\" -> \"" << "N/A (unresolved)" << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
//                 if(ir_assignments[cmp_id][child_id].val() > ID_START){ //Only print assigned solutions
//                    if(!s.ir_assignments[cmp_id][child_id].assigned()){
 //                       file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
//                        file << "\t\"" << composition->getName() << "\" -> \"" << "N/A (unresolved)" << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
  //                  }else{
//                    }
//                 }
            }else if(s.ir_assignments[cmp_id][child_id].assigned()){
                if(s.ir_assignments[cmp_id][child_id].val() > ID_START){
                    file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[s.ir_assignments[cmp_id][child_id].val()]->getName() << "\"[color=red,label=\"" << child.first << "\"];" << std::endl;
                    file << "\t\"" << composition->getName() << "\" -> \"" << "N/A (unresolved)" << "\"[color=green,label=\"" << child.first << "\"];" << std::endl;
                }
            }else{
                std::cerr << "somethign is really strange here" << std::endl;
            }
            child_id++;
        }
        cmp_id++;
    }


    file << "}" << std::endl;
    sprintf(buff,"dot -Tsvg /tmp/dep-%i.dot -o /tmp/dep-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Child Selection: </h1><br/><img src=\"/tmp/dep-" << print_count << ".svg\"\\>" << std::endl;

    const_cast<InstanceSolution*>(this)->print_count++;
#endif
}

void InstanceSolution::printToDot(std::ostream& os) const
{
#if 0
    Pool *pool = Pool::getInstance();
    /*
    os << "Count: " << active.size() << std::endl;

    os << "Running State: { " << std::endl;
    for(int i = full?0:ID_START; i < active.size(); i++)
    {
        //os << "\t" << (*Pool::getInstance())[i]->getName() << "=";
        if(active[i].assigned())
        {
            if(active[i].val() || full){
                os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << std::endl;
            }
        }
        else
        {
            os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << std::endl;
        }
//        os << ", "<< std::endl;
    }
    os << "}" << std::endl;


    os << "Child Selection: { " << std::endl;
    */

    //TODO ugly
    char buff[512];
    sprintf(buff,"/tmp/dep-%i.dot", print_count);
    std::ofstream file(buff);
    file << "digraph G {" << std::endl;
    size_t cmp_id=0;
    for(auto composition : pool->getItems<Composition*>()){
        size_t child_id=0;
        for(auto child : composition->getChildren()){
            if(ir_assignments[cmp_id][child_id].assigned()){
                 if(ir_assignments[cmp_id][child_id].val() > ID_START){ //Only print assigned solutions
                    file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[label=\"" << child.first << "\"];" << std::endl;
                    //os <<  " (" << ir_assignments[cmp_id][child_id] << ")" << std::endl;
                 }
            }else{
                for (IntVarValues j(ir_assignments[cmp_id][child_id]); j(); ++j){
                    //file << "\t\"" << composition->getName() << "\" -> \"!" << ir_assignments[cmp_id][child_id] << "\"[color=red];" << std::endl;
                    //file << "\t\"!" << ir_assignments[cmp_id][child_id] << "\"[color=red];" << std::endl;
                    file << "\t\"" << composition->getName() << "\" -> \"" << (*pool)[j.val()]->getName() << "\"[color=red,label=\"" << child.first << "\"];" << std::endl;
                    file << "\t\"" << (*pool)[j.val()]->getName() << "\"[color=red];" << std::endl;
                    file << "\t\"" << composition->getName() << "\"[color=red];" << std::endl;
                    if(active[j.val()].assigned()){
                        if(active[j.val()].val()){
                            file << "\t\"" << (*pool)[j.val()]->getName() << "\"[style=filled,fillcolor=green];" << std::endl;
                        }else{
                            file << "\t\"" << (*pool)[j.val()]->getName() << "\"[style=filled,fillcolor=red];" << std::endl;
                        }
                    }else{
                        file << "\t\"" << (*pool)[j.val()]->getName() << "\"[style=filled,fillcolor=yellow];" << std::endl;
                    }
                }
//                os << "\t" << composition->getName() << "." << child.first << "=" << ir_assignments[cmp_id][child_id] << std::endl;
            }
            child_id++;
        }
        cmp_id++;
    }
    file << "}" << std::endl;
    sprintf(buff,"dot -Tsvg /tmp/dep-%i.dot -o /tmp/dep-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Child Selection: </h1><br/><img src=\"/tmp/dep-" << print_count << ".svg\"\\>" << std::endl;

#if 1
    sprintf(buff,"/tmp/dep-graph-%i.dot", print_count);
    std::ofstream file2(buff);
    file2 << "digraph G {" << std::endl;
    for(size_t i = 1; i< depends.size();i++){
        auto o = (*pool)[i];

        for (SetVarGlbValues j(depends[i]); j(); ++j){
            //if(j.val() < ID_START && !full) continue;
            file2 <<  "\t\"" << o->getName() << "\" -> \""  <<  (*pool)[j.val()]->getName() << "\";" << std::endl;
        }
        for (SetVarUnknownValues j(depends[i]); j(); ++j){
            file2 <<  "\t\"" << o->getName() << "\" -> \""  <<  (*pool)[j.val()]->getName() << "\"[color=yellow];" << std::endl;
        }
    }
    file2 << "}" << std::endl;
    file2.close();
    sprintf(buff,"dot -Tsvg /tmp/dep-graph-%i.dot -o /tmp/dep-graph-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Dependancy Selection: </h1><br/><img src=\"/tmp/dep-graph-" << print_count << ".svg\"\\>" << std::endl;
#endif
#if 1
    sprintf(buff,"/tmp/dep-rgraph-%i.dot", print_count);
    std::ofstream file3(buff);
    file3 << "digraph G {" << std::endl;
    for(size_t i = 1; i< depends_recursive.size();i++){
        auto o = (*pool)[i];
        for (SetVarGlbValues j(depends_recursive[i]); j(); ++j){
            //if(j.val() < ID_START && !full) continue;
            file3 <<  "\t\"" << o->getName() << "\" -> \""  <<  (*pool)[j.val()]->getName() << "\";" << std::endl;
        }
        for (SetVarUnknownValues j(depends_recursive[i]); j(); ++j){
            file3 <<  "\t\"" << o->getName() << "\" -> \""  <<  (*pool)[j.val()]->getName() << "\"[color=yellow];" << std::endl;
        }
    }
    file3 << "}" << std::endl;
    file3.close();
    sprintf(buff,"dot -Tsvg /tmp/dep-rgraph-%i.dot -o /tmp/dep-rgraph-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Recursive dependancy Selection: </h1><br/><img src=\"/tmp/dep-rgraph-" << print_count << ".svg\"\\>" << std::endl;
#endif


    print_count++;

#endif

}

void InstanceSolution::printToStream(std::ostream& os, bool full) const
{
#if 0
    os << "Count: " << active.size() << std::endl;

    os << "Running State: { " << std::endl;
    for(int i = full?0:ID_START; i < active.size(); i++)
    {
        //os << "\t" << (*Pool::getInstance())[i]->getName() << "=";
        if(active[i].assigned())
        {
            if(active[i].val() || full){
                os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << std::endl;
            }
        }
        else
        {
            os << "\t" << (*Pool::getInstance())[i]->getName() << "=" << active[i] << std::endl;
        }
//        os << ", "<< std::endl;
    }
    os << "}" << std::endl;

    Pool *pool = Pool::getInstance();

    os << "Child Selection: { " << std::endl;
    size_t cmp_id=0;
    for(auto composition : pool->getItems<Composition*>()){
        size_t child_id=0;
        for(auto child : composition->getChildren()){
            if(ir_assignments[cmp_id][child_id].assigned()){
                 if(ir_assignments[cmp_id][child_id].val() > ID_START  || full){ //Only print assigned solutions
                    os << "\t" << composition->getName() << "." << child.first << "=";
                    os <<  (*pool)[ir_assignments[cmp_id][child_id].val()]->getName();
                    os <<  " (" << ir_assignments[cmp_id][child_id] << ")" << std::endl;
                 }
            }else{
                os << "\t" << composition->getName() << "." << child.first << "=" << ir_assignments[cmp_id][child_id] << std::endl;
            }
            child_id++;
        }
        cmp_id++;
    }
    os << "}" << std::endl;
#if 1
    os << "Dependencies { " << std::endl;
    for(size_t i = full?0:ID_START; i< depends.size();i++){
        //auto o = pool->getItems<Composition*>()[i];//(*pool)[i];
        auto o = (*pool)[i];
        //os << "Deps for " << o->getName() << ": " << std::endl;//<< depends << std::endl;

        //TODO ugly check
        bool empty=true;
        for (SetVarGlbValues j(depends[i]); j(); ++j){
            if(j.val() < ID_START && !full) continue;
            empty=false;
            break;
        }
        if(!empty){
            os << "\t" << "Object " << o->getName() << "(" << o->getID() << ") is depending on:"  << depends[i] << std::endl;
            for (SetVarGlbValues j(depends[i]); j(); ++j){
                if(j.val() < ID_START && !full) continue;
                os  << "\t" << "- " << j.val() << " " <<  (*pool)[j.val()]->getName() << std::endl;
            }
        }
    }
    os << "}" <<std::endl;
#endif
#if 0
    os << "Recursive Dependencies { " << std::endl;
    for(size_t i = full?0:ID_START; i< depends_recursive.size();i++){
        auto o = (*pool)[i];

        //TODO ugly check
        bool empty=true;
        for (SetVarGlbValues j(depends_recursive[i]); j(); ++j){
            if(j.val() < ID_START && !full) continue;
            empty=false;
            break;
        }
        if(!empty){
            os << "\t" << "Object " << o->getName() << "(" << o->getID() << ") is depending on:"  << depends_recursive[i] << std::endl;
            for (SetVarGlbValues j(depends_recursive[i]); j(); ++j){
                if(j.val() < ID_START && !full) continue;
                os  << "\t" << "- " << j.val() << " " <<  (*pool)[j.val()]->getName() << std::endl;
            }
        }
    }
    os << "}" <<std::endl;
#endif

#endif
}

InstanceSolution* InstanceSolution::babSearch2(ClassSolution *cs, Pool *pool)
{
    return InstanceSolution::babSearch(cs,pool);
}

InstanceSolution* InstanceSolution::babSearch(ClassSolution *cs, Pool *pool)
{
    // Initial situation
    InstanceSolution* so = new InstanceSolution(cs, pool);
    // BAB search engine
    //BAB<InstanceSolution> e(so);
    BAB<InstanceSolution> e(so);
    //DFS<InstanceSolution> e(so);
    // search
    InstanceSolution* best = NULL;

    while (InstanceSolution* s = e.next()){
        if(best != NULL)
        {
            delete best;
            best=0;
        }
        // Save current solution as best
        //s->rprint();
        //std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        //std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        best = s;

    }

    // throw exception if there is no solution
    if(best == NULL)
    {
        delete so;
        throw std::runtime_error("InstanceSolution babSearch: No solutions");
    }
    delete so;
    return best;
}

InstanceSolution *InstanceSolution::gistBaBSeach(ClassSolution *cs, Pool *pool){
    InstanceSolution* m = new InstanceSolution(cs,pool);
    //InstanceSolution* m = InstanceSolution::babSearch(Pool::getInstance());
    Gist::Print<InstanceSolution> printer("Print solution");
    Gist::VarComparator<InstanceSolution> c("Compare nodes");
    Gist::Options o;
    o.c_d = 2;
    o.a_d = 10;
    o.threads = 4;
    o.inspect.click(&printer);
    o.inspect.compare(&c);
    Gist::bab(m,o);
    return m;
}

} // end namespace constrained_based_networks

