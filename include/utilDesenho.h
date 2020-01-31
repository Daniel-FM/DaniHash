/*
 * File:   utilDesenho.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef FUNCOES_AUXILIARES_DESENHO_H_INCLUDED
#define FUNCOES_AUXILIARES_DESENHO_H_INCLUDED
#include <SFML/Graphics.hpp>

namespace dh{

namespace desenho{

    const int RAIO = 14;
    const int TAM_TEXTO_IND = 20;
    const int TAM_TEXTO_RET = 18;
    const int TAM_TEXTO_CIRC = 13;

    void desenhaRetangulo(int posX, int posY, int width, int height, sf::Color color, sf::RenderWindow* janela){

        sf::RectangleShape ret;
        ret.setSize(sf::Vector2f(width, height));
        ret.setOutlineColor(sf::Color::Black);
        ret.setFillColor(color);
        ret.setOutlineThickness(1);
        ret.setPosition(posX, posY);

        janela->draw(ret);
    }

    void desenhaCirculo(int posX, int posY, int radius, sf::Color color, sf::RenderWindow* janela){

        sf::CircleShape circ;
        circ.setRadius(radius);
        circ.setOutlineColor(sf::Color::Black);
        circ.setOutlineThickness(1);
        circ.setFillColor(color);
        circ.setPosition(posX, posY);

        janela->draw(circ);
    }

    void desenhaLinha(int startX, int startY, int endX, int endY, sf::RenderWindow* janela){
        sf::Vertex linha[] =
        {
            sf::Vertex(sf::Vector2f(startX, startY), sf::Color::Black),
            sf::Vertex(sf::Vector2f(endX, endY), sf::Color::Black)
        };

        janela->draw(linha,2,sf::Lines);
    }

    void desenhaTexto(int valorInt, int x, int y, sf::RenderWindow* janela, int tam, sf::Color color){

        sf::Font fonte;
        fonte.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
        string valorStr;
        valorStr = to_string(valorInt);

        sf::Text texto(valorStr, fonte);

        texto.setFillColor(color);
        texto.setPosition(x,y);
        texto.setCharacterSize(tam);

        janela->draw(texto);
    }

    //Funcao para poder mover o desenho para os lados (caso ele nao caiba na tela)
    void update_pos(sf::RenderWindow* janela, int* pos){

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            *pos = *pos - 10;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            *pos = *pos + 10;

        //Pra mover mais rapido...
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
            *pos = *pos - 20;
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
            *pos = *pos + 20;


    }

    void desenhaLista(lista* l, sf::RenderWindow* janela, int x, int y){
        no_lista* noimpr = l->getRaiz();

        while (noimpr != NULL){

            /****** PREPARAR A POSICAO DO NO E DO NUMERO DENTRO DENTRO DELE ******/
            desenhaRetangulo(x, y, 39, 33, sf::Color(0, 162, 232), janela);            //desenha o quadrado
            desenhaTexto(noimpr->info, x+3, y, janela, TAM_TEXTO_RET,sf::Color::Red);     //e o numero dentro dele

            noimpr = noimpr->prox;
            y += 42;
        }

    }

    void desenhaNoAVL(no_avl* noimpr, sf::RenderWindow* janela, int x, int y, int dist){

        //Caso o no atual tenha um filho esquerdo
        if (noimpr->esq != NULL){
            //Desenha a linha que ira ligar o no atual ao seu filho
            desenhaLinha(x+13, y+11 , x-dist+13 , y+60+11, janela);

            //e chama a funcao para desenhar o filho esquerdo
            desenhaNoAVL(noimpr->esq, janela, x-dist, y+60, dist/2);
        }

        //Caso o no atual tenha um filho direito
        if (noimpr->dir != NULL){
            //Desenha a linha que ira ligar o no atual ao seu filho
            desenhaLinha(x+13, y+11 , x+dist+13 , y+60+11, janela);

            //e chama a funcao para desenhar o filho direito
            desenhaNoAVL(noimpr->dir, janela, x+dist, y+60, dist/2);
        }

        //Depois que desenhou as linhas, desenha a bola do no atual em cima da ponta superior das linhas
        desenhaCirculo(x, y, RAIO, sf::Color(0, 162, 232), janela);
        desenhaTexto(noimpr->info, x+3, y+3, janela, TAM_TEXTO_CIRC, sf::Color::Red);

    }

    void desenhaAVL(arv_avl* arv, sf::RenderWindow* janela, int x, int y, int dist){
        no_avl* raiz = arv->getRaiz();
        if (raiz != NULL)
            desenhaNoAVL(raiz, janela, x, y, dist);
    }

    void eventHandler(sf::RenderWindow* app){
        sf::Event event;
        while (app->pollEvent(event)){
            if ((event.type == sf::Event::Closed) || (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)))
                app->close();

            if (event.type == sf::Event::Resized){
                // update the view to the new size of the window
                sf::FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                app->setView(sf::View(visibleArea));
            }
        }

    }

    int defineDistInicialOpenHash(dh::Ohash* h, int indiceAtual){
        int alt = h->tabela[indiceAtual]->getAltura();

        if (alt <= 3)
            return 40;
        else
            return (40 + 40*(alt-3));

    }

    /**************************************** DESENHA OPEN HASH *************************************************/

    void desenhaOpenHashPrivate(dh::Ohash* h, sf::RenderWindow* janela, int posAtual){
        int x_indice, x_no, distanciaEntreNosAvl;//, distanciaParaProxIndice;

        if (h->tipo == 1){
            x_indice = 6 + posAtual;
            x_no = 13 + posAtual;
        }else{
            x_indice = 66 + posAtual;
            x_no = 79 + posAtual;
        }

        for(int i = 0; i < h->TH; i++){
            if (h->tipo == 2) distanciaEntreNosAvl = defineDistInicialOpenHash(h, i);

            /******* DESENHA O QUADRADO DO INDICE E O NUMERO DENTRO DELE ***********/
            desenhaLinha(x_indice + 27, 40, x_indice + 27, 100, janela);      //antes, desenha a linha que vai ligar o indice ao no abaixo dele
            desenhaRetangulo(x_indice, 30, 54, 54, sf::Color(255, 127, 39), janela);
            desenhaTexto(i, x_indice + 10, 40, janela, TAM_TEXTO_IND, sf::Color::White);     //e o numero do indice dentro dele

            /********** PRA DEPOIS DESENHAR A LISTA/ARVORE ABAIXO DO QUADRADO DO INDICE ************/
            if (h->tipo == 1)
                desenhaLista(dynamic_cast<lista*>(h->tabela[i]), janela, x_no, 100);
            else
                desenhaAVL(dynamic_cast<arv_avl*>(h->tabela[i]), janela, x_no, 100, distanciaEntreNosAvl);
            //Eh necessario fazer um dynamic casting para converter um objeto de classe base (EstruturaAuxiliar) em um de classe derivada (AVL/Lista)

            if (h->tipo == 1){
                x_indice += 60;
                x_no += 60;
            }else{
                
                if (i < h->TH-1){

                    //Esses ifs verificam a altura da arvore do indice atual e do indice posterior, para ver qual deve ser a distancia entre eles,
                    //para evitar arvores adjacentes se sobrepondo.
                    if (( h->tabela[i]->getAltura() <= 1 ) && ( h->tabela[i+1]->getAltura() <= 1 )){
                        x_indice += 60;
                        x_no += 60;
                    }
                    else if (( h->tabela[i]->getAltura() <= 2 ) && ( h->tabela[i+1]->getAltura() <= 2 )){
                        x_indice += 120;
                        x_no += 120;
                    }
                    else if (( h->tabela[i]->getAltura() <= 3 ) && ( h->tabela[i+1]->getAltura() <= 3 )){
                        x_indice += 180;
                        x_no += 180;
                    }
                    else if (( h->tabela[i]->getAltura() <= 4 ) && ( h->tabela[i+1]->getAltura() <= 4 )){
                        x_indice += 240;
                        x_no += 240;
                    }

                }
            }

        }
    }

    void desenhaOpenHash(dh::Ohash* h){

        unsigned int w;
        int posAtual = 0;

        if (h->tipo == 1)
            w = h->TH * 61;
        else
            w = h->TH * 175;

        if (w > sf::VideoMode::getDesktopMode().width)
            w = sf::VideoMode::getDesktopMode().width;

        sf::RenderWindow* janela = new sf::RenderWindow(sf::VideoMode(w,500),"Open Hashing");

        while (janela->isOpen()){
            eventHandler(janela);
            janela->clear(sf::Color::White);
            update_pos(janela, &posAtual);
            desenhaOpenHashPrivate(h, janela, posAtual);
            janela->display();
        }

    }

/**************************************** DESENHA HALF-OPEN HASH *************************************************/

    int defineDistInicialHalfOpenHash(HOhash* h, int i){
        int alt = h->tabela[i]->getAltura();

        if (alt <= 3)
            return 40;
        else
            return (40 + 40*(alt-3));

    }
    
    void desenhaHalfOpenHashPrivate(HOhash* h, sf::RenderWindow* janela, int posAtual){
        int x_indice, x_no, distancia;

        x_indice = 66 + posAtual;
        x_no = 80 + posAtual;      //posicao inicial = 13 + 60 + um pouquinho, por causa do encolhimento do desenho do no da arvore

        for(int i = 0; i < h->TH; i++){
            distancia = defineDistInicialHalfOpenHash(h, i);

            /******* DESENHA O QUADRADO DO INDICE E O NUMERO DENTRO DELE ***********/
            desenhaLinha(x_indice + 27, 40, x_indice + 27, 100, janela);                    //antes, desenha a linha que vai ligar o indice ao no abaixo dele
            desenhaRetangulo(x_indice, 30, 54, 54, sf::Color(255, 127, 39), janela);        //desenha o quadrado
            desenhaTexto(i, x_indice + 10, 40, janela, TAM_TEXTO_IND, sf::Color::White);    //e o numero do indice dentro dele

            /********** PRA DEPOIS DESENHAR A ARVORE ABAIXO DO QUADRADO DO INDICE ************/

            desenhaAVL(h->tabela[i], janela, x_no, 100, distancia);

            if (i < h->TH-1){

                //Esses ifs verificam a altura da arvore do indice atual e do indice posterior, para ver qual deve ser a distancia entre eles,
                //para evitar arvores adjacentes se sobrepondo.
                if (( h->tabela[i]->getAltura() <= 1 ) && ( h->tabela[i+1]->getAltura() <= 1 )){
                    x_indice += 60; //largura do quadrado grande + 4
                    x_no += 60;
                }
                else if (( h->tabela[i]->getAltura() <= 2 ) && ( h->tabela[i+1]->getAltura() <= 2 )){
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

    void desenhaHalfOpenHash(HOhash* h){

        unsigned int w = h->TH * 175;
        int posAtual = 0;
        if (w > sf::VideoMode::getDesktopMode().width)
            w = sf::VideoMode::getDesktopMode().width;

        sf::RenderWindow* janela = new sf::RenderWindow(sf::VideoMode(w,500),"Half-Open Hashing");

        while (janela->isOpen()){
            eventHandler(janela);
            janela->clear(sf::Color::White);
            update_pos(janela, &posAtual);
            desenhaHalfOpenHashPrivate(h, janela, posAtual);
            janela->display();
        }

    }


};

};

//
#endif // FUNCOES_AUXILIARES_DESENHO_H_INCLUDED
