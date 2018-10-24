/*
 * File:   HalfOpenHashing.h
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#ifndef HALF_OPEN_HASHING_H_INCLUDED
#define HALF_OPEN_HASHING_H_INCLUDED

bool pode_inserir(arv_avl* a, int valor, int alt_max){

    arv_avl* dummy = a;

    dummy->inserir(valor);

    if (dummy->getAltura() < alt_max){
        delete dummy;
        return true;
    }
    else{
        delete dummy;
        return false;
    }

}

class HOhash: public TabelaHash{

    public:

        int TH, FC, tipo, alt_max, num_arvores_cheias, colisoesDaInsercaoAtual;
        bool fezRehashing, RH_FLAG;
        arv_avl* *tabela;

        HOhash(int tamanho, int type, int limit){

            TH = tamanho;
            tipo = type;
            alt_max = limit;
            colisoesDaInsercaoAtual = 0;
            num_arvores_cheias = 0;
            fezRehashing = false;
            RH_FLAG = false;

            tabela = new arv_avl* [TH];

            for (int i = 0; i < TH; i++){
                tabela[i] = new arv_avl();

            }

        }

        ~HOhash(){
            for (int i=0;i<TH;i++)
                delete tabela[i];
        }

        //Foi necessario criar essa funcao de inserir "falsa" que chama a funcao de inserir real, pois a falsa eh
        //a funcao abstrata da classe TabelaHash que eh chamada no programa, e que precisa ter os mesmos argumentos
        //em todas as classes que a impelentam: no caso, (int valor, bool PI)
        void inserir(int valor, bool PI){
            inserir(valor, tabela, alt_max, PI);
        }

        void inserir(int valor, arv_avl* *tab, int altmax, bool PI){
            if (tipo == 6)
                inserir_CTQ(valor, tab, altmax, PI);
            else
                inserir_STQ(valor, tab, altmax, PI);
        }

        /************************************************* INSERIR (COM TENTATIVA QUADRATICA) *****************************************************************/

        void inserir_CTQ(int k, arv_avl* *tabela, int alt_max, bool PI){

            if (RH_FLAG == false){
                colisoesDaInsercaoAtual = 0;
                fezRehashing = false;
            }
            int H1 = k % TH;
            int Hfinal = H1;

            if ((tabela[Hfinal]->raiz == NULL) || (tabela[Hfinal]->raiz->altura < alt_max)){
                tabela[Hfinal]->inserir(k);
            }else{
                //Se a arvore do hash inicial estiver na altura maxima (uma arvore "cheia"), faz tentativa
                //quadratica ate encontrar uma arvore que nao esteja cheia
                if (PI)
                    cout<<"\nA arvore na posicao "<<Hfinal<<" estava na altura maxima, por isso vou fazer tentativa quadratica.\n";

                int i = 1;
                while ((tabela[Hfinal]->raiz != NULL) && tabela[Hfinal]->raiz->altura == alt_max){

                    if (i == TH){
                        printPause("A chave nao pode ser inserida. Numero maximo de tentativas atingido.",PI);
                        return;
                    }
                    Hfinal = (int)(k + pow(i,2)) % TH;

                    colisoesDaInsercaoAtual++;
                    i++;

                }
                tabela[Hfinal]->inserir(k);
            }

            if (PI)
                cout<<"\nA chave "<<k<<" foi inserida na posicao "<<Hfinal<<"\n";

            //Se a altura de uma arvore chegar ao valor maximo definido, ela eh considerada como "cheia"
            if (tabela[Hfinal]->getAltura() == alt_max){
                if (tabela[Hfinal]->cheia == false){        //Se ela tiver sido enchida agora
                    tabela[Hfinal]->cheia = true;           //Seta a flag que a identifica como cheia
                    num_arvores_cheias++;               //Incrementa o numero de arvores cheias
                    if (PI) cout<<"Arvore atingiu altura maxima.\n";
                }
            }

            //Checa se mais da metade das arvores da tabela estao cheias. Se sim, faz rehashing
            if ((getFC() > LIMIAR_PARA_REHASHING) && (RH_FLAG == false)){
                if (PI)
                    cout<<"O fator de carga ultrapassou o limiar definido como "<<LIMIAR_PARA_REHASHING<<". Vai fazer rehashing.\n";

                RH_FLAG = true;
                rehashing(PI);
                fezRehashing = true;
                RH_FLAG = false;
                if (PI) cout<<"Rehashing concluido.\n";
            }
            if (PI)
                system("pause > 0");


        }

        /************************************************* INSERIR (SEM TENTATIVA QUADRATICA)  *****************************************************************/

        void inserir_STQ(int k, arv_avl* *tabela, int alt_max, bool PI){

            if (RH_FLAG == false){
                fezRehashing = false;
            }
            int H1 = k % TH;

            tabela[H1]->inserir(k);
            if (PI)
                cout<<"\nA chave "<<k<<" foi inserida na posicao "<<H1<<"\n";

            //Se a altura de uma arvore chegar ao valor maximo definido, ela eh considerada como "cheia"
            if (tabela[H1]->getAltura() == alt_max){
                if (tabela[H1]->cheia == false){        //Se ela tiver sido enchida agora
                    tabela[H1]->cheia = true;           //Seta a flag que a identifica como cheia
                    num_arvores_cheias++;               //Incrementa o numero de arvores cheias
                }
            }


            //Checa se mais da metade das arvores da tabela estao cheias. Se sim, faz rehashing
            if  ((getFC() > LIMIAR_PARA_REHASHING) && (RH_FLAG == false)){
                if (PI)
                    cout<<"O fator de carga ultrapassou o limiar definido como "<<LIMIAR_PARA_REHASHING<<". Vai fazer rehashing.\n";

                RH_FLAG = true;
                rehashing(PI);
                fezRehashing = true;
                RH_FLAG = false;
            }
            if (PI)
                system("pause > 0");


        }

        /************************************************* DELETAR *****************************************************************/

        void remover(int k){

            int H1 = k % TH;

            tabela[H1]->remover(k);

            //Checa se eh uma arvore que era cheia, e agora nao esta mais cheia
            if (tabela[H1]->raiz->altura < alt_max){
                if (tabela[H1]->cheia == true){
                    tabela[H1]->cheia = false;
                    num_arvores_cheias--;
                }
            }

            system("pause>0");

        }

        /************************************************* REHASHING *****************************************************************/

        void rehashing(bool PI){

            int TH_novo = pegaProxPrimMaior(TH*2);
            arv_avl* *tabela_nova = new arv_avl*[TH_novo];  //Cria a nova tabela, com o tamanho novo
            num_arvores_cheias = 0;
            for (int i = 0; i < TH_novo; i++){          //Inicializa a nova tabela fazendo com arvores

                tabela_nova[i] = new arv_avl();

            }

            arv_avl* *tabela_dummy = tabela;       //Cria uma tabela auxiliar, que recebe os elementos da antiga
            int TH_antigo = TH;

            TH = TH_novo;               //Atualiza o tamanho da hashing
            for (int i = 0; i < TH_antigo; i++){        //Vai percorrendo a tabela auxiliar, inserindo os elementos dela na nova tabela

                if (tabela_dummy[i] != NULL){
                    insereNoAVL(tabela_dummy[i]->raiz, tabela_nova, TH_novo, PI);
                }

            }
            tabela = tabela_nova;          //A tabela antiga agora eh a tabela nova

            for (int i=0;i<TH_antigo;i++)
                delete tabela_dummy[i];         //Deleta a tabela auxiliar

        }

        /************************** FUNCAO PARA IR PERCORRENDO A ARVORE TODA, INSERINDO OS NOS DELA NA HASH *************************/

        void insereNoAVL(no_avl* noQueVaiSerInserido, arv_avl* *tabela_que_vai_receber, int limit, bool PI){

            if(noQueVaiSerInserido != NULL){

                inserir(noQueVaiSerInserido->info, tabela_que_vai_receber, limit, PI);

                insereNoAVL(noQueVaiSerInserido->esq, tabela_que_vai_receber, limit, PI);
                insereNoAVL(noQueVaiSerInserido->dir, tabela_que_vai_receber, limit, PI);
            }

        }


        /************************************************* IMPRIMIR *****************************************************************/

        void imprimir(){

            for(int i = 0; i < TH; i++){

                cout<< "["<<i<<"]:\t";

                tabela[i]->imprimir();

                cout<<"\n";

            }

            system("pause>0");

        }

        /************************************************* BUSCAR (SEM TENTATIVA QUADRATICA) *****************************************************************/

        int buscar(int k, bool PI){

            int chave = k % TH;
            bool encontrou = false;

            encontrou = tabela[chave]->buscar(k);

            if (encontrou){
                return chave;
            } else{
                return -2;
            }

        }

        /************************************************* BUSCAR (COM TENTATIVA QUADRATICA) *****************************************************************/

        int buscar_CTQ(int k, bool PI){

            int H1 = k % TH;
            int Hfinal = H1;

            if (tabela[Hfinal]->raiz == NULL){
                return -1;
            }else{

                int i = 1;
                while (tabela[Hfinal]->raiz != NULL){

                    if (tabela[Hfinal]->buscar(k) == true)
                        return Hfinal;

                    if (i == TH){
                        return -2;
                    }

                    Hfinal = (int)(k + pow(i,2)) % TH;
                    i++;

                }   //Se saiu desse laco, significa que encontrou a posicao do no (ou chegou numa posicao nula)

                //Testa se foi o caso de ser nulo mesmo
                if (tabela[Hfinal] == NULL)
                    return -2;

            }

            //Se nao tiver sido, retorna a posicao final
            return Hfinal;

        }


        /************************************************* PEGAR FATOR DE CARGA *****************************************************************/

        float getFC(){

            return (float)num_arvores_cheias/TH;       //Para obter um float atraves da divisao de inteiros, eu tenho que converter um desses
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
                valorParaInserir = GVA(opcao_insbmk);

                if (valorParaInserir < 0) valorParaInserir = 0;

                if (benchmarkComArquivoDeInsercao(opcao_insbmk))
                    fileINS<<"INS "<<valorParaInserir<<endl;

                benchmark b;
                //Depois fazemos a insercao, medindo o tempo
                inserir(valorParaInserir,tabela,alt_max,false);
                tempo += b.elapsed();

                resultado.colisoes += colisoesDaInsercaoAtual;
                if (fezRehashing == true)
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

            x_indice = 66 + move;
            x_no = 80 + move;      //posicao inicial = 13 + 60 + um pouquinho, por causa que do encolhimento do desenho do no da arvore

            for(int i = 0; i < TH; i++){
                distancia = defineDistInicial(i);

                /******* DESENHA O QUADRADO DO INDICE E O NUMERO DENTRO DELE ***********/
                desenha_linha(x_indice + 27, 40, x_indice + 27, 100, janela);   //antes, desenha a linha que vai ligar o indice ao no abaixo dele
                desenha_retangulo(x_indice, 30, 54, 54, Color(255, 127, 39), janela);        //desenha o quadrado
                desenha_texto(i, x_indice + 10, 40, janela, TAM_TEXTO_IND, Color::White);     //e o numero do indice dentro dele

                /********** PRA DEPOIS DESENHAR A ARVORE ABAIXO DO QUADRADO DO INDICE ************/

                desenha_arvore(tabela[i], janela, x_no, 100, distancia);

                if (i < TH-1){

                    //Esses ifs verificam a altura da arvore do indice atual e do indice posterior, para ver qual deve ser a distancia entre eles,
                    //para evitar arvores adjacentes se sobrepondo.
                    if (( tabela[i]->getAltura() <= 1 ) && ( tabela[i+1]->getAltura() <= 1 )){
                        x_indice += 60; //largura do quadrado grande + 4
                        x_no += 60;
                    }
                    else if (( tabela[i]->getAltura() <= 2 ) && ( tabela[i+1]->getAltura() <= 2 )){
                        x_indice += 120; //largura do quadrado grande + 4 (*2)
                        x_no += 120;
                    }
                    else{
                        x_indice += 180; //largura do quadrado grande + 4 (*3)
                        x_no += 180;
                    }
                }

            }
        }

        void preparar_janela(){

            unsigned int w = TH * 175;
            int move = 0;
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
            int alt = tabela[i]->getAltura();

            if (alt <= 3)
                return 40;
            else
                return (40 + 40*(alt-3));

        }

        /**************************** OUTRAS ********************************/

        int getColisoesDaInsercaoAtual(){
            return colisoesDaInsercaoAtual;
        }

        bool getFezRehashing(){
            return fezRehashing;
        }

};


//
#endif // HALF_OPEN_HASHING_H_INCLUDED
