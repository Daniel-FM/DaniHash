#ifndef UTIL_MATH_H_INCLUDED
#define UTIL_MATH_H_INCLUDED

namespace dh{
namespace math{

    //Funcao para checar se um numero eh primo. Se so houver um divisor entre 1 e o numero/2
    //(que obviamente eh o numero 1), retorna 1.
    bool checaPrimo(int i){

        for (int dummy = 2; dummy < i/2; dummy++){
            if (i % dummy == 0)
                return false;
        }

        return true;
    }

    int pegaProxPrimMaior(int n){

        do{
            n++;
        }while (checaPrimo(n) == false);

        return n;

    }

    int pegaProxPrimMenor(int n){

        do{
            n--;
        }while ((checaPrimo(n) == false) && (n > 1));

        return n;
    }

    int maximo(int a, int b){
        if (a > b)
            return a;
        else
            return b;
    }

    int minimo(int a, int b){
        if (b > a)
            return a;
        else
            return b;
    }

};
};

#endif
