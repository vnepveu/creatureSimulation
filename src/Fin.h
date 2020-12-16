#ifndef FIN_H
#define FIN_H

#include "CreatureDecorator.h"
#include "ICreature.h"
#include "UImg.h"

class Fin: public CreatureDecorator {
public:
    Fin(ICreature* c);
    float get_speed() const override;
    void draw(UImg &support) const override;
};


#endif //FIN_H
