

#include "asteroid2016.h"

int main(void)
{
	int i=1, ast_spawned=0;    /* Variaveis que controlam o tempo e o nr de asteroides que vivem/viveram */
	int d, dev;		/* Informacao dos dispositivos físico e virtual */
	int flag=0, n=100, score=0; // variáveis que controlam o fim de jogo, numero de asteroides total e o score
  	int botao=0, controlo_botao=0, contador_botao=0;  //variaveis utilizadas para controlar o uso do botão
  	int inf[100][2]; /* tempo de spawn = inf [n][0] vida =inf[n][1] */
  	float posicao[100][4],largura,altura; /* posicao[][0]=Velocidade;  posicao[][1]=raio; posicao[][2]=po_x;  posicao[][3]=po_y;*/
  	int x_rato,x0_nave,x1_nave, centro_nave; /* x0_nave= posicao mais a esquerda da nave, x1_nave=posicao mais a direita da nave */
  

   ler_fich(inf,posicao,&largura,&altura); /*Ler as informacões de ficheiro para as matrizes inf[100][3] e posicao[100][4]
   	 e adquirir os valores de altura e largura*/
   	printf("\nG2_VERSION %s\n", G2_VERSION);
 
   	printf("Open ..X11 device");
   	dev=g2_open_X11(largura, altura);    

   	d=g2_open_vd();    /* open virtual device */
   	g2_clear(d);  
   
   	g2_attach(d, dev); /* attach the virtual device with the physical device */
    
   	g2_set_auto_flush(d,0); /* Used to automatic refreshing */ 

  	printf("\n");
  	
    rand_pos(posicao, n,largura,altura); // randomizar a posicao em x dos asteroides
  

  	while (flag==0) 
  	{
      if ((i%(inf[ast_spawned][0])==0) && (ast_spawned<n)) // enquanto nao passar o tempo de spawn do proximo asteroide, nao se aumenta a permissao para desenha-lo
      {
        ast_spawned++; //incrementar o numero de asteroides vivos
        i=0; //reset do contador de "tempo" a 0
      }  
      flag=calcula_posicao(inf,posicao,ast_spawned); //se a flag vier a 1 o jogo acaba.
      desenha_bolas(d,inf,posicao,ast_spawned); //desenhar as bolas cuja posicao foi calculada e o resultado corrente

      botao=pos_rato(dev,d, &x_rato); //adquirir a posicao do rato e o estado do botao do rato 
      pos_nave(&x0_nave,&x1_nave,x_rato,largura,altura); //calcular a posicao da nave
      g2_pen(d, 22);
      g2_filled_rectangle(d,x0_nave,5,x1_nave,15); //desenhar a nave
      
  		
	  if (botao) //botao ativo
	  {
	  	centro_nave=((x0_nave+x1_nave)/2); //obter a posicao do disparo
	  	if ((botao!=controlo_botao)) // apenas quando existe mudanca de estado de 1 para 0 da variavel botao é que se pode retirar vida
	  	{
	  		tira_vida(d,inf,posicao,centro_nave,&score,ast_spawned);// retirar vida
	  		contador_botao=0; //inicializar um contador de modo ao laser nao estar sempre ativo
	  	}
	  	contador_botao++; 
	  	if((contador_botao>0)&&(contador_botao<=50))	//o tiro dura no intervalo [0;50] milisegundos
	  		g2_line(d,centro_nave,0,centro_nave,altura); //desenhar o tiro
	  }

      g2_flush(d); // Escrever no dispositivo
      usleep(1000);//parar por 1 milisegundo
      g2_clear(dev); //Limpar tudo o que foi escrito na interface gráfica
 
  	  controlo_botao=botao; // Atualizar a variavel que controla o botao

  	  i++; // contar 1 milisegundo
  	  if (score==n) //condição de Vitória
  	  	flag=1;
  		
  		
  	}

  	fim_de_jogo(d,score);
    printf("\n FIM.\n[Enter]\n");
    getchar();
    g2_close(d);


  	return 0;
}


int ler_fich(int inf[][2],float posicao[][4],float *largura, float *altura)
{
 char string[2];
 int n_conversao,n_asteroide=0, tempo_spawn,vida,mudar_nivel; 
 float velocidade,raio_min,raio_max ,nivel; 
 FILE *fid = fopen("config.txt", "r"); // Declarar um apontador para o ficheiro 
 
 if (fid==NULL) //mensagem de erro
 {
  printf("Erro na Abertura do Ficheiro de configuração");
  return 0;
 }

    do
    {
    	fgets(string,2,fid);
			if( string[0] != '/'&& string[1]!='/')
    		{
      			n_conversao=fscanf(fid,"%f %f %d %f %f %d %d", &nivel,&velocidade,&tempo_spawn,
      			&raio_min,&raio_max,&vida,&mudar_nivel); //obter valores para as variaveis a partir da leitura das linhas
       			if (n_conversao==7)
       			{
       				for(n_asteroide=n_asteroide;n_asteroide<mudar_nivel;n_asteroide++)
					{	
						posicao[n_asteroide][0]=((float)rand()/(float)(RAND_MAX))*(10*velocidade-10) +10;//numero aleatorio (float) entre 1 e o valor lido
       					posicao[n_asteroide][1]=raio_min + ((float)rand()/(float)(RAND_MAX))*(raio_max-raio_min);/* numero aleatorio do tipo float
       					entre o raio minimo e o raio maximo lidos*/
       					inf[n_asteroide][0]=(rand()%(tempo_spawn*1000)); /*numero aleatório (inteiro) entre o valor lido e 1
       					fator de conversão de segundos---> milisengundos = 1000 ; */
       					if (vida!=1)
       						inf[n_asteroide][1]=rand()%(vida)+1; //Numero Aleatório (inteiro) entre o valor lido e 1
       					if (vida==1) //caso o valor lido par vida seja 1 (minimo de vida) associar esse valor;
       						inf[n_asteroide][1]=vida;
       				}
       			}
       			else
     			{
     				if(n_conversao==2)
     				{	
     					*largura=nivel; // aproveitar as variaveis nivel e velocidade para lerem a largura e altura da consola
     					*altura=velocidade;
     	
     				}	
     			}
      		}		
      }
      while (feof(fid)==0); //enquanto nao chega ao final do ficheiro
 fclose(fid); //fechar ficheiro
 return 0;
}


void rand_pos(float posicao[][4],int n,int largura,int altura)
{
  int i=0;
  /* po_y= posicao[i][3] po_x= posicao[i][2]  raio = posicao[i][1]*/
  
  while (i< n)
  {  
	  posicao[i][2] = ((float)rand()/(float)(RAND_MAX))*(largura-posicao[i][1]); //X inicial

	  if (posicao[i][2] < posicao[i][1]) // se for menor que o raio associar o raio a po_x
		  posicao[i][2]= posicao[i][1]; 

    posicao[i][3]= altura- posicao[i][1]; 
    i++;
  }

   

}   
void desenha_bolas(int d,int inf[][2],float posicao[][4],int ast_spawned)
{
  int i=0;
  char vida[2] ;
  while (i<ast_spawned)
  {
    if ((inf[i][1]>0))
    {
      g2_pen(d,7+inf[i][1]); //mudar de cor de acordo com a vida
      g2_filled_circle(d,posicao[i][2] , posicao[i][3],posicao[i][1]); //desenhar circulo
      g2_pen(d,1); //mudar de cor para preto
      g2_circle(d, posicao[i][2],posicao[i][3],posicao[i][1]); //desenhar circunferencia
      sprintf(vida,"%d",inf[i][1]);
      g2_string(d,posicao[i][2],posicao[i][3],vida);// escrever a vida
    }
    i++;
  }
}


int calcula_posicao(int inf[][2],float posicao[][4],int ast_spawned)
{
  int j=0;
  while(j<ast_spawned)
  {
    if (inf[j][1]>0)
    {  
      posicao[j][3]=posicao[j][3]-(posicao[j][0]*vel); // retirar valor em y ao asteroide tendo em conta a taxa de iteracao
    }  
    if (posicao[j][3]<=posicao[j][1]){ // se o centro do asteroide + o seu raio chegarem à posicao 0 acaba o jogo
      return 1;
    }
    j++;
  }
  return 0;

}





int pos_rato(int dev, int d, int *x_rato)
{
  double x_r, y_r;  
  unsigned int click ; //variaveis onde serão guardadas as informações do rato
  g2_query_pointer (dev, &x_r, &y_r, &click); //obter a informacao de estado do rato

  *x_rato = (int) x_r; //obter o x_rato sob forma de inteiro

  return click;
}

void pos_nave(int *x0_nave,int *x1_nave ,int x_rato,int largura,int altura)
{
  if (x_rato <= largura - (dx_nave/2) && (x_rato >= (dx_nave)/2)) //condicoes para calcular a posicao da nave quando o rato está nos limites
  {
    *x0_nave= x_rato - (dx_nave/2);
    *x1_nave= x_rato + (dx_nave/2);
  }
  
  if(x_rato >= (largura - (dx_nave/2)) ) //condicoes para calcular a posicao da nave quando o rato excede a direita
  {
    *x0_nave= largura - dx_nave;
    *x1_nave= largura;
  }
 
  if (x_rato <= (dx_nave/2)) // condicoes para calcular a posicao da nave quando o rato excede a esquerda
  {
    *x0_nave= 0;
    *x1_nave= dx_nave;
  }

}

void tira_vida(int d,int inf[][2],float posicao[][4] ,int centro_nave,int *score,int ast_spawned)
{
  int i=0;
  while (i<ast_spawned)
  {  
    
    if((centro_nave>=posicao[i][2]-posicao[i][1])&&(centro_nave<=posicao[i][2]+ posicao[i][1])&&(inf[i][1]>0))
    //colocando um referencial no centro do asteroide ,se a posicao do centro da nave aquando do tiro foi entre r e -r  então retira-se uma vida
    { 
      inf[i][1]= inf[i][1] - 1;
      if ((inf[i][1]==0))
      {
        *score=*score+1; // incrementacao do score quando a vida de 1 asteroide chega a 0
 	  }   
    }
    i++;

  }
}

void fim_de_jogo( int d,int score )
{
	char nome[10],mensagem[20]; //declaração das strings
	g2_pen(d,8);
  	g2_filled_rectangle(d,200,200,400,300); // Fundo da mensagem de Final de jogo
  	g2_pen(d,3);
  	if (score==100)
  	{	
  		sprintf(mensagem,"Parabens! VENCESTE");
  		g2_string(d,260,250,mensagem);
  	}
  	else
  	{	
  		sprintf(mensagem,"GAME OVER");
    	g2_string(d,260,250,mensagem);
    }	
  	g2_flush(d);
  	printf("introduza o seu nome  : ");
  	scanf("%s",nome);
    guardar_resultados(d,nome,score);
    printf("os resultados foram guardados no ficheiro resultados.txt \n");
}

int guardar_resultados(int d , char nome[10], int score)
{
	FILE *fp; 


	fp=fopen("resultados.txt", "w"); //abrir com permissao de escrita
	if (fp==0)
		printf("ocorreu um erro ao criar o ficheiro\n");


	fprintf(fp,"%s o teu resultado foi de %d ! \n És um autêntico/a ", nome,score); // condicoes do highscore
	if (score<=5)
	{	
		fprintf(fp, "Aprendiz");
		return 0;
	}	
	if (score<=80)
	{
		fprintf(fp, "Atirador/a Profissional");
		return 0;
	}
	if (score<=100)
	{	
		fprintf(fp, "Expert" );
		
	}
	
	fclose(fp);
	return 0;
}

