/*
 * File:   utilInput.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#include "Excecoes.h"

#ifndef FUNCOES_AUXILIARES_MENU_H_INCLUDED
#define FUNCOES_AUXILIARES_MENU_H_INCLUDED


namespace dh{

namespace in{

    int pegaInt(){
        int retorno = 0;

        if (!(cin>>retorno)) {         //Para tratar entradas que nao sejam int
            throw exc::entrada_invalida();
        }

        return retorno;
    }

    int pegaRespostaMinMax(string pedido, string prompt, int a, int b){

        int resposta=0;
        int minim = math::minimo(a,b);
        int maxim = math::maximo(a,b);

        cout<<pedido;
        while (true){
            cout<<prompt;

            try{
                resposta = pegaInt();
                if ((resposta < minim) || (resposta > maxim)){
                    out::printPauseNoNewline("\nOpcao invalida!");
                    out::apagaLinha();
                }else{
                    return resposta;
                }
            }catch(exc::entrada_invalida e){
                out::logError(e.what());
                out::apagaLinha();
            }
        }

    }
    int pegaRespostaMinMax(string pedido, int a, int b){
        return pegaRespostaMinMax(pedido,"",a,b);
    }
    int pegaRespostaMinMax(int a, int b){
        return pegaRespostaMinMax("","",a,b);
    }

    int pegaRespostaMin(string pedido, string prompt, int minim){

        int resposta=0;

        cout<<pedido;
        while (true){
            cout<<prompt;
            try{
                resposta = pegaInt();
            }catch(exc::entrada_invalida e){
                out::logError(e.what());
            }

            if (resposta < minim){
                out::printPauseNoNewline("\nNao pode ser menor que ",minim,"!");
                out::apagaLinha();
            }else{
                return resposta;
            }
        }

    }
    int pegaRespostaMin(string pedido,int minim){
        return pegaRespostaMin(pedido,"",minim);
    }
    int pegaRespostaMin(int minim){
        return pegaRespostaMin("","",minim);
    }

    int pegaRespostaInt(string pedido, string prompt){
        int resposta;

        cout<<pedido;
        while (true){
            cout<<prompt;
            try{
                resposta = pegaInt();
            }catch(exc::entrada_invalida e){
                out::logError(e.what());
            }

            return resposta;
        }

    }
    int pegaRespostaInt(string pedido){
        return pegaRespostaInt(pedido,"");
    }
    int pegaRespostaInt(){
        return pegaRespostaInt("","");
    }

    string pegaRespostaStr(string pedido){
        string resposta;

        fflush(stdin);
        cout<<pedido;
        if (cin.peek() == '\n') {     //se o proximo caractere eh um newline
            resposta = "";
        } else{
            cin>>resposta;
        }

        return resposta;
    }

};

};

//
#endif // FUNCOES_AUXILIARES_MENU_H_INCLUDED
