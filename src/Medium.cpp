#include "Medium.h"
#include "Config.h"
#include "utils.h"
#include "Creature.h"

#include <cmath>        // sqrt
#include <ctime>
#include <iostream>

const T    Medium::white[] = {(T) 255, (T) 255, (T) 255};

Medium::Medium(int width, int height) :
    UImg(width, height, 1, 3), width(width), height(height) {
  Config *config_singleton = Config::get_instance();
  max_birth = config_singleton->get_config_int("max_birth");
  proba_clone = config_singleton->get_config_float("proba_clone");
  starting_pop = config_singleton->get_config_int("starting_pop");
  creature_factory = new Factory();
  std::srand(time(NULL));
  for (auto i = 0; i < starting_pop; i++) {
    add_creature();
  }
}

Medium::~Medium(void) {
  for (auto c : list_creatures) {
    delete c;
  }
  list_creatures.clear();
  creature_factory->~Factory();
}

void Medium::add_creature() {
  auto *c = creature_factory->create_creature();
  list_creatures.push_back(c);
}

void Medium::add_creature_clone(ICreature &c) {
  auto *creature = c.clone();
  list_creatures.push_back(creature);
}

bool Medium::are_colliding(const ICreature &c1, const ICreature &c2) const {
  int dx = c1.get_x() - c2.get_x();
  int dy = c1.get_y() - c2.get_y();
  int sum_size = c1.get_size() + c2.get_size();
  int dist_squared = dx * dx + dy * dy;
  int size_squared = sum_size * sum_size;
  return dist_squared <= size_squared;
}

bool Medium::collide(ICreature &c1, ICreature &c2) {
  float vx, vy, v_t_norm, v_n_norm, new_vx, new_vy;
  bool is_c1_dead = c1.is_collision_deadly();

  float tan_x = c2.get_y()-c1.get_y();
  float tan_y = c1.get_x()-c2.get_x();

  float tan_norm = std::sqrt(tan_x*tan_x+tan_y*tan_y);
  tan_x/=tan_norm;
  tan_y/=tan_norm;

  if (is_c1_dead) {
    c1.set_lifetime(0);
  } else {
    vx = c1.get_vx();
    vy = c1.get_vy();
    v_t_norm = vx*tan_x+vy*tan_y;
    v_n_norm = vx*tan_y-vy*tan_x;
    new_vx = tan_x*v_t_norm-tan_y*v_n_norm;
    new_vy = tan_y*v_t_norm+tan_x*v_n_norm;
    c1.set_vx_vy(new_vx,new_vy);
  }
  bool is_c2_dead = c2.is_collision_deadly();
  if (is_c2_dead) {
    c2.set_lifetime(0);
  } else {
    vx = c2.get_vx();
    vy = c2.get_vy();
    v_t_norm = vx*tan_x+vy*tan_y;
    v_n_norm = vx*tan_y-vy*tan_x;
    new_vx = tan_x*v_t_norm-tan_y*v_n_norm;
    new_vy = tan_y*v_t_norm+tan_x*v_n_norm;
    c2.set_vx_vy(new_vx,new_vy);
  }
  return is_c1_dead;
}

int Medium::get_height(void) const { return height; };

int Medium::get_width(void) const { return width; };

void Medium::kill_creature(ICreature &c) { list_creatures.remove(&c); }

int Medium::nb_neighbours(const ICreature &c) const {
  int nb = 0;
  for (const ICreature *other : list_creatures) {
    if (c.get_identity() != other->get_identity()) {
      if (c.is_detected(*other)) {
        nb++;
      }
    }
  }
  return nb;
}

std::unique_ptr<std::list<ICreature *>>
Medium::list_neighbours(const ICreature &c) const {
  std::unique_ptr<std::list<ICreature *>> neighbours = std::make_unique < std::list < ICreature * >> ();
  for (ICreature *other : list_creatures) {
    if (c.get_identity() != other->get_identity()) {
      if (c.is_detected(*other)) {
        neighbours->push_back(other);
      }
    }
  }
  return neighbours;
};

void Medium::step(void) {
  // macro from CImg, x and y are just variable names
  cimg_forXY(*this, x, y) fillC(x, y, 0, white[0], white[1], white[2]);
  bool is_dead;
  int c_id, other_id;
  std::list<ICreature *>::iterator c_iter;
  std::list<ICreature *>::iterator other_iter;
  ICreature *c, *other;
  c_iter = list_creatures.begin();
  for (; c_iter != list_creatures.end(); ++c_iter) {
    // Move
    c = *c_iter;
    c->action(*this, c);
    other_iter = c_iter;
    for (; other_iter != list_creatures.end(); ++other_iter) {
      // Check for collisions
      other = *other_iter;
      c_id = c->get_identity();
      other_id = other->get_identity();
      if (other_id != c_id) {
        if (are_colliding(*c, *other)) {
          is_dead = collide(*c, *other);
          if (is_dead) {
            break;
          }
        }
      }
    }
  }

  std::list<ICreature *>::iterator kill_iter = list_creatures.begin();
  while (kill_iter != list_creatures.end()) {
    c = *kill_iter;
    // The creature is too old or was killed by collision
    if (c->get_lifetime() <= 0) {
      delete c;
      list_creatures.erase(kill_iter++);
    }
    else {
      c->draw(*this);
      ++kill_iter;
      if (rand_range(0, 1, 1000) < proba_clone) {
        add_creature_clone(*c);
      }
    }
  }

}

std::list<ICreature *> &Medium::get_creatures_list() {
  return list_creatures;
}
