/*
 * File:   OpenHashing.h
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#ifndef OPEN_HASHING_H_INCLUDED
#define OPEN_HASHING_H_INCLUDED

class Ohash: public TabelaHash{

    public:

        int TH, tipo, numPosOcupadas;
        EstruturaAuxiliar* *tabela;

        Ohash(int tamanho, int type){

            TH = tamanho;
            tipo = type;
            numPosOcupadas = 0;
            tabela = new EstruturaAuxiliar*[TH];

            for (int i = 0; i < TH; i++){
                tabela[i] = instanciaEstrutura(tipo);
            }

        }

        ~Ohash(){
            for (int i=0;i<TH;i++)
                delete tabela[i];
        }

        /************************************************* INSERIR *****************************************************************/

        void inserir(int valor, bool PI){

            int chave = valor % TH;

            //Checa se uma posicao anteriormente desocupada vai ficar ocupada agora
            if (tabela[chave]->isNull())
                numPosOcupadas++;

            tabela[chave]->inserir(valor);

            if (PI){
                cout<<"\nO valor "<<valor<<" foi inserido na chave "<<chave<<"\n";
                system("pause>0");
            }

        }

        /************************************************* REMOVER *****************************************************************/

        void remover(int valor){

            int chave = valor % TH;

            if (tabela[chave]->remover(valor) == true){
                //Checa se uma posicao anteriormente ocupada vai ficar desocupada agora
                if (tabela[chave]->isNull())
                    numPosOcupadas--;

                printPause("O valor foi deletado com sucesso da chave "+chave,true);
            }else
                printPause("O valor nao foi encontrado",true);

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

        /************************************************* BUSCAR *****************************************************************/

        int buscar(int valor, bool PI){

            int chave = valor % TH;
            bool encontrou = false;

            encontrou = tabela[chave]->buscar(valor);

            if (encontrou){
                return chave;
            } else{
                return -2;
            }

        }


        /************************************************* PEGAR FATOR DE CARGA *****************************************************************/

        float getFC(){

            return (float)numPosOcupadas /TH;       //Para obter um float atraves da divisao de inteiros, eu tenho que converter um desses
                                                //inteiros para float
        }

        /************************************************* DESENHO *****************************************************************/

        void desenha_hash(RenderWindow* janela, int posAtual){
            int x_indice, x_no, distancia;

            if (tipo == 1){
                x_indice = 6 + posAtual;
                x_no = 13 + posAtual;
            }else{
                x_indice = 66 + posAtual;
                x_no = 79 + posAtual;
            }

            for(int i = 0; i < TH; i++){
                if (tipo == 2) distancia = defineDistInicial(i);

                /******* DESENHA O QUADRADO DO INDICE E O NUMERO DENTRO DELE ***********/
                desenhaLinha(x_indice + 27, 40, x_indice + 27, 100, janela);      //antes, desenha a linha que vai ligar o indice ao no abaixo dele
                desenhaRetangulo(x_indice, 30, 54, 54, Color(255, 127, 39), janela);
                desenhaTexto(i, x_indice + 10, 40, janela, TAM_TEXTO_IND, Color::White);     //e o numero do indice dentro dele

                /********** PRA DEPOIS DESENHAR A LISTA/ARVORE ABAIXO DO QUADRADO DO INDICE ************/
                if (tipo == 1)
                    desenhaLista(dynamic_cast<lista*>(tabela[i]), janela, x_no, 100);
                else
                    desenhaAVL(dynamic_cast<arv_avl*>(tabela[i]), janela, x_no, 100, distancia);
                //Eh necessario fazer um dynamic casting para converter um objeto de classe derivada em um de classe base

                if (tipo == 1){
                    x_indice += 60;
                    x_no += 60;
                }else{

                    if (i < TH-1){

                        //Esses ifs verificam a altura da arvore do indice atual e do indice posterior, para ver qual deve ser a distancia entre eles,
                        //para evitar arvores adjacentes se sobrepondo.
                        if (( tabela[i]->getAltura() <= 1 ) && ( tabela[i+1]->getAltura() <= 1 )){
                            x_indice += 60;
                            x_no += 60;
                        }
                        else if (( tabela[i]->getAltura() <= 2 ) && ( tabela[i+1]->getAltura() <= 2 )){
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
            int posAtual = 0;

            if (tipo == 1)
                w = TH * 61;
            else
                w = TH * 175;

            if (w > VideoMode::getDesktopMode().width)
                w = VideoMode::getDesktopMode().width;

            RenderWindow* janela = new RenderWindow(VideoMode(w,500),"Open Hashing");

            while (janela->isOpen()){
                eventHandler(janela);
                janela->clear(Color::White);
                update_pos(janela, &posAtual);
                desenha_hash(janela, posAtual);
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
            return 0;
        }

        bool getFezRehashing(){
            return false;
        }
};

//
#endif // OPEN_HASHING_H_INCLUDED
