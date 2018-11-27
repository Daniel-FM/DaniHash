/*
 * File:   utilGeral.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef FUNCOES_AUXILIARES_GERAL_H_INCLUDED
#define FUNCOES_AUXILIARES_GERAL_H_INCLUDED
#include <iostream>
#include <exception>
#include "Constantes.h"

namespace dh{

namespace geral{

    //Funcao para checar se um numero eh primo. Se so houver um divisor entre 1 e o numero/2
    //(que obviamente eh o numero 1), retorna 1.
    bool checaPrimo(int i){

        for (int dummy = 2; dummy < i/2; dummy++){
            if (i % dummy == 0)
                return false;
        }

        return true;
    }

    int pegaProxPrimMaior(int n){

        do{
            n++;
        }while (checaPrimo(n) == false);

        return n;

    }

    int pegaProxPrimMenor(int n){

        do{
            n--;
        }while ((checaPrimo(n) == false) && (n > 1));

        return n;
    }

    int maximo(int a, int b){
        if (a > b)
            return a;
        else
            return b;
    }

    /************* FUNCOES DE IMPRESSAO CONDICIONAL COM E SEM PAUSA *************/

    void printPause(bool b){
        if (b){
            system("pause>0");
            cout<<"\n";
        }
    }

    void printNoPause(bool b){
        if (b){
            cout<<"\n";
        }
    }

    void printNoPauseNoNewline(bool b){
    }

    //Funcoes recursivas para impressao, que aceitam uma quantidade indefinida de
    //tipos genericos (tendo pelo menos 1 bool no inicio).
    //Aquelas de cima sao chamadas quando so resta um argumento no conjunto, que
    //eh a variavel booleana (ou seja, ja imprimiu todas as variaveis que queremos)
    template<typename T, typename... Args>
    void printPause(bool pode, T t, Args... args){
        if (pode){
            cout<<t ;
            printPause(pode, args...) ;
        }
    }

    template<typename T, typename... Args>
    void printNoPause(bool pode, T t, Args... args){
        if (pode){
            cout<<t ;
            printNoPause(pode, args...) ;
        }
    }

    template<typename T, typename... Args>
    void printNoPauseNoNewline(bool pode, T t, Args... args){
        if (pode){
            cout<<t ;
            printNoPauseNoNewline(pode, args...) ;
        }
    }

    //Essas de baixo sao apenas uma opcao resumida das do meio, sem o bool antes
    template<typename T, typename... Args>
    void printPause(T t, Args... args){
        printPause(true, t, args...);
    }

    template<typename T, typename... Args>
    void printNoPause(T t, Args... args){
        printNoPause(true, t, args...);
    }

    template<typename T, typename... Args>
    void printNoPauseNoNewline(T t, Args... args){
        printNoPauseNoNewline(true, t, args...);
    }


    void printNewPerc(int dividendo, int divisor){

        int perc = ((float)dividendo/divisor)*100;
        cout<<"\b\b\b\b";
        if (perc < 10)
            cout<<"  ";
        else if (perc < 100)
            cout<<" ";

        cout<<perc<<"%";

    }

    void printConstantes(){
        cout<<endl;
        cout<<"MEDIA_CHAVE_BMK       = "<<dh::constantes::MEDIA_CHAVE_BMK<<endl;
        cout<<"QTDMAX_INS            = "<<dh::constantes::QTDMAX_INS<<endl;
        cout<<"GAP_INS               = "<<dh::constantes::GAP_INS<<endl;
        cout<<"LIMIAR_PARA_REHASHING = "<<dh::constantes::LIMIAR_PARA_REHASHING<<endl;
        cout<<"TAM_INICIAL_HASH_BMK  = "<<dh::constantes::TAM_INICIAL_HASH_BMK<<endl;
        cout<<"ALT_MAX_BMK           = "<<dh::constantes::ALT_MAX_BMK<<endl;
        cout<<"FILEPATH_INS          = "<<dh::constantes::FILEPATH_INS<<endl;
        cout<<"FILEPATH_BMK          = "<<dh::constantes::FILEPATH_BMK<<endl;
        system("pause>0");
    }

};
};


//
#endif // FUNCOES_AUXILIARES_GERAL_H_INCLUDED
