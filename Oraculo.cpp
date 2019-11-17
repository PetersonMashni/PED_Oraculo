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
       char informacao [ORA_LARG_INFO];       // Informação armazenada: Pergunta em caso de nó e resposta em caso de folha
} INFORMACAO;
       
typedef struct arv {
       INFORMACAO info;   // dados do registro
       struct arv* subd;  // ponteiro para o nodo da direita
       struct arv* sube;  // ponteiro para o nodo da esquerda
} ARVORE; 


/*********************************************************/ 
/* Variaveis Publicas                                    */
/*********************************************************/ 
int acertosOraculo;       // contador de acertos da árvore


/*********************************************************************************************/
/* Declaração das funções                                                                    */
/*********************************************************************************************/
void criaArvore ( ARVORE** r );                                    // Zera o ponteiro da árvore
void iniciaArvore ( ARVORE** r );                                  // Gera a raíz e duas primeiras folhas com dados preenchidos pelo usuário
void imprimeArvore ( ARVORE* r, int pos );                         // Demonstra em Tela a árvore
void testaArvore ( ARVORE * arvore );                              // Navega pela árvore realizando as perguntas, apresentando a resposta encontrada
                                                                      // ou solicitando o novo valor e ramificando a árvore
void salvaArvore ( ARVORE * arvore );                              // salva toda a árvore em arquivo
void carregaArvore ( ARVORE ** arvore );                           // carrega árvore salva em arquivo
void mostraEstatisticas ( ARVORE* r );                             // Exibe os dados da árvore

// Funcoes de acesso direto à árvore
void salvaRecursivo ( FILE *pont_arq, ARVORE* r, int pos );        // método auxiliar para salvamento da árvore
int  carregaRecursivo ( FILE *pont_arq, ARVORE** r, int pos );     // método auxiliar para carga de árvore salva em arquivo
void ramificaArvore( ARVORE* posicao, char* msgNovo, char* msgPerguntaDiferenca );  // torna folha em galho, subordinando duas novas folhas a este
int  contaFolhas ( ARVORE* r);                                     // conta a quantidade de folhas da árvore (palavras aprendidas)
ARVORE* alocaArvore ( );                                          // aloca uma folha nova a ser agregada à árvore

// Funcoes auxiliares
char confirma ( char* pergunta, char* par1, char* par2, char* opcoes ); // realiza pergunta ao usuário, aceitando apenas uma das opções informadas

/*********************************************************************************************/
/* Ponto de Entrada                                                                          */
/*********************************************************************************************/
int main( void )
{
	ARVORE * arvore = NULL;                  // ponteiro para a árvore
	char op;                                 // variável para receber a opção do menu
	
	acertosOraculo = 0;
	
	setlocale(LC_ALL, "Portuguese");
	
	while( 1 ){
	    printf( "+---------------------------------------------------+\n" );
	    printf( "| ORÁCULO - Uso de Árvore Binária                   |\n" );
	    printf( "+---------------------------------------------------+\n" );
	    printf( "| [1] Cria e Inicializa a Árvore                    |\n" );
	    printf( "| [2] Testa a Árvore                                |\n" );
	    printf( "| [3] Imprime a Árvore                              |\n" );
    	printf( "| [4] Salva a Árvore em Arquivo                     |\n" );
    	printf( "| [5] Carrega a Árvore de Arquivo                   |\n" );
    	printf( "| [6] Mostrar Estatisticas                          |\n" );
		printf( "| [9] Para sair do programa                         |\n" );
	    printf( "+---------------------------------------------------+\n" );
	    
		op = confirma("Digite uma das Opções do Menu: ", NULL, NULL, "1234569");
	    
	    switch( op ) {
	        case '1':   // rotina cria ARVORE       
	            criaArvore( &arvore );
	            iniciaArvore( &arvore );
	            break;                               
				                      
			case '2':   // rotina testar ARVORE       
                testaArvore( arvore );
                break;
                
			case '3':   // rotina imprime ARVORE       
                imprimeArvore( arvore, 0 );
                break;
                
            case '4':   // rotina salva ARVORE       
                salvaArvore( arvore );
                break;
                
            case '5':   // rotina carrega ARVORE       
	            carregaArvore( &arvore );
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

void criaArvore( ARVORE** r )
{
    *r = NULL; 
}

void carregaArvore( ARVORE ** arvore ){
	
	char linha [ORA_LARG_BUFFER_ARQ];
	
	FILE *pont_arq = fopen(ORA_NOME_ARQ, "r");
	
	if(confirma("\n Deseja carregar do arquivo (s/n)? ", NULL, NULL, "sn") != 's') {
		printf("\n Operação cancelada!\n\n");
		return;
	}

	if(pont_arq == NULL)
	{
		printf("\n Erro ao abrir o arquivo '%s'!\n", ORA_NOME_ARQ);
		return;
	}

	if(fgets(linha, ORA_LARG_BUFFER_ARQ, pont_arq) == NULL) 
		printf("\n Erro de Leitura!\n");
		
	else if(strcmp(ORA_CABECALHO_ARQ, linha) != 0)
		printf("\n Formato de Arquivo não Reconhecido!\n");
		
	else {
		if(fgets(linha, ORA_LARG_BUFFER_ARQ, pont_arq) == NULL) 
			printf("\n Erro de Leitura!\n");
		
		acertosOraculo = atoi(linha);
		
		criaArvore(arvore);
	  	if(carregaRecursivo(pont_arq, arvore, 0) == 0)
			printf("\n Arquivo carregado com sucesso!\n\n");
	}
	
	fclose(pont_arq);
}

int carregaRecursivo ( FILE *pont_arq, ARVORE** r, int pos ){
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
			
	*r = alocaArvore();
	
	strncpy((*r)->info.informacao, linha+pos+1, ORA_LARG_INFO);
	
	(*r)->info.informacao[strcspn((*r)->info.informacao, "\n")] = 0;
	
	fflush(stdin);
		
	if (carregaRecursivo(pont_arq, &((*r)->sube), pos+1) != 0)
		return 1;
		
	if(carregaRecursivo(pont_arq, &((*r)->subd), pos+1) != 0)
		return 1;
	
}

void ramificaArvore(ARVORE* posicao, char* msgNovo, char* msgPerguntaDiferenca) {
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
				
	ARVORE * no = alocaArvore();
	ARVORE * ant = alocaArvore();		

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

ARVORE * alocaArvore ( )
{
	ARVORE * no = ( ARVORE * ) malloc ( sizeof( ARVORE ));
	
	if( no == NULL) {
		printf("\n### Estouro de Memória! ###\n");
		exit (1);
	}
	else {
		no->sube = NULL;
		no->subd = NULL;
	}
	
	return no;
}

void salvaArvore( ARVORE * arvore ) {
	
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
	
	salvaRecursivo (pont_arq, arvore, 0);
	
	fclose(pont_arq);
	
	printf("\n Árvore Salva com Sucesso!\n\n");
}

void testaArvore ( ARVORE * arvore )
{
	ARVORE * posicao = arvore;
	
	printf("\n => Pense em algo e responda as perguntas: \n\n");
	
	int numPerg = 0;		
	while ( posicao->sube != NULL ) {
		
		numPerg++;
		
		printf(" %d) ", numPerg);
		
		if (confirma("%s ", posicao->info.informacao, NULL, "sn")=='s')
			posicao = posicao->sube;
		else
			posicao = posicao->subd;
			
	}

	if(confirma("\n Já sei! Voce pensou em '%s'. Acertei (s/n)? ", posicao->info.informacao, NULL, "sn") == 's') {
		printf(" \n Mais um ponto pra mim! :) \n\n");
		acertosOraculo ++;
	}
	else {
		ramificaArvore(
			posicao, 
			" Ok, você venceu! Me diga em que estava pensando: ",
			" Tá bom espertalhão! Faça uma pergunta que diferencie '%s' de '%s': "
		);
	}
	
}

			
void iniciaArvore ( ARVORE** r )
{
	
    ARVORE* no = alocaArvore();

	printf("\n[ Inicializando a Árvore ]\n");

	fflush(stdin);
	printf("\n Informe algo que esteja pensando: ");
	fgets(no->info.informacao, ORA_LARG_INFO, stdin);
	no->info.informacao[strcspn(no->info.informacao, "\n")] = 0;
	
	*r = no;
	
	ramificaArvore(
		no,
		" Informe outra coisa que esteja pensando: ",
		" Informe uma pergunta que diferencie '%s' de '%s': ");
		
	printf("\n Árvore inicializada com Sucesso!\n\n");
}

void salvaRecursivo ( FILE *pont_arq, ARVORE* r, int pos )
{
	pos ++;
	if (r != NULL) {
		for(int i=0; i<pos; i++)
			fprintf(pont_arq, "%c", ORA_NIVEL_CHAR);
			
		fprintf(pont_arq, "%s\n", r->info.informacao);
	
    	salvaRecursivo(pont_arq, r->sube, pos);
    	salvaRecursivo(pont_arq, r->subd, pos);
	}
}


void imprimeArvore ( ARVORE* r, int pos )
{
	if (pos == 0)
		printf("\n[ Visualização da Árvore ]\n");

	if (r != NULL) {
		for(int i=0; i<pos; i++)
			printf("| ");
			
		printf("+%s\n", r->info.informacao);
	
    	imprimeArvore(r->sube, pos+1);
    	imprimeArvore(r->subd, pos+1);
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

