/*
 * File:   OpenHashing.h
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#ifndef OPEN_HASHING_H_INCLUDED
#define OPEN_HASHING_H_INCLUDED

class Ohash{

    public:

        int TH, FC, tipo;
        lista*   *tabelaL;
        arv_avl* *tabelaA;

        Ohash(int tamanho, int type){

            TH = tamanho;
            tipo = type;

            if (tipo == 1){                     //Se o tipo tiver valor "1", eh criada uma tabela de listas

                tabelaL = new lista* [TH];

                for (int i = 0; i < TH; i++){
                    tabelaL[i] = new lista();
                }

            }
            else{                               //Senao, eh criada uma tabela de arvores

                tabelaA = new arv_avl* [TH];

                for (int i = 0; i < TH; i++){
                    tabelaA[i] = new arv_avl();
                }
            }

        }

        ~Ohash(){
            if (tipo == 1){
                for (int i=0;i<TH;i++)
                    delete tabelaL[i];
            }
            else{
                for (int i=0;i<TH;i++)
                    delete tabelaA[i];
            }
        }

        /************************************************* INSERIR *****************************************************************/

        void inserir(int valor, bool PI){

            int chave = valor % TH;

            if (tipo == 1)
                tabelaL[chave]->inserir(valor);
            else
                tabelaA[chave]->raiz = tabelaA[chave]->inserir(valor, tabelaA[chave]->raiz);

            if (PI){
                cout<<"\nO valor "<<valor<<" foi inserido na chave "<<chave<<"\n";
                system("pause>0");
            }

        }

        /************************************************* REMOVER *****************************************************************/

        void remover(int valor){

            int chave = valor % TH;

            if (tipo == 1){
                if (tabelaL[chave]->remover(valor) == true)
                    printPause("O valor foi deletado com sucesso da chave "+chave,true);
                else
                    printPause("O valor nao foi encontrado",true);
            }else{
                tabelaA[chave]->remover(valor);
            }

        }

        /************************************************* IMPRIMIR *****************************************************************/

        void imprimir(){

            for(int i = 0; i < TH; i++){

                cout<< "Indice ["<<i<<"]:\t";

                if (tipo == 1)
                    tabelaL[i]->imprimir();
                else
                    tabelaA[i]->imprimir(tabelaA[i]->raiz);

                cout<<"\n";

            }

            system("pause>0");

        }

        /************************************************* BUSCAR *****************************************************************/

        int buscar(int valor, bool PI){

            int chave = valor % TH;
            bool encontrou = false;

            if (tipo == 1)
                encontrou = tabelaL[chave]->buscar(valor, tabelaL[chave]->cabeca);
            else
                encontrou = tabelaA[chave]->buscar(valor, tabelaA[chave]->raiz);

            if (encontrou){
                return chave;
            } else{
                return -2;
            }

        }


        /************************************************* PEGAR FATOR DE CARGA *****************************************************************/

        float getFC(){

            int posOcupadas = 0;
            for (int i = 0; i < TH; i++){
                if ((tipo == 1 && tabelaL[i]->cabeca != NULL) ||
                    (tipo == 2 && tabelaA[i]->raiz != NULL)){
                    posOcupadas++;
                }
            }

            return (float)posOcupadas /TH;       //Para obter um float atraves da divisao de inteiros, eu tenho que converter um desses
                                                //inteiros para float
        }


        /************************************************* INSERCAO MEDINDO TEMPO *****************************************************************/

        Results benchmarkINSERCAO(int quantidadeDeInsercoes, int opcao_insbmk, string fileName_insercao){
            int valorParaInserir;
            double tempo = 0;
            ofstream fileINS;
            Results resultado = inicializaResults();

            if (benchmarkComArquivoDeInsercao(opcao_insbmk)){
                fileINS.open(FILEPATH_INS+fileName_insercao, ios::app);
            }

            for (int i = 0; i < quantidadeDeInsercoes; i++){
                //Geramos a variavel aleatoria a ser inserida
                valorParaInserir = GVA(opcao_insbmk,i);

                if (valorParaInserir < 0) valorParaInserir = 0;

                if (benchmarkComArquivoDeInsercao(opcao_insbmk))
                    fileINS<<"INS "<<valorParaInserir<<endl;

                benchmark b;
                inserir(valorParaInserir,false);   //Depois fazemos a insercao, medindo o tempo
                tempo += b.elapsed();
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


        /************************************************* BUSCA MEDINDO TEMPO *****************************************************************/

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

                    benchmark b;
                    buscar(numeroNaLinha,false);
                    tempo += b.elapsed();

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

        /************************************************* DESENHO *****************************************************************/

        void desenha_hash(RenderWindow* janela, int move){
            int x_indice, x_no, distancia;

            if (tipo == 1){
                x_indice = 6 + move;
                x_no = 13 + move;
            }else{
                x_indice = 66 + move;
                x_no = 79 + move;
            }

            for(int i = 0; i < TH; i++){
                if (tipo == 2) distancia = defineDistInicial(i);

                /******* DESENHA O QUADRADO DO INDICE E O NUMERO DENTRO DELE ***********/
                desenha_linha(x_indice + 27, 40, x_indice + 27, 100, janela);      //antes, desenha a linha que vai ligar o indice ao no abaixo dele
                desenha_retangulo(x_indice, 30, 54, 54, Color(255, 127, 39), janela);
                desenha_texto(i, x_indice + 10, 40, janela, TAM_TEXTO_IND, Color::White);     //e o numero do indice dentro dele

                /********** PRA DEPOIS DESENHAR A LISTA/ARVORE ABAIXO DO QUADRADO DO INDICE ************/
                if (tipo == 1)
                    desenha_lista(tabelaL[i], janela, x_no);
                else
                    desenha_arvore(tabelaA[i]->raiz, janela, x_no, 100, distancia);


                if (tipo == 1){
                    x_indice += 60; //largura do quadrado grande + 4
                    x_no += 60;
                }else{

                    if (i < TH-1){

                        //Esses ifs verificam a altura da arvore do indice atual e do indice posterior, para ver qual deve ser a distancia entre eles,
                        //para evitar arvores adjacentes se sobrepondo.
                        if (( tabelaA[i]->getAltura(tabelaA[i]->raiz) <= 1 ) && ( tabelaA[i+1]->getAltura(tabelaA[i+1]->raiz) <= 1 )){
                            x_indice += 60;
                            x_no += 60;
                        }
                        else if (( tabelaA[i]->getAltura(tabelaA[i]->raiz) <= 2 ) && ( tabelaA[i+1]->getAltura(tabelaA[i+1]->raiz) <= 2 )){
                            x_indice += 120;
                            x_no += 120;
                        }
                        else{
                            x_indice += 180;
                            x_no += 180;
                        }
                    }
                }

            }
        }

        void preparar_janela(){

            unsigned int w;
            int move = 0;

            if (tipo == 1)
                w = TH * 61;
            else
                w = TH * 175;

            if (w > VideoMode::getDesktopMode().width)
                w = VideoMode::getDesktopMode().width;

            RenderWindow* janela = new RenderWindow(VideoMode(w,500),"Half-Open Hashing");

            while (janela->isOpen()){
                eventHandler(janela);
                janela->clear(Color::White);
                update_pos(janela, &move);
                desenha_hash(janela, move);
                janela->display();
            }

        }

        int defineDistInicial(int i){
            int alt = tabelaA[i]->getAltura(tabelaA[i]->raiz);

            if (alt <= 3)
                return 40;
            else
                return (40 + 40*(alt-3));

        }

};


#endif // OPEN_HASHING_H_INCLUDED
