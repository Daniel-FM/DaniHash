/*
 * File:   EstruturaAuxiliarBuilder.h
 * Author: Daniel Franklin
 *
 * Created on 2020
 * Part of the DaniHash project
 */

#ifndef ESTAUXBUILDER
#define ESTAUXBUILDER

#include "EstruturaAuxiliar.h"
#include "Lista.h"
#include "ArvoreAVL.h"

namespace dh{

    namespace builder{

        EstruturaAuxiliar* buildEstruturaAuxiliar(int tipo){
            if (tipo == 1)
                return new lista();
            else
                return new arv_avl();
        }
    }
}
#endif