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

//Foward declaration dos tipos de hashing
class Ohash;
class Chash;
class HOhash;

//Foward declaration de algumas funcoes do utilMenu
void fazerInsercao(Ohash*& OH, Chash*& CH, HOhash*& HOH, int valor, bool PI);
int getColisoesDaInsercaoAtual(Ohash* OH, Chash* CH, HOhash* HOH);
double fazerBenchmarkBUSCA(Ohash* OH, Chash* CH, HOhash* HOH, int opcao, string nomeDoArquivoINS);
bool getFezRehashing(Ohash* OH, Chash* CH, HOhash* HOH);

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
        perror ("Nao foi possivel abrir o diretorio!");
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

    //Depois a indentificacao se eh benchmark de insercao ou de busca
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

void insereDeArquivo(string nomeDoArquivo, Ohash*& OH, Chash*& CH, HOhash*& HOH, int checagem){

    /*
    Checagem:
    3  = insercao no DaniHashBASIC
    2  = insercao no DaniHashBMK sem busca
    10 = insercao no DaniHashBMK com busca
    0  =
    */

    int numeroNaLinha, colisoes = 0, rehashings = 0, numeroDaLinha = 1;
    string linha, substringINS;
    ifstream fileREAD;
    fileREAD.open(FILEPATH_INS+nomeDoArquivo);
    double tempoINSERCOES = 0;

    while(fileREAD){
        getline(fileREAD, linha);
        substringINS = linha.substr(0,4);

        if(substringINS =="INS "){
            try{
                numeroNaLinha = stoi(linha.substr(4,linha.size()-4));
            }catch(invalid_argument &e){
                cerr<<"\nProblema na linha "<<numeroDaLinha<<" do arquivo (invalid_argument).";
                system("pause>0");
                break;
            }

            benchmark b;
            fazerInsercao(OH, CH, HOH, numeroNaLinha, false);
            tempoINSERCOES += b.elapsed();

            colisoes += getColisoesDaInsercaoAtual(OH,CH,HOH);

            if (getFezRehashing(OH,CH,HOH) == true)
                rehashings++;

            numeroDaLinha++;

        }else if (linha == ""){
            break;
        }else{
            cerr<<"\nProblema na linha "<<numeroDaLinha<<" do arquivo (sem identificador \"INS \").";
            system("pause>0");
            checagem = 0;
            break;
        }
    }
    fileREAD.close();

    if (checagem == 0) return;

    if (checagem == 3){
        printPause("\nInsercoes feitas!",true);
    }

    double tempoBUSCAS = 0;

    if (checagem == 10){
        tempoBUSCAS = fazerBenchmarkBUSCA(OH,CH,HOH,0,nomeDoArquivo);
    }

    cout<<"\nTudo feito!\n\n";
    cout<<"Tempo insercoes: "<<tempoINSERCOES<<"ms\n";

    if (checagem == 10)
    cout<<"Tempo buscas:    "<<tempoBUSCAS<<"ms\n";

    cout<<"Colisoes:        "<<colisoes<<"\n";
    cout<<"Rehashings:      "<<rehashings<<"\n";
    system("pause>0");

}
//
#endif // FUNCOES_AUXILIARES_ARQUIVOS_H_INCLUDED
