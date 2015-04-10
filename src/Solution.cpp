#include "Solution.hpp"

#include <gecode/gist.hh>
#include <gecode/minimodel.hh>

#include <vector>
#include <map>
#include <stdexcept>
#include "Pool.hpp"
#include "Task.hpp"
#include "Composition.hpp"
#include "DataService.hpp"
#include <stdlib.h>
#include <fstream>

#include <gecode/gist.hh>

using namespace Gecode;

namespace constrained_based_networks {

void Solution::markInactiveAsInactive(){
    //Remove all loops that are unneeded by a master requirement
    for(auto c: pool->getItems<Component*>()){
        size_t id_child= c->getID();
        for(int id_parent=0;id_parent != pool->getItems<Composition*>().size();id_parent++){
            Composition *cmp = pool->getItems<Composition*>()[id_parent];
            unsigned int id = cmp->getID();
        }
    }

    //Nothing can depend on the DUMMY node
    for(auto c: pool->getItems<Component*>()){

    }

    //Remove all unneeded depends
    for(auto c: pool->getItems<Component*>()){
        unsigned int cmp_id = pool->getId(c);
        
        //Remove all recursive depends if a component is inactive
        rel(*this,!active[c->getID()]  >> (depends_recursive[c->getID()] == IntSet::empty));
        
        if(c->getID() != ID_ROOT_KNOT){
            rel(*this,expr(*this,active[cmp_id] == 0) << (depends[cmp_id] == IntSet::empty));
            rel(*this,expr(*this,active[cmp_id] == 1) << (depends[cmp_id] != IntSet::empty));
        }
    }
}

void Solution::markAbstractAsInactive(){
    rel(*this,active[0],IRT_EQ, 0);
    for(auto provider : pool->getItems<DataService*>()){
        if(provider->abstract()){
            rel(*this,active[pool->getId(provider)],IRT_EQ, 0);
            rel(*this,depends[pool->getId(provider)] == IntSet::empty);
        }else{
            std::cout << "Got non abstract data-service?\n";
        }
    }
}

void Solution::markActiveAsActive(){
    for(auto c: pool->getItems<Component*>()){
        if(c->getID() == ID_ROOT_KNOT){
            //If a component is active it must depend on THIS
            rel(*this,active[c->getID()],IRT_EQ, 1);
           
            //Itself has no other dependancies, so making sure not --anything-- is selected
            dom(*this,depends_recursive[c->getID()], SRT_EQ, IntSet::empty);
        }
    }
}

void Solution::removeSelfCompositonsFromDepends(){
    for(auto child : pool->getItems<Component*>()){
        for(auto component : pool->getItems<Component*>()){
            if(!dynamic_cast<Composition*>(component)){
                dom(*this,depends[child->getID()], SRT_DISJ, component->getID());
            }
        }
    }
}

void Solution::depsOnlyOnCmp(){
    for(auto component: pool->getItems<Component*>()){
        //Nothing can depend on the dummy task
        dom(*this,depends[pool->getId(component)], SRT_DISJ, ID_NIL);
        dom(*this,depends_recursive[pool->getId(component)], SRT_DISJ, ID_NIL);
    }
    //The NIL node cannot have deps
    dom(*this, depends[ID_NIL], SRT_EQ, IntSet::empty);
    dom(*this, depends_recursive[ID_NIL], SRT_EQ, IntSet::empty);
}

bool Solution::markCompositionAsChildless(Composition *composition, size_t composition_id){
    if(composition->getChildren().size() == 0){
        std::cout << "Composition (cmp id:" << composition->getID() << ", component id " << composition->getID() << "): " << composition->getName() << " is childless" << std::endl;
        for(auto provider : pool->getItems<Component*>()){
//            std::cout << "- Forbidding: " << provider->getName() << " (" << provider->getID() << ")" << std::endl;
            dom(*this,depends[provider->getID()], SRT_DISJ, composition->getID());
        }
        return true;
    }
    return false;
}

int Solution::print_count = 0;

Solution::Solution(Pool *_pool): pool(_pool)
    , active(*this, pool->getComponentCount(), 0, 1)
    , depends(*this,pool->getComponentCount(), IntSet::empty, IntSet(0,pool->getComponentCount()-1)) //pool->getCount<Composition*>()-1))
    , depends_recursive(*this,pool->getComponentCount(), IntSet::empty, IntSet(0,pool->getComponentCount()-1)) //pool->getCount<Composition*>()-1))
{
/*
    std::cout << "Got " << pool->getItems<DataService*>().size() << "DataServices" << std::endl;
    std::cout << "Got " << pool->getItems<Composition*>().size() << "Compositions" << std::endl;
    std::cout << "Got " << pool->getItems<Task*>().size() << "Tasks" << std::endl;
*/
    //Defining inactive as the opposide to active 


    markAbstractAsInactive();
    markActiveAsActive();
    removeSelfCompositonsFromDepends();
    depsOnlyOnCmp();

    auto compositions = pool->getItems<Composition*>();
    for(size_t cmp_counter = 0; cmp_counter != compositions.size();cmp_counter++){
        auto composition = compositions[cmp_counter];
//        std::cout << "Processing composition " << composition->getName() << " (" << cmp_counter << ")" << std::endl;
        auto composition_child_constraints = composition->getPossibleTaskAssignments(this);

        markCompositionAsChildless(composition, cmp_counter);

        for(size_t child_id = 0; child_id != composition->getChildren().size(); child_id++){
            auto child = composition->getChildren()[child_id];
            //If this composition is used all children needs to be active
            rel(*this, composition_child_constraints[child_id], IRT_NQ, 0 , imp(active[composition->getID()]));

            //Mark all children as invalid if composition is inactive
            rel(*this, !active[composition->getID()] >> (composition_child_constraints[child_id] == 0) );

            //Optmization to reduce the search-space is redundant to the loop closing #10
            if(composition->getID() != ID_ROOT_KNOT){
                rel(*this, active[composition->getID()] >> (depends_recursive[composition->getID()] >= IntSet(ID_ROOT_KNOT,ID_ROOT_KNOT) ));
            }


            for(auto provider : pool->getItems<Component*>()){
                //Remove all children That are NOT used as dependancy
                //Unfourtnalty it is not possible to have a mini-model for membership constraints
                //this make this ugly and the workaround needed
                BoolVar is_member(*this,0,1);
                member(*this, composition_child_constraints, expr(*this,provider->getID()), is_member);
                dom(*this, depends[pool->getId(provider)], SRT_DISJ, composition->getID(), imp(expr(*this,!is_member)));
                    
                //A component cannot (anyhow) depend on itself (testcase #10) 
                dom(*this, depends_recursive[provider->getID()], SRT_DISJ, provider->getID() );// <= composition_child_constraints[child_id]));
                    
                //Bulding recusrive dependencies
                rel(*this, depends_recursive[provider->getID()], SRT_EQ, expr(*this, depends_recursive[composition->getID()] | depends[provider->getID()]), imp(is_member)) ;// <= composition_child_constraints[child_id]));
            
                //Prevent selection of data-services for children
                if(provider->abstract()){
                    rel(*this,composition_child_constraints[child_id],IRT_NQ, pool->getId(provider));
                    dom(*this,depends[pool->getId(provider)], SRT_DISJ, composition->getID());
                    rel(*this,is_member,IRT_EQ,0);
                    continue;
                }
                
                //Check if this provider is fullfilling the requested DataService
                if(provider->isFullfilling(child.second->getName())){
                //std::cout << "+++++++ allowing for " << child.first << " -- " << (*pool)[pool->getId(provider)]->getName() << std::endl;
                    
                    //If something is used then it depends 
                    BoolVar is_used = expr(*this, composition_child_constraints[child_id] == provider->getID());
                    rel(*this, depends[provider->getID()], SRT_SUP, expr(*this,composition->getID()), imp(is_used) );// <= composition_child_constraints[child_id]));

                    //The following should be equivalent but isn't
//                  rel(*this, (expr(*this,composition_child_constraints[child_id] == pool->getId(provider)) >>  (IntSet(cmp_counter,cmp_counter) <= depends[provider->getID()]) ));// <= composition_child_constraints[child_id]));
                }else{
                        if(provider->getID() != 0){
                            //std::cout << "####### forbidding for " << composition->getName() << "." <<child.first << " -- " << (*pool)[pool->getId(provider)]->getName() << std::endl;
                            //THIS provider cannot be Used for THIS child (always true because service does not fit
                            rel(*this,composition_child_constraints[child_id],IRT_NQ, pool->getId(provider));//, imp(active[cmp_id]));
                        }
                }
            }
        }
    

//        branch(*this, composition_child_constraints, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
        ir_assignments.push_back(composition_child_constraints);
    }

    //No composition can directly be assigned as parent to another one becasue this would create a loop in the dependancy graph
    //TODO re-think if this is always valid
    for(size_t p = 0; p < ir_assignments.size();p++){
        Composition *parent = pool->getItems<Composition*>()[p];
        for(size_t c = 0; c < ir_assignments.size();c++){
            Composition *child = pool->getItems<Composition*>()[c];
            if(c==p)continue;

            //Walk throught childs of first
            for(size_t idxp=0; idxp < ir_assignments[p].size(); idxp++){ 
                for(size_t idxc=0; idxc < ir_assignments[c].size(); idxc++){ 
                    rel(*this,(parent->getID() == ir_assignments[c][idxc]) >> (ir_assignments[p][idxp] != child->getID()));
                    rel(*this,(child->getID() == ir_assignments[p][idxp]) >> (ir_assignments[c][idxc] != parent->getID()));
                }
            }
        }

    }
    

    
    markInactiveAsInactive();
    for(size_t i = 0; i < ir_assignments.size();i++){
        //branch(*this, ir_assignments[i], INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
        branch(*this, ir_assignments[i], INT_VAR_SIZE_MIN(), INT_VAL_MIN());
        //branch(*this, depends_recursive[pool->getItems<Composition*>()[i]->getID()], SET_VAL_MIN_INC());
        //branch(*this, depends_recursive[pool->getItems<Composition*>()[i]->getID()], SET_VAL_MAX_EXC());
    }
    
    //branch(*this, depends, SET_VAR_SIZE_MAX(), SET_VAL_MIN_EXC());
    /*
    branch(*this, active, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
    for(size_t i = 0; i < ir_assignments.size();i++){
        branch(*this, ir_assignments[i], INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
    }

    branch(*this, depends_recursive, SET_VAR_SIZE_MAX(), SET_VAL_MIN_EXC());
    */

    //branch(*this, depends, SET_VAR_NONE(), SET_VAL_MIN_INC());
    //branch(*this, depends_recursive, SET_VAR_NONE(), SET_VAL_MIN_INC());
    //branch(*this, active, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
  /*  
    for(size_t i = 0; i < ir_assignments.size();i++){
        branch(*this, ir_assignments[i], INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
    }
    */
//    branch(*this, active, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
//    branch(*this, depends, SET_VAR_NONE(), SET_VAL_MIN_INC());
   
    printf("Finished creating constraints\n");
//    branch(*this, depends_recursive, SET_VAR_NONE(), SET_VAL_MIN_INC());

}


#ifdef CONSTRAIN
void Solution::constrain(const Space& _b) 
{
    printf("In constrain block %i\n",print_count);

    return;

    const Solution& b = static_cast<const Solution&>(_b);
   
    // Number of used components
    // Determine number of used values in b
    int valuesCount = 0;
    std::vector<int> values;
    for(BoolVarArray::const_iterator it = b.active.begin(); it != b.active.end(); ++it)
    {
        // if not contains...
        if(std::find(values.begin(), values.end(), it->val()) == values.end())
        {
            values.push_back(it->val());
            valuesCount++;
        }
    }
    
    // We must have at most that many components used as the so far best solution
    // FIXME LQ, and stuff below
    //nvalues(*this, active, IRT_LE, valuesCount);
    //nvalues(*this, active, IRT_GE, valuesCount);
    
    // If we have an equal amount of values used, the number of reconfigured components must be less
    BoolVar equalAmountOfValues;
    //nvalues(*this, assignments_int, IRT_LQ, valuesCount, equalAmountOfValues);
    
    //std::cout << " Adding best search constraint. This ";
    
    std::cerr << "##########################################################################" << std::endl;
    rprint();
    std::cerr << "##########################################################################" << std::endl;
    std::cout << " must be better than so far best ";
    b.rprint();
    std::cerr << "##########################################################################" << std::endl;
    
}
#endif

Solution::Solution(bool share, Solution& s) 
    : Space(share, s)
    //, query(s.query)
    //, componentPool(s.componentPool)
{
    active.update(*this, share, s.active);
    depends.update(*this, share, s.depends);
    depends_recursive.update(*this, share, s.depends_recursive);
    ir_assignments.resize(s.ir_assignments.size());
    for(size_t i = 0; i < s.ir_assignments.size();i++){
        ir_assignments[i].update(*this,share,s.ir_assignments[i]);
    }
}

Space* Solution::copy(bool share) 
{
    return new Solution(share,*this);
}

//void Solution::print(std::ostream& os) const 
//{
//    printToStream(os);
//}

void Solution::printToDot(std::ostream& os) const 
{
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
    }
    file3 << "}" << std::endl;
    file3.close();
    sprintf(buff,"dot -Tsvg /tmp/dep-rgraph-%i.dot -o /tmp/dep-rgraph-%i.svg",print_count,print_count);
    system(buff);
    os << "<h1> Recursive dependancy Selection: </h1><br/><img src=\"/tmp/dep-rgraph-" << print_count << ".svg\"\\>" << std::endl;
#endif


    print_count++;
}

void Solution::printToStream(std::ostream& os, bool full) const 
{
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
#if 1
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
}

void Solution::print(const Space& home, const BrancherHandle& bh, unsigned int a, BoolVar x, int i, const int& n, std::ostream& o) {
    //const Solution& c = static_cast<const Solution&>(home);
    o << "foo" << a << " " << x << " " << i << " " << n << std::endl;
}

void Solution::print(const Space& home, const BrancherHandle& bh, unsigned int a, IntVar x, int i, const int& n, std::ostream& o) {
    //const Solution& c = static_cast<const Solution&>(home);
    o << "foo" << a << " " << x << " " << i << " " << n << std::endl;
    /*
    int x = i % c.w, y = i / c.w;
    o << "letters[" << x << "," << y << "] "
        << ((a == 0) ? "=" : "!=") << " "
        << static_cast<char>(n);
    */
}

Solution* Solution::babSearch2()
{
    return Solution::babSearch(Pool::getInstance());
}

Solution* Solution::babSearch(Pool *pool)
{
    // Initial situation
    Solution* so = new Solution(pool);
    // BAB search engine
    //BAB<Solution> e(so);
    BAB<Solution> e(so);
    //DFS<Solution> e(so);
    // search
    Solution* best = NULL;
    
    while (Solution* s = e.next()){
        if(best != NULL)
        {
            delete best;
            best=0;
        }
        // Save current solution as best
        s->rprint();
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        std::cout << "------------------------------------------------------------------------------------------" << std::endl;
        best = s;

    } 
    
    // throw exception if there is no solution
    if(best == NULL)
    {
        delete so;
        throw std::runtime_error("Solution babSearch: No solutions");
    }
    delete so;
    return best;
}

Solution *Solution::gistBaBSeach(){
    Solution* m = new Solution(Pool::getInstance());
    //Solution* m = Solution::babSearch(Pool::getInstance());
    Gist::Print<Solution> printer("Print solution");
//    Gist::VarComparator<Solution> c("Compare nodes");
   Gist::Options o;
    o.inspect.click(&printer);
//    o.inspect.compare(&c);
    Gist::bab(m,o);
    return m;
}

} // end namespace constrained_based_networks

