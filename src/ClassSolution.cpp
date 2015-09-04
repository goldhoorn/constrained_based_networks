#include "ClassSolution.hpp"

#include <gecode/gist.hh>
#include <gecode/minimodel.hh>

#include <vector>
#include <map>
#include <stdexcept>
#include "Pool.hpp"
#include "Task.hpp"
#include "Composition.hpp"
#include "SpecializedComponent.hpp"
#include "DataService.hpp"
#include <stdlib.h>
#include <fstream>

#include <gecode/gist.hh>
using namespace Gecode;

#define REMOVE_REC

namespace constrained_based_networks {

void ClassSolution::markInactiveAsInactive(){
    /*
    //Remove all loops that are unneeded by a master requirement
    for(auto c: pool->getItems<Component*>()){
        size_t id_child= c->getID();
        for(unsigned int id_parent=0;id_parent != pool->getItems<Composition*>().size();id_parent++){
            Composition *cmp = pool->getItems<Composition*>()[id_parent];
            unsigned int id = cmp->getID();
        }
    }
    */
    /*
    //Nothing can depend on the DUMMY node
    for(auto c: pool->getItems<Component*>()){

    }
    */

    //Remove all unneeded depends
    for(auto c: pool->getItems<Component*>()){
        unsigned int cmp_id = pool->getId(c);

        //Remove all recursive depends if a component is inactive
#ifdef REMOVE_REC
        rel(*this,!active[c->getID()]  >> (depends_recursive[c->getID()] == IntSet::empty));
#endif

        if(c->getID() != ID_ROOT_KNOT){
            rel(*this,expr(*this,active[cmp_id] == 0) << (depends[cmp_id] == IntSet::empty));
            rel(*this,expr(*this,active[cmp_id] == 1) << (depends[cmp_id] != IntSet::empty));
        }
    }
}

void ClassSolution::markAbstractAsInactive(){
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

void ClassSolution::markActiveAsActive(){
    for(auto c: pool->getItems<Component*>()){
        if(c->getID() == ID_ROOT_KNOT){
            //If a component is active it must depend on THIS
            rel(*this,active[c->getID()],IRT_EQ, 1);
#ifdef REMOVE_REC
            //Itself has no other dependancies, so making sure not --anything-- is selected
            dom(*this,depends_recursive[c->getID()], SRT_EQ, IntSet::empty);
#endif
        }
    }
}

void ClassSolution::removeSelfCompositonsFromDepends(){
    for(auto child : pool->getItems<Component*>()){
        for(auto component : pool->getItems<Component*>()){
            if(!dynamic_cast<Composition*>(component)){
                dom(*this,depends[child->getID()], SRT_DISJ, component->getID());
            }
        }
    }
}

void ClassSolution::depsOnlyOnCmp(){
    for(auto component: pool->getItems<Component*>()){
        //Nothing can depend on the dummy task
        dom(*this,depends[pool->getId(component)], SRT_DISJ, ID_NIL);
#ifdef REMOVE_REC
        dom(*this,depends_recursive[pool->getId(component)], SRT_DISJ, ID_NIL);
#endif
    }
    //The NIL node cannot have deps
    dom(*this, depends[ID_NIL], SRT_EQ, IntSet::empty);
#ifdef REMOVE_REC
    dom(*this, depends_recursive[ID_NIL], SRT_EQ, IntSet::empty);
#endif
}

bool ClassSolution::markCompositionAsChildless(Composition *composition, size_t composition_id){
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

int ClassSolution::print_count = 0;

ClassSolution::ClassSolution(Pool *_pool): pool(_pool)
    , active(*this, pool->getComponentCount(), 0, 1)
    , depends(*this,pool->getComponentCount(), IntSet::empty, IntSet(0,pool->getComponentCount()-1)) //pool->getCount<Composition*>()-1))
#ifdef REMOVE_REC
    , depends_recursive(*this,pool->getComponentCount(), IntSet::empty, IntSet(0,pool->getComponentCount()-1)) //pool->getCount<Composition*>()-1))
#endif
{
//    std::cout << "Hallo" << __LINE__ << std::endl;
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

        if(dynamic_cast<SpecializedComponentBase*>(composition)){
            std::cout << "Got a specialized " <<  composition->getName() << "(" << composition->getID() << ")"  << std::endl;
            if(composition->isActive()){
                std::cout << "-- And it should started!!" << std::endl;
            }
//            throw std::runtime_error("Jeha");
        }
#if 0
                    if(composition->getID() == 360){
                        std::cout << "pointer in class solution: " << composition << std::endl;
                        std::cout << "Name: " << composition->getName() << std::endl;
                        std::cout << "Pool: " << pool << std::endl;
                        std::cout << "pointer in vector: " << pool->operator[](360)  << std::endl;
                        if(composition->isActive()){
                            std::cout << "-- And it should started!!" << std::endl;
                        }else{
                            std::cout << "-- And it should NOT started!!" << std::endl;
                        }

                        auto s = dynamic_cast<SpecializedComponentBase*>(composition);

                        if(!s){
                            std::cerr << "bla id: " << composition->getID() << std::endl;
                            throw std::runtime_error("Das ist schlecht ");
                        }else{
                            throw std::runtime_error("Jeha das ist gut" );
                        }

                    }
#endif
        /*
                    auto s = dynamic_cast<SpecializedComponentBase*>(composition);
                    if(s){
                        std::string str = composition->getName() + "(!)";
                        for(auto c : s->configuration){
                            str = str + "\n" + c.first + "=" + c.second;
                        }
                        std::cout << "Huch: " << str << std::endl;
                    }
                    */
//        std::cout << "Processing composition " << composition->getName() << " (" << cmp_counter << ")" << std::endl;
        auto composition_child_constraints = composition->getPossibleTaskAssignments(this);
        //std::cout << "Hallo" << __LINE__ << std::endl;
        //std::cout << "Counter vs max: " << cmp_counter << "/" << compositions.size() << std::endl;
        markCompositionAsChildless(composition, cmp_counter);

        for(size_t child_id = 0; child_id != composition->getChildren().size(); child_id++){
            auto child = composition->getChildren()[child_id];
            if(child.second == 0){
                std::cout << " - "  << child.first << std::endl;
                throw std::runtime_error("Child is nullpointer");
            }
            //If this composition is used all children needs to be active
            rel(*this, composition_child_constraints[child_id], IRT_NQ, 0 , imp(active[composition->getID()]));

            //It cannot depend on itself
            rel(*this, composition_child_constraints[child_id], IRT_NQ, composition->getID());

            //Mark all children as invalid if composition is inactive
            rel(*this, !active[composition->getID()] >> (composition_child_constraints[child_id] == 0) );
#ifdef REMOVE_REC
            //Optmization to reduce the search-space is redundant to the loop closing #10
            if(composition->getID() != ID_ROOT_KNOT){
                rel(*this, active[composition->getID()] >> (depends_recursive[composition->getID()] >= IntSet(ID_ROOT_KNOT,ID_ROOT_KNOT) ));
            }
#endif


            for(auto provider : pool->getItems<Component*>()){
                //Remove all children That are NOT used as dependancy
                //Unfourtnalty it is not possible to have a mini-model for membership constraints
                //this make this ugly and the workaround needed
                BoolVar is_member(*this,0,1);
                member(*this, composition_child_constraints, expr(*this,provider->getID()), is_member);
                dom(*this, depends[pool->getId(provider)], SRT_DISJ, composition->getID(), imp(expr(*this,!is_member)));
#ifdef REMOVE_REC
                //A component cannot (anyhow) depend on itself (testcase #10)
                dom(*this, depends_recursive[provider->getID()], SRT_DISJ, provider->getID() );// <= composition_child_constraints[child_id]));

                //Bulding recusrive dependencies
                rel(*this, depends_recursive[provider->getID()], SRT_EQ, expr(*this, depends_recursive[composition->getID()] | depends[provider->getID()]), imp(is_member)) ;// <= composition_child_constraints[child_id]));
#endif
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
    
    std::cout << "Hallo" << __LINE__ << std::endl;

#if 1
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
#endif


    std::cout << "Hallo" << __LINE__ << std::endl;

    markInactiveAsInactive();
    branch(*this, active[ID_ROOT_KNOT], INT_VAL_MIN());
    branch(*this, &ClassSolution::postBranching2);
    //active_brancher =  branch(*this, active, INT_VAR_SIZE_MIN(), INT_VAL_MIN(),NULL,&print);
    //active_brancher =  branch(*this, active, INT_VAR_ACTIVITY_MAX(0.8), INT_VAL_MIN());
    //branch(*this, &ClassSolution::postBranching);
    for(size_t i = 0; i < ir_assignments.size();i++){
//        ir_assignments_brancher.push_back(branch(*this, ir_assignments[i], INT_VAR_SIZE_MIN(), INT_VAL_MIN()));
        //ir_assignments_brancher.push_back(branch(*this, ir_assignments[i], INT_VAR_ACTIVITY_MAX(0.8), INT_VAL_MIN()));
        //branch(*this, &ClassSolution::postBranching);
//        branch(*this, depends_recursive[pool->getItems<Composition*>()[i]->getID()], SET_VAL_MIN_INC());
        //branch(*this, depends_recursive[pool->getItems<Composition*>()[i]->getID()], SET_VAL_MAX_EXC());
    }
    //branch(*this, &ClassSolution::postBranching);
    //branch(*this, depends_recursive[pool->getItems<Composition*>()[i]->getID()], SET_VAL_MIN_INC());
//    depends_brancher = branch(*this, depends, SET_VAR_SIZE_MAX(), SET_VAL_MIN_EXC());
    //depends_brancher = branch(*this, depends, SET_VAR_SIZE_MAX(), SET_VAL_MIN_EXC());

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

    printf("Finished creatingerHandleconstraints\n");
//    branch(*this, depends_recursive, SET_VAR_NONE(), SET_VAL_MIN_INC());

}

bool ClassSolution::allDepsResolved(unsigned int cmp_id, std::vector<size_t> &ids){
//    std::cout << "checking root " << pool->getItems<Composition*>()[cmp_id]->getName() << std::endl;
    for(size_t i =0;i< ir_assignments[cmp_id].size();i++){
        if(ir_assignments[cmp_id][i].assigned()){
            unsigned int id = ir_assignments[cmp_id][i].val();
//            std::cout << "checking " << id << std::endl;
//            std::cout << "pool size is: " << pool->size() << std::endl;
            auto c = (*pool)[id];
            if(auto cmp = dynamic_cast<Composition*>(c)){
                for(auto id2 : ids) if(id == id2){
                    std::cout << "This cannot be we depend and outself?" << std::endl;
                    //this is a failure posting invalid constrain
                    rel(*this, active[0],IRT_EQ, 1);
                    rel(*this, active[1],IRT_EQ, 0);
                    return false; //TODO forbit this solution
                }
                ids.push_back(id);
                if(!allDepsResolved(cmp->getCmpID(), ids)){
                    return false;
                }else{
                }
            }
        }else{
//            std::cout << "Unresolved: " << (*pool)[i]->getName() << std::endl;
            return false;
        }
    }
    return true;
}


bool ClassSolution::build_tree(graph_analysis::BaseGraph::Ptr g, unsigned int cmp_id){
    for(size_t i =0;i< ir_assignments[cmp_id].size();i++){
        if(ir_assignments[cmp_id][i].assigned()){
            unsigned int id = ir_assignments[cmp_id][i].val();
            auto c = (*pool)[id];

            graph_analysis::Edge::Ptr e(new graph_analysis::Edge());
            auto a = pool->getItems<Composition*>()[cmp_id];
            auto b = (pool->getItems<Composition*>()[cmp_id]->getPtr());
            std::string aa = (dynamic_cast<SpecializedComponentBase*>(a) == 0)?"true":"false";
            std::string bb = (dynamic_cast<SpecializedComponentBase*>(b.get()) == 0)?"true":"false";
            std::cout << "Must equal: " << aa << " vs. " << bb  << " "  << a->getName() << "(" << a->getID() << ")" <<  std::endl;
            std::cout << "pointer in CS: " << a  << " | " << dynamic_cast<SpecializedComponentBase*>(a) << std::endl;
            e->setSourceVertex((pool->getItems<Composition*>()[cmp_id]->getPtr()));
            e->setTargetVertex(c->getPtr());
            g->addEdge(e);

            if(auto cmp = dynamic_cast<Composition*>(c)){
                /* //We don't need this check here was already done before
                for(auto id2 : ids) if(id == id2){
                    std::cout << "This cannot be we depend and outself?" << std::endl;
                    //this is a failure posting invalid constrain
                    rel(*this, active[0],IRT_EQ, 1);
                    rel(*this, active[1],IRT_EQ, 0);
                    return false; //TODO forbit this solution
                }
                */
                //ids.push_back(id);
                build_tree(g,cmp->getCmpID());
            }else{
                //Nothing to do, the child is a leaf, which got added by the block before this if
                //
            }
        }else{
            std::cout << "Unresolved: " << (*pool)[i]->getName() << std::endl;
        }
    }
    return true;
}

void ClassSolution::removeAllUnsedCmps(std::vector<size_t> ids){
    /*
    std::cout << "Keep " << ids.size() << " components active #########################" << std::endl;
       for(auto id : ids){
           std::cout << "Cmp: " << (*pool)[id]->getName()  << std::endl;
       }
    std::cout << "#########################" << std::endl;
*/
    std::cout << "__FUNCTION_" << std::endl;
   for(auto cmp : pool->getItems<Composition*>()){
       bool contain=false;
       for(auto id : ids){
//           std::cout << "Cmp " << cmp->getName() << " has id: " << cmp->getCmpID() << std::endl;
           if(id == cmp->getID()){
            contain = true;
            break;
           }
       }

       if(!contain){
//           std::cout << "Setting " << cmp->getName() << " to inactive" << std::endl;
           rel(*this, active[cmp->getID()],IRT_EQ, 0);
//           assign(*this, active[cmp->getID()], INT_ASSIGN_MAX());
           //assign(*this, depends[cmp->getID()], SET_ASSIGN_MIN_INC());
           //assign(*this, depends_recursive[cmp->getID()], SET_ASSIGN_MIN_INC());
            rel(*this,depends[cmp->getID()] == IntSet::empty);
#ifdef REMOVE_REC
            rel(*this,depends_recursive[cmp->getID()] == IntSet::empty);
#endif
       }else{
//           std::cout << "Ignore: " << cmp->getName() << " to inactive" << std::endl;
       }
   }
}




void ClassSolution::postBranching2(Space &space){
//    std::cout << __FUNCTION__ << std::endl;
    ClassSolution& home = static_cast<ClassSolution&>(space);
//    std::vector<size_t> ids;
    if(home.findNextBrancher(ID_ROOT_KNOT)){
//        ClassSolution::postBranching(space);
        home.doMissingBranching();
    }else{
        branch(home, &ClassSolution::postBranching2);
    }
//    std::cout << "end " << __FUNCTION__ << std::endl;
}

void ClassSolution::doMissingBranching(){
    for(size_t i=0;i<active.size();i++){
        if(!active[i].assigned()){ //It seems this is not used
            rel(*this,active[i],IRT_EQ,0);
            rel(*this,depends[i] == IntSet::empty);
            rel(*this,depends_recursive[i] == IntSet::empty);
       //     branch(*this, active[i], INT_VAL_MIN());
        }
    }
    std::cout << "BUJAAACHACKAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA" << std::endl;
    branch(*this, depends, SET_VAR_SIZE_MIN(), SET_VAL_MIN_EXC());
    branch(*this, depends_recursive, SET_VAR_SIZE_MIN(), SET_VAL_MIN_EXC());

}

bool ClassSolution::findNextBrancher(unsigned int id){
    bool finish = true;
    if(auto cmp = dynamic_cast<Composition*>((*pool)[id])){
        if(!depends[id].assigned()){
            finish=false;
            branch(*this, depends[id], SET_VAL_MIN_INC());
        }
        if(!depends_recursive[id].assigned()){
            finish=false;
            branch(*this, depends_recursive[id], SET_VAL_MIN_INC());
        }

        for(size_t c_id = 0; c_id < cmp->getChildren().size(); c_id++){
//        for(auto child_p : cmp->getChildren()){
            auto child = cmp->getChildren()[c_id].second;
            if(!ir_assignments[cmp->getCmpID()][c_id].assigned()){
                branch(*this, ir_assignments[cmp->getCmpID()][c_id], INT_VAL_MIN());

                if(!active[child->getID()].assigned()){
                    branch(*this, active[child->getID()],INT_VAL_MIN());
                }
                finish=false;
            }else{
                //printf("-- Got here id: %lu (%lu,%lu)\n",ir_assignments[cmp->getCmpID()][c_id].val(),cmp->getCmpID(),c_id);
                if(!findNextBrancher(ir_assignments[cmp->getCmpID()][c_id].val())){
                    finish=false;
                }
            }
        }
    }else{
        if(!active[id].assigned()){
            finish=false;
            branch(*this, active[id],INT_VAL_MIN());
        }
    }
    return finish;
}

void ClassSolution::postBranching(Space &space){

    //ClassSolution* home = static_cast<ClassSolution*>(&space);
    ClassSolution& home = static_cast<ClassSolution&>(space);
    std::vector<size_t> ids;
    //ids.push_back((*pool)[ID_ROOT_KNOT]->getCmpID());
    ids.push_back(ID_ROOT_KNOT);
    //Check if all dependancies are fullfilled, then post another brancher here
    if(home.allDepsResolved(0,ids)){
        home.removeAllUnsedCmps(ids);
        std::cout << "All Deps are resolved" << std::endl;
    }else{
//        std::cout << "All Deps are NOT resolved" << std::endl;
    }
}

#ifdef CONSTRAIN
void ClassSolution::constrain(const Space& _b)
{
    /*
    printf("In constrain block %i\n",print_count);
    printf("active is: %s\n", active_brancher(*this)?"active":"inactive");
    for(size_t i=0;i<ir_assignments_brancher.size();i++){
        printf("ir_brancher[%lu] is: %s\n",i ,ir_assignments_brancher[i](*this)?"active":"inactive");
    }

//    this->brancher();
    //printf("Current Brancher: %i\n",this->brancher(2));

    ClassSolution::postBranching(*this);
    */
    return;

    const ClassSolution& b = static_cast<const ClassSolution&>(_b);

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

ClassSolution::ClassSolution(bool share, ClassSolution& s)
    : Space(share, s)
    //, query(s.query)
    //, componentPool(s.componentPool)
{
    /*
    active_brancher = s.active_brancher;
    depends_brancher = s.depends_brancher;
    depends_recursive_brancher = s.depends_recursive_brancher;
    for(auto i : s.ir_assignments_brancher){
        ir_assignments_brancher.push_back(i);
    }
    */


    pool = s.pool;
    active.update(*this, share, s.active);
    depends.update(*this, share, s.depends);
#ifdef REMOVE_REC
    depends_recursive.update(*this, share, s.depends_recursive);
#endif
    ir_assignments.resize(s.ir_assignments.size());
    for(size_t i = 0; i < s.ir_assignments.size();i++){
        ir_assignments[i].update(*this,share,s.ir_assignments[i]);
    }
}

Space* ClassSolution::copy(bool share)
{
    return new ClassSolution(share,*this);
}

//void ClassSolution::print(std::ostream& os) const
//{
//    printToStream(os);
//}

void ClassSolution::compare(const Space& _s, std::ostream& os) const{
    auto s = static_cast<const ClassSolution&>(_s);
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

    const_cast<ClassSolution*>(this)->print_count++;

}

void ClassSolution::printToDot(std::ostream& os) const
{
    //Pool *pool = Pool::getInstance();
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
                    auto s = dynamic_cast<SpecializedComponentBase*>(composition);
                    if(s){
                        std::string str = composition->getName() + "(!)";
                        for(auto c : s->configuration){
                            str = str + "\n" + c.first + "=" + c.second;
                        }
                        file << "\t\"" << str << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[label=\"" << child.first << "\"];" << std::endl;
                    }else{
                        std::string name = composition->getName();
                        if(composition->isActive()){
                            name = name + "(A)";
                        }
                        file << "\t\"" << name << "\" -> \"" << (*pool)[ir_assignments[cmp_id][child_id].val()]->getName() << "\"[label=\"" << child.first << "\"];" << std::endl;
                    }
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
}

void ClassSolution::printToStream(std::ostream& os, bool full) const
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

//    Pool *pool = Pool::getInstance();

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
}

void ClassSolution::print(const Space& home, const BrancherHandle& bh, unsigned int a, BoolVar x, int i, const int& n, std::ostream& o) {
    //const ClassSolution& c = static_cast<const ClassSolution&>(home);
    o << "foo" << a << " " << x << " " << i << " " << n << std::endl;
}

void ClassSolution::print(const Space& home, const BrancherHandle& bh, unsigned int a, IntVar x, int i, const int& n, std::ostream& o) {
    //const ClassSolution& c = static_cast<const ClassSolution&>(home);
    o << "foo" << a << " " << x << " " << i << " " << n << std::endl;
    /*
    int x = i % c.w, y = i / c.w;
    o << "letters[" << x << "," << y << "] "
        << ((a == 0) ? "=" : "!=") << " "
        << static_cast<char>(n);
    */
}

ClassSolution* ClassSolution::babSearch2()
{
    return ClassSolution::babSearch(Pool::getInstance());
}

ClassSolution* ClassSolution::babSearch(Pool *pool)
{
    // Initial situation
    ClassSolution* so = new ClassSolution(pool);
    // BAB search engine
    //BAB<ClassSolution> e(so);
    BAB<ClassSolution> e(so);
    //DFS<ClassSolution> e(so);
    // search
    ClassSolution* best = NULL;

    while (ClassSolution* s = e.next()){
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
        throw std::runtime_error("ClassSolution babSearch: No solutions");
    }
    delete so;
    return best;
}

ClassSolution *ClassSolution::gistBaBSeach(){
    ClassSolution* m = new ClassSolution(Pool::getInstance());
    //ClassSolution* m = ClassSolution::babSearch(Pool::getInstance());
    Gist::Print<ClassSolution> printer("Print solution");
    Gist::VarComparator<ClassSolution> c("Compare nodes");
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

