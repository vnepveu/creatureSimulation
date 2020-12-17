#include "Camouflage.h"
#include <cstdlib> // for rand  and RAND_MAX
#include <algorithm> // for min(float , float) function 
#include "Config.h"


Camouflage::Camouflage(ICreature *c) : CreatureDecorator(c){
  Config* config_singleton = Config::get_instance();
  this->max_camouflage = config_singleton->get_config_float("max_camouflage");

  float max_camouflage_efficency = 
          config_singleton->get_config_float("max_camouflage_efficency");
  float min_camouflage_efficency = 
          config_singleton->get_config_float("min_camouflage_efficency");
  float rnd = float(std::rand()) / float(RAND_MAX);
  this->camouflage_efficiency = rnd * (max_camouflage_efficency - min_camouflage_efficency) + min_camouflage_efficency;
}

ICreature *Camouflage::clone(){
    ICreature* c = CreatureDecorator::clone();
    ICreature* CreatureDecorated = new Camouflage(c);
    return CreatureDecorated;
}

float Camouflage::get_camouflage() const{
  float original_camouflage = CreatureDecorator::get_camouflage();
  // those two variable should be imported from the config file 
  float new_camouflage = original_camouflage + this->camouflage_efficiency;
  return std::min(new_camouflage, this->max_camouflage);
}


void Camouflage::draw(UImg &support) const{
  CreatureDecorator::draw(support);

    float size =  this->get_size();
    int x0 = int(size/10);
    int y0 = int(size/10);

    T* black = new T[ 3 ];
    black[ 0 ] = 0;
    black[ 1 ] = 0;
    black[ 2 ] = 0;

    int x1 = this->get_x() + x0;
    int y1 = this->get_y();

    int x2 = this->get_x() - x0;
    int y2 = this->get_y();

    support.draw_rectangle(x1,y1,x2,y2,black,1,2);
  
}