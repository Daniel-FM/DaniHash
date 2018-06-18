#ifndef FUNCOES_AUXILIARES_DESENHO_H_INCLUDED
#define FUNCOES_AUXILIARES_DESENHO_H_INCLUDED
#include <SFML/Graphics.hpp>

using namespace sf;


const int RAIO = 14;
const int TAM_TEXTO_IND = 20;
const int TAM_TEXTO_RET = 18;
const int TAM_TEXTO_CIRC = 13;

void desenha_retangulo(int posX, int posY, int width, int height, Color color, RenderWindow* janela){

    RectangleShape ret;
    ret.setSize(Vector2f(width, height));
    ret.setOutlineColor(Color::Black);
    ret.setFillColor(color);
    ret.setOutlineThickness(1);
    ret.setPosition(posX, posY);

    janela->draw(ret);
}

void desenha_circulo(int posX, int posY, int radius, Color color, RenderWindow* janela){

    CircleShape circ;
    circ.setRadius(radius);
    circ.setOutlineColor(Color::Black);
    circ.setOutlineThickness(1);
    circ.setFillColor(color);
    circ.setPosition(posX, posY);

    janela->draw(circ);
}

void desenha_linha(int startX, int startY, int endX, int endY, RenderWindow* janela){
    Vertex linha[] =
    {
        Vertex(Vector2f(startX, startY), Color::Black),
        Vertex(Vector2f(endX, endY), Color::Black)
    };

    janela->draw(linha,2,Lines);
}

void desenha_texto(int valorInt, int x, int y, RenderWindow* janela, int cSize, Color color){

    static Font fonte;
    fonte.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
    string valorStr;
    valorStr = to_string(valorInt);

    Text texto(valorStr, fonte);

    texto.setFillColor(color);
    texto.setPosition(x,y);
    texto.setCharacterSize(cSize);

    janela->draw(texto);
}

//Funcao para poder mover o desenho para os lados (caso ele nao couber na tela)
void update_pos(RenderWindow* janela, int* move){

    if (Keyboard::isKeyPressed(Keyboard::Left))
        *move = *move - 10;
    if (Keyboard::isKeyPressed(Keyboard::Right))
        *move = *move + 10;

    //Pra mover mais rapido...
    if (Keyboard::isKeyPressed(Keyboard::A))
        *move = *move - 20;
    if (Keyboard::isKeyPressed(Keyboard::D))
        *move = *move + 20;


}

void desenha_lista(lista* l, RenderWindow* janela, int x){
    no* noimpr = l->cabeca;
    int y = 100; //30 + altura do quadrado grande - um pouquinho(2)

    while (noimpr != NULL){

        /****** PREPARAR A POSICAO DO NO E DO NUMERO DENTRO DENTRO DELE ******/
        desenha_retangulo(x, y, 39, 33, Color(0, 162, 232), janela);            //desenha o quadrado
        desenha_texto(noimpr->info, x+3, y, janela, TAM_TEXTO_RET,Color::Red);     //e o numero dentro dele

        noimpr = noimpr->prox;
        y += 42;
    }

}

void desenha_arvore(no_avl* noimpr, RenderWindow* janela, int x, int y, int dist){

    if(noimpr != NULL){

        if (noimpr->esq != NULL){
            //Caso o no atual tenha um filho esquerdo
            //primeiro desenha a linha que ira ligar o no atual ao seu filho
            desenha_linha(x+13, y+11 , x-dist+13 , y+60+11, janela);

            //Depois desenha o no (uma bola) e o valor em cima dela
            desenha_circulo(x, y, RAIO, Color(0, 162, 232), janela);
            desenha_texto(noimpr->info, x+3, y+3, janela, TAM_TEXTO_CIRC, Color::Red);

            //Chama a funcao para desenhar o filho esquerdo
            desenha_arvore(noimpr->esq, janela, x-dist, y+60, dist/2);

        }else{

            //caso nao tenha um filho esquerdo, desenha apenas a bola atual, sem uma linha
            desenha_circulo(x, y, RAIO, Color(0, 162, 232), janela);
            desenha_texto(noimpr->info, x+3, y+3, janela, TAM_TEXTO_CIRC,Color::Red);

        }

        if (noimpr->dir != NULL){
            //Caso o no atual tenha um filho direito
            //primeiro desenha a linha que ira ligar o no atual ao seu filho
            desenha_linha(x+13, y+11 , x+dist+13 , y+60+11, janela);

            //Depois desenha o no (uma bola) e o valor em cima dela
            desenha_circulo(x, y, RAIO, Color(0, 162, 232), janela);
            desenha_texto(noimpr->info, x+3, y+3, janela,TAM_TEXTO_CIRC,Color::Red);

            //Chama a funcao para desenhar o filho direito
            desenha_arvore(noimpr->dir, janela, x+dist, y+60, dist/2);

        }else{
            if (noimpr->esq != NULL){
                //caso nao tenha um filho direito, desenha apenas a bola atual, sem uma linha
                //(tambem checa de novo se o filho esquerdo nao ta nulo tambem, pra nao desenhar o mesmo no
                //duas vezes
                desenha_circulo(x, y, RAIO, Color(0, 162, 232), janela);
                desenha_texto(noimpr->info, x+3, y+3, janela, TAM_TEXTO_CIRC, Color::Red);
            }
        }

    }

}

void eventHandler(RenderWindow* app){
    Event event;
    while (app->pollEvent(event)){
        if ((event.type == Event::Closed) || (Keyboard::isKeyPressed(Keyboard::Q)))
            app->close();

        if (event.type == sf::Event::Resized){
            // update the view to the new size of the window
            FloatRect visibleArea(0, 0, event.size.width, event.size.height);
            app->setView(View(visibleArea));
        }
    }

}




#endif // FUNCOES_AUXILIARES_DESENHO_H_INCLUDED
