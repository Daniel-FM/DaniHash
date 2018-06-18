#include <fstream>
#include "../DaniHash_headers/utilGeral.h"
#include "../DaniHash_headers/TabelaHash.h"
#include "../DaniHash_headers/utilMenu.h"

using namespace std;

/*
A fazer:

-Corrigir bugs na delecao da arvore

*/
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
        cout<<"***DANIHASH BMK***\n";
        atributos = pegaAtributosDaHash(false);

        Ohash* OH   = NULL;
        Chash* CH   = NULL;
        HOhash* HOH = NULL;

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
            opcao = pegaRespostaMinMax("Opcao: ",0,9);

            switch (opcao){

                case 1:
                    instanciaHash(OH,CH,HOH,atributos.tipo,atributos.tamanho,atributos.limite);

                    comandoConsole = "IF NOT EXIST .\\"+FILEPATH_INS+" mkdir "+FILEPATH_INS;
                    system(comandoConsole.c_str());

                    if (!imprimeArquivosINS()) break;

                    nomeDoArquivo = setNomeDoArquivo();
                    if (nomeDoArquivo == "sair") break;

                    cout<<"\nIncluir buscas? (1-Sim; 2-Nao)\n";
                    num = pegaRespostaMinMax("",1,2);

                    if (num == 1){
                        num = 10;
                    }

                    insereDeArquivo(nomeDoArquivo,OH,CH,HOH,num);

                    destroiHash(OH,CH,HOH);
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
                    respfinal = pegaRespostaMinMax("",1,2);

                    if (respfinal == 1){
                        respfinal = 10;
                        inicializaDiretorioINS(FILENAME_DUMMY);
                    }

                    inicializaDiretorioINS(nomeDoArquivo);
                    resultadoBMK = inicializaResults();

                    try{
                        instanciaHash(OH,CH,HOH,atributos.tipo,atributos.tamanho,atributos.limite);
                        resultadoBMK = fazerBenchmarkINSERCAO(OH,CH,HOH,num,opcao,nomeDoArquivo);

                        if(respfinal == 10);
                        tempoTotal_buscas = fazerBenchmarkBUSCA(OH,CH,HOH,0,nomeDoArquivo);

                        destroiHash(OH,CH,HOH);

                        cout<<"\nTudo feito!\n\n";
                    }catch(bad_alloc &e){
                        cerr<<BAD_ALLOC_MSG;
                    }
                    cout<<"Tempo insercoes: "<<resultadoBMK.tempoBMK<<"ms\n";

                    if (respfinal == 10)
                    cout<<"Tempo buscas:    "<<tempoTotal_buscas<<"ms\n";

                    cout<<"Colisoes:        "<<resultadoBMK.colisoes<<"\n";
                    cout<<"Rehashings:      "<<resultadoBMK.rehashings<<"\n";
                    system("pause>0");

                    break;

                case 6:
                case 7:
                case 8:
                    cout<<"\nIncluir buscas? (1-Sim; 2-Nao)\n";
                    num = pegaRespostaMinMax("",1,2);

                    if (num == 1){
                        opcao *= 10;    //Faz isso para poder identificar quando o benchmark inclui buscas
                                        //(a variavel referente a opcao escolhida fica divisivel por 10)
                    }

                    tempoTotal_insercoes = 0;  //O somatorio dos tempos de cada insercao do benchmark
                    tempoTotal_buscas = 0;  //O somatorio dos tempos de cada busca do benchmark
                    tempoDecorrido = 0;         //O tempo real do processo inteiro de benchmark
                    rehashingsTotais = 0;
                    MAXcolisoes = 0;
                    qtdInsercoes = 1;
                    //Inicializa o diretorio do arquivo de benchmark de insercoes
                    inicializaDiretorioBMK(montarNomeDoArquivoBMK(atributos.tipo,opcao,true));

                    //E inicializa o diretorio do de buscas, caso isso tenha sido pedido
                    if ((opcao%10) == 0) inicializaDiretorioBMK(montarNomeDoArquivoBMK(atributos.tipo,opcao,false));

                    resultadoBMK = inicializaResults();

                    cout<<"\nTrabalhando...    0%";
                    try{
                        benchmark b;
                        while (true){
                            if ((opcao%10) == 0) inicializaDiretorioINS(FILENAME_DUMMY);

                            if (qtdInsercoes > QTDMAX_INS)
                                qtdInsercoes = QTDMAX_INS;

                            instanciaHash(OH,CH,HOH,atributos.tipo,atributos.tamanho,atributos.limite);

                            resultadoBMK = fazerBenchmarkINSERCAO(OH,CH,HOH,qtdInsercoes, opcao, FILENAME_DUMMY);
                            if ((opcao%10) == 0) tempoTotal_buscas += fazerBenchmarkBUSCA(OH,CH,HOH, opcao, FILENAME_DUMMY)/1000;
                            //destroiHash(OH,CH,HOH);

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
                        tempoDecorrido = b.elapsed();
                        cout<<"\n\nBenchmark completo!\n\n";

                    }catch(bad_alloc& ba){
                        cerr << BAD_ALLOC_MSG;
                        system("pause>0");
                    }

                    /*o "tempo decorrido" indica o tempo real decorrido do inicio ao fim do processo de
                    benchmark, pois inclui o tempo de leitura e escrita dos arquivos, o que eh muito mais
                    evidente em benchmarks incluindo buscas.*/
                    cout<<"Tempo decorrido: "<<tempoDecorrido/1000<<"s\n";
                    cout<<"Tempo insercoes: "<<tempoTotal_insercoes<<"s\n";

                    if ((opcao%10==0))
                    cout<<"Tempo buscas:    "<<tempoTotal_buscas<<"s\n";

                    cout<<"MAX Colisoes:    "<<MAXcolisoes<<"\n";
                    cout<<"Rehashings:      "<<rehashingsTotais<<"\n";
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
        respfinal = pegaRespostaMinMax("",1,2);

        if (respfinal == 2)
            loopPrograma = false;

    }

}
