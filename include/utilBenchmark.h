/*
 * File:   utilBenchmark.h
 * Author: Daniel Franklin
 *
 * Created on 2018
 * Part of the DaniHash project
 */

#ifndef FUNCOES_AUXILIARES_BENCHMARK_H_INCLUDED
#define FUNCOES_AUXILIARES_BENCHMARK_H_INCLUDED
#include <chrono>

namespace dh{

namespace bmk{

    struct Results{

        public:

        unsigned int colisoes;
        int rehashings;
        double tempoBMK;

        Results(){
            inicializaResults();
        }

        void inicializaResults(){
            colisoes = 0;
            rehashings = 0;
            tempoBMK = 0;
        }
    };

    class cronometro {

        private:
        typedef std::chrono::high_resolution_clock clock;
        typedef std::chrono::nanoseconds Nanoseconds;

        clock::time_point start;

        public:
        cronometro(bool startCounting = true) {
            if(startCounting)
                start = clock::now();
        }

        void reset() {
            start = clock::now();
        }

        // in milliseconds
        double tempoDecorrido() {
            Nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(clock::now() - start);
            double elapsed_millisecs = ns.count()/1000000.0;
            return elapsed_millisecs;
        }
    };

    bool querFazerUniforme(int opc){
        if ((opc == 3) ||       //Insercao de Aleatorios - uniforme
            (opc == 6) ||       //Benchmark completo - uniforme
            (opc == 60))        //Benchmark completo (insercao + busca) - uniforme
            return true;
        else
            return false;
    }

    bool querFazerNormal(int opc){
        if ((opc == 4) ||       //Insercao de Aleatorios - normal
            (opc == 7) ||       //Benchmark completo - normal
            (opc == 70))        //Benchmark completo (insercao + busca) - normal
            return true;
        else
            return false;
    }

    bool querFazerExponencial(int opc){
        if ((opc == 5) ||       //Insercao de Aleatorios - exponencial
            (opc == 8) ||       //Benchmark completo - exponencial
            (opc == 80))        //Benchmark completo (insercao + busca) - exponencial
            return true;
        else
            return false;
    }

    bool benchmarkComArquivoDeInsercao(int opc){

        if ((opc == 2) ||       //Insercao de numeros ordenados
            (opc == 3) ||       //Insercao de Aleatorios - uniforme
            (opc == 4) ||       //Insercao de Aleatorios - normal
            (opc == 5) ||       //Insercao de Aleatorios - exponencial
            (opc == 60) ||      //Benchmark completo (insercao + busca) - uniforme
            (opc == 70) ||      //Benchmark completo (insercao + busca) - normal
            (opc == 80))        //Benchmark completo (insercao + busca) - exponencial
            return true;
        else
            return false;

    }

    bool benchmarkComArquivoBenchmark(int opc){

        if ((opc == 6) ||       //Benchmark completo - uniforme
            (opc == 7) ||       //Benchmark completo - normal
            (opc == 8) ||       //Benchmark completo - exponencial
            (opc == 60) ||      //Benchmark completo (insercao + busca) - uniforme
            (opc == 70) ||      //Benchmark completo (insercao + busca) - normal
            (opc == 80))        //Benchmark completo (insercao + busca) - exponencial
            return true;
        else
            return false;

    }

};

};

#include "GVA.h"
//
#endif // FUNCOES_AUXILIARES_BENCHMARK_H_INCLUDED
