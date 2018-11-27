/*
 * File:   ClosedHashing.h
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#ifndef CLOSED_HASHING_H_INCLUDED
#define CLOSED_HASHING_H_INCLUDED

namespace dh{
class no_ch{

    public:
        int info_no;
        bool deleted;

        no_ch(int i){
            info_no = i;
            deleted = false;
        }

        ~no_ch(){}

};

//Funcao para conferir se ha um certo numero no no (se nao houver ou o no for nulo, retorna 0)
bool confere_num(no_ch* no, int num){

    if (no != NULL){
        if (no->info_no == num)
            return true;
    }
    return false;

}

bool DeletedOrNull(no_ch* no){

    bool result = true;

    if (no != NULL){
        if (no->deleted == false)
            result = false;
    }

    return result;
}

class Chash: public TabelaHash{

    public:

        int TH, tipo, numPosOcupadas, colisoesDaInsercaoAtual, r;
        no_ch* *tabela;
        bool RH_FLAG, fezRehashing;

        Chash(int tamanho, int type){

            TH = tamanho;
            tipo = type;
            RH_FLAG = false;        //Esta flag identifica se um rehashing esta sendo feito no momento
            numPosOcupadas = 0;
            colisoesDaInsercaoAtual = 0;
            fezRehashing = false;
            r = pegaProxPrimMenor(TH);   //valor usado em duplo hashing

            tabela = new no_ch*[TH];

            for (int i = 0; i < TH; i++){
                tabela[i] = NULL;
            }

        }

        ~Chash(){
            for (int i=0;i<TH;i++)
                delete tabela[i];
        }


        /************************************************* INSERIR *****************************************************************/

        //Foi necessario criar essa funcao de inserir "falsa" que chama a funcao de inserir real, pois a falsa eh
        //a funcao abstrata da classe TabelaHash que eh chamada no programa, e que precisa ter os mesmos argumentos
        //em todas as classes que a implementam: no caso, (int chave, bool PI)
        public:
        void inserir(int chave, bool PI){
            inserir(chave, tabela, TH, PI);
        }

        private:
        void inserir(int k, no_ch* *Tabela, int TamTabela, bool PI){

            if (RH_FLAG == false){
                colisoesDaInsercaoAtual = 0;
                fezRehashing = false;
            }

            int H1 = k % TamTabela;
            printNoPauseNoNewline(PI,k," % ",TamTabela," = ",H1,"\t\t");

            int Hfinal = H1;

            if (DeletedOrNull(Tabela[Hfinal])){
                //Se havia um no originalmente "deletado" nesta posicao (nao eh nulo, mas possui a flag "deleted" true),
                //agora sim deletamos ele de verdade, antes de inserir um novo no na posicao onde ele estava
                if (Tabela[Hfinal] != NULL)
                    delete Tabela[Hfinal];

                Tabela[Hfinal] = new no_ch(k);
            }
            else{
                int i = 1;

                //TENTATIVA LINEAR
                if (tipo == 3){
                    while (!DeletedOrNull(Tabela[Hfinal])){

                        if (Tabela[Hfinal]->info_no == k){
                            printPause(PI,"\nJa existe uma chave ",k," no indice ",Hfinal,".\n");
                            return;
                        }

                        if (i == TamTabela){
                            printPause(PI,"A chave ",k," nao pode ser inserida. Numero maximo de tentativas atingido.\n");
                            return;
                        }
                        printNoPause(PI," (Colisao)");

                        Hfinal = (H1 + i) % TamTabela;
                        printNoPauseNoNewline(PI,"(",H1," + ",i,") % ",TamTabela," = ",Hfinal,"\t");

                        colisoesDaInsercaoAtual++;
                        i++;

                    }
                }

                //TENTATIVA QUADRATICA
                else if (tipo == 4){

                    while (!DeletedOrNull(Tabela[Hfinal])){

                        if (Tabela[Hfinal]->info_no == k){
                            printPause(PI,"\nJa existe uma chave ",k," no indice ",Hfinal,".\n");
                            return;
                        }

                        if (i == TamTabela){
                            printPause(PI,"A chave ",k," nao pode ser inserida. Numero maximo de tentativas atingido.\n");
                            return;
                        }
                        printNoPause(PI," (Colisao)");

                        Hfinal = (int)(H1 + pow(i,2)) % TamTabela;
                        printNoPauseNoNewline(PI,"(",H1," + ",i,"^2) % ",TamTabela," = ",Hfinal,"\t");

                        colisoesDaInsercaoAtual++;
                        i++;

                    }

                }

                //DUPLO HASHING
                else{

                    int H2 = r - (k % r);
                    bool printOnce = PI;

                    while (!DeletedOrNull(Tabela[Hfinal])){

                        if (Tabela[Hfinal]->info_no == k){
                            printPause(PI,"\nJa existe uma chave ",k," no indice ",Hfinal,".\n");
                            return;
                        }

                        if (i == TamTabela){
                            printPause(PI,"\nA chave ",k," nao pode ser inserida. Numero maximo de tentativas atingido.\n");
                            return;
                        }
                        printNoPauseNoNewline(PI,"(Colisao)");
                        printNoPauseNoNewline(printOnce," \t\t[H2 = ",r," - (",k," % ",r,") = ",H2,"]");
                        printOnce = false;

                        Hfinal = (H1 + (i*H2)) % TamTabela;
                        printNoPauseNoNewline(PI,"\n(",H1," + ",i,"*",H2,") % ",TamTabela," = ",Hfinal,"\t");

                        colisoesDaInsercaoAtual++;
                        i++;
                    }

                }

                //Despois de realizar um desses metodos, encontramos a posicao da tabela na qual nao
                //havera colisao (ou seja, que eh igual a NULL, ou um no "deletado"). Entao...

                //Caso tenha sido um no "deletado", agora sim deletamos ele de verdade, para ele dar
                //lugar ao novo no
                if (Tabela[Hfinal] != NULL)
                    delete Tabela[Hfinal];

                //Criamos o novo no na posicao
                Tabela[Hfinal] = new no_ch(k);
            }

            numPosOcupadas++;

            //Ai, depois que inseriu, imprime
            printPause(PI,"\nA chave ",k," foi inserida no indice ",Hfinal,".\n");

            //Checa se mais da metade das arvores da Tabela estao cheias. Se sim, faz rehashing
            if ((getFC() > LIMIAR_PARA_REHASHING) && (RH_FLAG == false)){
                printPause(PI,"O fator de carga ultrapassou o limiar definido como ",LIMIAR_PARA_REHASHING,". Sera feito rehashing.\n");

                RH_FLAG = true;
                rehashing(PI);
                fezRehashing = true;
                RH_FLAG = false;
            }

        }

        /*************************************************** BUSCAR *************************************************************/

        public:
        int buscar(int k, bool PI){

            int H1 = k % TH;
            int Hfinal = H1;

            if (tabela[H1] == NULL)
                return -1;
            else{
                int i = 1;

                //BUSCA USANDO TENTATIVA LINEAR
                while (tabela[Hfinal] != NULL){
                    if ((tabela[Hfinal]->deleted == false) && (tabela[Hfinal]->info_no == k)){
                        return Hfinal;
                    }

                    if (i == TH){
                        return -2;
                    }

                    Hfinal = (H1 + i) % TH;
                    i++;

                }

                //Testa se foi o caso de ser nulo mesmo
                if (tabela[Hfinal] == NULL)
                    return -2;
            }

            //Se nao tiver sido, retorna o indice final
            return Hfinal;
        }
        /*************************************************** REHASHING *************************************************************/

        void rehashing(bool PI){

            int TH_novo = pegaProxPrimMaior(TH*2);
            no_ch* *tabela_nova = new no_ch*[TH_novo];      //Cria a nova tabela, com o tamanho novo
            numPosOcupadas = 0;

            for (int i = 0; i < TH_novo; i++){          //Inicializa a nova tabela fazendo todos os seus nos serem nulos

                tabela_nova[i] = NULL;

            }
            no_ch* *tabela_dummy = tabela;          //Cria uma tabela auxiliar, que recebe os elementos da antiga
            int TH_antigo = TH;

            TH = TH_novo;                   //Atualiza o tamanho da hashing
            r = pegaProxPrimMenor(TH);   //e o proximo primo menor

            for (int i = 0; i < TH_antigo; i++){        //Vai percorrendo a tabela auxiliar, inserindo os elementos dela na nova tabela

                if (tabela_dummy[i] != NULL)
                    inserir(tabela_dummy[i]->info_no, tabela_nova, TH_novo, PI);

            }

            tabela = tabela_nova;           //A tabela antiga agora eh a tabela nova

            for (int i=0;i<TH_antigo;i++)
                delete tabela_dummy[i];         //Deleta a tabela auxilia

            printPause(PI,"Rehashing concluido.\n");
        }


        /************************************************* REMOVER *****************************************************************/

        void remover(int chave){
            int posParaRemover;

            //Pegamos o indice onde a chave esta
            posParaRemover = buscar(chave, false);

            //Depois que encontramos a posParaRemover, "deletamos" o no (nao podemos simplesmente setar ele como NULL, pois
            //iria impossibilitar que nos que foram inseridos em posicoes a frente dele escolhidos por tratamento de colisao
            //fossem encontrados futuramente.
            if (posParaRemover < 0){
                tabela[posParaRemover]->deleted = true;
                numPosOcupadas--;

                printPause("A chave foi removida do indice ",posParaRemover);
            }else
                printPause("A chave nao foi encontrada na tabela.");

        }

        /************************************************* PEGAR FATOR DE CARGA *****************************************************************/

        float getFC(){

            return (float)numPosOcupadas/TH;       //Para obter um float atraves da divisao de inteiros, eu tenho que converter um desses
                                                //inteiros para float
        }



        /************************************************* IMPRIMIR *****************************************************************/

        void imprimir(){

            for(int i = 0; i < TH; i++){

                if ((tabela[i] == NULL) || (tabela[i]->deleted == true))
                    cout<< "["<<i<<"]:\n";
                else{
                    cout<< "["<<i<<"]:\t"<<tabela[i]->info_no<<"\n";
                }

            }

            system("pause>0");

        }

        /************************************************* DESENHO *****************************************************************/

        void desenha_hash(){

            printPause("Esta funcionalidade sera implementada em breve!");

        }

        /**************************** OUTRAS ********************************/

        int getColisoesDaInsercaoAtual(){
            return colisoesDaInsercaoAtual;
        }

        bool getFezRehashing(){
            return fezRehashing;
        }

};
};
//
#endif // CLOSED_HASHING_H_INCLUDED
