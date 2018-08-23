/*
 * File:   TabelaHash.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef TABELAHASH_H_INCLUDED
#define TABELAHASH_H_INCLUDED

#include "EstrAuxiliares.h"

#include "utilDesenho.h"
#include "utilBenchmark.h"
#include "utilArquivos.h"

#include "OpenHashing.h"
#include "ClosedHashing.h"
#include "HalfOpenHashing.h"

class TabelaHash{

    public:

    Ohash* OH = NULL;
    Chash* CH = NULL;
    HOhash* HOH = NULL;

    TabelaHash(int tipo, int tamanho, int limite){
        instanciaHash(tipo, tamanho, limite);
    }

    void instanciaHash(int tipo, int tamanho, int limite){

        if (tipo <= 2){
            OH = new Ohash(tamanho, tipo);
        }else if (tipo <= 5){
            CH = new Chash(tamanho, tipo);
        }else{
            HOH = new HOhash(tamanho, tipo, limite);
        }
    }

    void destroiHash(){

        if (OH != NULL){
            delete OH;
        }else if (CH != NULL){
            delete CH;
        }else if (HOH != NULL){
            delete HOH;
        }

    }

    Results benchmarkINSERCAO(int qtdInsercoes, int opcao, string nomeDoArquivoINS){

        if (OH != NULL)
            return OH->benchmarkINSERCAO(qtdInsercoes, opcao, nomeDoArquivoINS);
        else if (CH != NULL)
            return CH->benchmarkINSERCAO(qtdInsercoes, opcao, nomeDoArquivoINS);
        else if (HOH != NULL)
            return HOH->benchmarkINSERCAO(qtdInsercoes, opcao, nomeDoArquivoINS);

        return inicializaResults();
    }

    double benchmarkBUSCA(int opcao, string nomeDoArquivoINS){

        if (OH != NULL)
            return OH->benchmarkBUSCA(opcao, nomeDoArquivoINS);
        else if (CH != NULL)
            return CH->benchmarkBUSCA(opcao, nomeDoArquivoINS);
        else if (HOH != NULL)
            return HOH->benchmarkBUSCA(opcao, nomeDoArquivoINS);

        return 0;
    }

    float getFC(){

        if (OH != NULL)
            return OH->getFC();
        else if (CH != NULL)
            return CH->getFC();
        else if (HOH != NULL)
            return HOH->getFC();

        return 0;
    }

    void inserir(int valor, bool PI){

        if (OH != NULL)
            OH->inserir(valor, PI);
        else if (CH != NULL)
            CH->inserir(valor, CH->tabela, CH->TH, PI);
        else if (HOH != NULL){
            if (HOH->tipo == 6)
                HOH->inserir_CTQ(valor, HOH->tabela, HOH->alt_max, PI);
            else
                HOH->inserir(valor, HOH->tabela, HOH->alt_max, PI);
        }
    }

    void remover(int valor){

        if (OH != NULL)
            OH->remover(valor);
        else if (CH != NULL)
            CH->remover(valor);
        else if (HOH != NULL)
            HOH->remover(valor);
    }

    int buscar(int valor, bool PI){

        if (OH != NULL)
            return OH->buscar(valor,PI);
        else if (CH != NULL)
            return CH->buscar(valor,PI);
        else if (HOH != NULL)
            return HOH->buscar(valor,PI);

        return -3;
    }

    void imprimir(){

        if (OH != NULL)
            OH->imprimir();
        else if (CH != NULL)
            CH->imprimir();
        else if (HOH != NULL)
            HOH->imprimir();
    }


    void desenhar(){
        if (OH != NULL)
            OH->preparar_janela();
        else if (CH != NULL)
            CH->preparar_janela();
        else if (HOH != NULL)
            HOH->preparar_janela();
    }

    int getColisoesDaInsercaoAtual(){

        if (OH != NULL)
            return 0;
        else if (CH != NULL)
            return CH->colisoesDaInsercaoAtual;
        else if (HOH != NULL)
            return HOH->colisoesDaInsercaoAtual;

        return 0;
    }

    bool fezRehashing(){

        if (OH != NULL)
            return false;
        else if (CH != NULL)
            return CH->fezRehashing;
        else if (HOH != NULL)
            return HOH->fezRehashing;

        return false;
    }

    void inserirDeArquivo(string nomeDoArquivo, int checagem){

        /*
        Checagem:
        3  = insercao no DaniHashBASIC
        2  = insercao no DaniHashBMK sem busca
        10 = insercao no DaniHashBMK com busca
        0  =
        */

        int numeroNaLinha, colisoes = 0, rehashings = 0, numeroDaLinha = 1;
        string linha, substringINS;
        ifstream fileREAD;
        fileREAD.open(FILEPATH_INS+nomeDoArquivo);
        double tempoINSERCOES = 0;

        while(fileREAD){
            getline(fileREAD, linha);
            substringINS = linha.substr(0,4);

            if(substringINS =="INS "){
                try{
                    numeroNaLinha = stoi(linha.substr(4,linha.size()-4));
                }catch(invalid_argument &e){
                    cerr<<"\nProblema na linha "<<numeroDaLinha<<" do arquivo (invalid_argument).";
                    system("pause>0");
                    break;
                }

                benchmark b;
                this->inserir(numeroNaLinha, false);
                tempoINSERCOES += b.elapsed();

                colisoes += getColisoesDaInsercaoAtual();

                if (this->fezRehashing() == true)
                    rehashings++;

                numeroDaLinha++;

            }else if (linha == ""){
                break;
            }else{
                cerr<<"\nProblema na linha "<<numeroDaLinha<<" do arquivo (sem identificador \"INS \").";
                system("pause>0");
                checagem = 0;
                break;
            }
        }
        fileREAD.close();

        if (checagem == 0) return;

        if (checagem == 3){
            printPause("\nInsercoes feitas!",true);
        }

        double tempoBUSCAS = 0;

        if (checagem == 10){
            tempoBUSCAS = this->benchmarkBUSCA(0,nomeDoArquivo);
        }

        cout<<"\nTudo feito!\n\n";
        cout<<"Tempo insercoes: "<<tempoINSERCOES<<"ms\n";

        if (checagem == 10)
        cout<<"Tempo buscas:    "<<tempoBUSCAS<<"ms\n";

        cout<<"Colisoes:        "<<colisoes<<"\n";
        cout<<"Rehashings:      "<<rehashings<<"\n";
        system("pause>0");

    }

    ~TabelaHash(){
        destroiHash();
    }

};

//
#endif // TABELAHASH_H_INCLUDED
