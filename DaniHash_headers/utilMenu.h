/*
 * File:   utilMenu.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef FUNCOES_AUXILIARES_MENU_H_INCLUDED
#define FUNCOES_AUXILIARES_MENU_H_INCLUDED

namespace dh{

namespace menu{

    void backspace(int posicoes){
        for (int i=0;i<posicoes;i++){
            cout<<"\b \b";
        }
    }

    int pegaRespostaMinMax(string pedido, int minim, int maxim){

        while (true){
            int resposta=0;
            cout<<pedido;

            if (!(cin>>resposta)) {         //Para tratar entradas que nao sejam int
                cout<<"\nEntrada invalida!";
                system("pause>0");
                backspace(17);
                cin.clear();
                cin.ignore(512,'\n');
                continue;
            }

            if ((resposta < minim) || (resposta > maxim)){
                cout<<"\nOpcao invalida!";
                system("pause>0");
                backspace(15);
            }else{
                return resposta;
            }
        }

    }

    int pegaRespostaMin(string pedido, int minim){

        int resposta=0;

        while (true){
            cout<<pedido;
            if (!(cin>>resposta)) {         //Para tratar entradas que nao sejam int
                cout << "Entrada invalida!";
                system("pause>0");
                backspace(17);
                cin.clear();                //Retorna ss flags de cin pro estado inicial
                cin.ignore(512,'\n');       //ignora tudo que tem na linha atual
                continue;
            }

            if (resposta < minim){
                cout<<"\nNao pode ser menor que "<<minim;
                system("pause>0");
                backspace(24);
            }else{
                return resposta;
            }
        }

    }

    int pegaRespostaInt(string pedido){
        int resposta;

        while (true){
            cout<<pedido;
            if (!(cin>>resposta)) {         //Para tratar entradas que nao sejam int
                cout << "Entrada invalida!";
                system("pause>0");
                backspace(17);
                cin.clear();
                cin.ignore(512,'\n');
                continue;
            }

            return resposta;
        }

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
