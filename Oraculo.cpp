#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>



/***********************************************/ 
/* Definição dos Registros                     */
/***********************************************/ 
typedef struct { 
       char informacao [256];       
} INFORMACAO;
       
typedef struct arv {
       INFORMACAO info;   // dados do registro
       struct arv* subd;  // ponteiro para o nodo da direita
       struct arv* sube;  // ponteiro para o nodo da esquerda
} ARVORE; 


void cria_ARVORE( ARVORE** r );
void init_ARVORE ( ARVORE** r);
void print_ARVORE (ARVORE* r, int pos);
char confirma(char* pergunta, char* par1, char* par2);

int main( void )
{
	ARVORE * arvore;
	ARVORE * posicao;
	
	char respContinua = 's';
	
	char novo [256];
	char diferenca [256];
	
	setlocale(LC_ALL, "pt_BR.iso-8859-1");
	
	cria_ARVORE( &arvore );
	init_ARVORE( &arvore );
	
	int c = 0;
	while( respContinua == 's')
	{
	
		printf("... Pense em algo e press alguma tecla para continuarmos ...\n");
		getch();

		posicao = arvore;
				
		while ( posicao->sube != NULL )
		{
			if (confirma("%s ", posicao->info.informacao, NULL)=='s')
				posicao = posicao->sube;
			else
				posicao = posicao->subd;
		}

		if(confirma("Ja sei! Voce pensou em '%s'. Acertei? ", posicao->info.informacao, NULL) == 's')
			printf("Ok! Mais um ponto pra mim!\n");
		else
		{
			fflush(stdin);
			printf("Ok, voce venceu! Me diga em que estava pensando: ");
			fgets(novo, 256, stdin);
			novo[strcspn(novo, "\n")] = 0;
			
			fflush(stdin);
			printf("Ta bom espertalhao! Faça uma pergunta que diferencie '%s' de '%s': ", novo, posicao->info.informacao);
			fgets(diferenca, 256, stdin);
			diferenca[strcspn(diferenca, "\n")] = 0;
						
			ARVORE * no = ( ARVORE * ) malloc ( sizeof( ARVORE ));
			no->sube = NULL;
			no->subd = NULL;
			
			ARVORE * ant = ( ARVORE * ) malloc ( sizeof( ARVORE ));
			ant->sube = NULL;
			ant->subd = NULL;

			if( no != NULL ){
				strcpy(ant->info.informacao, posicao->info.informacao);
				strcpy(posicao->info.informacao, diferenca);
				strcpy(no->info.informacao, novo);
			
				if (confirma("Esta pergunta e verdadeira para '%s'? ", novo, NULL) == 's')
				{
					posicao->sube = no;
					posicao->subd = ant;
				}
				else
				{
					posicao->subd = no;
					posicao->sube = ant;
				}	
    		}
    		else
    			printf("Ops! Estouro de Memoria!");
		}
		print_ARVORE(arvore, 0);
		respContinua = confirma("Vamos continuar isso ? ", NULL, NULL);

	}
	
	
	return 0;
} 

void cria_ARVORE( ARVORE** r )
{
    *r = NULL; 
}

void init_ARVORE ( ARVORE** r)
{
    ARVORE* no = ( ARVORE * ) malloc ( sizeof( ARVORE ));

	if( no != NULL ){
    	strcpy(no->info.informacao, "bicicleta");
    	no->subd = NULL;
    	no->sube = NULL;
    	*r = no;
    }
    else
    	printf("Ops! Estouro de Memória!");
}

void print_ARVORE (ARVORE* r, int pos)
{
	if (pos == 0)
		printf("\n==================== VISAO DA ARVORE ====================\n");
	pos ++;
	if (r != NULL)
	{
		for(int i=0; i<pos; i++)
			printf("  ");
			
		printf("-%s\n", r->info.informacao);
	
    	print_ARVORE(r->sube, pos);
    	print_ARVORE(r->subd, pos);
	}
	if (pos == 1)
		printf("=========================================================\n");
}

char confirma(char* pergunta, char* par1, char* par2)
{
	char resp = ' ';
	
	while (resp != 's' && resp != 'n')
	{
		fflush(stdin);
		printf(pergunta, par1, par2);
		scanf("%c", &resp);
	}
	return resp;
}

