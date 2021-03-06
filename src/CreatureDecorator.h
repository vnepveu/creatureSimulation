#ifndef CREATUREDECORATOR_H
#define CREATUREDECORATOR_H

#include "ICreature.h"
#include "Medium.h"
#include "UImg.h"

class CreatureDecorator : public ICreature {
 public:
  CreatureDecorator(ICreature *c);

  ~CreatureDecorator();
  // Methods whose behavior can be modified by concrete decorators

  void action(Medium &myMedium, ICreature* creature) override;
  
  ICreature *clone();

  void draw(UImg &support) const override;

  float get_camouflage() const override;

  int get_identity() const override;

  int get_lifetime() const override;

  float get_size() const override;

  float get_speed() const override;

  int get_x() const override;

  int get_y() const override;

  float get_vx() const override;

  float get_vy() const override;

  bool is_collision_deadly() const override;

  bool is_detected(const ICreature &c) const override;

  void set_coords(const int x, const int y) override;

  void set_lifetime(const int new_val) override;

  void set_vx_vy(const float vx, const float vy) override;

  void set_color(T* new_color) override;


private:
  ICreature *decoree;
};
#endif  // CREATUREDECORATOR_H
