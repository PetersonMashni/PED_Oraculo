#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>    // setlocale

#define FALSE   0      // constante falsa
#define TRUE   !FALSE  // constante verdadeira


/***********************************************/ 
/* Defini��o dos Registros                     */
/***********************************************/ 
typedef struct {          // registro 
       char informacao [256];       
} INFORMACAO;
       
typedef struct arv {
       INFORMACAO info;   // dados do registro
       struct arv* subd;  // ponteiro para o nodo da direita
       struct arv* sube;  // ponteiro para o nodo da esquerda
} ARVORE; 


// declara��o das fun��es
void cria_ARVORE( ARVORE** r );
void adivinha( ARVORE** r );
	
int main( void )
{
    char op;       // op��o do menu                               
    ARVORE* r;
        
    setlocale(LC_ALL, "Portuguese");
    cria_ARVORE( &r );
    
    printf("Bem-Vindo ao Or�culo!\n\n");
    
    char pergunta = 's';
    
    while(pergunta == 's')
    {
		printf("Vamos jogar (s/n)?");
		fflush(stdin);
		scanf("%c", &pergunta);
	
		if (pergunta == 's')
			printf("Pense em alguma coisa e press <enter> para continuar...");
			
		adivinha( &r );
	}
     
 return 0;
} // fim do programa principal

/************************************************
 * cria_ARVORE                                   *
 * objetivo: rotina para inicializar a ARVORE    *
 * entrada : nenhuma                             *
 * sa�da   : NULL (inicializa ARVORE)            *
 ************************************************/ 
void cria_ARVORE( ARVORE** r ){
    *r = NULL; // arvore criada, raiz nao aponta
}

void adivinha( ARVORE** r )
{
	if( *r == NULL )
	{
		printf("N�o sei o que voc� est� pensando!\n");
		
	}
}


