#ifndef CLOSED_HASHING_H_INCLUDED
#define CLOSED_HASHING_H_INCLUDED

class no_ch{

    public:
        int info_no;
        bool deleted;

        no_ch(int i){
            info_no = i;
            deleted = false;
        }

        ~no_ch(){
            //cout<<"no_deletado\n";
        }

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

    if (no == NULL){
        result = true;
    }else{
        if (no->deleted == true)
            result = true;
        else
            result = false;
    }

    return result;
}

class Chash{

    public:

        int TH, FC, tipo, numPosOcupadas, colisoesDaInsercaoAtual, r;
        no_ch* *tabela;
        bool RH_FLAG, fezRehashing;

        Chash(int tamanho, int type){

            TH = tamanho;
            tipo = type;
            RH_FLAG = false;        //Esta flag identifica se um rehashing esta sendo feito no momento
            numPosOcupadas = 0;
            colisoesDaInsercaoAtual = 0;
            fezRehashing = false;
            r = pega_prox_prim_menor(TH);   //valor usado em duplo hashing

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

        void inserir(int k, no_ch* *Tabela, int TamTabela, bool PI){

            if (RH_FLAG == false){
                colisoesDaInsercaoAtual = 0;
                fezRehashing = false;
            }

            int H1 = k % TamTabela;
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
                            printPause("Chave repetida!",PI);
                            return;
                        }

                        if (i == TamTabela){
                            printPause("O numero nao pode ser inserido. Numero maximo de tentativas atingido.",PI);
                            return;
                        }
                        Hfinal = (H1 + i) % TamTabela;

                        colisoesDaInsercaoAtual++;
                        i++;

                    }
                }

                //TENTATIVA QUADRATICA
                else if (tipo == 4){

                    while (!DeletedOrNull(Tabela[Hfinal])){

                        if (Tabela[Hfinal]->info_no == k){
                            printPause("Chave repetida!",PI);
                            return;
                        }

                        if (i == TamTabela){
                            printPause("O numero nao pode ser inserido. Numero maximo de tentativas atingido.",PI);
                            return;
                        }
                        Hfinal = (int)(H1 + pow(i,2)) % TamTabela;

                        colisoesDaInsercaoAtual++;
                        i++;

                    }

                }

                //DUPLO HASHING (FUNCAO 1)
                else if (tipo == 5){

                    int H2 = r - (k % r);;

                    while (!DeletedOrNull(Tabela[Hfinal])){
                        if (Tabela[Hfinal]->info_no == k){
                            printPause("Chave repetida!",PI);
                            return;
                        }

                        if (i == TamTabela){
                            printPause("O numero nao pode ser inserido. Numero maximo de tentativas atingido.",PI);
                            return;
                        }
                        Hfinal = (H1 + (i*H2)) % TamTabela;

                        colisoesDaInsercaoAtual++;
                        i++;
                    }

                }

                //DUPLO HASHING (FUNCAO 2)
                else{

                    int THaux = TamTabela;

                    while (!DeletedOrNull(Tabela[Hfinal])){

                        if (Tabela[Hfinal]->info_no == k){
                            printPause("\nChave repetida!",PI);
                            return;
                        }

                        THaux = pega_prox_prim_menor(THaux);
                        Hfinal = k % THaux;
                        colisoesDaInsercaoAtual++;

                        //Se, enquanto h for 2, houver outra colisao, significa queo elemento nao pode
                        //ser inserido, por isso a funcao retorna sem fazer mais nada.
                        if (THaux <= 2){

                            printPause("O numero nao pode ser inserido. Numero maximo de tentativas atingido.",PI);
                            return;
                        }

                    }

                    //Se saiu desse laco, significa que encontrou uma posParaInserir desocupada
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
            if (PI){
                cout<<"\nO valor "<<k<<" foi inserido na posicao "<<Hfinal<<".\n";
                system("pause>0");
            }

            //Checa se mais da metade das arvores da Tabela estao cheias. Se sim, faz rehashing
            if ((getFC() > LIMIAR_PARA_REHASHING) && (RH_FLAG == false)){
                if (PI)
                    cout<<"O fator de carga ultrapassou o limiar definido como "<<LIMIAR_PARA_REHASHING<<". Vai fazer rehashing.\n";

                RH_FLAG = true;
                rehashing(PI);
                fezRehashing = true;
                RH_FLAG = false;
            }
            if (PI)
                system("pause > 0");

        }

        /*************************************************** BUSCAR *************************************************************/

        int buscar(int k, bool PI){

            int H1 = k % TH;
            int Hfinal = H1;

            if (tabela[H1] == NULL)
                return -1;
            else{
                int i = 1;

                //BUSCA USANDO TENTATIVA LINEAR
                //if (tipo == 3){
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
                //}
                /*
                //TENTATIVA QUADRATICA
                else if (tipo == 4){

                    while (tabela[Hfinal] != NULL){

                        if ((tabela[Hfinal]->deleted == false) && (tabela[Hfinal]->info_no == k)){
                            return Hfinal;
                        }

                        if (i == TH){
                            return -2;
                        }

                        Hfinal = (int)(H1 + pow(i,2)) % TH;
                        i++;

                    }

                }

                //DUPLO HASHING
                else if (tipo == 5){

                    int r  = pega_prox_prim_menor(TH);
                    int H2;

                    while (tabela[Hfinal] != NULL){
                        if ((tabela[Hfinal]->deleted == false) && (tabela[Hfinal]->info_no == k)){
                            return Hfinal;
                        }

                        if (i == TH){
                            return -2;
                        }

                        H2 = r - (k % r);
                        Hfinal = (H1 + (i*H2)) % TH;

                        i++;
                    }

                }*/

                //Testa se foi o caso de ser nulo mesmo
                if (tabela[Hfinal] == NULL)
                    return -2;
            }

            //Se nao tiver sido, retorna a posicao final
            return Hfinal;
        }
        /*************************************************** REHASHING *************************************************************/

        void rehashing(bool PI){

            int TH_novo = pega_prox_prim_maior(TH*2);
            no_ch* *tabela_nova = new no_ch*[TH_novo];      //Cria a nova tabela, com o tamanho novo
            numPosOcupadas = 0;

            for (int i = 0; i < TH_novo; i++){          //Inicializa a nova tabela fazendo todos os seus nos serem nulos

                tabela_nova[i] = NULL;

            }
            no_ch* *tabela_dummy = tabela;          //Cria uma tabela auxiliar, que recebe os elementos da antiga
            int TH_antigo = TH;

            TH = TH_novo;                   //Atualiza o tamanho da hashing
            r = pega_prox_prim_menor(TH);   //e o proximo primo menor

            for (int i = 0; i < TH_antigo; i++){        //Vai percorrendo a tabela auxiliar, inserindo os elementos dela na nova tabela

                if (tabela_dummy[i] != NULL)
                    inserir(tabela_dummy[i]->info_no, tabela_nova, TH_novo, PI);

            }

            tabela = tabela_nova;           //A tabela antiga agora eh a tabela nova

            for (int i=0;i<TH_antigo;i++)
                delete tabela_dummy[i];         //Deleta a tabela auxilia

        }


        /************************************************* REMOVER *****************************************************************/

        void remover(int valor){
            int posParaRemover;

            //Pegamos a posicao onde o valor esta
            posParaRemover = buscar(valor, false);

            //Depois que encontramos a posParaRemover, "deletamos" o no (nao podemos simplesmente setar ele como NULL, pois
            //iria impossibilitar que nos que foram inseridos em posicoes a frente dele escolhidos por tratamento de colisao
            //fossem encontrados futuramente.
            if (posParaRemover < 0){
                tabela[posParaRemover]->deleted = true;
                numPosOcupadas--;

                cout<<"O valor foi removido da posicao "<<posParaRemover<<"\n";
                system("pause>0");
            }else
                printPause("O valor nao foi encontrado na tabela.",true);

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
                    cout<< "Indice ["<<i<<"]:\n";
                else{
                    cout<< "Indice ["<<i<<"]:\t"<<tabela[i]->info_no<<"\n";
                }

            }

            system("pause>0");

        }

        /************************************************* INSERCAO MEDINDO TEMPO *****************************************************************/

        Results benchmarkINSERCAO(int quantidadeDeInsercoes, int opcao_insbmk, string fileName_insercao){
            int valorParaInserir;
            double tempo = 0;
            ofstream fileINS;
            Results resultado = inicializaResults();

            if (benchmarkComArquivoDeInsercao(opcao_insbmk)){
                fileINS.open(FILEPATH_INS+fileName_insercao, ios::app);
            }

            for (int i = 0; i < quantidadeDeInsercoes; i++){
                //Geramos a variavel aleatoria a ser inserida
                valorParaInserir = GVA(opcao_insbmk,i);

                if (valorParaInserir < 0) valorParaInserir = 0;

                if (benchmarkComArquivoDeInsercao(opcao_insbmk))
                    fileINS<<"INS "<<valorParaInserir<<endl;

                benchmark b;
                inserir(valorParaInserir, tabela, TH, false);   //Depois fazemos a insercao, medindo o tempo
                tempo += b.elapsed();

                resultado.colisoes += colisoesDaInsercaoAtual;
                if (fezRehashing == true)
                    resultado.rehashings++;
            }
            resultado.tempoBMK = tempo;

            if (benchmarkComArquivoDeInsercao(opcao_insbmk))
                fileINS.close();

            //Se for um benchmark completo, inclui uma adicao ao arquivo com os tempos do bmk atual
            if (benchmarkComArquivoBenchmark(opcao_insbmk)){
                ofstream fileBMK;

                fileBMK.open(FILEPATH_BMK+montarNomeDoArquivoBMK(tipo,opcao_insbmk,true),ios::app);
                //Poe no arquivo o tempo da insercao
                fileBMK<<quantidadeDeInsercoes<<"\t"<<tempo<<endl;
                fileBMK.close();
            }

            return resultado;
        }


        /************************************************* BUSCA MEDINDO TEMPO *****************************************************************/

        double benchmarkBUSCA(int opcao_insbmk, string fileName_insercao){
            //Praticamente igual ao benchmark de insercao, mas usando a operacao de busca
            int numeroNaLinha, quantidadeDeBuscas = 0;
            double tempo = 0;
            string linha, ins_str;

            ifstream fileREAD;
            fileREAD.open(FILEPATH_INS+fileName_insercao);  //Abre o arquivo de insercao gerado antes, pra saber o que deve ser buscado

            while(fileREAD){
                getline(fileREAD, linha);
                ins_str = linha.substr(0,3);
                if(ins_str=="INS"){
                    quantidadeDeBuscas++;
                    numeroNaLinha = stoi(linha.substr(4,linha.size()-4));

                    benchmark b;
                    buscar(numeroNaLinha,false);
                    tempo += b.elapsed();

                }else if (linha == ""){
                    break;
                }else{
                    cout<<"Arquivo com problema!"<<endl;
                    break;
                }
            }
            fileREAD.close();

            if (benchmarkComArquivoBenchmark(opcao_insbmk)){
                ofstream fileBMK;

                fileBMK.open(FILEPATH_BMK+montarNomeDoArquivoBMK(tipo,opcao_insbmk,false),ios::app);

                fileBMK<<quantidadeDeBuscas<<"\t"<<tempo<<endl;
                fileBMK.close();
            }

            return tempo;

        }

        /************************************************* DESENHO *****************************************************************/

        void preparar_janela(){

            printPause("Comming soon!!!", true);

        }

};

#endif // CLOSED_HASHING_H_INCLUDED
