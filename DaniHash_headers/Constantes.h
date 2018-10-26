/*
 * File:   Constantes.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef CONSTANTES_H_INCLUDED
#define CONSTANTES_H_INCLUDED

using namespace std;

namespace dh{
namespace constantes{

const string FILENAME_INI = "constantes.ini";
const string nada = "";  //Somar pelo menos uma string permite que usemos o operador + para somar const chars*

//Isto representa o conteudo padrao do arquivo INI, que sera escrito assim se um nao for encontrado
const string INI_FILE = nada+
                    "#A media passada para os Geradores de Valores Aleatorios\n"+
                    "MEDIA_CHAVE_BMK       =500000000\n\n"+
                    "#A quantidade maxima de insercoes feitas em um loop de um benchmark completo\n"+
                    "QTDMAX_INS            =200000\n\n"+
                    "#A razao entre as quantidades de insercoes realizadas em cada loop em um benchmark completo\n"+
                    "GAP_INS               =2000\n\n"+
                    "#O valor do fator de carga a ser atingido para que seja feito rehashing\n"+
                    "LIMIAR_PARA_REHASHING =0.5\n\n"+
                    "#O tamanho inicial das tabelas criadas pelo DaniHash BMK\n"+
                    "TAM_INICIAL_HASH_BMK  =11\n\n"+
                    "#A altura maxima das arvores em tabelas com Half-Open Hashing criadas pelo DaniHash BMK\n"+
                    "ALT_MAX_BMK           =5\n\n\n"+
                    "#O diretorio aonde serao salvos os arquivos de insercao\n"+
                    "FILEPATH_INS          =.\\\\insercao\\\\\n\n"+
                    "#O diretorio aonde serao salvos os arquivos de benchmark\n"+
                    "FILEPATH_BMK          =.\\\\benchmark\\\\";

void criarIniSeNaoExistir(){

    ifstream fileREAD;
    fileREAD.open(FILENAME_INI);

    if (!fileREAD){
        ofstream fileWRITE;

        fileWRITE.open(FILENAME_INI);
        fileWRITE<<INI_FILE;
        fileWRITE.close();
    }

    fileREAD.close();

}

string getValorDaConstanteINI(string constProcurada){

    ifstream fileREAD;
    string linha, retorno;
    fileREAD.open(FILENAME_INI);

    while (fileREAD){
        getline(fileREAD, linha);
        if ((linha == "") || (linha[0] == '#')){
            continue;
        }else{
            size_t found = linha.find(constProcurada);
            if (found != std::string::npos){
                size_t posIgual = linha.find("=")+1;
                retorno = linha.substr(posIgual,linha.size()-posIgual);
                return retorno;
            }
        }
    }

    fileREAD.close();
    return "0";

}

int getMEDIA_CHAVE_BMK(){
    criarIniSeNaoExistir();
    return stoi(getValorDaConstanteINI("MEDIA_CHAVE_BMK"));
}
int getQTDMAX_INS(){
    return stoi(getValorDaConstanteINI("QTDMAX_INS"));
}
int getGAP_INS(){
    return stoi(getValorDaConstanteINI("GAP_INS"));
}
float getLIMIAR_PARA_REHASHING(){
    return stof(getValorDaConstanteINI("LIMIAR_PARA_REHASHING"));
}
int getTAM_INICIAL_HASH_BMK(){
    return stoi(getValorDaConstanteINI("TAM_INICIAL_HASH_BMK"));
}
int getALT_MAX_BMK(){
    return stoi(getValorDaConstanteINI("ALT_MAX_BMK"));
}
string getFILEPATH_INS(){
    return getValorDaConstanteINI("FILEPATH_INS");
}
string getFILEPATH_BMK(){
    return getValorDaConstanteINI("FILEPATH_BMK");
}

const int MEDIA_CHAVE_BMK = getMEDIA_CHAVE_BMK();
const int QTDMAX_INS = getQTDMAX_INS();
const int GAP_INS = getGAP_INS();

const float LIMIAR_PARA_REHASHING = getLIMIAR_PARA_REHASHING();
const int TAM_INICIAL_HASH_BMK = getTAM_INICIAL_HASH_BMK();
const int ALT_MAX_BMK = getALT_MAX_BMK();

const string FILEPATH_INS = getFILEPATH_INS();
const string FILEPATH_BMK = getFILEPATH_BMK();

const string FILENAME_DUMMY = "bmkdummy.ins";
const string DEFAULT_FILENAME_INS = "ultimaInsercao.ins";
const string DEFAULT_FULLNAME_INS = FILEPATH_INS + DEFAULT_FILENAME_INS;

const string BAD_ALLOC_MSG = "\nVazamento de memoria detectado. Reinicie o programa e tente fazer menos insercoes.";

};
};
//
#endif // CONSTANTES_H_INCLUDED
