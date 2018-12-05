/*
 * File:   HalfOpenHashing.h
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#ifndef HALF_OPEN_HASHING_H_INCLUDED
#define HALF_OPEN_HASHING_H_INCLUDED

namespace dh{

class HOhash: public TabelaHash{

    private:
    int colisoesDaInsercaoAtual;
    bool fezRehashing, RH_FLAG;

    public:

    int TH, tipo, altMax, numArvoresCheias;
    arv_avl* *tabela;

    HOhash(int tamanho, int type, int limit){

        TH = tamanho;
        tipo = type;
        altMax = limit;
        colisoesDaInsercaoAtual = 0;
        numArvoresCheias = 0;
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

    bool arvoreCheia(arv_avl* arv){
        return (arv->getAltura() >= altMax);
    }

    //Uma arvora nao-cheia eh chamada de "vaga"
    bool VagaOrNull(arv_avl* arv){

        bool result = true;

        if (arv != NULL){
            if (arvoreCheia(arv))
                result = false;
        }

        return result;
    }

    /************************************************* INSERIR (FUNCAO BASE) *****************************************************************/
    //Foi necessario criar essa funcao de inserir "falsa" que chama a funcao de inserir real, pois a falsa eh
    //a funcao abstrata da classe TabelaHash que eh chamada no programa, e que precisa ter os mesmos argumentos
    //em todas as classes que a impelentam: no caso, (int chave, bool PI)
    void inserir(int chave, bool PI){
        inserir(chave, this->tabela, PI);
    }

    private:
    void inserir(int chave, arv_avl* *tabela_, bool PI){
        if (tipo == 6)
            inserir_CTQ(chave, tabela_, PI);
        else
            inserir_STQ(chave, tabela_, PI);
    }


    /************************************************* INSERIR (COM TENTATIVA QUADRATICA) *****************************************************************/
    void inserir_CTQ(int k, arv_avl* *tabela_, bool PI){

        if (RH_FLAG == false){
            colisoesDaInsercaoAtual = 0;
            fezRehashing = false;
        }
        int H1 = k % TH;
        printNoPauseNoNewline(PI,k," % ",TH," = ",H1,"\t\t");

        int Hfinal = H1;

        if (VagaOrNull(tabela_[Hfinal])){
            tabela_[Hfinal]->inserir(k);
        }else{
            //Se a arvore do hash inicial estiver na altura maxima (uma arvore "cheia"), faz tentativa
            //quadratica ate encontrar uma arvore que nao esteja cheia

            int i = 1;
            while (!VagaOrNull(tabela_[Hfinal])){

                if (i == TH){
                    printPause(PI,"A chave nao pode ser inserida. Numero maximo de tentativas atingido.");
                    return;
                }
                printNoPause(PI," (Colisao)");

                Hfinal = (int)(k + pow(i,2)) % TH;
                printNoPauseNoNewline(PI,"(",H1," + ",i,"^2) % ",TH," = ",Hfinal,"\t");

                colisoesDaInsercaoAtual++;
                i++;

            }
            tabela_[Hfinal]->inserir(k);
        }

        printNoPauseNoNewline(PI,"\nA chave ",k," foi inserida na posicao ",Hfinal,".");

        //Se a altura de uma arvore chegar ao valor maximo definido
        if (arvoreCheia(tabela_[Hfinal])){
            numArvoresCheias++;               //Incrementa o numero de arvores cheias
            printNoPauseNoNewline(PI," A arvore agora esta cheia.");
        }

        printPause(PI,"\n");

        //Checa se mais da metade das arvores da tabela estao cheias. Se sim, faz rehashing
        if ((getFC() > LIMIAR_PARA_REHASHING) && (RH_FLAG == false)){
            printPause(PI,"O fator de carga ultrapassou o limiar definido como ",
                       LIMIAR_PARA_REHASHING,". Vai fazer rehashing.\n");

            RH_FLAG = true;
            rehashing(PI);
            fezRehashing = true;
            RH_FLAG = false;
            printPause(PI,"Rehashing concluido.\n");
        }


    }

    /************************************************* INSERIR (SEM TENTATIVA QUADRATICA)  *****************************************************************/

    void inserir_STQ(int k, arv_avl* *tabela_, bool PI){

        if (RH_FLAG == false){
            fezRehashing = false;
        }
        int H1 = k % TH;
        bool eraCheia = (tabela_[H1]->getAltura() >= altMax);

        tabela_[H1]->inserir(k);
        printPause(PI,"\nA chave ",k," foi inserida na posicao ",H1);

        //Se a altura de uma arvore chegar ao valor maximo definido
        if (arvoreCheia(tabela_[H1])){
            if (!eraCheia){                     //Se ela tiver sido enchida agora
                numArvoresCheias++;               //Incrementa o numero de arvores cheias
                output::printPause(PI,"Esta arvore agora esta cheia.");
            }
        }


        //Checa se mais da metade das arvores da tabela estao cheias. Se sim, faz rehashing
        if  ((getFC() > LIMIAR_PARA_REHASHING) && (RH_FLAG == false)){
            printPause(PI,"O fator de carga ultrapassou o limiar definido como ",LIMIAR_PARA_REHASHING,". Vai fazer rehashing.");

            RH_FLAG = true;
            rehashing(PI);
            fezRehashing = true;
            RH_FLAG = false;
        }


    }

    /************************************************* DELETAR *****************************************************************/
    public:
    void remover(int k){

        int H1 = k % TH;
        bool eraCheia = (tabela[H1]->getAltura() >= altMax);

        tabela[H1]->remover(k);

        //Checa se eh uma arvore que era cheia, e agora nao esta mais cheia
        if (!arvoreCheia(tabela[H1])){
            if (eraCheia){
                numArvoresCheias--;
            }
        }

        printPause("A chave ",k," foi removida da arvore no indice ",H1,".");

    }

    /************************************************* REHASHING *****************************************************************/
    private:
    void rehashing(bool PI){

        int TH_novo = pegaProxPrimMaior(TH*2);
        arv_avl* *tabela_nova = new arv_avl*[TH_novo];  //Cria a nova tabela, com o tamanho novo
        numArvoresCheias = 0;
        for (int i = 0; i < TH_novo; i++){          //Inicializa a nova tabela fazendo com arvores

            tabela_nova[i] = new arv_avl();

        }

        arv_avl* *tabela_dummy = this->tabela;       //Cria uma tabela auxiliar, que recebe os elementos da antiga
        int TH_antigo = TH;

        TH = TH_novo;               //Atualiza o tamanho da hashing
        for (int i = 0; i < TH_antigo; i++){        //Vai percorrendo a tabela auxiliar, inserindo os elementos dela na nova tabela

            if (tabela_dummy[i] != NULL){
                insereAvlNaHash(tabela_dummy[i]->getRaiz(), tabela_nova, PI);
            }

        }
        this->tabela = tabela_nova;          //A tabela antiga agora eh a tabela nova

        for (int i=0;i<TH_antigo;i++)
            delete tabela_dummy[i];         //Deleta a tabela auxiliar

    }

    /************************** FUNCAO PARA IR PERCORRENDO A ARVORE TODA, INSERINDO OS NOS DELA NA HASH *************************/

    void insereAvlNaHash(no_avl* noAtualParaInserir, arv_avl* *tabelaQueVaiReceber, bool PI){

        if(noAtualParaInserir != NULL){

            inserir(noAtualParaInserir->info, tabelaQueVaiReceber, PI);

            insereAvlNaHash(noAtualParaInserir->esq, tabelaQueVaiReceber, PI);
            insereAvlNaHash(noAtualParaInserir->dir, tabelaQueVaiReceber, PI);
        }

    }


    /************************************************* IMPRIMIR *****************************************************************/
    public:
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

        if (tabela[Hfinal]->getRaiz() == NULL){
            return -1;
        }else{

            int i = 1;
            while (tabela[Hfinal]->getRaiz() != NULL){

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

        return (float)numArvoresCheias/TH;       //Para obter um float atraves da divisao de inteiros, eu tenho que converter um desses
                                                //inteiros para float
    }


    /**************************** OUTRAS ********************************/
    private:
    int getColisoesDaInsercaoAtual(){
        return colisoesDaInsercaoAtual;
    }

    bool getFezRehashing(){
        return fezRehashing;
    }

    /************************************************* DESENHO *****************************************************************/
    public:
    void desenha_hash(){

        unsigned int w = TH * 175;
        int posAtual = 0;
        if (w > VideoMode::getDesktopMode().width)
            w = VideoMode::getDesktopMode().width;

        RenderWindow* janela = new RenderWindow(VideoMode(w,500),"Half-Open Hashing");

        while (janela->isOpen()){
            eventHandler(janela);
            janela->clear(Color::White);
            update_pos(janela, &posAtual);
            desenha_hash(janela, posAtual);
            janela->display();
        }

    }

    private:
    void desenha_hash(RenderWindow* janela, int posAtual){
        int x_indice, x_no, distancia;

        x_indice = 66 + posAtual;
        x_no = 80 + posAtual;      //posicao inicial = 13 + 60 + um pouquinho, por causa que do encolhimento do desenho do no da arvore

        for(int i = 0; i < TH; i++){
            distancia = defineDistInicial(i);

            /******* DESENHA O QUADRADO DO INDICE E O NUMERO DENTRO DELE ***********/
            desenhaLinha(x_indice + 27, 40, x_indice + 27, 100, janela);   //antes, desenha a linha que vai ligar o indice ao no abaixo dele
            desenhaRetangulo(x_indice, 30, 54, 54, Color(255, 127, 39), janela);        //desenha o quadrado
            desenhaTexto(i, x_indice + 10, 40, janela, TAM_TEXTO_IND, Color::White);     //e o numero do indice dentro dele

            /********** PRA DEPOIS DESENHAR A ARVORE ABAIXO DO QUADRADO DO INDICE ************/

            desenhaAVL(tabela[i], janela, x_no, 100, distancia);

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

    int defineDistInicial(int i){
        int alt = tabela[i]->getAltura();

        if (alt <= 3)
            return 40;
        else
            return (40 + 40*(alt-3));

    }

};
};

//
#endif // HALF_OPEN_HASHING_H_INCLUDED
