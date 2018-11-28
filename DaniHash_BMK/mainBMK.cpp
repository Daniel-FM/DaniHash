/*
 * File:   mainBMK.cpp
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#include <fstream>
#include "../DaniHash_headers/utilOutput.h"
#include "../DaniHash_headers/utilMath.h"
#include "../DaniHash_headers/utilInput.h"
#include "../DaniHash_headers/TabelaHash.h"

using namespace std;
using namespace dh;

int main(){

    srand ( time(NULL) );

    Atributos atributos;
    bool loopPrograma = true, loopMenu;
    int respfinal, opcao, num, rehashingsTotais;
    unsigned int MAXcolisoes;
    double tempoTotal_insercoes, tempoTotal_buscas, tempoDecorrido, qtdInsercoes;
    Results resultadoBMK;
    ifstream fileREAD;
    string linha, nomeDoArquivo, comandoConsole, substringINS;

    while (loopPrograma){

        loopMenu = true;
        system("cls");
        cout<<"***DANIHASH BMK v1.0.2***\n";
        atributos = pegaAtributosDaHash(false);
        TabelaHash* h;

        while (loopMenu){

            system("cls");

            cout<<"O que deseja?\n\n";
            cout<<"1) Ler e inserir numeros de um arquivo\n";
            cout<<"2) Inserir numeros ordenados\n";
            cout<<"3) Inserir numeros aleatorios (Uniforme)\n";
            cout<<"4) Inserir numeros aleatorios (Normal)\n";
            cout<<"5) Inserir numeros aleatorios (Exponencial)\n";
            cout<<"6) Realizar benchmark completo (Uniforme)\n";
            cout<<"7) Realizar benchmark completo (Normal)\n";
            cout<<"8) Realizar benchmark completo (Exponencial)\n";
            cout<<"9) Sair\n\n";
            opcao = input::pegaRespostaMinMax("Opcao: ",0,9);

            switch (opcao){

                case 1:
                    h = instanciaHash(atributos);

                    comandoConsole = "IF NOT EXIST .\\"+FILEPATH_INS+" mkdir "+FILEPATH_INS;
                    system(comandoConsole.c_str());

                    if (!arquivos::imprimeArquivosINS()) break;

                    nomeDoArquivo = input::pegaRespostaStr(
                        "\nEntre o nome do arquivo que deseja usar (ou aperte apenas ENTER para cancelar):\n");
                    if (nomeDoArquivo == "")
                        break;
                    else
                        nomeDoArquivo += ".ins";

                    cout<<"\nIncluir buscas? (1-Sim; 2-Nao)\n";
                    respfinal = input::pegaRespostaMinMax("",1,2);

                    try{
                        h = instanciaHash(atributos);
                        resultadoBMK = h->inserirDeArquivo(nomeDoArquivo, false);

                        if(respfinal == 1);
                        tempoTotal_buscas = h->benchmarkBUSCA(0,nomeDoArquivo);

                        cout<<"\nTudo feito!\n\n";
                        cout<<"Tempo insercoes: "<<resultadoBMK.tempoBMK<<"ms\n";

                        if (respfinal == 10)
                        cout<<"Tempo buscas:    "<<tempoTotal_buscas<<"ms\n";

                        cout<<"Colisoes:        "<<resultadoBMK.colisoes<<"\n";
                        cout<<"Rehashings:      "<<resultadoBMK.rehashings<<"\n";
                    }catch(bad_alloc &e){
                        cerr<<BAD_ALLOC_MSG;
                    }catch(arquivo_defeituoso &e){
                        cerr<<e.what();
                    }

                    system("pause>0");
                    delete h;
                    break;

                case 2:
                case 3:
                case 4:
                case 5:
                    cout<<"\nQuantos?\n";
                    cin>>num;

                    fflush(stdin);
                    cout<<"\nNome do arquivo de insercao a ser gerado (aperte apenas ENTER para usar o nome padrao):\n";
                    if (cin.peek() == '\n') {                   //se o proximo caractere eh um newline
                        nomeDoArquivo = DEFAULT_FILENAME_INS;   //atribui o nome padrao
                    } else{
                        cin>>nomeDoArquivo;                     //senao, atribui o nome digitado
                        nomeDoArquivo += ".ins";
                    }

                    cout<<"\nIncluir buscas? (1-Sim; 2-Nao)\n";
                    respfinal = input::pegaRespostaMinMax("",1,2);

                    if (respfinal == 1){
                        respfinal = 10;
                    }

                    arquivos::inicializaDiretorioINS(nomeDoArquivo);

                    try{
                        h = instanciaHash(atributos);
                        resultadoBMK = h->benchmarkINSERCAO(num,opcao,nomeDoArquivo);

                        if(respfinal == 10);
                        tempoTotal_buscas = h->benchmarkBUSCA(0,nomeDoArquivo);

                        cout<<"\nTudo feito!\n\n";
                        cout<<"Tempo insercoes: "<<resultadoBMK.tempoBMK<<"ms\n";

                        if (respfinal == 10)
                        cout<<"Tempo buscas:    "<<tempoTotal_buscas<<"ms\n";

                        cout<<"Colisoes:        "<<resultadoBMK.colisoes<<"\n";
                        cout<<"Rehashings:      "<<resultadoBMK.rehashings<<"\n";

                    }catch(bad_alloc &e){
                        cerr<<BAD_ALLOC_MSG;
                    }

                    system("pause>0");
                    delete h;
                    break;

                case 6:
                case 7:
                case 8:
                    cout<<"\nIncluir buscas? (1-Sim; 2-Nao)\n";
                    num = input::pegaRespostaMinMax("",1,2);

                    if (num == 1){
                        opcao *= 10;    //Faz isso para poder identificar quando o benchmark inclui buscas
                                        //(a variavel referente a opcao escolhida fica divisivel por 10)
                    }

                    tempoTotal_insercoes = 0;   //O somatorio dos tempos de cada insercao do benchmark
                    tempoTotal_buscas = 0;      //O somatorio dos tempos de cada busca do benchmark
                    tempoDecorrido = 0;         //O tempo real do processo inteiro de benchmark
                    rehashingsTotais = 0;
                    MAXcolisoes = 0;
                    qtdInsercoes = 1;
                    //Inicializa o diretorio do arquivo de benchmark de insercoes
                    arquivos::inicializaDiretorioBMK(arquivos::montarNomeDoArquivoBMK(atributos.tipo,opcao,true));

                    //E inicializa o diretorio do de buscas, caso isso tenha sido pedido
                    if ((opcao%10) == 0) arquivos::inicializaDiretorioBMK(arquivos::montarNomeDoArquivoBMK(atributos.tipo,opcao,false));

                    cout<<"\nTrabalhando...    0%";
                    try{
                        benchmark::cronometro cron;
                        while (true){
                            if ((opcao%10) == 0) arquivos::inicializaDiretorioINS(FILENAME_DUMMY);

                            if (qtdInsercoes > QTDMAX_INS)
                                qtdInsercoes = QTDMAX_INS;

                            h = instanciaHash(atributos);
                            resultadoBMK = h->benchmarkINSERCAO(qtdInsercoes, opcao, FILENAME_DUMMY);
                            if ((opcao%10) == 0) tempoTotal_buscas += h->benchmarkBUSCA(opcao, FILENAME_DUMMY)/1000;
                            delete h;

                            printNewPerc(qtdInsercoes,QTDMAX_INS);

                            tempoTotal_insercoes += resultadoBMK.tempoBMK/1000;
                            rehashingsTotais += resultadoBMK.rehashings;
                            if (resultadoBMK.colisoes > MAXcolisoes)
                                MAXcolisoes = resultadoBMK.colisoes;

                            if (qtdInsercoes == QTDMAX_INS)
                                break;
                            else
                                qtdInsercoes += GAP_INS;
                        }
                        tempoDecorrido = cron.tempoDecorrido();

                        cout<<"\n\nBenchmark completo!\n\n";
                        /*o "tempo decorrido" indica o tempo real decorrido do inicio ao fim do processo de
                        benchmark, pois inclui o tempo de leitura e escrita dos arquivos, o que eh muito mais
                        evidente em benchmarks incluindo buscas.*/
                        cout<<"Tempo decorrido: "<<tempoDecorrido/1000<<"s\n";
                        cout<<"Tempo insercoes: "<<tempoTotal_insercoes<<"s\n";

                        if ((opcao%10==0))
                        cout<<"Tempo buscas:    "<<tempoTotal_buscas<<"s\n";

                        cout<<"MAX Colisoes:    "<<MAXcolisoes<<"\n";
                        cout<<"Rehashings:      "<<rehashingsTotais<<"\n";

                    }catch(bad_alloc& ba){
                        cerr << BAD_ALLOC_MSG;
                    }

                    system("pause>0");
                    break;

                case 9:
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

        cout<<"\nDeseja voltar ao menu inicial? (1-Sim; 2-Nao)\n";
        respfinal = input::pegaRespostaMinMax("",1,2);

        if (respfinal == 2)
            loopPrograma = false;

    }

}
