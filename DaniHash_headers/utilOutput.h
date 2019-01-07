/*
 * File:   utilOutput.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef FUNCOES_AUXILIARES_GERAL_H_INCLUDED
#define FUNCOES_AUXILIARES_GERAL_H_INCLUDED
#include "Constantes.h"

namespace dh{

namespace output{

    /************* FUNCOES DE IMPRESSAO CONDICIONAL COM E SEM PAUSA *************/

    void printPause(bool b){
        if (b){
            system("pause>0");
            cout<<"\n";
        }
    }

    void printPauseNoNewline(bool b){
        if (b){
            system("pause>0");
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
    void printPauseNoNewline(bool pode, T t, Args... args){
        if (pode){
            cout<<t ;
            printPauseNoNewline(pode, args...) ;
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
    void printPauseNoNewline(T t, Args... args){
        printPauseNoNewline(true, t, args...);
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

    void backspace(int posicoes){
        for (int i=0;i<posicoes;i++){
            cout<<"\b \b";
        }
    }

    void apagaLinha(){
        cout<<"\r";
        for (int i=0;i<50;i++)
            cout<<" ";
        cout<<"\r";
    }

    void printConstantes(){
        cout<<endl;
        cout<<"MEDIA_CHAVE_BMK       = "<<constantes::MEDIA_CHAVE_BMK<<endl;
        cout<<"QTDMAX_INS            = "<<constantes::QTDMAX_INS<<endl;
        cout<<"GAP_INS               = "<<constantes::GAP_INS<<endl;
        cout<<"LIMIAR_PARA_REHASHING = "<<constantes::LIMIAR_PARA_REHASHING<<endl;
        cout<<"TAM_INICIAL_HASH_BMK  = "<<constantes::TAM_INICIAL_HASH_BMK<<endl;
        cout<<"ALT_MAX_BMK           = "<<constantes::ALT_MAX_BMK<<endl;
        cout<<"FILEPATH_INS          = "<<constantes::FILEPATH_INS<<endl;
        cout<<"FILEPATH_BMK          = "<<constantes::FILEPATH_BMK<<endl;
        system("pause>0");
    }

    void printResultadoBusca(bool PI, int indice){
        if (indice >= 0)
            printPause(PI,"\nA chave foi encontrada na posicao ",indice,".\n");
        else if (indice == -1)
            printPause(PI,"\nA chave nao existe na tabela.\n");
        else if (indice == -2)
            printPause(PI,"\nNumero maximo de tentativas atingido. A chave nao foi encontrada.\n");
        else
            printPause(PI,"\nErro ",indice,".\n");
    }

};
};


//
#endif // FUNCOES_AUXILIARES_GERAL_H_INCLUDED
