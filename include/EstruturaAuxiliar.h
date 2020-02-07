/*
 * File:   EstruturaAuxiliar.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef ESTRUTURAAUXILIAR_H_INCLUDED
#define ESTRUTURAAUXILIAR_H_INCLUDED

namespace dh{
class No{

    public:

    int info;
    virtual ~No(){}
};

class EstruturaAuxiliar{

    public:

    virtual void inserir(int i)=0;
    virtual bool remover(int i)=0;
    virtual bool buscar(int i)=0;
    virtual void imprimir()=0;

    virtual bool isNull()=0;
    virtual No* getRaiz()=0;
    virtual int getAltura()=0;

    virtual ~EstruturaAuxiliar(){}

};
};

//
#endif // ESTRUTURAAUXILIAR_H_INCLUDED
