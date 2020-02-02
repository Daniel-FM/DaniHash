# DaniHash
Simulador e visualizador de tabelas hash. Feito originalmente como Trabalho de Conclusão de Curso de graduação em Ciência da Computação pela Universidade Estadual do Ceará (UECE).

tst

<b>Versão 1.0.3:</b>
<ul>
<li>Classes e funções organizados em namespaces.</li>
<li>Funções e variáveis agora estão devidamente encapsulados.</li>
<li>Passo-a-passo das operações no DaniHash BASIC está mais detalhado.</li>
<li>utilMenu agora é utilInput. utilGeral se dividiu em utilOutput e utilMath.</li>
<li>Substituição de todos os couts por variações do método printPause, que está muito mais flexível.</li>
</ul>

<b>Versão 1.0.2:</b>
<ul>
<li>TabelaHash agora é uma classe abstrata. Ohash, Chash e HOhash herdam dela.</li>
<li>Função inserirDeArquivo faz parte da classe TabelaHash</li>
<li>Exceção customizda para arquivo defeituoso</li>
<li>Criação das classes abstratas EstruturaAuxiliar e No. Lista e arv_avl herdam dela.</li>
<li>OpenHashing agora só usa um único vetor de objetos EstruturaAuxiliar, ao invés de um vetor de lista e um de arv_avl.</li>
</ul>

<b>Versão 1.0.1:</b>
<ul>
<li>Movimentação das funções do utilMenu para a classe TabelaHash, funcionando como um projeto de classe abstrata</li>
</ul>


<b>Pontos a serem corrigidos futuramente:</b>
<ul>
<li>Deleção da lista com problemas, quebrando o programa.</li>
<li>Deleção da árvore com problemas (às vezes apaga vários nós de uma vez)</li>
<li>Desenho do Hashing Fechado faltando</li>
<li>Posicionamento das árvores no desenho do hashing aberto/meio-aberto com problemas (árvores vizinhas se sobrepondo)</li>
</ul>
