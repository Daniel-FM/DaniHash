# DaniHash
Simulador de tabelas hash. Feito como Trabalho de Conclusão de Curso para a UECE

Versão 1.0.2:

-TabelaHash agora é uma classe abstrata. Ohash, Chash e HOhash herdam dela.
-Função inserirDeArquivo faz parte da classe TabelaHash
-Exceção customizda para arquivo defeituoso
-Criação das classes abstratas EstruturaAuxiliar e No. Lista e arv_avl herdam dela.
-OpenHashing agora só usa um único vetor de objetos EstruturaAuxiliar, ao invés de um vetor de lista e um de arv_avl.


Versão 1.0.1:

-Movimentação das funções do utilMenu para a classe TabelaHash, funcionando como um projeto de classe abstrata



Pontos a serem corrigidos futuramente:

-Deleção da árvore com problemas (às vezes apaga vários nós de uma vez)
-Desenho do Hashing Fechado faltando
-Posicionamento das árvores no desenho do hashing aberto/meio-aberto com problemas (árvores vizinhas se sobrepõem a partir de certa altura)
