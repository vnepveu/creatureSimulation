//
// Created by Ménard Luc on 15/12/2020.
//

#ifndef CREATURESIMULATION_FIN_H
#define CREATURESIMULATION_FIN_H

#include "CreatureDecorator.h"
#include "ICreature.h"
#include "UImg.h"

class Fin: public CreatureDecorator {
public:
    Fin(ICreature* c);
    float get_speed() const override;
    void draw(UImg &support) const override;
};


#endif //CREATURESIMULATION_FIN_H
