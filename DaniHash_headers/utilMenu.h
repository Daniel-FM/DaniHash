/*
 * File:   utilMenu.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef FUNCOES_AUXILIARES_MENU_H_INCLUDED
#define FUNCOES_AUXILIARES_MENU_H_INCLUDED

void backspace(int posicoes){
    for (int i=0;i<posicoes;i++){
        cout<<"\b \b";
    }
}

int pegaRespostaMinMax(string pedido, int minim, int maxim){

    while (true){
        int resposta=0;
        cout<<pedido;

        if (!(cin>>resposta)) {         //Para tratar entradas que nao sejam int
            cout<<"\nEntrada invalida!";
            system("pause>0");
            backspace(17);
            cin.clear();
            cin.ignore(512,'\n');
            continue;
        }

        if ((resposta < minim) || (resposta > maxim)){
            cout<<"\nOpcao invalida!";
            system("pause>0");
            backspace(15);
        }else{
            return resposta;
        }
    }

}

int pegaRespostaMin(string pedido, int minim){

    int resposta=0;

    while (true){
        cout<<pedido;
        if (!(cin>>resposta)) {         //Para tratar entradas que nao sejam int
            cout << "Entrada invalida!";
            system("pause>0");
            backspace(17);
            cin.clear();                //Retorna ss flags de cin pro estado inicial
            cin.ignore(512,'\n');       //ignora tudo que tem na linha atual
            continue;
        }

        if (resposta < minim){
            cout<<"\nNao pode ser menor que "<<minim;
            system("pause>0");
            backspace(24);
        }else{
            return resposta;
        }
    }

}

int pegaResposta(string pedido){
    int resposta;

    while (true){
        cout<<pedido;
        if (!(cin>>resposta)) {         //Para tratar entradas que nao sejam int
            cout << "Entrada invalida!";
            system("pause>0");
            backspace(17);
            cin.clear();
            cin.ignore(512,'\n');
            continue;
        }

        return resposta;
    }

}

struct Atributos{

    public:

    int tipo, tamanho, limite;
};

Atributos pegaAtributosDaHash(bool trueBASIC_falseBMK){
    int type;
    Atributos atr;

    cout<<"Que tipo de hash?\n\n";
    cout<<"1) Open Hashing com Lista\n";
    cout<<"2) Open Hashing com Arvore\n";
    cout<<"3) Closed Hashing\n";
    cout<<"4) Half-Open Hashing\n\n";

    type = pegaRespostaMinMax("Opcao: ",1,4);

    if (type == 3){

        system("cls");
        cout<<"Escolha como lidar com colisoes:\n\n";
        cout<<"3) Tentativa linear\n";
        cout<<"4) Tentativa quadratica\n";
        cout<<"5) Duplo hashing\n\n";

        type = pegaRespostaMinMax("Opcao: ",3,5);
    }else if (type == 4){

        system("cls");
        cout<<"Deseja que a hash utilize tentativa quadratica?\n\n";
        cout<<"6) Sim, insercoes em arvores cheias contam como colisoes\n";
        cout<<"7) Nao, pode continuar inserindo em arvores cheias\n\n";

        type = pegaRespostaMinMax("Opcao: ",6,7);
    }

    atr.tipo = type;
    atr.limite = 1; //so pra deixar inicializado com algum valor

    if (trueBASIC_falseBMK == true){
        atr.tamanho = pegaRespostaMin("\nTamanho da tabela: ", 1);

        if (type >= 6)
            atr.limite = pegaRespostaMin("Altura maxima das arvores: ", 1);

    }else{
        atr.tamanho = TAM_INICIAL_HASH_BMK;
        atr.limite = ALT_MAX_BMK;
    }

    return atr;

}
//
#endif // FUNCOES_AUXILIARES_MENU_H_INCLUDED
