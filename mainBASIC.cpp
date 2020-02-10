/*
 * File:   mainBASIC.cpp
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */
#include "include/DaniHashCore.h"
#include "include/utilDesenho.h"

using namespace std;
using namespace dh;

int main(){

    Atributos atributos;
    bool loopMenu, piTmp;
    int opcaoMenu, opcaoTmp, numeroTmp;
    string comandoTmp, fileNameTmp;

    while (true){

        system("cls");
        cout<<"***DANIHASH BASIC v1.0.3***\n";
        atributos = pegaAtributosDaHash(true);

        //para sair do loop principal e encerrar o programa
        if (atributos.tipo == 0) break;

        loopMenu = true;

        TabelaHash* h = instanciaHash(atributos);
        arq::inicializaDiretorioINS(cons::DEFAULT_FILENAME_INS);

        while (loopMenu){

            system("cls");
            cout<<"Fator de carga: "<<h->getFC()<<"\n\n";
            cout<<"O que deseja?\n\n";
            cout<<"1) Inserir um numero manualmente\n";
            cout<<"2) Realizar instrucoes de um arquivo\n";
            cout<<"3) Remover\n";
            cout<<"4) Buscar\n";
            cout<<"5) Imprimir conteudo da hash\n";
            cout<<"6) Inserir varios numeros ordenados\n";
            cout<<"7) Inserir varios numeros aleatorios\n";
            cout<<"8) Desenhar a hash em uma nova janela\n";
            cout<<"9) Retornar ao menu inicial\n\n";
            opcaoMenu = in::pegaRespostaMinMax("","Opcao: ",0,9);

            switch (opcaoMenu){

                case 1:
                    numeroTmp = in::pegaRespostaInt("\n\nQual numero?\n");

                    h->inserir(numeroTmp,true);
                    arq::acrescentarNoArquivoDeInstrucoes("INS",numeroTmp);

                    break;

                case 2:
                    comandoTmp = "IF NOT EXIST .\\"+cons::FILEPATH_INS+" mkdir "+cons::FILEPATH_INS;
                    system(comandoTmp.c_str());

                    if (!arq::imprimeArquivosINS()) break;

                    fileNameTmp = in::pegaRespostaStr(
                        "\nEntre o nome do arquivo que deseja usar (ou aperte apenas ENTER para cancelar):\n");
                    if (fileNameTmp == "")
                        break;
                    else
                        fileNameTmp += ".ins";

                    opcaoTmp = in::pegaRespostaMinMax("\nImprimir passo a passo? (1-Sim, 2-Nao)\n",1,2);

                    if (opcaoTmp == 1)
                        piTmp = true;
                    else
                        piTmp = false;

                    out::printNoPause(true);

                    try{
                        bmk::Results r = h->realizarInstrucoesDeArquivo(fileNameTmp, piTmp);
                        out::printNoPauseNoNewline("Instrucoes concluidas!");
                        out::printNoPause(" (",r.colisoes," colisoes, ",
                                          r.rehashings," rehashings)");
                        in::pegaRespostaStr("\n\nPressione ENTER para continuar... ");
                    }catch(exc::excecao_arquivo e){
                        out::logError(e.what());
                    }

                    break;

                case 3:
                    numeroTmp = in::pegaRespostaInt("\n\nQue numero?\n");

                    h->remover(numeroTmp, true);
                    arq::acrescentarNoArquivoDeInstrucoes("DEL",numeroTmp);

                    break;

                case 4:
                    numeroTmp = in::pegaRespostaInt("\n\nQue numero?\n");

                    out::printResultadoBusca(true,h->buscar(numeroTmp,true));
                    arq::acrescentarNoArquivoDeInstrucoes("BSC",numeroTmp);

                    break;

                case 5:
                    system("cls");
                    h->imprimir();
                    break;

                case 6:
                    opcaoTmp = in::pegaRespostaInt("Quantos?\n");
                    for (int i = 0; i < opcaoTmp; i++){
                        h->inserir(i,false);
                        arq::acrescentarNoArquivoDeInstrucoes("INS",i);
                    }
                    out::printPause("\nValores inseridos!");
                    system("pause>0");
                    break;

                case 7:
                    opcaoTmp = in::pegaRespostaInt("Quantos?\n");
                    for (int i = 0; i < opcaoTmp; i++){
                        numeroTmp = rand::getUniforme(500);
                        h->inserir(numeroTmp,false);
                        arq::acrescentarNoArquivoDeInstrucoes("INS",numeroTmp);
                    }
                    out::printPause("\nValores inseridos!");
                    break;

                case 8:
                    if (atributos.tipo <= 2){
                        desenho::desenhaOpenHash(dynamic_cast<Ohash*>(h));
                    }else if (atributos.tipo > 5){
                        desenho::desenhaHalfOpenHash(dynamic_cast<HOhash*>(h));
                    }else {
                        out::printPause("\nComing soon!");
                    }
                    break;

                case 9:
                    delete h;
                    fileNameTmp = in::pegaRespostaStr( cons::nada+
                            "\nEntre o nome para o arquivo de insercao criado "+
                            "(ou aperte apenas ENTER para manter o nome padrao): \n");
                    if (fileNameTmp != "") {
                        comandoTmp = "IF EXIST .\\"+cons::FILEPATH_INS+fileNameTmp+".ins del "+cons::FILEPATH_INS+fileNameTmp+".ins";
                        system(comandoTmp.c_str());

                        comandoTmp = "rename "+cons::FILEPATH_INS+cons::DEFAULT_FILENAME_INS+" "+fileNameTmp+".ins";
                        system(comandoTmp.c_str());
                    }
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
//
}
