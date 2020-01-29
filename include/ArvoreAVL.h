/*
 * File:   ArvoreAVL.h
 * Author: Daniel Franklin
 *
 * Created on 2013, 2018
 * Part of the DaniHash project
 */

#ifndef ARVORE_AVL_H_INCLUDED
#define ARVORE_AVL_H_INCLUDED

namespace dh{
class no_avl : public No{

    public:
    int info, altura;

    no_avl* esq;
    no_avl* dir;

    no_avl(int n){
        esq = NULL;
        dir = NULL;
        info = n;
        altura = 0;
    }

    ~no_avl(){
        delete esq;
        delete dir;
    }

};

class arv_avl : public EstruturaAuxiliar{

    no_avl* raiz;

    public:
    arv_avl(){
        raiz = NULL;
    }

    ~arv_avl(){
        delete raiz;
    }

    /********* FUNCOES PEQUENAS AUXILIARES *********/
    int getAltura(){
        return getAltura(raiz);
    }

    int getAltura(no_avl* no){
        if (no == NULL)
            return 0;       //Quando se trabalha com hashing, consideramos a altura de uma arvore vazia como 0
        else
            return no->altura;
    }

    int getFB(no_avl* no){
        return getAltura(no->esq) - getAltura(no->dir); //O fator de balanceamento eh a diferenca das alturas dos filhos esquerdo e direito
    }

    bool isNull(){
        if (raiz == NULL)
            return true;
        else
            return false;
    }

    no_avl* getRaiz(){
        return raiz;
    }

    /************************************************ INSERIR *******************************************************/

    void inserir(int i){
        raiz = Inserir(i, raiz);
    }

    private:
    no_avl* Inserir(int novo_el, no_avl* no){

        if (no == NULL){
            no = new no_avl(novo_el);
        }
        else if (novo_el == no->info){
            return no;                     //esse no ja existe na arvore
        }
        else if (novo_el < no->info)
            no->esq = Inserir(novo_el, no->esq);
        else
            no->dir = Inserir(novo_el, no->dir);

        no = balancear(no);
        return no;
    }


    /*********************************************** BALANCEAR ********************************************************/

    no_avl* balancear(no_avl* no){

        if (getFB(no) == 2){   //se o filho esquerdo for muito maior que o direito

            if (getFB (no->esq) > 0) //Se o filho esquerdo do filho esquerdo do no desbalanceado for maior que o direito
                no = zig(no);  //Entao faz uma rotacao simples direita (zig)
            else                    //Senao
                no = zagZig(no);    //Faz uma rotacao dupla direita (zag-zig)

        }
        else if (getFB(no) == -2){  //se o filho direito for muito maior que o esquerdo
                                    //mesma coisa de antes, so que invertendo tudo
            if (getFB (no->dir) < 0)
                no = zag(no);
            else
                no = zigZag(no);

        }

        no->altura = math::maximo(getAltura(no->esq), getAltura(no->dir)) + 1;    //Depois atualiza a altura do novo no (isso acontece mesmo se
                                                                            //nao tiver sido feito nenhum balanceamento)
        return no;
    }

    /******************************************* ROTA플O DIREITA *****************************************************/
    no_avl* zig(no_avl* k2){
        no_avl* k1 = k2->esq;
        k2->esq = k1->dir;
        k1->dir = k2;
        k2->altura = math::maximo(getAltura(k2->esq), getAltura(k2->dir)) + 1;
        k1->altura = math::maximo(getAltura(k1->esq), k2->altura) + 1;
        return k1;
    }
    /*
              K2            K1
             /  \          /  \
            K1   c   -->  a    K2
           /  \               /  \
          a    b             b    c
    */
    /******************************************* ROTA플O ESQUERDA *****************************************************/
    no_avl* zag(no_avl* k1){
        no_avl* k2 = k1->dir;
        k1->dir = k2->esq;
        k2->esq = k1;
        k1->altura = math::maximo(getAltura(k1->esq), getAltura(k1->dir)) + 1;
        k2->altura = math::maximo(getAltura(k2->dir), k1->altura) + 1;
        return k2;
    }
    /*
              K1                 K2
             /  \               /  \
            a    K2    -->    K1    c
                /  \         /  \
               b    c       a    b
    */
    /**************************************** ROTA플O DUPLA DIREITA **************************************************/
    no_avl* zagZig(no_avl* k3){
        no_avl* k1 = k3->esq;
        no_avl* k2 = k1->dir;

        k3->esq = k2->dir;
        k1->dir = k2->esq;

        k2->esq = k1;
        k2->dir = k3;

        k1->altura = math::maximo(getAltura(k1->esq), getAltura(k1->dir)) + 1;
        k3->altura = math::maximo(getAltura(k3->esq), getAltura(k3->dir)) + 1;
        k2->altura = math::maximo(getAltura(k1), getAltura(k3)) + 1;


        return k2;
    }
    /*
                K3                    K2
               /  \                 /   \
             K1    d              K1     K3
            /  \          -->    /  \   /  \
           a    K2              a    b c    d
               /  \
              b    c
    */
    /**************************************** ROTA플O DUPLA ESQUERDA ************************************************/
    no_avl* zigZag(no_avl* k3){

        no_avl* k1 = k3->dir;
        no_avl* k2 = k1->esq;

        k3->dir = k2->esq;
        k1->esq = k2->dir;

        k2->esq = k3;
        k2->dir = k1;

        k3->altura = math::maximo(getAltura(k3->esq), getAltura(k3->dir)) + 1;
        k1->altura = math::maximo(getAltura(k1->esq), getAltura(k1->dir)) + 1;
        k2->altura = math::maximo(getAltura(k3), getAltura(k1)) + 1;

        return k2;

    }
    /*
             K3
            /  \                    K2
           a    K1                /   \
               /  \     -->     K1     K3
             K2    d           /  \   /  \
            /  \              a    b c    d
           b    c
    */
    /*********************************************  DELETAR (TEM ERROS!) ***********************************************************************/
    public:
    bool remover(int i)
    {
        return Delete_data(i, raiz);
    }

    private:
    bool Delete_data(int data, no_avl *&node)
    {

        if(node == NULL) return false;

        if(data > node->info)
            Delete_data(data, node->dir);
        if(data < node->info)
            Delete_data(data, node->esq);
        if(node->info == data)
        {
            Search_4change(data, node);
        }

        if(node == NULL) return false;

        node = balancear(node);

        return true;
    }

    void Search_4change(int data, no_avl *&node)
    {
        no_avl *slave = node;

        slave = Search_node(slave);

        Change_node(node, slave);

        if(node == NULL) return;

        node = balancear(node);
    }

    no_avl* Search_node(no_avl *&node)
    {
        no_avl *slave = node;
        if(slave->dir !=NULL && slave->dir->esq != NULL)
        {
            slave= slave->dir;
            while(slave->esq->esq != NULL) slave= slave->esq;
        }
        else
        {
            if(slave->esq != NULL && slave->esq->dir != NULL)
            {
                slave= slave->esq;
                while(slave->dir->dir != NULL) slave= slave->dir;
            }
        }

        //cout << "Foi encontrado o: " << slave->info << endl;

        return slave;
    }

    void Change_node(no_avl *&node, no_avl *&slave)
    {
        no_avl *temp, *kamikaze;

        if (node->info == slave->info)
        {
            kamikaze = slave = node;
            slave = node = NULL;
            delete kamikaze;
            return;
        }

        if(node->info <= slave->info)
        {
            temp = slave->esq;
            slave->esq = NULL;
        }
        if(node->info > slave->info)
        {
            temp = slave->dir;
            slave->dir = NULL;
        }

        // Change
        temp->dir = node->dir;
        temp->esq = node->esq;
        node->dir = node->esq = NULL;

        kamikaze = node;

        node = temp;

        delete kamikaze;
    }



    /**************************************  IMPRIMIR NO PROMPT DE COMANDO  *****************************************************************************/
    public:
    void imprimir(){
        imprimir(raiz);
    }

    private:
    void imprimir(no_avl* noimpr){              //Pre-ordem (R-E-D)

        if(noimpr != NULL){
            cout<<noimpr->info<<", ";
            imprimir(noimpr->esq);
            imprimir(noimpr->dir);
        }

    }

    /**************************************  BUSCAR  *****************************************************************************/
    public:
    bool buscar(int i){
        return buscar(i, raiz);
    }
    private:
    bool buscar(int valor, no_avl* no_atual){

        if (no_atual != NULL){
            if (no_atual->info == valor){
                return true;
            }else{
                if (valor < no_atual->info)
                    return buscar(valor, no_atual->esq);
                else
                    return buscar(valor, no_atual->dir);
            }
        }
        return false;

    }

};
};
//

#endif // ARVORE_AVL_H_INCLUDED
