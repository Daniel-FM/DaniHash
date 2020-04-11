/*
 * File:   utilArquivos.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef UTIL_ARQUIVOS_H_INCLUDED
#define UTIL_ARQUIVOS_H_INCLUDED
#include <dirent.h>

namespace dh
{

namespace arq{

    bool ehArquivoDeInstrucao(char fileName[]){

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

        if ((dir = opendir (cons::FILEPATH_INS.c_str())) != NULL) {
            //Imprime o nome dos arquivos de insercao do diretorio definido nas constantes
            while ((ent = readdir (dir)) != NULL) {
                if (ehArquivoDeInstrucao(ent->d_name))
                    cout<<ent->d_name<<"\n";
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
            fileName += "HalfOpenH (CTQ)_";
        else if (tipo == 7)
            fileName += "HalfOpenH (STQ)_";
        else
            throw exc::unhandled_type(tipo,"utilArquivos");

        //Depois a distribuicao das variaveis aleatorias
        if (bmk::querFazerUniforme(opcao_insbmk))
            fileName += "Uniforme_";
        else if (bmk::querFazerNormal(opcao_insbmk))
            fileName += "Normal_";
        else if (bmk::querFazerExponencial(opcao_insbmk))
            fileName += "Exponencial_";
        else
            fileName += "ERROR2_";

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

        string str = "IF NOT EXIST "+cons::FILEPATH_INS+
            " mkdir "+cons::FILEPATH_INS;
        system(str.c_str());
        ofstream fileINS;

        fileINS.open(filename);
        fileINS.close();
    }

    //Cria o diretorio para os arquivos de benchmark (se nao existir), e
    //cria um novo arquivo limpo, sobrescrevendo o anterior de mesmo nome, se existir
    void inicializaDiretorioBMK(string filenameMontado){

        string str = "IF NOT EXIST .\\"+cons::FILEPATH_BMK+
            " mkdir "+cons::FILEPATH_BMK;
        system(str.c_str());
        ofstream fileBMK;

        fileBMK.open(cons::FILEPATH_BMK+filenameMontado);
        fileBMK.close();
    }

    void acrescentarNoArquivoDeInstrucoesDefault(string instrucao, int valor){

        ofstream fileWRITE;

        fileWRITE.open(cons::DEFAULT_FULLNAME_INS,ios::app);
        fileWRITE<<instrucao<<" "<<valor<<endl;
        fileWRITE.close();
    }

    int getIndiceInicial(string linha){
        unsigned int indiceInicial=0;
        if (!linha.empty()){
            for (indiceInicial=0;indiceInicial<linha.size()-1;indiceInicial++){
                if (linha.at(indiceInicial) != ' ' &&
                    linha.at(indiceInicial) != '\t'){
                    break;
                }
            }
        }
        return indiceInicial;
    }

};

};
//
#endif // FUNCOES_AUXILIARES_ARQUIVOS_H_INCLUDED
