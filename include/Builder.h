/*
 * File:   Builder.h
 * Author: Daniel Franklin
 *
 * Created on 2020
 * Part of the DaniHash project
 */

#ifndef BUILDER_H_INCLUDED
#define BUILDER_H_INCLUDED

#include <iostream>
#include "utilInput.h"
#include "Constantes.h"

#include "EstruturaAuxiliar.h"
#include "ArvoreAVL.h"
#include "Lista.h"

#include "TabelaHash.h"
#include "OpenHashing.h"
#include "ClosedHashing.h"
#include "HalfOpenHashing.h"


using namespace std;


namespace dh{

    namespace builder{

        struct Atributos{

            public:

            int tipo, tamanho, limite;
        };

        TabelaHash* buildHash(Atributos attr){

            if (attr.tipo <= 2){
                return new Ohash(attr.tamanho,attr.tipo);
            }else if (attr.tipo <= 5){
                return new Chash(attr.tamanho,attr.tipo);
            }else{
                return new HOhash(attr.tamanho,attr.tipo,attr.limite);
            }

        }

        EstruturaAuxiliar* buildEstruturaAuxiliar(int tipo){
            if (tipo == 1)
                return new lista();
            else
                return new arv_avl();
        }

        Atributos pegaAtributosDaHash(bool trueBASIC_falseBMK){
            int opcao1, opcao2, tipoEscolhido = 0;
            Atributos atr;

            cout<<"Que tipo de hash deseja utilizar?\n\n";
            cout<<"1) Open Hashing com Lista\n";
            cout<<"2) Open Hashing com Arvore\n";
            cout<<"3) Closed Hashing\n";
            cout<<"4) Half-Open Hashing\n";
            cout<<"5) Sair\n\n";

            opcao1 = in::pegaRespostaMinMax("","Opcao: ",1,5);

            if (opcao1 == 1 || opcao1 == 2){
                tipoEscolhido = opcao1;
            }if (opcao1 == 3){

                system("cls");
                cout<<"Escolha como lidar com colisoes:\n\n";
                cout<<"1) Tentativa linear\n";
                cout<<"2) Tentativa quadratica\n";
                cout<<"3) Duplo hashing\n\n";

                opcao2 = in::pegaRespostaMinMax("","Opcao: ",1,3);

                    if (opcao2 == 1) tipoEscolhido = 3;
                else if (opcao2 == 2) tipoEscolhido = 4;
                else if (opcao2 == 3) tipoEscolhido = 5;

            }else if (opcao1 == 4){

                system("cls");
                cout<<"Deseja que a hash utilize tentativa quadratica?\n\n";
                cout<<"1) Sim, insercoes em arvores cheias contam como colisoes (CQT)\n";
                cout<<"2) Nao, pode continuar inserindo em arvores cheias (SQT)\n\n";

                opcao2 = in::pegaRespostaMinMax("","Opcao: ",1,2);

                    if (opcao2 == 1) tipoEscolhido = 6;
                else if (opcao2 == 2) tipoEscolhido = 7;

            }else if (opcao1 == 5){
                //Para forcar a ir logo pro final
                tipoEscolhido = 0;
                trueBASIC_falseBMK = false;
            }

            atr.tipo = tipoEscolhido;
            atr.limite = 1; //so pra deixar inicializado com algum valor

            if (trueBASIC_falseBMK == true){
                atr.tamanho = in::pegaRespostaMin("\nTamanho da tabela: ", 1);

                if (tipoEscolhido >= 6)
                    atr.limite = in::pegaRespostaMin("Altura maxima das arvores: ", 1);

            }else{
                atr.tamanho = cons::TAM_INICIAL_HASH_BMK;
                atr.limite = cons::ALT_MAX_BMK;
            }

            return atr;

        }

        
    };
};


//
#endif // BUILDER_H_INCLUDED