/*
 * File:   TabelaHash.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef TABELAHASH_H_INCLUDED
#define TABELAHASH_H_INCLUDED

namespace dh{

    class TabelaHash{

        public:

        int TH, tipo;

        virtual void inserir(int chave, bool PI)=0;
        virtual void remover(int chave, bool PI)=0;
        virtual int buscar(int chave, bool PI)=0;
        virtual void imprimir()=0;
        virtual float getFC()=0;
        virtual int getColisoesDaInsercaoAtual()=0;
        virtual bool getFezRehashing()=0;
        virtual int getTipo()=0;

        bmk::Results realizarInstrucoesDeArquivo(string nomeDoArquivo, bool PI){

            int numeroNaLinha, numeroDaLinha = 0, tamanhoDaLinha, indiceInicial;
            string linha, operacao, opAnterior = "";
            ifstream fileREAD;
            fileREAD.open(cons::FILEPATH_INS+nomeDoArquivo);
            bmk::Results resultado;
            bmk::cronometro cron;

            if (!fileREAD)
                throw exc::excecao_arquivo(exc::EX_CODE_FILE_FILENOTFOUND,cons::FILEPATH_INS+nomeDoArquivo);

            while(fileREAD){
                numeroDaLinha++;
                getline(fileREAD, linha);
                tamanhoDaLinha = linha.size();
                indiceInicial = arq::getIndiceInicial(linha);

                if (linha.empty()){
                    continue;
                }else if (linha.at(indiceInicial) == '#'){
                    if (linha.at(indiceInicial)  == '#' &&
                        linha.at(indiceInicial+1)!= '#'){

                        out::printNoPause(PI," * ",linha.substr(
                        indiceInicial+1,tamanhoDaLinha-indiceInicial+1)," * \n");
                    }
                }else{
                    if (tamanhoDaLinha >= 5){
                        operacao = linha.substr(indiceInicial,4);

                        try{
                            numeroNaLinha = stoi(linha.substr(indiceInicial+4,tamanhoDaLinha-4));
                        }catch(invalid_argument e){
                            throw exc::excecao_arquivo(exc::EX_CODE_FILE_INVALIDCHAR,to_string(numeroDaLinha));
                        }
                    }else{
                        throw exc::excecao_arquivo(exc::EX_CODE_FILE_INCOMPLETELINE,to_string(numeroDaLinha));
                    }

                    if(operacao =="INS "){
                        if (opAnterior != "INSERCAO"){
                            opAnterior = "INSERCAO";
                            out::printNoPause(PI,"INSERCAO:\n");
                        }
                        cron.reset();
                        this->inserir(numeroNaLinha, PI);
                        resultado.tempoBMK += cron.tempoDecorrido();

                        resultado.colisoes += getColisoesDaInsercaoAtual();

                        if (this->getFezRehashing())
                            resultado.rehashings++;

                    }else if (operacao == "DEL "){
                        if (opAnterior != "DELECAO"){
                            opAnterior = "DELECAO";
                            out::printNoPause(PI,"DELECAO:\n");
                        }
                        cron.reset();
                        this->remover(numeroNaLinha, PI);
                        resultado.tempoBMK += cron.tempoDecorrido();

                        resultado.colisoes += getColisoesDaInsercaoAtual();

                    }else if (operacao == "BSC "){
                        if (opAnterior != "BUSCA"){
                            opAnterior = "BUSCA";
                            out::printNoPause(PI,"BUSCA:\n");
                        }
                        cron.reset();
                        out::printResultadoBusca(PI,this->buscar(numeroNaLinha, PI));
                        resultado.tempoBMK += cron.tempoDecorrido();

                        resultado.colisoes += getColisoesDaInsercaoAtual();

                    }else{
                        fileREAD.close();
                        throw exc::excecao_arquivo(exc::EX_CODE_FILE_NOIDENTIFIER,to_string(numeroDaLinha));
                    }
                }
            }
            fileREAD.close();
            return resultado;
        }

        bmk::Results benchmarkINSERCAO(int quantidadeDeInsercoes, int opcao_insbmk, string fileName_insercao){
            int chaveParaInserir;
            double tempo = 0;
            ofstream fileINS;
            bmk::Results resultado;

            if (bmk::benchmarkComArquivoDeInsercao(opcao_insbmk)){
                fileINS.open(cons::FILEPATH_INS+fileName_insercao, ios::app);
            }

            for (int i = 0; i < quantidadeDeInsercoes; i++){
                //Geramos a variavel aleatoria a ser inserida
                chaveParaInserir = rand::getRandom(opcao_insbmk);

                if (chaveParaInserir < 0) chaveParaInserir = 0;

                if (bmk::benchmarkComArquivoDeInsercao(opcao_insbmk))
                    fileINS<<"INS "<<chaveParaInserir<<endl;

                bmk::cronometro cron;
                inserir(chaveParaInserir, false);   //Depois fazemos a insercao, medindo o tempo
                tempo += cron.tempoDecorrido();

                resultado.colisoes += getColisoesDaInsercaoAtual();
                if (this->getFezRehashing())
                    resultado.rehashings++;
            }
            resultado.tempoBMK = tempo;

            if (bmk::benchmarkComArquivoDeInsercao(opcao_insbmk))
                fileINS.close();

            //Se for um benchmark completo, inclui uma adicao ao arquivo com os tempos do bmk atual
            if (bmk::benchmarkComArquivoBenchmark(opcao_insbmk)){
                ofstream fileBMK;

                fileBMK.open(cons::FILEPATH_BMK+arq::montarNomeDoArquivoBMK(getTipo(),opcao_insbmk,true),ios::app);
                //Poe no arquivo o tempo da insercao
                fileBMK<<quantidadeDeInsercoes<<"\t"<<tempo<<endl;
                fileBMK.close();
            }

            return resultado;
        }

        double benchmarkBUSCA(int opcao_insbmk, string fileName_insercao){
            //Para cada numero inserido pelo arquivo de instrucoes, realizamos uma busca
            int numeroNaLinha, quantidadeDeBuscas = 0;
            double tempo = 0;
            string linha, ins_str;

            ifstream fileREAD;
            fileREAD.open(cons::FILEPATH_INS+fileName_insercao);  //Abre o arquivo de instrucoes gerado antes, pra saber o que deve ser buscado

            while(fileREAD){
                getline(fileREAD, linha);
                ins_str = linha.substr(0,3);
                if(ins_str=="INS"){
                    quantidadeDeBuscas++;
                    numeroNaLinha = stoi(linha.substr(4,linha.size()-4));

                    bmk::cronometro cron;
                    buscar(numeroNaLinha,false);
                    tempo += cron.tempoDecorrido();

                }else if (linha.empty() || linha.front() == '#' ||
                          ins_str=="DEL" || ins_str=="BSC"){
                    continue;
                }else{
                    cout<<"Arquivo com problema!"<<endl;
                    break;
                }
            }
            fileREAD.close();

            if (bmk::benchmarkComArquivoBenchmark(opcao_insbmk)){
                ofstream fileBMK;

                fileBMK.open(cons::FILEPATH_BMK + arq::montarNomeDoArquivoBMK(tipo,opcao_insbmk,false),ios::app);

                fileBMK<<quantidadeDeBuscas<<"\t"<<tempo<<endl;
                fileBMK.close();
            }

            return tempo;

        }

        virtual ~TabelaHash(){};

    };
};

//
#endif // TABELAHASH_H_INCLUDED
