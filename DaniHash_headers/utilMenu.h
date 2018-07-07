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

void instanciaHash(Ohash*& OH, Chash*& CH, HOhash*& HOH, int tipo, int tamanho, int limite){

    if (tipo <= 2){
        OH = new Ohash(tamanho, tipo);
    }else if (tipo <= 5){
        CH = new Chash(tamanho, tipo);
    }else{
        HOH = new HOhash(tamanho, tipo, limite);
    }
}

void destroiHash(Ohash*& OH, Chash*& CH, HOhash*& HOH){

    if (OH != NULL){
        delete OH;
    }else if (CH != NULL){
        delete CH;
    }else if (HOH != NULL){
        delete HOH;
    }

}

Results fazerBenchmarkINSERCAO(Ohash* OH, Chash* CH, HOhash* HOH, int qtdInsercoes, int opcao, string nomeDoArquivoINS){

    if (OH != NULL)
        return OH->benchmarkINSERCAO(qtdInsercoes, opcao, nomeDoArquivoINS);
    else if (CH != NULL)
        return CH->benchmarkINSERCAO(qtdInsercoes, opcao, nomeDoArquivoINS);
    else if (HOH != NULL)
        return HOH->benchmarkINSERCAO(qtdInsercoes, opcao, nomeDoArquivoINS);

    return inicializaResults();
}

double fazerBenchmarkBUSCA(Ohash* OH, Chash* CH, HOhash* HOH, int opcao, string nomeDoArquivoINS){

    if (OH != NULL)
        return OH->benchmarkBUSCA(opcao, nomeDoArquivoINS);
    else if (CH != NULL)
        return CH->benchmarkBUSCA(opcao, nomeDoArquivoINS);
    else if (HOH != NULL)
        return HOH->benchmarkBUSCA(opcao, nomeDoArquivoINS);

    return 0;
}

float pegarFC(Ohash*& OH, Chash*& CH, HOhash*& HOH){

    if (OH != NULL)
        return OH->getFC();
    else if (CH != NULL)
        return CH->getFC();
    else if (HOH != NULL)
        return HOH->getFC();

    return 0;
}

void fazerInsercao(Ohash*& OH, Chash*& CH, HOhash*& HOH, int valor, bool PI){

    if (OH != NULL)
        OH->inserir(valor, PI);
    else if (CH != NULL)
        CH->inserir(valor, CH->tabela, CH->TH, PI);
    else if (HOH != NULL){
        if (HOH->tipo == 6)
            HOH->inserir_CTQ(valor, HOH->tabela, HOH->alt_max, PI);
        else
            HOH->inserir(valor, HOH->tabela, HOH->alt_max, PI);
    }
}

void fazerRemocao(Ohash*& OH, Chash*& CH, HOhash*& HOH, int valor){

    if (OH != NULL)
        OH->remover(valor);
    else if (CH != NULL)
        CH->remover(valor);
    else if (HOH != NULL)
        HOH->remover(valor);
}

int fazerBusca(Ohash*& OH, Chash*& CH, HOhash*& HOH, int valor, bool PI){

    if (OH != NULL)
        return OH->buscar(valor,PI);
    else if (CH != NULL)
        return CH->buscar(valor,PI);
    else if (HOH != NULL)
        return HOH->buscar(valor,PI);

    return -3;
}

void fazerImpressao(Ohash*& OH, Chash*& CH, HOhash*& HOH){

    if (OH != NULL)
        OH->imprimir();
    else if (CH != NULL)
        CH->imprimir();
    else if (HOH != NULL)
        HOH->imprimir();
}


void chamarDesenho(Ohash*& OH, Chash*& CH, HOhash*& HOH){

    if (OH != NULL)
        OH->preparar_janela();
    else if (CH != NULL)
        CH->preparar_janela();
    else if (HOH != NULL)
        HOH->preparar_janela();
}

int getColisoesDaInsercaoAtual(Ohash* OH, Chash* CH, HOhash* HOH){

    if (OH != NULL)
        return 0;
    else if (CH != NULL)
        return CH->colisoesDaInsercaoAtual;
    else if (HOH != NULL)
        return HOH->colisoesDaInsercaoAtual;

    return 0;
}

bool getFezRehashing(Ohash* OH, Chash* CH, HOhash* HOH){

    if (OH != NULL)
        return false;
    else if (CH != NULL)
        return CH->fezRehashing;
    else if (HOH != NULL)
        return HOH->fezRehashing;

    return false;
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
