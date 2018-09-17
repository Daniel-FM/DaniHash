/*
 * File:   Lista.h
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

using namespace std;

class no_lista : public No{

    public:

        int info;
        no_lista* prox;

        no_lista(int valor){
            info = valor;
            prox = NULL;
        }

        ~no_lista(){
            delete prox;
        }

};

class lista : public EstruturaAuxiliar{

    public:

    no_lista* cabeca;
    int num_nos;

    lista(){
        cabeca = NULL;
        num_nos = 0;
    }

    ~lista(){
        delete cabeca;
    }

    void inserir(int i){

        no_lista* novono = new no_lista(i);

        novono->prox = cabeca;
        cabeca = novono;
        num_nos++;
    }

    bool remover(int i){

        no_lista* dummy = cabeca;
        no_lista* ant = NULL;

        while (dummy != NULL && dummy->info != i){

            ant = dummy;
            dummy = dummy->prox;

        }

        if (dummy == NULL){       //Se nao achar o numero que quero remover
            return false;             //Retorna sem fazer nada
        }

        if (ant == NULL){           //Se o numero que quero remover estiver na cabeca
            cabeca = cabeca->prox;
        }else{                      //Se estiver no meio ou final
            ant->prox = dummy->prox;
        }

        delete dummy;

        num_nos--;

        return true;


    }

    void imprimir(){

        no_lista* dummy = cabeca;

        while (dummy != NULL){
            cout<<dummy->info<<", ";
            dummy = dummy->prox;
        }

        delete dummy;

    }

    bool buscar(int i){
        return Buscar(i,cabeca);
    }

    bool Buscar(int valor, no_lista* no_atual){
        if (no_atual != NULL){
            if (no_atual->info == valor){
                return true;
            }else{
                return Buscar(valor, no_atual->prox);
            }
        }
        return false;
    }

    //Ja que essa funcao nao sera usada para listas, mas precisamos sobrescreve-la,
    //apenas retornamos um valor qualquer.
    int getAltura(){
        return -1;
    }

    bool isNull(){
        if (cabeca == NULL)
            return true;
        else
            return false;
    }

    no_lista* getRaiz(){
        return cabeca;
    }
};

//
#endif // LISTA_H_INCLUDED
