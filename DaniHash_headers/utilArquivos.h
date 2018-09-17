/*
 * File:   utilArquivos.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef FUNCOES_AUXILIARES_ARQUIVOS_H_INCLUDED
#define FUNCOES_AUXILIARES_ARQUIVOS_H_INCLUDED
#include <dirent.h>

//Forward declaration de algumas funcoes do utilMenu
void fazerInsercao(int valor, bool PI);
int getColisoesDaInsercaoAtual();
double fazerBenchmarkBUSCA(int opcao, string nomeDoArquivoINS);
bool getFezRehashing();

string setNomeDoArquivo(){

    string nome = "";

    fflush(stdin);
    cout<<"\nEntre o nome do arquivo que deseja usar, sem \".ins\" (ou aperte apenas ENTER para cancelar):\n";

    if (cin.peek() == '\n') {     //se o proximo caractere eh um newline
        nome = "sair";
        return nome;
    } else{
        cin>>nome;
        nome += ".ins";
    }

    return nome;

}

void salvarArqInsTemp(int valor){

    ofstream fileWRITE;

    fileWRITE.open(DEFAULT_FULLNAME_INS,ios::app);
    fileWRITE<<"INS "<<valor<<endl;
    fileWRITE.close();
}

bool ehArquivoDeInsercao(char fileName[]){

    for (int i=0;i<256;i++){
        if (fileName[i] == '.'){
            if ((fileName[i+1] == 'i') &&
                (fileName[i+2] == 'n') &&
                (fileName[i+3] == 's')){

                return true;
            }
            break;
        }
    }
    return false;
}

bool imprimeArquivosINS(){

    DIR *dir;
    struct dirent *ent;

    cout<<"\nArquivos de insercao presentes no diretorio:\n\n";

    if ((dir = opendir (FILEPATH_INS.c_str())) != NULL) {
        //Imprime o nome dos arquivos de insercao do diretorio definido nas constantes
        while ((ent = readdir (dir)) != NULL) {
            if (ehArquivoDeInsercao(ent->d_name))
                printf ("%s\n", ent->d_name);
        }
        closedir (dir);
    } else {
        cerr<<"Nao foi possivel abrir o diretorio!";
        system("pause>0");
        return false;
    }
    return true;

}

string montarNomeDoArquivoBMK(int tipo, int opcao_insbmk, bool inserTRUE_buscaFALSE){

    string fileName;

    //Vamos montar a string referente ao nome do arquivo do benchmark, anexando...

    //O tipo do hashing
    if (tipo == 1)
        fileName += "OpenH (Lista)_";
    else if (tipo == 2)
        fileName += "OpenH (AVL)_";
    else if (tipo == 3)
        fileName += "ClosedH (TentativaLinear)_";
    else if (tipo == 4)
        fileName += "ClosedH (TentativaQuadratica)_";
    else if (tipo == 5)
        fileName += "ClosedH (DoubleHashing)_";
    else if (tipo == 6)
        fileName += "HalfOpenH (Com TQ)_";
    else
        fileName += "HalfOpenH (Sem TQ)_";

    //Depois a distribuicao das variaveis aleatorias
    if (querFazerUniforme(opcao_insbmk))
        fileName += "Uniforme_";
    else if (querFazerNormal(opcao_insbmk))
        fileName += "Normal_";
    else if (querFazerExponencial(opcao_insbmk))
        fileName += "Exponencial_";
    else
        fileName += "Ordenado_";

    //Depois a identificacao se eh benchmark de insercao ou de busca
    if (inserTRUE_buscaFALSE == true)
        fileName += "INSERCAO.bmk";
    else
        fileName += "BUSCA.bmk";

    return fileName;
}

//Cria o diretorio para os arquivos de insercao (se nao existir), e
//cria um novo arquivo limpo, sobrescrevendo o anterior de mesmo nome, se existir
void inicializaDiretorioINS(string filename){

    string str = "IF NOT EXIST .\\"+FILEPATH_INS+" mkdir "+FILEPATH_INS;
    system(str.c_str());
    ofstream fileINS;

    fileINS.open(FILEPATH_INS+filename);
    fileINS.close();
}

//Cria o diretorio para os arquivos de benchmark (se nao existir), e
//cria um novo arquivo limpo, sobrescrevendo o anterior de mesmo nome, se existir
void inicializaDiretorioBMK(string filenameMontado){

    string str = "IF NOT EXIST .\\"+FILEPATH_BMK+" mkdir "+FILEPATH_BMK;
    system(str.c_str());
    ofstream fileBMK;

    fileBMK.open(FILEPATH_BMK+filenameMontado);
    fileBMK.close();
}

//
#endif // FUNCOES_AUXILIARES_ARQUIVOS_H_INCLUDED
