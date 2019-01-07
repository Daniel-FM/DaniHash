#ifndef EXCECOES_H_INCLUDED
#define EXCECOES_H_INCLUDED
#include <exception>

namespace dh{
namespace excecao{

    struct entrada_invalida : public exception{

        string errorMsg = "\nEntrada invalida!";

        entrada_invalida(string tipoEsperado){

            if (tipoEsperado != ""){
                errorMsg == errorMsg + " Espera " + tipoEsperado + ".";
            }

            cerr<<errorMsg;
            system("pause>0");
            output::apagaLinha();
            cin.clear();                //Retorna as flags de cin pro estado inicial
            cin.ignore(512,'\n');       //ignora tudo que tem na linha atual

        }

        const char * what () const throw () {
            return errorMsg.c_str();
        }

    };

    struct excecao_arquivo : public exception{

        excecao_arquivo(){
            cout<<"oi";
        }

        const char* what() const throw (){
            cout<<"yo1";
            return "yo2";
        }

    };

    struct arquivo_defeituoso : public excecao_arquivo {

        int l = 0;
        string t = "";

        arquivo_defeituoso(int linha, int tipo){
            l = linha;

            switch (tipo){
                case 1:
                    t = "Caractere invalido.";
                    break;
                case 2:
                    t = "Sem identificador valido.";
                    break;
                default:
                    t = "Erro desconhecido.";
            }
        }

        const char * what () const throw () {
            string str = ("\nProblema na linha " + to_string(l)) + (": " + t);
            return str.c_str();
        }
    };

    struct arquivo_inexistente : public excecao_arquivo {

        string fullPath;

        arquivo_inexistente(string fullPath_){
            fullPath = fullPath_;
        }

        const char * what () const throw () {
            string str = ("\nArquivo Inexistente: " + fullPath);
            return str.c_str();
        }
    };

};
};

#endif // EXCECOES_H_INCLUDED
