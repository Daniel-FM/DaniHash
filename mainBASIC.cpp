/*
 * File:   mainBASIC.cpp
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */
#include "include/DaniHashCore.h"

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

        //Para sair do loop principal e encerrar o programa
        if (atributos.tipo == 0) break;

        loopMenu = true;

        TabelaHash* h = instanciaHash(atributos);
        arquivos::inicializaDiretorioINS(DEFAULT_FILENAME_INS);

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
            opcaoMenu = input::pegaRespostaMinMax("","Opcao: ",0,9);

            switch (opcaoMenu){

                case 1:
                    numeroTmp = input::pegaRespostaInt("\n\nQual numero?\n");

                    h->inserir(numeroTmp,true);
                    arquivos::acrescentarNoArquivoDeInstrucoes("INS",numeroTmp);

                    break;

                case 2:
                    comandoTmp = "IF NOT EXIST .\\"+FILEPATH_INS+" mkdir "+FILEPATH_INS;
                    system(comandoTmp.c_str());

                    if (!arquivos::imprimeArquivosINS()) break;

                    fileNameTmp = input::pegaRespostaStr(
                        "\nEntre o nome do arquivo que deseja usar (ou aperte apenas ENTER para cancelar):\n");
                    if (fileNameTmp == "")
                        break;
                    else
                        fileNameTmp += ".ins";

                    opcaoTmp = input::pegaRespostaMinMax("\nImprimir passo a passo? (1-Sim, 2-Nao)\n",1,2);

                    if (opcaoTmp == 1)
                        piTmp = true;
                    else
                        piTmp = false;

                    output::printNoPause(true);

                    try{
                        Results r = h->realizarInstrucoesDeArquivo(fileNameTmp, piTmp);
                        output::printNoPauseNoNewline("Instrucoes concluidas!");
                        output::printNoPause(" (",r.colisoes," colisoes, ",
                                          r.rehashings," rehashings)");
                        input::pegaRespostaStr("\n\nPressione ENTER para continuar... ");
                    }catch(excecao::excecao_arquivo e){
                        logError(e.what());
                    }

                    break;

                case 3:
                    numeroTmp = input::pegaRespostaInt("\n\nQue numero?\n");

                    h->remover(numeroTmp, true);
                    arquivos::acrescentarNoArquivoDeInstrucoes("DEL",numeroTmp);

                    break;

                case 4:
                    numeroTmp = input::pegaRespostaInt("\n\nQue numero?\n");

                    printResultadoBusca(true,h->buscar(numeroTmp,true));
                    arquivos::acrescentarNoArquivoDeInstrucoes("BSC",numeroTmp);

                    break;

                case 5:
                    system("cls");
                    h->imprimir();
                    break;

                case 6:
                    opcaoTmp = input::pegaRespostaInt("Quantos?\n");
                    for (int i = 0; i < opcaoTmp; i++){
                        h->inserir(i,false);
                        arquivos::acrescentarNoArquivoDeInstrucoes("INS",i);
                    }
                    output::printPause("\nValores inseridos!");
                    system("pause>0");
                    break;

                case 7:
                    opcaoTmp = input::pegaRespostaInt("Quantos?\n");
                    for (int i = 0; i < opcaoTmp; i++){
                        numeroTmp = random::getUniforme(500);
                        h->inserir(numeroTmp,false);
                        arquivos::acrescentarNoArquivoDeInstrucoes("INS",numeroTmp);
                    }
                    output::printPause("\nValores inseridos!");
                    break;

                case 8:
                    h->desenha_hash();
                    break;

                case 9:
                    delete h;
                    fileNameTmp = input::pegaRespostaStr( nada+
                            "\nEntre o nome para o arquivo de insercao criado "+
                            "(ou aperte apenas ENTER para manter o nome padrao): \n");
                    if (fileNameTmp != "") {
                        comandoTmp = "IF EXIST .\\"+FILEPATH_INS+fileNameTmp+".ins del "+FILEPATH_INS+fileNameTmp+".ins";
                        system(comandoTmp.c_str());

                        comandoTmp = "rename "+FILEPATH_INS+DEFAULT_FILENAME_INS+" "+fileNameTmp+".ins";
                        system(comandoTmp.c_str());
                    }
                    loopMenu = false;
                    break;

                case 0:
                    output::printConstantes();
                    break;

                default:

                    output::printPause("\n\nOpcao invalida!");

            }

        }

    }
    return 1;
//
}
