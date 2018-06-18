#ifndef LISTA_H_INCLUDED
#define LISTA_H_INCLUDED

using namespace std;

class no{

    public:

        int info;
        no* prox;

        no(int valor){
            info = valor;
            prox = NULL;
        }

        ~no(){
            delete prox;
        }

};

class lista{

    public:

        no* cabeca;
        int num_nos;

        lista(){
            cabeca = NULL;
            num_nos = 0;
        }

        ~lista(){
            delete cabeca;
        }

        void inserir(int i){

            no* novono = new no(i);

            novono->prox = cabeca;
            cabeca = novono;
            num_nos++;
        }

        bool remover(int i){

            no* dummy = cabeca;
            no* ant = NULL;

            while (dummy != NULL && dummy->info != i){

                ant = dummy;
                dummy = dummy->prox;

            }

            if (dummy == NULL){       //Se nao achar o numero que quero remover
                return false;             //Retorna sem fazer nada
            }

            if (ant == NULL){           //Se o numero que quero remover estiver na cabeca
                cabeca = cabeca->prox;
            }else{                      //Se estiver no meio ou final
                ant->prox = dummy->prox;
            }

            delete dummy;

            num_nos--;

            return true;


        }

        void imprimir(){

            no* no_de_impressao = cabeca;

            while (no_de_impressao != NULL){
                cout<<no_de_impressao->info<<", ";
                no_de_impressao = no_de_impressao->prox;
            }

            delete no_de_impressao;

        }

        bool buscar(int valor, no* no_atual){
            if (no_atual != NULL){
                if (no_atual->info == valor){
                    return true;
                }else{
                    return buscar(valor, no_atual->prox);
                }
            }
            return false;
        }
};


#endif // LISTA_H_INCLUDED
