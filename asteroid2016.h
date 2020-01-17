
#ifndef ASTEROID2016_H_INCLUDED
#define ASTEROID2016_H_INCLUDED

#include <stdio.h>
#include <stdlib.h>
#include <g2.h>      /* funcoes da biblioteca g2 */
#include <g2_X11.h>  /* Dispositivo fisico */
#include <time.h>
#include <unistd.h>

#define vel 0.001  // definir fator de conversao pois a velocidade de atualizacao dos dados é na ordem de 10⁻3 segundos
#define dx_nave 30 //definir tamanho da nave 


int ler_fich(int inf[][2],float posicao[][4],float *largura, float *altura);
void rand_pos(float posicao[][4],int n,int largura,int altura);
void desenha_bolas(int d, int inf[][2], float posicao[][4],int ast_spawned);
int calcula_posicao(int inf[][2],float posicao[][4], int ast_spawned);
int pos_rato(int dev, int d, int *x_rato);
void pos_nave(int *x0_nave, int *x1_nave,int x_rato,int largura,int altura);
void tira_vida (int d,int inf[][2],float posicao[][4] ,int centro_nave,int *score,int ast_spawned);
int guardar_resultados(int d,char nome[10], int score);
void fim_de_jogo(int d,int score);

#endif 
