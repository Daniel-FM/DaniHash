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
        int contDivisores = 0;

        for (int dummy = 1; dummy < i/2; dummy++){

            if (i % dummy == 0)
                contDivisores++;

        }

        if (contDivisores == 1)
            return true;
        else
            return false;

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

    void printPause(string texto, bool podeImprimir){

        if (podeImprimir){
            cout<<texto<<"\n";
            system("pause>0");
        }

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

struct arquivo_defeituoso : public exception {

    int l = 0;
    string t = "";

    arquivo_defeituoso(int linha, int tipo){
        l = linha;

        switch (tipo){
            case 1:
                t = "Caractere invalido";
                break;
            case 2:
                t = "Sem identificador \"INS\"";
                break;
            default:
                t = "???";
        }
    }

    //override do metodo virtual what(), de std::exception
    const char * what () const throw () {
        string str = ("\nProblema na linha " + to_string(l)) + (": " + t);
        return str.c_str();
    }
};


//
#endif // FUNCOES_AUXILIARES_GERAL_H_INCLUDED
