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

    using namespace sf;

    const int RAIO = 14;
    const int TAM_TEXTO_IND = 20;
    const int TAM_TEXTO_RET = 18;
    const int TAM_TEXTO_CIRC = 13;

    void desenhaRetangulo(int posX, int posY, int width, int height, Color color, RenderWindow* janela){

        RectangleShape ret;
        ret.setSize(Vector2f(width, height));
        ret.setOutlineColor(Color::Black);
        ret.setFillColor(color);
        ret.setOutlineThickness(1);
        ret.setPosition(posX, posY);

        janela->draw(ret);
    }

    void desenhaCirculo(int posX, int posY, int radius, Color color, RenderWindow* janela){

        CircleShape circ;
        circ.setRadius(radius);
        circ.setOutlineColor(Color::Black);
        circ.setOutlineThickness(1);
        circ.setFillColor(color);
        circ.setPosition(posX, posY);

        janela->draw(circ);
    }

    void desenhaLinha(int startX, int startY, int endX, int endY, RenderWindow* janela){
        Vertex linha[] =
        {
            Vertex(Vector2f(startX, startY), Color::Black),
            Vertex(Vector2f(endX, endY), Color::Black)
        };

        janela->draw(linha,2,Lines);
    }

    void desenhaTexto(int valorInt, int x, int y, RenderWindow* janela, int tam, Color color){

        Font fonte;
        fonte.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
        string valorStr;
        valorStr = to_string(valorInt);

        Text texto(valorStr, fonte);

        texto.setFillColor(color);
        texto.setPosition(x,y);
        texto.setCharacterSize(tam);

        janela->draw(texto);
    }

    //Funcao para poder mover o desenho para os lados (caso ele nao caiba na tela)
    void update_pos(RenderWindow* janela, int* pos){

        if (Keyboard::isKeyPressed(Keyboard::Left))
            *pos = *pos - 10;
        if (Keyboard::isKeyPressed(Keyboard::Right))
            *pos = *pos + 10;

        //Pra mover mais rapido...
        if (Keyboard::isKeyPressed(Keyboard::A))
            *pos = *pos - 20;
        if (Keyboard::isKeyPressed(Keyboard::D))
            *pos = *pos + 20;


    }

    void desenhaLista(lista* l, RenderWindow* janela, int x, int y){
        no_lista* noimpr = l->getRaiz();

        while (noimpr != NULL){

            /****** PREPARAR A POSICAO DO NO E DO NUMERO DENTRO DENTRO DELE ******/
            desenhaRetangulo(x, y, 39, 33, Color(0, 162, 232), janela);            //desenha o quadrado
            desenhaTexto(noimpr->info, x+3, y, janela, TAM_TEXTO_RET,Color::Red);     //e o numero dentro dele

            noimpr = noimpr->prox;
            y += 42;
        }

    }

    void desenhaNoAVL(no_avl* noimpr, RenderWindow* janela, int x, int y, int dist){

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
        desenhaCirculo(x, y, RAIO, Color(0, 162, 232), janela);
        desenhaTexto(noimpr->info, x+3, y+3, janela, TAM_TEXTO_CIRC,Color::Red);

    }

    void desenhaAVL(arv_avl* arv, RenderWindow* janela, int x, int y, int dist){
        no_avl* raiz = arv->getRaiz();
        if (raiz != NULL)
            desenhaNoAVL(raiz, janela, x, y, dist);
    }

    void eventHandler(RenderWindow* app){
        Event event;
        while (app->pollEvent(event)){
            if ((event.type == Event::Closed) || (Keyboard::isKeyPressed(Keyboard::Q)))
                app->close();

            if (event.type == Event::Resized){
                // update the view to the new size of the window
                FloatRect visibleArea(0, 0, event.size.width, event.size.height);
                app->setView(View(visibleArea));
            }
        }

    }

};

};

//
#endif // FUNCOES_AUXILIARES_DESENHO_H_INCLUDED
