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

//Funcao para checar se um numero eh primo. Se so houver um divisor entre 1 e o numero/2
//(que obviamente eh o numero 1), retorna 1.
bool checa_primo(int i){
    int cont_divisores = 0;

    for (int dummy = 1; dummy < i/2; dummy++){

        if (i % dummy == 0)
            cont_divisores++;

    }

    if (cont_divisores == 1)
        return true;
    else
        return false;

}

int pega_prox_prim_maior(int n){

    do{
        n++;
    }while (checa_primo(n) == false);

    return n;

}

int pega_prox_prim_menor(int n){

    do{
        n--;
    }while ((checa_primo(n) == false) && (n > 1));

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
    cout<<"MEDIA_CHAVE_BMK       = "<<MEDIA_CHAVE_BMK<<endl;
    cout<<"QTDMAX_INS            = "<<QTDMAX_INS<<endl;
    cout<<"GAP_INS               = "<<GAP_INS<<endl;
    cout<<"LIMIAR_PARA_REHASHING = "<<LIMIAR_PARA_REHASHING<<endl;
    cout<<"TAM_INICIAL_HASH_BMK  = "<<TAM_INICIAL_HASH_BMK<<endl;
    cout<<"ALT_MAX_BMK           = "<<ALT_MAX_BMK<<endl;
    cout<<"FILEPATH_INS          = "<<FILEPATH_INS<<endl;
    cout<<"FILEPATH_BMK          = "<<FILEPATH_BMK<<endl;
    system("pause>0");
}

struct arquivo_defeituoso : public exception {

    int l = 0;
    string t = "";

    arquivo_defeituoso(int linha, int tipo){
        l = linha;

        if (tipo == 1)
            t = "Caractere invalido";
        else if (tipo == 2)
            t = "Sem identificador \"INS\"";
        else
            t = "???";
        //deixando assim para facilitar caso va adicionar outras razoes
    }

    //override do metodo virtual what(), de std::exception
    const char * what () const throw () {
        string str = ("\nProblema na linha " + to_string(l)) + (": " + t);
        return str.c_str();
    }
};

//
#endif // FUNCOES_AUXILIARES_GERAL_H_INCLUDED
