/*********************************************************/ 
/* Importação das Bibliotecas Utilizados                 */
/*********************************************************/ 
#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


/********************************************************************************************************************************/
/* Constantes utilizadas no Código                                                                                              */
/********************************************************************************************************************************/
const char ORA_CABECALHO_ARQ [] = "===ORACULO===\n";       // utilizada no cabeçalhos dos arquivos para identificação do arquivo
const char ORA_NIVEL_CHAR = '_';                           // char utilizadado para definir o nivel de cada no arquivo
const int  ORA_LARG_INFO = 256;                            // Largura dos valores armazenados na Arvore
const int  ORA_LARG_BUFFER_ARQ = ORA_LARG_INFO * 2;        // Largura do buffer utilizado para leitura de linha de arquivo
const char ORA_NOME_ARQ [] = "oraculo.txt";                // Arquivo utilizado para persistir a árvore


/*********************************************************/ 
/* Definição dos Registros Utilizados                    */
/*********************************************************/ 
typedef struct { 
       char informacao [ORA_LARG_INFO];       
} INFORMACAO;
       
typedef struct arv {
       INFORMACAO info;   // dados do registro
       struct arv* subd;  // ponteiro para o nodo da direita
       struct arv* sube;  // ponteiro para o nodo da esquerda
} ARVORE; 


/*********************************************************/ 
/* Variaveis Publicas                                    */
/*********************************************************/ 
int acertosOraculo;


/*********************************************************************************************/
/* Declaração das funções                                                                    */
/*********************************************************************************************/
void cria_ARVORE( ARVORE** r );
void init_ARVORE ( ARVORE** r );
void ramifica_ARVORE( ARVORE* posicao, char* msgNovo, char* msgPerguntaDiferenca );
void imprime_ARVORE ( ARVORE* r, int pos );
void testa_ARVORE( ARVORE * arvore );
void salva_ARVORE( ARVORE * arvore );
void salva_recursivo ( FILE *pont_arq, ARVORE* r, int pos );
void carrega_ARVORE( ARVORE ** arvore );
int  carrega_recursivo ( FILE *pont_arq, ARVORE** r, int pos );
int contaFolhas ( ARVORE* r);
ARVORE * aloca_ARVORE ( );
char confirma ( char* pergunta, char* par1, char* par2, char* opcoes );
void mostraEstatisticas ( ARVORE* r );


/*********************************************************************************************/
/* Ponto de Entrada                                                                          */
/*********************************************************************************************/
int main( void )
{
	ARVORE * arvore = NULL;
	char op;
	
	acertosOraculo = 0;
	
	setlocale(LC_ALL, "Portuguese");
	
	while( 1 ){
	    printf( "+---------------------------------------------------+\n" );
	    printf( "| ORACULO - Uso de Arvore Binaria                   |\n" );
	    printf( "+---------------------------------------------------+\n" );
	    printf( "| [1] Cria e Inicializa a Arvore                    |\n" );
	    printf( "| [2] Testa a Arvore                                |\n" );
	    printf( "| [3] Imprime a Arvore                              |\n" );
    	printf( "| [4] Salva a Arvore em Arquivo                     |\n" );
    	printf( "| [5] Carrega a Arvore de Arquivo                   |\n" );
    	printf( "| [6] Mostrar Estatisticas                          |\n" );
		printf( "| [9] Para sair do programa                         |\n" );
	    printf( "+---------------------------------------------------+\n" );
	    
		op = confirma("Digite uma das Opcoes do Menu: ", NULL, NULL, "1234569");
	    
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
                
			case '6':   // rotina carrega ARVORE       
	            mostraEstatisticas ( arvore );
	            break;
				                
           case '9':  // término do programa                                                 
                exit( 1 ); 
                break;                
                   
        } // switch( op )
	        
    } // fim do while( 1 )

	return 0;
} 

void mostraEstatisticas ( ARVORE* r ){
	
	printf("\n Palavras Aprendidas.: %d", contaFolhas(r));
	printf("\n Meus Acertos........: %d\n\n", acertosOraculo);

}

void cria_ARVORE( ARVORE** r )
{
    *r = NULL; 
}

void carrega_ARVORE( ARVORE ** arvore ){
	
	char linha [ORA_LARG_BUFFER_ARQ];
	
	FILE *pont_arq = fopen(ORA_NOME_ARQ, "r");
	
	if(pont_arq == NULL)
	{
		printf("\n Erro ao abrir o arquivo '%s'!\n", ORA_NOME_ARQ);
		return;
	}

	if(fgets(linha, ORA_LARG_BUFFER_ARQ, pont_arq) == NULL) 
		printf("\n Erro de Leitura!\n");
		
	else if(strcmp(ORA_CABECALHO_ARQ, linha) != 0)
		printf("\n Formato de Arquivo nao Reconhecido!\n");
		
	else {
		if(fgets(linha, ORA_LARG_BUFFER_ARQ, pont_arq) == NULL) 
			printf("\n Erro de Leitura!\n");
		
		acertosOraculo = atoi(linha);
		
		cria_ARVORE(arvore);
	  	if(carrega_recursivo(pont_arq, arvore, 0) == 0)
			printf("\n Arquivo carregado com sucesso!\n\n");
	}
	
	fclose(pont_arq);
}

int carrega_recursivo ( FILE *pont_arq, ARVORE** r, int pos ){
	char linha [ORA_LARG_BUFFER_ARQ];
	fpos_t posArq;
	fgetpos(pont_arq, &posArq);
		
	if(fgets(linha, ORA_LARG_BUFFER_ARQ, pont_arq) == NULL)
		return 0;
	
	if( linha [pos] != ORA_NIVEL_CHAR) {
		fsetpos(pont_arq, &posArq);
		return 0;
	}
	
	for(int i=0; i<pos; i++) {
		if(linha[i] != ORA_NIVEL_CHAR) {		
			printf("Erro de formato na linha: '%s'!", linha);
			return 1;
		}
	}
			
	*r = aloca_ARVORE();
	
	strncpy((*r)->info.informacao, linha+pos+1, ORA_LARG_INFO);
	
	(*r)->info.informacao[strcspn((*r)->info.informacao, "\n")] = 0;
	
	fflush(stdin);
		
	if (carrega_recursivo(pont_arq, &((*r)->sube), pos+1) != 0)
		return 1;
		
	if(carrega_recursivo(pont_arq, &((*r)->subd), pos+1) != 0)
		return 1;
	
}

void ramifica_ARVORE(ARVORE* posicao, char* msgNovo, char* msgPerguntaDiferenca) {
	char novo [ORA_LARG_INFO];
	char diferenca [ORA_LARG_INFO];
	
	fflush(stdin);
	printf(msgNovo);
	fgets(novo, ORA_LARG_INFO, stdin);
	novo[strcspn(novo, "\n")] = 0;
	
	fflush(stdin);
	printf(msgPerguntaDiferenca, novo, posicao->info.informacao);
	fgets(diferenca, ORA_LARG_INFO, stdin);
	diferenca[strcspn(diferenca, "\n")] = 0;
				
	ARVORE * no = aloca_ARVORE();
	ARVORE * ant = aloca_ARVORE();		

	strcpy(ant->info.informacao, posicao->info.informacao);
	strcpy(posicao->info.informacao, diferenca);
	strcpy(no->info.informacao, novo);

	if (confirma(" Esta pergunta e verdadeira para '%s' (s/n)?", novo, NULL, "sn") == 's') {
		posicao->sube = no;
		posicao->subd = ant;
	}
	else {
		posicao->subd = no;
		posicao->sube = ant;
	}	
}

ARVORE * aloca_ARVORE ( )
{
	ARVORE * no = ( ARVORE * ) malloc ( sizeof( ARVORE ));
	
	if( no == NULL) {
		printf("\n### Estouro de Memoria! ###\n");
		exit (1);
	}
	else {
		no->sube = NULL;
		no->subd = NULL;
	}
	
	return no;
}

void salva_ARVORE( ARVORE * arvore ) {
	
	if(confirma("\n Deseja salvar a árvore atual (s/n)? ", NULL, NULL, "sn") != 's') {
		printf("\n Operação cancelada!\n\n");
		return;
	}
		
	FILE *pont_arq = fopen(ORA_NOME_ARQ, "w");
	
	if(pont_arq == NULL) {
		printf("\n Erro ao abrir o arquivo '%s'!\n");
		return;
	}
	
	fprintf(pont_arq, "%s", ORA_CABECALHO_ARQ);
	
	fprintf(pont_arq, "%d\n", acertosOraculo);
	
	salva_recursivo(pont_arq, arvore, 0);
	
	fclose(pont_arq);
	
	printf("\n Arvore Salva com Sucesso!\n\n");
}

void testa_ARVORE ( ARVORE * arvore )
{
	ARVORE * posicao = arvore;
	
	printf("\n ... Pense em algo e press alguma tecla para continuarmos ...\n\n");
	getch();
			
	while ( posicao->sube != NULL ) {
		if (confirma("%s ", posicao->info.informacao, NULL, "sn")=='s')
			posicao = posicao->sube;
		else
			posicao = posicao->subd;
	}

	if(confirma("\n Ja sei! Voce pensou em '%s'. Acertei (s/n)? ", posicao->info.informacao, NULL, "sn") == 's') {
		printf(" \n Mais um ponto pra mim! :) \n\n");
		acertosOraculo ++;
	}
	else {
		ramifica_ARVORE(
			posicao, 
			" Ok, voce venceu! Me diga em que estava pensando: ",
			" Ta bom espertalhao! Faça uma pergunta que diferencie '%s' de '%s': "
		);
	}
	
}

			
void init_ARVORE ( ARVORE** r )
{
	
    ARVORE* no = aloca_ARVORE();

	printf("\n[ Inicializando a Arvore ]\n");

	fflush(stdin);
	printf("\n Informe algo que esteja pensando: ");
	fgets(no->info.informacao, ORA_LARG_INFO, stdin);
	no->info.informacao[strcspn(no->info.informacao, "\n")] = 0;
	
	*r = no;
	
	ramifica_ARVORE(
		no,
		" Informe outra coisa que esteja pensando: ",
		" Informe uma pergunta que diferencie '%s' de '%s': ");
		
	printf("\n Arvore inicializado com Sucesso!\n\n");
}

void salva_recursivo ( FILE *pont_arq, ARVORE* r, int pos )
{
	pos ++;
	if (r != NULL) {
		for(int i=0; i<pos; i++)
			fprintf(pont_arq, "%c", ORA_NIVEL_CHAR);
			
		fprintf(pont_arq, "%s\n", r->info.informacao);
	
    	salva_recursivo(pont_arq, r->sube, pos);
    	salva_recursivo(pont_arq, r->subd, pos);
	}
}


void imprime_ARVORE ( ARVORE* r, int pos )
{
	if (pos == 0)
		printf("\n[ Visualização da Árvore ]\n");

	if (r != NULL) {
		for(int i=0; i<pos; i++)
			printf("| ");
			
		printf("+%s\n", r->info.informacao);
	
    	imprime_ARVORE(r->sube, pos+1);
    	imprime_ARVORE(r->subd, pos+1);
	}
	
	if (pos == 0)
		printf("\n");
}

int contaFolhas ( ARVORE* r )
{
	if (r == NULL)
		return 0;

	if ( r->sube == NULL && r->subd == NULL ) // sou uma folha?
		return 1;
		
	return 
		contaFolhas(r->sube) +
		contaFolhas(r->subd);
}

char confirma( char* pergunta, char* par1, char* par2, char* opcoes )
{
	char resp = ' ';
	
	while (1) {
		printf(pergunta, par1, par2);
		
		fflush(stdin);
		resp = getch();
		
		printf("%c\n", resp);
		for(int i=0; i<strlen(opcoes); i++)
			if(opcoes[i] == resp)
				return resp;
	}
	
	return resp;
	
}

