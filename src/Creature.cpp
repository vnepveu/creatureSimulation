#include "Creature.h"

#include <cmath>  // for cos, sin, acos and sqrt
#include <iostream>
#include <vector>

#include "Config.h"
#include "utils.h"

uint Creature::NEXT_IDENTITY = 0;

Creature::Creature(std::unique_ptr<IBehaviour> behaviour, T *color, int lifetime,
                   float speed, float size, int x, int y) {
  this->behaviour = move(behaviour);
  this->color = color;
  this->identity = Creature::NEXT_IDENTITY;
  Creature::NEXT_IDENTITY = Creature::NEXT_IDENTITY + 1;
  this->lifetime = lifetime;
  float orientation = rand_range(0, 2 * M_PI, 10000);
  this->size = size;
  this->speed = speed;
  this->vx = (this->speed) * cos(orientation);
  this->vy = (this->speed) * sin(orientation);
  this->x = x;
  this->y = y;
}

Creature::Creature(const Creature &c) {
  this->lifetime = c.lifetime;
  this->size = c.size;
  this->speed = c.speed;
  // Clones go in the opposite directions
  this->vx = -c.vx;
  this->vy = -c.vy;
  float off_x = -c.vx;
  float off_y = -c.vy;
  float norm_off = c.size*2/sqrt(off_x*off_x+off_y*off_y);
  this->x = c.x+off_x*norm_off;
  this->y = c.y+off_y*norm_off;
  this->color = c.color;
  this->identity = Creature::NEXT_IDENTITY;
  this->behaviour = c.behaviour->clone_behaviour();
  Creature::NEXT_IDENTITY = Creature::NEXT_IDENTITY + 1;

  // Apply an offset to the coord to avoird birth collision
  this->x -= c.vx * 2 * c.size / (c.vx * c.vx + c.vy * c.vy);
  this->y -= c.vy * 2 * c.size / (c.vx * c.vx + c.vy * c.vy);
}

Creature::~Creature() {};

void Creature::action(Medium &myMedium, ICreature* creature) {
  this->behaviour->next_step(creature, &myMedium);
  this->lifetime = this->lifetime - 1;
}
ICreature *Creature::clone() {
  return new Creature(*this);
};

void Creature::draw(UImg &support) const {
  float orientation = 0;
  if (vy <= 0) {
    orientation = acos(vx / (sqrt(vx * vx + vy * vy)));
  } else {
    orientation = 2 * M_PI - acos(vx / (sqrt(vx * vx + vy * vy)));
  }
  double xt = x + cos(orientation) * this->size / 2;
  double yt = y - sin(orientation) * this->size / 2;

  support.draw_ellipse(x, y, this->size, this->size/5,
                       -orientation / M_PI * 180., color);
  support.draw_circle(xt, yt, this->size / 2., color);
}

float Creature::get_camouflage() const { return 0; };

int Creature::get_identity() const { return identity; }

int Creature::get_lifetime() const { return lifetime; };

float Creature::get_size() const { return size; };

float Creature::get_speed() const { return speed; };

int Creature::get_x() const { return x; };

int Creature::get_y() const { return y; };

float Creature::get_vx() const { return vx; };

float Creature::get_vy() const { return vy; };

bool Creature::is_collision_deadly() const {
  Config *cfg = Config::get_instance();
  float death_proba = cfg->get_config_float("collision_death");
  if (rand_range(0, 1, 10000) < death_proba)
    return true;
  else
    return false;
};

bool Creature::is_detected(const ICreature &c) const { return false; }

void Creature::set_coords(const int x, const int y) {
  this->x = x;
  this->y = y;
}

void Creature::set_lifetime(const int new_lifetime) {
  lifetime = new_lifetime;
}

void Creature::set_vx_vy(const float new_vx, const float new_vy) {
  this->vx = new_vx;
  this->vy = new_vy;
}


void Creature::set_color(T* new_color){
  this->color = new_color;
}