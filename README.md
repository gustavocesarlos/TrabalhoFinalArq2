# Trabalho Final de Laboratório de Arquitetura e Organização de Computadores 2
Nome: Gustavo Cesar Leite de Oliveira Santos

#Apresentação
Este trabalho tem o intuito de explorar conhecimentos adquiridos ao longo das disciplinas prática e teórica. O objetivo do meu trabalho é desenvolver uma versão paralelizada do Algoritmo de Mandelbrot fornecido pelo professor.

O trabalho culminou em muitas pesquisas, principalmente quanto aos conjuntos de instruções(MMX, SSE, AVX e etc...), e às próprias instruções de cada um deles, os tamanhos admitidos. O programa original é consituído de 3 laços de repetição for e o meu objetivo foi paralelizar o seu laço mais interno, utilizando o conjunto de instruções AVX, com registradores de 256 bits. No entanto um problema apreceu, trabalhei dias inteiros nele e não consegui resolver, e foi difícil conseguir ajuda pois a maioria otimizou os laços mais externos. 

Meu objetivo era fazer com que o meu programa calculasse 4 pixels ao mesmo tempo, no entanto a imagem resultante demora um tempo muito exorbitante para ser gerada o que não consegui explicar mesmo após muita pesquisa. No meu código tentei ao máximo fazer os cálculos em registradores, evitando muitos acessos à memória embora em alguns trechos isso tenha sido inevitável.

Também realizei a compilação do código original no Visual Studio utilizando Otimização disabled e full optimization. A versão sem otimização demora 1:27 segundos para gerar a imagem, enquanto a versão otimizada leva 52, diferença muito considerável que se deve ao nível de paralelização elevado quando escolhemos a Otimização completa. Na pasta gráficos, existe um que ilustra essa situação.

Após esse problema, tentei implementar outra versão paralelizada, dessa vez alterando o for mais externo. Dessa vez com o objetivo de calcular Cy para 4 iterações de uma só vez. No entanto como houve a necessidade de criar vetores de 4 posições para realizar operações com PixelHeight e CyMin, visto que eu e meus colegas não conseguimos encontrar instruções que somem a constante a cada 64 bits sem a necessidade de criação de vetores. O desempenho da versão original sem AVX e com AVX foram analisadas em dois processadores diferentes, um Core i5-4200U e um Core i7-4500U, ambos da linha Haswell, com 1,6GHz e 1,8GHz respectivamente de clock. Devivo a pequena diferença de clocks, pode-se perceber uma pequena diferença apenas nos tempos que o programa leva para gerar a imagem final. Os resultados foram plotados no gráfico abaixo.

![Laser Cutter Panel #1](https://raw.github.com/gustavocesarlos/TrabalhoFinalArq2/master/Gráficos/grafico1.png)

Como pode-se ver, a versão que deveria ser otimizada leva em média 1 segundo a mais para gerar a imagem, o que provavelmente é devido a necessidade de criar vetores. Novamente foi utilizada a arquitetura AVX, utilizando registradores de 256 bits.

Nesse novo algoritmo, denominado simdavx2.c, no entanto, a imagem resultante está perfeitamente de acordo com o esperado, ao contrário do que acontece na versão simdavx.c.
