/*
 * File:   GVA.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef GVA_H_INCLUDED
#define GVA_H_INCLUDED
#include <math.h>
#include <sys/time.h>
//#define M_E 2.718281828459

int getZ1(){

    timeval TV;
    double db_TV;

    gettimeofday(&TV,NULL);
    db_TV = TV.tv_usec;

    //Pegamos o seed inicial com o numero de microssegundos atuais do relogio
    int seed = (int)db_TV;

    return seed;

}

//A variavel global que muda a cada chamada de getRand(),
//fazendo o periodo de numeros pseudo-aleatorios entre 0 e 1.
int z = getZ1();

float getRand(){

    int a = 16807;          //7^5 (primo sugerido para periodo completo)
    int m = 2147483647;     //2^31 (int maximo) - 1
    int q = 127773;         //(m div a)
    int r = 2836;           //(m mod a)

    int hi = z/q;
    int lo = z%q;

    int test = (a * lo) - (r * hi);

    if (test > 0)
        z = test;
    else
        z = test+m;

    return (float)z/m;

}

int uniforme(int media){

    float u = getRand();

    //Numero maximo (media*2) multiplicado por um numero entre 0 e 1,
    //resultando em um numero entre 0 e o maximo.
    return (int)2*media*u;

}

int normal(int media, float desvio){

    while (true){

        float u1 = getRand();
        float u2 = getRand();

        float x  = (float) -log(u1);

        //                    2
        //              -(x-1)
        //             e
        //Se   u2 > _________     , recomeca a funcao
        //             2
        if (   u2 > pow(M_E, -(  pow((x-1),2))/2  )   ){
            continue;
        }

        float u3 = getRand();

        if ( u3 > 0.5){
            return (unsigned int) (media + desvio*x);
        }
        else{
            return (unsigned int) (media - desvio*x);
        }

    }

}

int exponencial(int media){

    float u  = getRand();
    float ln = (float) log(u);

    return (int) -media*ln;
}

#endif // GVA_H_INCLUDED
