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

char HEADER_ARQ [] = "===ORACULO===\n";
char NO_CHAR = '_';

void cria_ARVORE( ARVORE** r );
void init_ARVORE ( ARVORE** r);
void ramifica_ARVORE(ARVORE* posicao, char* msgNovo, char* msgPerguntaDiferenca);
void imprime_ARVORE (ARVORE* r, int pos);
void testa_ARVORE(ARVORE * arvore);
void salva_ARVORE(ARVORE * arvore);
void salva_recursivo (FILE *pont_arq, ARVORE* r, int pos);
void carrega_ARVORE(ARVORE ** arvore);
int  carrega_recursivo (FILE *pont_arq, ARVORE** r, int pos);
ARVORE * aloca_ARVORE();
char confirma(char* pergunta, char* par1, char* par2);

int main( void )
{
	ARVORE * arvore;
	char op;
	
	setlocale(LC_ALL, "pt_BR.iso-8859-1");
	
	while( 1 ){
	    printf( "\n +---------------------------------------------------+" ); 
	    printf( "\n | ORACULO - Uso de Arvore Binaria                   |" );
	    printf( "\n +---------------------------------------------------+" ); 
	    printf( "\n | [1] Cria e Inicializa a Arvore                    |" );
	    printf( "\n | [2] Testa a Arvore                                |" );
	    printf( "\n | [3] Imprime a Arvore                              |" );	    
    	printf( "\n | [4] Salva a Arvore                                |" );	    	    
    	printf( "\n | [5] Carrega uma Arvore                            |" );	    	    
		printf( "\n | [9] Para sair do programa                         |" );         
	    printf( "\n +---------------------------------------------------+" );      
	    printf( "\n Opcao: " );
	    
	    fflush( stdin ); // limpa buffer do teclado, funciona junto com entrada de dados
	    op = getche(); // tecla de opção do menu
	    printf( "\n" );
	
	    switch( op ) {
	        case '1':   // rotina cria ARVORE       
	            cria_ARVORE( &arvore );
	            init_ARVORE( &arvore );
	            break;                               
				                      
			case '2':   // rotina testar ARVORE       
                testa_ARVORE( arvore );
                break;
                
			case '3':   // rotina imprime ARVORE       
                imprime_ARVORE( arvore, 0 );
                break;
                
            case '4':   // rotina salva ARVORE       
                salva_ARVORE( arvore );
                break;
                
            case '5':   // rotina carrega ARVORE       
	            carrega_ARVORE( &arvore );
	            break;
                
           case '9':  // término do programa                                                 
                exit( 1 ); 
                break;                
                   
           default : 
                printf( "\n Digite uma opcao!" );
                break;
        } // switch( op )
	        
    } // fim do while( 1 )

	return 0;
} 

void cria_ARVORE( ARVORE** r )
{
    *r = NULL; 
}

void carrega_ARVORE(ARVORE ** arvore){
	
	char destino [256] = "oraculo.txt";
	char linha [512];
	
	FILE *pont_arq;
	
	pont_arq = fopen(destino, "r");
	
	if(pont_arq == NULL)
	{
		printf("Erro ao abrir o arquivo '%s'!\n");
		return;
	}

	if(fgets(linha, 512, pont_arq) == NULL)
		printf("Erro de Leitura!\n");
	else if(strcmp(HEADER_ARQ, linha) != 0)
		printf("Formato de Arquivo nao Reconhecido!Erro de Leitura!\n");
	else
	{
		cria_ARVORE(arvore);
	  	if(carrega_recursivo(pont_arq, arvore, 0) == 0)
			printf("Arquivo carregado com sucesso!\n");
	}
	fclose(pont_arq);
}

int carrega_recursivo (FILE *pont_arq, ARVORE** r, int pos){
	
	char linha [512];
	
	if(fgets(linha, 512, pont_arq) == NULL)
	{
		printf("Erro de Leitura!\n");
		return 1;
	}
		
	pos ++;
	
	for(int i=0; i<pos; i++){
		if(linha[i] != NO_CHAR)
		{		
			printf("Erro de formato na linha: '%s'!", linha);
			return 1;
		}
			
		*r = aloca_ARVORE();
		
		// falta ignorar os caractres iniciais e eliminar o enter do final da linha
		
		strncpy((*r)->info.informacao, linha+pos, 256);
		
		(*r)->info.informacao[strcspn((*r)->info.informacao, "\n")] = 0;
		
		printf("%s\n", (*r)->info.informacao);
		fflush(stdin);
		getch();
		
	
    	if (carrega_recursivo(pont_arq, &((*r)->sube), pos) != 0)
    		return 1;
    		
    	if(carrega_recursivo(pont_arq, &((*r)->subd), pos) != 0)
    		return 1;
	}
	
}

void ramifica_ARVORE(ARVORE* posicao, char* msgNovo, char* msgPerguntaDiferenca)
{
	char novo [256];
	char diferenca [256];
	
	fflush(stdin);
	printf(msgNovo);
	fgets(novo, 256, stdin);
	novo[strcspn(novo, "\n")] = 0;
	
	fflush(stdin);
	printf(msgPerguntaDiferenca, novo, posicao->info.informacao);
	fgets(diferenca, 256, stdin);
	diferenca[strcspn(diferenca, "\n")] = 0;
				
	ARVORE * no = aloca_ARVORE();
	ARVORE * ant = aloca_ARVORE();		

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

ARVORE * aloca_ARVORE()
{
	ARVORE * no = ( ARVORE * ) malloc ( sizeof( ARVORE ));
	
	if( no == NULL)
	{
		printf("\n### Estouro de Memoria! ###\n");
		exit (1);
	}
	else
	{
		no->sube = NULL;
		no->subd = NULL;
	}
	return no;
}

void salva_ARVORE(ARVORE * arvore)
{
	char destino [256] = "oraculo.txt";
	
	FILE *pont_arq;
	
	pont_arq = fopen(destino, "w");
	
	if(pont_arq == NULL)
	{
		printf("Erro ao abrir o arquivo '%s'!\n");
		return;
	}
	
	fprintf(pont_arq, "%s", HEADER_ARQ);
	
	salva_recursivo(pont_arq, arvore, 0);
	
	fclose(pont_arq);
}

void testa_ARVORE(ARVORE * arvore)
{
	ARVORE * posicao = arvore;
	
	printf("... Pense em algo e press alguma tecla para continuarmos ...\n");
	getch();
			
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
		ramifica_ARVORE(
			posicao, 
			"Ok, voce venceu! Me diga em que estava pensando: ",
			"Ta bom espertalhao! Faça uma pergunta que diferencie '%s' de '%s': "
		);
	}
	
}

			
void init_ARVORE ( ARVORE** r)
{
	
    ARVORE* no = aloca_ARVORE();

	fflush(stdin);
	printf("Informe algo que esteja pensando: ");
	fgets(no->info.informacao, 256, stdin);
	no->info.informacao[strcspn(no->info.informacao, "\n")] = 0;
	
	*r = no;
	
	ramifica_ARVORE(
		no,
		"Informe outra coisa que esteja pensando: ",
		"Informe uma pergunta que diferencie '%s' de '%s': ");
}

void salva_recursivo (FILE *pont_arq, ARVORE* r, int pos)
{
	pos ++;
	if (r != NULL)
	{
		for(int i=0; i<pos; i++)
			fprintf(pont_arq, "%c", NO_CHAR);
			
		fprintf(pont_arq, "%s\n", r->info.informacao);
	
    	salva_recursivo(pont_arq, r->sube, pos);
    	salva_recursivo(pont_arq, r->subd, pos);
	}
}


void imprime_ARVORE (ARVORE* r, int pos)
{
	if (pos == 0)
		printf("\n==================== VISAO DA ARVORE ====================\n");
	pos ++;
	if (r != NULL)
	{
		for(int i=0; i<pos; i++)
			printf("  ");
			
		printf("-%s\n", r->info.informacao);
	
    	imprime_ARVORE(r->sube, pos);
    	imprime_ARVORE(r->subd, pos);
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

