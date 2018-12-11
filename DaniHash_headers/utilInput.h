/*
 * File:   utilInput.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef FUNCOES_AUXILIARES_MENU_H_INCLUDED
#define FUNCOES_AUXILIARES_MENU_H_INCLUDED

namespace dh{

namespace input{

    struct entrada_invalida : public exception{

        string errorMsg = "\nEntrada invalida!";

        entrada_invalida(string tipoEsperado){

            if (tipoEsperado != ""){
                errorMsg == errorMsg + " Espera " + tipoEsperado + ".";
            }

            cerr<<errorMsg;
            system("pause>0");
            output::apagaLinha();
            cin.clear();                //Retorna as flags de cin pro estado inicial
            cin.ignore(512,'\n');       //ignora tudo que tem na linha atual

        }

        //override do metodo virtual what(), de std::exception
        const char * what () const throw () {
            return errorMsg.c_str();
        }

    };

    int pegaInt(){
        int retorno = 0;

        if (!(cin>>retorno)) {         //Para tratar entradas que nao sejam int
            throw entrada_invalida("");
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
            }catch(entrada_invalida e){
                continue;
            }

            if ((resposta < minim) || (resposta > maxim)){
                output::printPauseNoNewline("\nOpcao invalida!");
                output::apagaLinha();
            }else{
                return resposta;
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
            }catch(entrada_invalida e){
                continue;
            }

            if (resposta < minim){
                output::printPauseNoNewline("\nNao pode ser menor que ",minim,"!");
                output::apagaLinha();
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
            }catch(entrada_invalida e){
                continue;
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
