#ifndef EXCECOES_H_INCLUDED
#define EXCECOES_H_INCLUDED
#include <exception>

namespace dh{
namespace exc{

    const string EX_MSG_BADALLOC = "\nVazamento de memoria detectado. Reinicie o programa e tente fazer menos insercoes.";

    const int EX_CODE_FILE_FILENOTFOUND = 1;
    const int EX_CODE_FILE_INVALIDCHAR = 2;
    const int EX_CODE_FILE_NOIDENTIFIER = 3;
    const int EX_CODE_FILE_INCOMPLETELINE = 4;

    struct entrada_invalida : public exception{

        string errorMsg = "\nEntrada invalida!";

        entrada_invalida(){
            cin.clear();                //Retorna as flags de cin pro estado inicial
            cin.ignore(512,'\n');       //ignora tudo que tem na linha atual
        }

        string what() {
            return errorMsg;
        }

    };


    struct excecao_arquivo : public exception{

        string errorMsg;
        excecao_arquivo(int codigo, string localizacao){

            string e1, e2;

            switch (codigo){

                case EX_CODE_FILE_FILENOTFOUND:
                    e1 = "Arquivo inexistente: ";
                    e2 = localizacao+"\n";
                    break;

                case EX_CODE_FILE_INVALIDCHAR:
                    e1 = "Arquivo defeituoso: Caractere invalido";
                    e2 = " (Linha "+localizacao+")\n";
                    break;

                case EX_CODE_FILE_NOIDENTIFIER:
                    e1 = "Arquivo defeituoso: Sem identificador valido";
                    e2 = " (Linha "+localizacao+")\n";
                    break;

                case EX_CODE_FILE_INCOMPLETELINE:
                    e1 = "Arquivo defeituoso: Linha incompleta";
                    e2 = " (Linha "+localizacao+")\n";
                    break;

                default:
                    e1 = "Arquivo com problema! ";
                    e2 = "Código: "+ to_string(codigo) + ", Localizacao: "+localizacao+"\n";
            }

            errorMsg = e1 + e2;
        }

        string what(){
            return errorMsg;
        }

    };

    struct unhandled_type : public exception {

        string errorMsg;

        unhandled_type(int type, string location){
            errorMsg = "\nERRO: Tipo nao-tratado "+ to_string(type) +" localizado em "+ location +".\n";
            cerr<<errorMsg;
            system("pause>0");
        }

        string what(){
            return errorMsg;
        }

    };

};
};

#endif // EXCECOES_H_INCLUDED
