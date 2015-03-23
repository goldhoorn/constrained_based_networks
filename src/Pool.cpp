#include "Pool.hpp"

#include "Composition.hpp"
#include "DataService.hpp"
#include "Task.hpp"

namespace constrained_based_networks{

        
Pool *Pool::pool=0;

Pool* Pool::getInstance()
{
    if(pool == 0){
        pool = new Pool();
        new Task(pool, "NIL-Task"); 
    }
    return pool;
}


Pool::Pool(){
}

void Pool::addComponent(Component *c){
    c->id = components.size();
    components.push_back(c);
}

unsigned int Pool::getId(const Component* obj) const{
    unsigned int erg=0;
    for(auto c : components){
        if(c == obj){
            return  erg;
        }
        erg++;
    }
    throw std::invalid_argument("Cannot get index for requested object");
}

unsigned int Pool::getNonAbstractCount()        
{
    unsigned int count=0;
    for(int i=0;i<components.size();i++){
        if(!components[i]->abstract()){
            count++;
        }
    }
    printf("We have %u non-abstract components\n",count);
    return count;
}
        
Component* Pool::operator[](unsigned int id){
    return components[id];
}

};
