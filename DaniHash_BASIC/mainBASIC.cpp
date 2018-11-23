/*
 * File:   mainBASIC.cpp
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#include <fstream>
#include "../DaniHash_headers/utilGeral.h"
#include "../DaniHash_headers/utilMenu.h"
#include "../DaniHash_headers/TabelaHash.h"

using namespace std;
using namespace dh;

int main(){

    srand ( time(NULL) );

    Atributos atributos;
    bool loopPrograma = true, loopMenu;
    int respfinal, opcao, num, i, posicao;
    string linha, substringINS, str, nomeDoArquivo;
    ifstream fileREAD;

    while (loopPrograma){

        loopMenu = true;
        system("cls");
        cout<<"***DANIHASH BASIC v1.0.2***\n";
        atributos = pegaAtributosDaHash(true);

        TabelaHash* h = instanciaHash(atributos);
        arquivos::inicializaDiretorioINS(DEFAULT_FILENAME_INS);

        while (loopMenu){

            system("cls");
            cout<<"Fator de carga: "<<h->getFC()<<"\n\n";
            cout<<"O que deseja?\n\n";
            cout<<"1) Inserir um numero manualmente\n";
            cout<<"2) Ler e inserir numeros de um arquivo\n";
            cout<<"3) Remover\n";
            cout<<"4) Buscar\n";
            cout<<"5) Imprimir conteudo da hash\n";
            cout<<"6) Inserir varios numeros ordenados\n";
            cout<<"7) Inserir varios numeros aleatorios\n";
            cout<<"8) Desenhar a hash em uma nova janela\n";
            cout<<"9) Sair\n\n";
            opcao = menu::pegaRespostaMinMax("Opcao: ",0,9);

            switch (opcao){

                case 1:
                    int valor;

                    valor = menu::pegaRespostaInt("\n\nQual numero?\n");

                    h->inserir(valor,true);
                    arquivos::salvarArqInsTemp(valor);

                    break;

                case 2:
                    str = "IF NOT EXIST .\\"+FILEPATH_INS+" mkdir "+FILEPATH_INS;
                    system(str.c_str());

                    if (!arquivos::imprimeArquivosINS()) break;

                    nomeDoArquivo = menu::pegaRespostaStr(
                        "\nEntre o nome do arquivo que deseja usar (ou aperte apenas ENTER para cancelar):\n");
                    if (nomeDoArquivo == "")
                        break;
                    else
                        nomeDoArquivo += ".ins";

                    try{
                        Results r = h->inserirDeArquivo(nomeDoArquivo, true);
                        geral::printNoPauseNoNewline(true, "Insercoes concluidas!");
                        geral::printNoPause(true, " (",r.colisoes," colisoes, ",
                                          r.rehashings," rehashings)");
                        menu::pegaRespostaStr("\n\nPressione ENTER para continuar... ");
                    }catch(arquivo_defeituoso &e){
                        cerr<<e.what();
                        system("pause>0");
                    }

                    break;

                case 3:
                    num = menu::pegaRespostaInt("\n\nQue numero?\n");

                    h->remover(num);

                    break;

                case 4:
                    num = menu::pegaRespostaInt("\n\nQue numero?\n");

                    posicao = h->buscar(num,true);
                    if (posicao == -1)
                        geral::printPause(true,"A chave nao existe na tabela.");
                    else if (posicao == -2)
                        geral::printPause(true,"Numero maximo de tentativas atingido. A chave nao foi encontrada.");
                    else if (posicao == -3)
                        geral::printPause(true,"NENHUMA TABELA FOI INSTANCIADA.");
                    else
                        geral::printPause(true,"A chave foi encontrada na posicao ",posicao);


                    break;

                case 5:
                    system("cls");
                    h->imprimir();
                    break;

                case 6:
                    num = menu::pegaRespostaInt("Quantos?\n");
                    for (i = 0; i < num; i++){
                        h->inserir(i,false);
                        arquivos::salvarArqInsTemp(i);
                    }
                    geral::printPause(true,"\nValores inseridos!");
                    system("pause>0");
                    break;

                case 7:
                    num = menu::pegaRespostaInt("Quantos?\n");
                    for (i = 0; i < num; i++){
                        valor = rand()%999;
                        h->inserir(valor,false);
                        arquivos::salvarArqInsTemp(valor);
                    }
                    geral::printPause(true,"\nValores inseridos!");
                    break;

                case 8:
                    h->desenha_hash();
                    break;

                case 9:
                    delete h;
                    nomeDoArquivo = menu::pegaRespostaStr( nada+
                            "\nEntre o nome para o arquivo de insercao criado "+
                            "(ou aperte apenas ENTER para manter o nome padrao): \n");
                    if (nomeDoArquivo != "") {
                        str = "IF EXIST .\\"+FILEPATH_INS+nomeDoArquivo+".ins del "+FILEPATH_INS+nomeDoArquivo+".ins";
                        system(str.c_str());

                        str = "rename "+FILEPATH_INS+DEFAULT_FILENAME_INS+" "+nomeDoArquivo+".ins";
                        system(str.c_str());
                    }
                    loopMenu = false;
                    break;

                case 0:
                    geral::printConstantes();
                    break;

                default:

                    geral::printPause("\n\nOpcao invalida!");

            }

        }

        cout<<"\nDeseja voltar ao menu inicial? (1-Sim; 2-Nao)\n";
        respfinal = menu::pegaRespostaMinMax("",1,2);

        if (respfinal == 2)
            loopPrograma = false;

    }
//
}
