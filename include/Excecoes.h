#ifndef EXCECOES_H_INCLUDED
#define EXCECOES_H_INCLUDED
#include <exception>

namespace dh{
namespace excecao{

    const int EX_FILE_FILENOTFOUND = 1;
    const int EX_FILE_INVALIDCHAR = 2;
    const int EX_FILE_NOIDENTIFIER = 3;
    const int EX_FILE_INCOMPLETELINE = 4;

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

                case EX_FILE_FILENOTFOUND:
                    e1 = "Arquivo inexistente: ";
                    e2 = localizacao+"\n";
                    break;

                case EX_FILE_INVALIDCHAR:
                    e1 = "Arquivo defeituoso: Caractere invalido";
                    e2 = " (Linha "+localizacao+")\n";
                    break;

                case EX_FILE_NOIDENTIFIER:
                    e1 = "Arquivo defeituoso: Sem identificador valido";
                    e2 = " (Linha "+localizacao+")\n";
                    break;

                case EX_FILE_INCOMPLETELINE:
                    e1 = "Arquivo defeituoso: Linha incompleta";
                    e2 = " (Linha "+localizacao+")\n";
                    break;

                default:
                    e1 = "Arquivo com problema! ";
                    e2 = "Erro desconhecido.\n";
            }

            errorMsg = e1 + e2;
        }

        string what(){
            return errorMsg;
        }

    };

};
};

#endif // EXCECOES_H_INCLUDED
