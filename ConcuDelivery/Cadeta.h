//
// Created by Kimi on 10/04/15.
//

#ifndef CONCUDELIVERY_CADETA_H
#define CONCUDELIVERY_CADETA_H

#include "Proceso.h"

class Cadeta: public Proceso {
private:
    virtual string nombre();

    virtual void realizarTarea();
};


#endif //CONCUDELIVERY_CADETA_H
