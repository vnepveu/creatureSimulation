#include "ICreature.h"
#include "Schizophrenic.h"

#include <cstdlib>      // std::rand()
#include <memory>       // std::unique_ptr


Schizophrenic::Schizophrenic(){
    this->list_behaviours[0] = std::make_unique<Farsighted>()
    this->list_behaviours[1] = std::make_unique<Fearful>()
    this->list_behaviours[2] = std::make_unique<Gregarious>()
    this->list_behaviours[3] = std::make_unique<Kamikaze>()
    this->current_behaviour = 0;
    this->iter_before_new_behaviour = 0;
}


std::unique_ptr<IBehaviour> Schizophrenic::clone_behaviour(){
    return std::make_unique<Schizophrenic>()
}


void Schizophrenic::next_step(ICreature* creature, Medium& my_medium){

    // Change the behaviour if needed
    if (this->phase_remain_duration==0){
        this->phase_remain_duration = this->phase_total_duration;
        this->current_behaviour = std::rand() % 4;
    }

    this->phase_remain_duration--;
    
    //Apply current behaviour
    this->list_behaviours[this->current_behaviour]->next_step(creature,my_medium);
}