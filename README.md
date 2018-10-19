# DaniHash
Simulador de tabelas hash. Feito como Trabalho de Conclusão de Curso para a UECE

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
<li>Deleção da árvore com problemas (às vezes apaga vários nós de uma vez)</li>
<li>Desenho do Hashing Fechado faltando</li>
<li>Posicionamento das árvores no desenho do hashing aberto/meio-aberto com problemas (árvores vizinhas se sobrepõem a partir de certa altura)</li>
</ul>
