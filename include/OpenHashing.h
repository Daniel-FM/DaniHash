/*
 * File:   OpenHashing.h
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#ifndef OPEN_HASHING_H_INCLUDED
#define OPEN_HASHING_H_INCLUDED

#include "EstruturaAuxiliarBuilder.h"

namespace dh{
class Ohash: public TabelaHash{

    public:

    int TH, tipo, numPosOcupadas;
    EstruturaAuxiliar* *tabela;

    Ohash(int tamanho, int type){

        TH = tamanho;
        tipo = type;
        numPosOcupadas = 0;
        tabela = new EstruturaAuxiliar*[TH];

        for (int i = 0; i < TH; i++){
            tabela[i] = builder::buildEstruturaAuxiliar(tipo);
        }

    }

    ~Ohash(){
        for (int i=0;i<TH;i++)
            delete tabela[i];
    }

    int getTipo(){
        return tipo;
    }

    /************************************************* INSERIR *****************************************************************/

    void inserir(int chave, bool PI){

        int h = chave % TH;
        out::printNoPause(PI,chave," % ",TH," = ",h);
        //Checa se uma posicao anteriormente desocupada vai ficar ocupada agora
        if (tabela[h]->isNull())
            numPosOcupadas++;

        tabela[h]->inserir(chave);

        out::printPause(PI,"A chave ",chave," foi inserida no indice ",h,".\n");

    }

    /************************************************* REMOVER *****************************************************************/

    void remover(int chave, bool PI){

        int h = chave % TH;
        out::printNoPause(PI,chave," % ",TH," = ",h);

        if (tabela[h]->remover(chave) == true){
            //Checa se uma posicao anteriormente ocupada vai ficar desocupada agora
            if (tabela[h]->isNull())
                numPosOcupadas--;

            out::printPause(PI,"A chave foi deletada com sucesso do indice ",h,".\n");
        }else
            out::printPause(PI,"A chave nao foi encontrada.\n");

    }

    /************************************************* IMPRIMIR *****************************************************************/

    void imprimir(){

        for(int i = 0; i < TH; i++){

            cout<< "["<<i<<"]:\t";

            tabela[i]->imprimir();

            cout<<"\n";

        }

        system("pause>0");

    }

    /************************************************* BUSCAR *****************************************************************/

    int buscar(int chave, bool PI){

        int h = chave % TH;
        out::printNoPause(PI,chave," % ",TH," = ",h);
        bool encontrou = false;

        encontrou = tabela[h]->buscar(chave);

        if (encontrou){
            return h;
        } else{
            return -2;
        }

    }


    /************************************************* PEGAR FATOR DE CARGA *****************************************************************/

    float getFC(){

        return (float)numPosOcupadas /TH;       //Para obter um float atraves da divisao de inteiros, eu tenho que converter um desses
                                            //inteiros para float
    }


    /**************************** OUTRAS ********************************/

    int getColisoesDaInsercaoAtual(){
        return 0;
    }

    bool getFezRehashing(){
        return false;
    }

};
};

//
#endif // OPEN_HASHING_H_INCLUDED
