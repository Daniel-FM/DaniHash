/*
 * File:   TabelaHash.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef TABELAHASH_H_INCLUDED
#define TABELAHASH_H_INCLUDED

#include "EstruturaAuxiliar.h"

#include "utilDesenho.h"
#include "utilBenchmark.h"
#include "utilArquivos.h"

namespace dh{

    using namespace dh::benchmark;
    using namespace dh::arquivos;
    using namespace dh::desenho;
    using namespace dh::output;
    using namespace dh::input;
    using namespace dh::math;
    using namespace dh::constantes;
    using namespace dh::gva;

    class TabelaHash{

        public:

        int TH, tipo;

        virtual void inserir(int chave, bool PI)=0;
        virtual void remover(int chave)=0;
        virtual int buscar(int chave, bool PI)=0;
        virtual void imprimir()=0;
        virtual void desenha_hash()=0;
        virtual float getFC()=0;
        virtual int getColisoesDaInsercaoAtual()=0;
        virtual bool getFezRehashing()=0;

        Results inserirDeArquivo(string nomeDoArquivo, bool PI){

            int numeroNaLinha, numeroDaLinha = 1;
            string linha, substringINS;
            ifstream fileREAD;
            fileREAD.open(FILEPATH_INS+nomeDoArquivo);
            Results resultado;

            while(fileREAD){
                getline(fileREAD, linha);
                substringINS = linha.substr(0,4);

                if(substringINS =="INS "){
                    try{
                        numeroNaLinha = stoi(linha.substr(4,linha.size()-4));
                    }catch(invalid_argument e){
                        throw arquivo_defeituoso(numeroDaLinha,1);
                    }

                    cronometro cron;
                    this->inserir(numeroNaLinha, PI);
                    resultado.tempoBMK += cron.tempoDecorrido();

                    resultado.colisoes += getColisoesDaInsercaoAtual();

                    if (this->getFezRehashing())
                        resultado.rehashings++;

                    numeroDaLinha++;

                }else if (linha == ""){
                    break;
                }else{
                    throw arquivo_defeituoso(numeroDaLinha,2);
                }
            }
            fileREAD.close();
            return resultado;
        }

        Results benchmarkINSERCAO(int quantidadeDeInsercoes, int opcao_insbmk, string fileName_insercao){
            int chaveParaInserir;
            double tempo = 0;
            ofstream fileINS;
            Results resultado;

            if (benchmarkComArquivoDeInsercao(opcao_insbmk)){
                fileINS.open(FILEPATH_INS+fileName_insercao, ios::app);
            }

            for (int i = 0; i < quantidadeDeInsercoes; i++){
                //Geramos a variavel aleatoria a ser inserida
                chaveParaInserir = getRand(opcao_insbmk);

                if (chaveParaInserir < 0) chaveParaInserir = 0;

                if (benchmarkComArquivoDeInsercao(opcao_insbmk))
                    fileINS<<"INS "<<chaveParaInserir<<endl;

                cronometro cron;
                inserir(chaveParaInserir, false);   //Depois fazemos a insercao, medindo o tempo
                tempo += cron.tempoDecorrido();

                resultado.colisoes += getColisoesDaInsercaoAtual();
                if (this->getFezRehashing())
                    resultado.rehashings++;
            }
            resultado.tempoBMK = tempo;

            if (benchmarkComArquivoDeInsercao(opcao_insbmk))
                fileINS.close();

            //Se for um benchmark completo, inclui uma adicao ao arquivo com os tempos do bmk atual
            if (benchmarkComArquivoBenchmark(opcao_insbmk)){
                ofstream fileBMK;

                fileBMK.open(FILEPATH_BMK+montarNomeDoArquivoBMK(tipo,opcao_insbmk,true),ios::app);
                //Poe no arquivo o tempo da insercao
                fileBMK<<quantidadeDeInsercoes<<"\t"<<tempo<<endl;
                fileBMK.close();
            }

            return resultado;
        }

        double benchmarkBUSCA(int opcao_insbmk, string fileName_insercao){
            //Praticamente igual ao benchmark de insercao, mas usando a operacao de busca
            int numeroNaLinha, quantidadeDeBuscas = 0;
            double tempo = 0;
            string linha, ins_str;

            ifstream fileREAD;
            fileREAD.open(FILEPATH_INS+fileName_insercao);  //Abre o arquivo de insercao gerado antes, pra saber o que deve ser buscado

            while(fileREAD){
                getline(fileREAD, linha);
                ins_str = linha.substr(0,3);
                if(ins_str=="INS"){
                    quantidadeDeBuscas++;
                    numeroNaLinha = stoi(linha.substr(4,linha.size()-4));

                    cronometro cron;
                    buscar(numeroNaLinha,false);
                    tempo += cron.tempoDecorrido();

                }else if (linha == ""){
                    break;
                }else{
                    cout<<"Arquivo com problema!"<<endl;
                    break;
                }
            }
            fileREAD.close();

            if (benchmarkComArquivoBenchmark(opcao_insbmk)){
                ofstream fileBMK;

                fileBMK.open(FILEPATH_BMK+montarNomeDoArquivoBMK(tipo,opcao_insbmk,false),ios::app);

                fileBMK<<quantidadeDeBuscas<<"\t"<<tempo<<endl;
                fileBMK.close();
            }

            return tempo;

        }

        virtual ~TabelaHash(){};

    };

    struct Atributos{

        public:

        int tipo, tamanho, limite;
    };

    Atributos pegaAtributosDaHash(bool trueBASIC_falseBMK){
        int opcao1, opcao2, tipoEscolhido = 0;
        Atributos atr;

        cout<<"Que tipo de hash deseja utilizar?\n\n";
        cout<<"1) Open Hashing com Lista\n";
        cout<<"2) Open Hashing com Arvore\n";
        cout<<"3) Closed Hashing\n";
        cout<<"4) Half-Open Hashing\n";
        cout<<"5) Sair\n\n";

        opcao1 = pegaRespostaMinMax("","Opcao: ",1,5);

        if (opcao1 == 1 || opcao1 == 2){
            tipoEscolhido = opcao1;
        }if (opcao1 == 3){

            system("cls");
            cout<<"Escolha como lidar com colisoes:\n\n";
            cout<<"1) Tentativa linear\n";
            cout<<"2) Tentativa quadratica\n";
            cout<<"3) Duplo hashing\n\n";

            opcao2 = pegaRespostaMinMax("","Opcao: ",1,3);

                 if (opcao2 == 1) tipoEscolhido = 3;
            else if (opcao2 == 2) tipoEscolhido = 4;
            else if (opcao2 == 3) tipoEscolhido = 5;

        }else if (opcao1 == 4){

            system("cls");
            cout<<"Deseja que a hash utilize tentativa quadratica?\n\n";
            cout<<"1) Sim, insercoes em arvores cheias contam como colisoes (CQT)\n";
            cout<<"2) Nao, pode continuar inserindo em arvores cheias (SQT)\n\n";

            opcao2 = pegaRespostaMinMax("","Opcao: ",1,2);

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
            atr.tamanho = pegaRespostaMin("\nTamanho da tabela: ", 1);

            if (tipoEscolhido >= 6)
                atr.limite = pegaRespostaMin("Altura maxima das arvores: ", 1);

        }else{
            atr.tamanho = TAM_INICIAL_HASH_BMK;
            atr.limite = ALT_MAX_BMK;
        }

        return atr;

    }
};

#include "OpenHashing.h"
#include "ClosedHashing.h"
#include "HalfOpenHashing.h"

namespace dh{

    TabelaHash* instanciaHash(Atributos attr){

        if (attr.tipo <= 2)
            return new Ohash(attr.tamanho,attr.tipo);
        else if (attr.tipo <= 5)
            return new Chash(attr.tamanho,attr.tipo);
        else
            return new HOhash(attr.tamanho,attr.tipo,attr.limite);

    }
};

//
#endif // TABELAHASH_H_INCLUDED
