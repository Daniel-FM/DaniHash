/*
 * File:   mainBASIC.cpp
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#include <fstream>
#include "../DaniHash_headers/utilGeral.h"
#include "../DaniHash_headers/TabelaHash.h"
#include "../DaniHash_headers/utilMenu.h"

using namespace std;

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
        cout<<"***DANIHASH BASIC v1.0.0***\n";
        atributos = pegaAtributosDaHash(true);

        Ohash* OH   = NULL;
        Chash* CH   = NULL;
        HOhash* HOH = NULL;

        instanciaHash(OH,CH,HOH,atributos.tipo,atributos.tamanho,atributos.limite);
        inicializaDiretorioINS(DEFAULT_FILENAME_INS);

        while (loopMenu){

            system("cls");
            cout<<"Fator de carga: "<<pegarFC(OH,CH,HOH)<<"\n\n";
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
            opcao = pegaRespostaMinMax("Opcao: ",0,9);

            switch (opcao){

                case 1:
                    int valor;

                    valor = pegaResposta("\n\nQual numero?\n");

                    fazerInsercao(OH,CH,HOH,valor,true);
                    salvarArqInsTemp(valor);

                    break;

                case 2:
                    str = "IF NOT EXIST .\\"+FILEPATH_INS+" mkdir "+FILEPATH_INS;
                    system(str.c_str());

                    if (!imprimeArquivosINS()) break;
                    nomeDoArquivo = setNomeDoArquivo();
                    if (nomeDoArquivo == "sair") break;

                    insereDeArquivo(nomeDoArquivo,OH,CH,HOH,3);

                    break;

                case 3:
                    num = pegaResposta("\n\nQue numero?\n");

                    fazerRemocao(OH,CH,HOH,num);

                    break;

                case 4:
                    num = pegaResposta("\n\nQue numero?\n");

                    posicao = fazerBusca(OH,CH,HOH,num,true);
                    if (posicao == -1)
                        printPause("O valor nao existe na tabela.",true);
                    else if (posicao == -2)
                        printPause("Numero maximo de tentativas atingido. O valor nao foi encontrado.",true);
                    else if (posicao == -3)
                        printPause("NENHUMA TABELA FOI INSTANCIADA.",true);
                    else{
                        cout<<"O valor foi encontrado na posicao "<<posicao;
                        system("pause>0");
                    }

                    break;

                case 5:
                    system("cls");
                    fazerImpressao(OH,CH,HOH);
                    break;

                case 6:
                    num = pegaResposta("Quantos?\n");
                    for (i = 0; i < num; i++){
                        fazerInsercao(OH,CH,HOH,i,false);
                        salvarArqInsTemp(i);
                    }
                    cout<<"\nValores inseridos!";
                    system("pause>0");
                    break;

                case 7:
                    num = pegaResposta("Quantos?\n");
                    for (i = 0; i < num; i++){
                        valor = rand()%999;
                        fazerInsercao(OH,CH,HOH,valor,false);
                        salvarArqInsTemp(valor);
                    }
                    cout<<"\nValores inseridos!";
                    system("pause>0");
                    break;

                case 8:
                    chamarDesenho(OH,CH,HOH);
                    break;

                case 9:
                    destroiHash(OH,CH,HOH);
                    fflush(stdin);
                    cout<<"\nEntre o nome do arquivo de insercao feito, sem \".ins\" (ou aperte apenas ENTER para manter o padrao):"<<endl;
                    if (cin.peek() != '\n') {                   //se o proximo caractere eh um newline
                        cin>>nomeDoArquivo;
                        str = "IF EXIST .\\"+FILEPATH_INS+nomeDoArquivo+".ins del "+FILEPATH_INS+nomeDoArquivo+".ins";
                        system(str.c_str());

                        str = "rename "+FILEPATH_INS+DEFAULT_FILENAME_INS+" "+nomeDoArquivo+".ins";
                        system(str.c_str());
                    }
                    loopMenu = false;
                    break;

                case 0:
                    printConstantes();
                    break;

                default:

                    cout<<"\n\nOpcao invalida!";
                    system("pause>0");

            }

        }

        cout<<"\nQuer fazer outra hashing? (1-Sim; 2-Nao)\n";
        respfinal = pegaRespostaMinMax("",1,2);

        if (respfinal == 2)
            loopPrograma = false;

    }

}
