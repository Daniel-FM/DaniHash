/*
 * File:   mainBMK.cpp
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#include "include/DaniHashCore.h"

using namespace std;
using namespace dh;

int main(){

    Atributos atributos;
    bool loopMenu;
    int opcaoMenu, opcaoTmp, numeroTmp, codigoBmkTmp;
    string comandoTmp, fileNameTmp;

    /** Variaveis usadas nos benchmarks **/
    int rehashingsTotais;
    unsigned int MAXcolisoes;
    double tempoTotal_insercoes, tempoTotal_buscas, tempoDecorrido, qtdInsercoes;
    bmk::Results resultadoBMK;
    /*************************************/

    while (true){

        loopMenu = true;
        system("cls");
        cout<<"***DANIHASH BMK v1.0.3***\n";
        atributos = pegaAtributosDaHash(false);

        //Para sair do loop principal e encerrar o programa
        if (atributos.tipo == 0) break;

        TabelaHash* h;

        while (loopMenu){

            system("cls");

            cout<<"O que deseja?\n\n";
            cout<<"1) Realizar instrucoes de um arquivo\n";
            cout<<"2) Inserir numeros ordenados\n";
            cout<<"3) Inserir numeros aleatorios (Uniforme)\n";
            cout<<"4) Inserir numeros aleatorios (Normal)\n";
            cout<<"5) Inserir numeros aleatorios (Exponencial)\n";
            cout<<"6) Realizar benchmark completo (Uniforme)\n";
            cout<<"7) Realizar benchmark completo (Normal)\n";
            cout<<"8) Realizar benchmark completo (Exponencial)\n";
            cout<<"9) Retornar ao menu inicial\n\n";
            opcaoMenu = in::pegaRespostaMinMax("","Opcao: ",0,9);

            switch (opcaoMenu){

                case 1:
                    h = instanciaHash(atributos);

                    comandoTmp = "IF NOT EXIST .\\"+cons::FILEPATH_INS+" mkdir "+cons::FILEPATH_INS;
                    system(comandoTmp.c_str());

                    if (!arq::imprimeArquivosINS()) break;

                    fileNameTmp = in::pegaRespostaStr(
                        "\nEntre o nome do arquivo que deseja usar (ou aperte apenas ENTER para cancelar):\n");
                    if (fileNameTmp == "")
                        break;
                    else
                        fileNameTmp += ".ins";

                    opcaoTmp = in::pegaRespostaMinMax("\nIncluir buscas? (1-Sim; 2-Nao)\n",1,2);

                    try{
                        h = instanciaHash(atributos);
                        resultadoBMK = h->realizarInstrucoesDeArquivo(fileNameTmp, false);

                        if(opcaoTmp == 1)
                            tempoTotal_buscas = h->benchmarkBUSCA(0,fileNameTmp);

                        cout<<"\nTudo feito!\n\n";
                        cout<<"Tempo insercoes: "<<resultadoBMK.tempoBMK<<"ms\n";

                        if (opcaoTmp == 1)
                        cout<<"Tempo buscas:    "<<tempoTotal_buscas<<"ms\n";

                        cout<<"Colisoes:        "<<resultadoBMK.colisoes<<"\n";
                        cout<<"Rehashings:      "<<resultadoBMK.rehashings<<"\n";
                    }catch(bad_alloc e1){
                        out::logError(cons::BAD_ALLOC_MSG);
                    }catch(exc::excecao_arquivo e2){
                        out::logError(e2.what());
                    }

                    delete h;
                    break;

                case 2:
                case 3:
                case 4:
                case 5:
                    numeroTmp = in::pegaRespostaInt("\nQuantos?\n");

                    fileNameTmp = in::pegaRespostaStr("\nNome do arquivo de insercao a ser gerado (aperte apenas ENTER para usar o nome padrao):\n");
                    if (fileNameTmp == "") {                   //se o usuario nao entrou nada
                        fileNameTmp = cons::DEFAULT_FILENAME_INS;   //atribui o nome padrao
                    } else{
                        fileNameTmp += ".ins";
                    }

                    opcaoTmp = in::pegaRespostaMinMax("\nIncluir buscas? (1-Sim; 2-Nao)\n",1,2);

                    if (opcaoTmp == 1){
                        opcaoTmp = 10;
                    }

                    arq::inicializaDiretorioINS(fileNameTmp);

                    try{
                        h = instanciaHash(atributos);
                        resultadoBMK = h->benchmarkINSERCAO(numeroTmp,opcaoMenu,fileNameTmp);
                        if(opcaoTmp == 10);
                        tempoTotal_buscas = h->benchmarkBUSCA(0,fileNameTmp);

                        cout<<"\nTudo feito!\n\n";
                        cout<<"Tempo insercoes: "<<resultadoBMK.tempoBMK<<"ms\n";

                        if (opcaoTmp == 10)
                        cout<<"Tempo buscas:    "<<tempoTotal_buscas<<"ms\n";

                        cout<<"Colisoes:        "<<resultadoBMK.colisoes<<"\n";
                        cout<<"Rehashings:      "<<resultadoBMK.rehashings<<"\n";

                    }catch(bad_alloc e){
                        out::logError(cons::BAD_ALLOC_MSG);
                    }

                    system("pause>0");
                    delete h;
                    break;

                case 6:
                case 7:
                case 8:
                    opcaoTmp = in::pegaRespostaMinMax("\nIncluir buscas? (1-Sim; 2-Nao)\n",1,2);

                    codigoBmkTmp = opcaoMenu;
                    if (opcaoTmp == 1){
                        codigoBmkTmp *= 10;      //Faz isso para poder identificar quando o benchmark inclui buscas
                                                        //(a variavel referente a opcao escolhida fica divisivel por 10)
                    }

                    tempoTotal_insercoes = 0;   //O somatorio dos tempos de cada insercao do benchmark
                    tempoTotal_buscas = 0;      //O somatorio dos tempos de cada busca do benchmark
                    tempoDecorrido = 0;         //O tempo real do processo inteiro de benchmark
                    rehashingsTotais = 0;
                    MAXcolisoes = 0;
                    qtdInsercoes = 1;
                    //Inicializa o diretorio do arquivo de benchmark de insercoes
                    arq::inicializaDiretorioBMK(arq::montarNomeDoArquivoBMK(atributos.tipo,codigoBmkTmp,true));

                    //E inicializa o diretorio do de buscas, caso isso tenha sido pedido
                    if ((opcaoTmp%10) == 0) arq::inicializaDiretorioBMK(arq::montarNomeDoArquivoBMK(atributos.tipo,codigoBmkTmp,false));

                    cout<<"\nTrabalhando...    0%";
                    try{
                        bmk::cronometro cron;
                        while (true){
                            if ((opcaoTmp%10) == 0) arq::inicializaDiretorioINS(cons::FILENAME_DUMMY);

                            if (qtdInsercoes > cons::QTDMAX_INS)
                                qtdInsercoes = cons::QTDMAX_INS;

                            h = instanciaHash(atributos);
                            resultadoBMK = h->benchmarkINSERCAO(qtdInsercoes, codigoBmkTmp, cons::FILENAME_DUMMY);
                            if ((codigoBmkTmp%10) == 0) tempoTotal_buscas += h->benchmarkBUSCA(codigoBmkTmp, cons::FILENAME_DUMMY)/1000;
                            delete h;

                            out::printNewPerc(qtdInsercoes,cons::QTDMAX_INS);

                            tempoTotal_insercoes += resultadoBMK.tempoBMK/1000;
                            rehashingsTotais += resultadoBMK.rehashings;
                            if (resultadoBMK.colisoes > MAXcolisoes)
                                MAXcolisoes = resultadoBMK.colisoes;

                            if (qtdInsercoes == cons::QTDMAX_INS)
                                break;
                            else
                                qtdInsercoes += cons::GAP_INS;
                        }
                        tempoDecorrido = cron.tempoDecorrido();

                        cout<<"\n\nBenchmark completo!\n\n";
                        /*o "tempo decorrido" indica o tempo real decorrido do inicio ao fim do processo de
                        benchmark, pois inclui o tempo de leitura e escrita dos arquivos, o que eh muito mais
                        evidente em benchmarks incluindo buscas.*/
                        cout<<"Tempo decorrido: "<<tempoDecorrido/1000<<"s\n";
                        cout<<"Tempo insercoes: "<<tempoTotal_insercoes<<"s\n";

                        if ((codigoBmkTmp%10==0))
                        cout<<"Tempo buscas:    "<<tempoTotal_buscas<<"s\n";

                        cout<<"MAX Colisoes:    "<<MAXcolisoes<<"\n";
                        cout<<"Rehashings:      "<<rehashingsTotais<<"\n";

                    }catch(bad_alloc e){
                        out::logError(cons::BAD_ALLOC_MSG);
                    }

                    system("pause>0");
                    break;

                case 9:
                    loopMenu = false;
                    break;

                case 0:
                    out::printConstantes();
                    break;

                default:
                    out::printPause("\n\nOpcao invalida!");

            }

        }

    }
    return 1;

}
