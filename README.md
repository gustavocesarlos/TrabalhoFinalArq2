# Trabalho Final de Laboratório de Arquitetura e Organização de Computadores 2
Nome: Gustavo Cesar Leite de Oliveira Santos

Este trabalho tem o intuito de explorar conhecimentos adquiridos ao longo das disciplinas prática e teórica. O objetivo do meu trabalho é desenvolver uma versão paralelizada do Algoritmo de Mandelbrot fornecido pelo professor.

O trabalho culminou em muitas pesquisas, principalmente quanto aos conjuntos de instruções(MMX, SSE, AVX e etc...), e às próprias instruções de cada um deles, os tamanhos admitidos. O programa original é consituído de 3 laços de repetição for e o meu objetivo foi paralelizar o seu laço mais interno, utilizando o conjunto de instruções AVX, com registradores de 256 bits.
