/*********************************************************************************************/ 
/* Importa��o das Bibliotecas Utilizados                                                     */
/*********************************************************************************************/ 


#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>


/*********************************************************************************************/
/* Constantes utilizadas no C�digo                                                           */
/*********************************************************************************************/


const char ORA_CABECALHO_ARQ [] = "===ORACULO===\n";                                         // utilizada no cabe�alhos dos arquivos para identifica��o do arquivo
const char ORA_NIVEL_CHAR = '_';                                                             // char utilizadado para definir o nivel de cada no arquivo
const int  ORA_LARG_INFO = 256;                                                              // Largura dos valores armazenados na Arvore
const int  ORA_LARG_BUFFER_ARQ = ORA_LARG_INFO * 2;                                          // Largura do buffer utilizado para leitura de linha de arquivo
const char ORA_NOME_ARQ [] = "oraculo.txt";                                                  // Arquivo utilizado para persistir a �rvore


/*********************************************************************************************/ 
/* Defini��o dos Registros Utilizados                                                        */
/*********************************************************************************************/ 


typedef struct { 
       char informacao [ORA_LARG_INFO];                                                      // Informa��o armazenada: Pergunta em caso de n� e resposta em caso de folha
} INFORMACAO;
       
typedef struct arv {
       INFORMACAO info;                                                                      // dados do registro
       struct arv* subd;                                                                     // ponteiro para o nodo da direita
       struct arv* sube;                                                                     // ponteiro para o nodo da esquerda
} ARVORE; 


/*********************************************************************************************/ 
/* Variaveis Publicas                                                                        */
/*********************************************************************************************/ 


int acertosOraculo;                                                                          // contador de acertos da �rvore


/*********************************************************************************************/
/* Declara��o das fun��es                                                                    */
/*********************************************************************************************/


void criaArvore         ( ARVORE** r );                                                      // Zera o ponteiro da �rvore
void iniciaArvore       ( ARVORE** r );                                                      // Gera a ra�z e duas primeiras folhas com dados preenchidos pelo usu�rio
void imprimeArvore      ( ARVORE*  r, int pos );                                             // Demonstra em Tela a �rvore
void testaArvore        ( ARVORE*  arvore );                                                 // Navega pela �rvore realizando as perguntas, apresentando a resposta encontrada
                                                                                             // ou solicitando o novo valor e ramificando a �rvore
void salvaArvore        ( ARVORE*  arvore );                                                 // salva toda a �rvore em arquivo
void carregaArvore      ( ARVORE** arvore );                                                 // carrega �rvore salva em arquivo
void mostraEstatisticas ( ARVORE*  r );                                                      // Exibe os dados da �rvore
void removeRecursivo	( ARVORE**, int pos );


/*********************************************************************************************/
/* Funcoes de acesso direto � �rvore                                                         */
/*********************************************************************************************/


void salvaRecursivo     ( FILE*   pont_arq, ARVORE*  r, int pos );                           // m�todo auxiliar para salvamento da �rvore
int  carregaRecursivo   ( FILE*   pont_arq, ARVORE** r, int pos );                           // m�todo auxiliar para carga de �rvore salva em arquivo
void ramificaArvore     ( ARVORE* posicao, char* msgNovo, char* msgPerguntaDiferenca );      // torna folha em galho, subordinando duas novas folhas a este
int  contaFolhas        ( ARVORE* r);                                                        // conta a quantidade de folhas da �rvore (palavras aprendidas)
ARVORE* alocaArvore     ( );                                                                 // aloca uma folha nova a ser agregada � �rvore


/*********************************************************************************************/
/* Funcoes auxiliares                                                                        */
/*********************************************************************************************/


char confirma ( char* pergunta, char* par1, char* par2, char* opcoes );                      // realiza pergunta ao usu�rio, aceitando apenas uma das op��es informadas
void mensagem( char* msg );																	 // exibe mensagem em tela e aguarda que o usu�rio press. alguma tecla

/*********************************************************************************************/
/* Ponto de Entrada                                                                          */
/*********************************************************************************************/


int main( void )
{
	ARVORE * arvore = NULL;                                                                  // ponteiro para a �rvore
	char op;                                                                                 // vari�vel para receber a op��o do menu
	
	acertosOraculo = 0;																		 // vari�vel para contagem de acertos do algoritmo
	
	setlocale(LC_ALL, "Portuguese");														 // permite acentos nas strings do c�digo
	
	while( 1 ){																		 		 // la�o de controle do menu
	    printf( "+---------------------------------------------------+\n" );				 // montagem do menu
	    printf( "| OR�CULO - Uso de �rvore Bin�ria                   |\n" );
	    printf( "+---------------------------------------------------+\n" );
	    printf( "| [1] Cria e Inicializa a �rvore                    |\n" );
	    printf( "| [2] Testa a �rvore                                |\n" );
	    printf( "| [3] Imprime a �rvore                              |\n" );
    	printf( "| [4] Salva a �rvore em Arquivo                     |\n" );
    	printf( "| [5] Carrega a �rvore de Arquivo                   |\n" );
    	printf( "| [6] Mostrar Estatisticas                          |\n" );
    	printf( "| [7] Destruir a �rvore                             |\n" );
		printf( "| [9] Sair do programa                              |\n" );
	    printf( "+---------------------------------------------------+\n" );
	    
		op = confirma("Digite uma das Op��es do Menu: ", NULL, NULL, "12345679");			 // chama fun��o que faz pergunta validando as poss�veis opc�es 
	    
	    switch( op ) {																		 // desvia fluxo de acordo com a op��o selecionada
	        case '1':                                                                        // rotina cria ARVORE       
	            criaArvore( &arvore );														 // cria a �rvore
	            iniciaArvore( &arvore );													 // inicaliza a �rvore solicitando o preenchimento de uma pergunta
	            																			 	// e duas respostas
	            break;                               										 
				                      
			case '2':                                                                        // rotina testar ARVORE, realizando as perguntas (n�s) at� alcancar
                testaArvore( arvore );															// as folhas ou solicitando novos dados
                break;
                
			case '3':                                                                        // rotina imprime ARVORE 
                imprimeArvore( arvore, 0 );
                break;
                
            case '4':                                                                        // rotina salva ARVORE no arquivo oraculo.txt
                salvaArvore( arvore );
                break;
                
            case '5':                                                                        // rotina carrega a �rvore do arquivo oraculo.txt
	            carregaArvore( &arvore );
	            break;
                
			case '6':                                                                        // rotina mostra estatisticas
	            mostraEstatisticas ( arvore );
	            break;
	            
	        case '7':                          												 // elimina a �rvore, liberando a mem�ria utilizada
                removeRecursivo( &arvore, 0 );
	            break;
				                
           case '9':                                                                         // t�rmino do programa                                                 
                exit( 1 ); 
                break;                
                   
        }                                                                                    // switch( op )
	        
    }                                                                                        // fim do while( 1 )

	return 0;																				 // finaliza o programa
} 


/*********************************************************************************************/
/* mostraEstatisticas                                                                        */
/* objetivo: apresentar a quantidade de palavras aprendidas e                                */
/* quantidade de acertos do programa.                                                        */
/*********************************************************************************************/
void mostraEstatisticas ( ARVORE* r ){
	
	printf("\n Palavras Aprendidas.: %d", contaFolhas(r));                                   // Mostra na tela a quantidade de palavras aprendidas
	printf("\n Meus Acertos........: %d\n\n", acertosOraculo);                               // Mostra na tela a quantidade de acertos
	system("pause");                                                                         // Pausa a tela at� pressionar algum bot�o do teclado
	system("cls");                                                                           // Limpa a tela

}


/*********************************************************************************************/
/* criaArvore                                                                                */
/* objetivo: Inicializar o ponteiro da �rvore com NULL                                       */
/*********************************************************************************************/
void criaArvore( ARVORE** r )
{
    *r = NULL; 																				 // inicializa o ponteiro com NULL
}


/****************************************************************************************************/
/* carregaArvore                                                                                    */
/* objetivo: Buscar o arquivo (oraculo.txt) com informa��es e carregar para �rvore. 			    */
/****************************************************************************************************/
void carregaArvore( ARVORE ** arvore ){
	
	char linha [ORA_LARG_BUFFER_ARQ];														 // vari�vel para leitura de linha do arquivo
	
	FILE *pont_arq = fopen(ORA_NOME_ARQ, "r");												 // abre o arquivo para leitura
	
	if(confirma("\n Deseja carregar do arquivo (s/n)? ", NULL, NULL, "sn") != 's') {         // Confirma��o de carregamento do arquivo (.txt)
		printf("\n Opera��o cancelada!\n\n");                                                // mostra mensagem de cancelamento e retorna para o menu
		return;
	}

	if(pont_arq == NULL)                                                                     // Testa se foi retornado ponteiro para o arquivo (NULL = erro)
	{
		printf("\n Erro ao abrir o arquivo '%s'!\n", ORA_NOME_ARQ);                          // Mostra na tela a mensagem de erro caso n�o for poss�vel carregar o arquivo (.txt)
		return;
	}

	if(fgets(linha, ORA_LARG_BUFFER_ARQ, pont_arq) == NULL) 								 // le a primeira linha do arquivo (cabe�alho)
		printf("\n Erro de Leitura!\n");													 // se NULL, erro de leitura
		
	else if(strcmp(ORA_CABECALHO_ARQ, linha) != 0)											 // se a linha lida for diferente da constante cabe�alho
		printf("\n Formato de Arquivo n�o Reconhecido!\n");									 // mostra erro
		
	else {																					 // se linha foi lida e confere com o cabe�alho especificado
		if(fgets(linha, ORA_LARG_BUFFER_ARQ, pont_arq) == NULL) 							 // le a segunda linha que armazena o numero de acertos salso
			printf("\n Erro de Leitura!\n");												 // mostra erro
		
		acertosOraculo = atoi(linha);														 // armazena na variavel o num de acertos carregado do arquivo
																								// convertendo de string para inteiro
		
		criaArvore(arvore);																	 // incializa a arvore preparando-a para a carga
	  	if(carregaRecursivo(pont_arq, arvore, 0) == 0)										 // chama a rotina recursiva de carga do arquivo (linha por linha)
	  																							// retorna zero caso a leitura ocorra com sucesso
			mensagem("\n Arquivo carregado com sucesso!\n\n");                                 // Mensagem de que o arquivo (.txt) foi carregado para dentro da �rvore
	}
	
	fclose(pont_arq);																		 // fecha o arquivo oraculo.txt
}


/***********************************************************************************************/
/* carregaRecursivo                                                                            */
/* objetivo: continua a leitura do arquivo aberto carregando as linhas para novos n�s e folhas */
/***********************************************************************************************/
int carregaRecursivo ( FILE *pont_arq, ARVORE** r, int pos ){
	char linha [ORA_LARG_BUFFER_ARQ];														// aloca variavel usada para buffer
	fpos_t posArq;																			// variavel para armazenar a posicao do ponteiro do arquivo
																								
	fgetpos(pont_arq, &posArq);																// salva a posicao do ponteiro para retroceder, caso necessario
		
	if(fgets(linha, ORA_LARG_BUFFER_ARQ, pont_arq) == NULL)									// le a pr�xima linha do arquivo, caso fim de arquivo retorna sem erros
		return 0;																			// retorna para o n�vel anterior
	
	if( linha [pos] != ORA_NIVEL_CHAR) {													// o parametro 'pos' retorna o nivel esperado para leitura
																								// caso o caracter na posicao 'pos' seja diferente do caracter esperado
																								// � sinal que deve-se encerrar a recursividade, retornando 
		fsetpos(pont_arq, &posArq);															// reposiciona o ponteiro para a linha anterior, pois a linha
																								// n�o deve processada neste nivel
		return 0;																			// retorna para o n�vel anterior
	}
	
	for(int i=0; i<pos; i++) {																// o parametro 'pos' retorna o nivel esperado para leitura
																								// verifica se cada caracter desde o 1o at� a posicao corresponde ao caracter
																								// indicador de nivel
		if(linha[i] != ORA_NIVEL_CHAR) {													// caso n�o corresponda com o caracter esperado
			printf("Erro de formato na linha: '%s'!", linha);								// mostra erro
			return 1;																		// retorna ao n�vel anterior com erro
		}
	}
			
	*r = alocaArvore();																		// chama fun��o que aloca um n�
	
	strncpy((*r)->info.informacao, linha+pos+1, ORA_LARG_INFO);								// copia a parte da linha descartando os caracteres de n�vel
	
	(*r)->info.informacao[strcspn((*r)->info.informacao, "\n")] = 0;						// elimina o \n do final da linha
	
	if (carregaRecursivo(pont_arq, &((*r)->sube), pos+1) != 0)								// chama carregaRecursivo para esquerda, se retornar erro, retorna ao nivel anterior com erro
		return 1;																			// retorna com erro
		
	if(carregaRecursivo(pont_arq, &((*r)->subd), pos+1) != 0)								// chama carregaRecursivo para direita, se retornar erro, retorna ao nivel anterior com erro
		return 1;																			// retorna com erro
	
}


/*************************************************************************************************/
/* ramificaArvore                                                                                */
/* objetivo: Transforma folha em n�, transferindo a resposta (folha) para nova folha e recebendo */
/*    outra resposta em outra folha e armazenado a pergunta recebida no n�						 */
/*************************************************************************************************/
void ramificaArvore(ARVORE* posicao, char* msgNovo, char* msgPerguntaDiferenca) {
	char novo [ORA_LARG_INFO];															// var para receber nova resposta
	char diferenca [ORA_LARG_INFO];														// var para receber a pergunta que diferencia as respostas
	
	fflush(stdin);																		// limpa buffer do teclado
	printf(msgNovo);																	// mostra msg que solicita nova informacao
	fgets(novo, ORA_LARG_INFO, stdin);													// recebe nova informacao
	novo[strcspn(novo, "\n")] = 0;														// elimina \n da resposta
	
	fflush(stdin);																		// limpa buffer do teclado
	printf(msgPerguntaDiferenca, novo, posicao->info.informacao);						// faz pergunta q solicita pergunta q diferencie uma resposta da outra
	fgets(diferenca, ORA_LARG_INFO, stdin);												// recebe a pergunta que diferencia as perguntas
	diferenca[strcspn(diferenca, "\n")] = 0;											// remove \n da pergunta
				
	ARVORE * no = alocaArvore();														// aloca n� para armazenar nova informacao
	ARVORE * ant = alocaArvore();														// aloca n� pra armazenar a informacao anterior

	strcpy(ant->info.informacao, posicao->info.informacao);								// armazena a informa��o anterior
	strcpy(posicao->info.informacao, diferenca);										// armazena no lugar a pergunta que diferencia as informacoes
	strcpy(no->info.informacao, novo);													// armazena a nova informacao

	if (confirma(" Esta pergunta e verdadeira para '%s' (s/n)?", novo, NULL, "sn") == 's') { // se a pergunta referencia informacao nova?
		posicao->sube = no;																		// salva a informacao nova a esquerda
		posicao->subd = ant;																	// salva a ionformacao antiga a direita
	}
	else {																					// se a pergunta referencia informacao antiga?
		posicao->subd = no;																		// salva a informacao nova a direita
		posicao->sube = ant;																	// salva a informacao antiga a esquerda
	}
	
}


/*************************************************************************************************/
/* alocaArvore                                                                                   */
/* objetivo: aloca novo n�, apontando esq e dir para NULL										 */
/*************************************************************************************************/
ARVORE * alocaArvore ( )
{
	ARVORE * no = ( ARVORE * ) malloc ( sizeof( ARVORE ));								// aloca mem�ria para n�
	
	if( no == NULL) {																	// sem mem�ria?
		printf("\n### Estouro de Mem�ria! ###\n");										// mostra erro
		exit (1);																		// encerra o prorama com erro
	}
	else {																				// conseguiu alocar?
		no->sube = NULL;																// aponta esq para NULL
		no->subd = NULL;																// aponta dir para NULL
	}
	
	return no;																			// retorna o n� inicializado
}


/*********************************************************************************************/
/* salvaArvore                                                                               */
/* objetivo: Salvar as informa��es da �rvore atual dentro de um arquivo (.txt)               */
/*********************************************************************************************/
void salvaArvore( ARVORE * arvore ) {
	
	if(confirma("\n Deseja salvar a �rvore atual (s/n)? ", NULL, NULL, "sn") != 's') {      // Confirma��o de que deseja salvar a �rvore
		mensagem("\n Opera��o cancelada!\n\n");                                             // Mensagem de cancelado
		return;																				// retorna
	}
		
	FILE *pont_arq = fopen(ORA_NOME_ARQ, "w");												// abre o arquivo oraculo.txt
	
	if(pont_arq == NULL) {																	// se n�o conseguiu abrir o arquivo
		printf("\n Erro ao abrir o arquivo '%s'!\n");										// mostra erro
		return;																				// retorna pro menu
	}
	
	fprintf(pont_arq, "%s", ORA_CABECALHO_ARQ);												// salva o 'cabe�alho' no arquivo
	
	fprintf(pont_arq, "%d\n", acertosOraculo);												// salva n�mero de acertos
	
	salvaRecursivo (pont_arq, arvore, 0);													// salva recursivamente a �rvore em linhas no arquivo
	
	fclose(pont_arq);																		// fecha o arquivo
	
	mensagem("\n �rvore Salva com Sucesso!\n\n");											// mensagem de sucesso

}


/*************************************************************************************************/
/* testaArvore                                                                                   */
/* objetivo: Iniciar o teste do programa apresentando as perguntas (n�s) at� chegar em uma folha */
/*   considerada resposta v�lida, caso o usu�rio negue a resposta, solicita nova informa��o e    */
/*   pergunta que diferencie a informa��o aprsenada da nova										 */
/*************************************************************************************************/
void testaArvore ( ARVORE * arvore )
{
	ARVORE * posicao = arvore;															// aponta ponteiro de navega��o para a raiz da arvore
	
	printf("\n => Pense em algo e responda as perguntas com s=sim ou n=n�o: \n\n");		// orienta o usuario quanto �s perguntas 
	
	int numPerg = 0;																	// variavel contadora de perguntas
	while ( posicao->sube != NULL ) {													// mantem-se nas perguntas enquanto n�o encontrar uma folha
		
		numPerg++;																		// acrescenta contador
		
		printf(" %d) ", numPerg);														// imprime o num da pergunta
		
		if (confirma("%s ", posicao->info.informacao, NULL, "sn")=='s')					// se resp para a pergunta apresentada for 's', vai pra esq
			posicao = posicao->sube;													// aponta para esq
		else																			// se resp para pergunta apresentada for 'n', vai pra dir
			posicao = posicao->subd;													// aponta para a direita
			
	}

	// apresenta a resposta encontrada de acordo com as perguntas
	// caso o usu�rio confirme a palavra apresentada...
	if(confirma("\n J� sei! Voce pensou em '%s'. Acertei (s/n)? ", posicao->info.informacao, NULL, "sn") == 's') { 
		acertosOraculo ++;																// acrescenta o contador de acertos do algoritmo
		
		mensagem(" \n Mais um ponto pra mim! :) \n\n");									// mostra mensagem de acerto
	
	}
	else {																				// caso a palavra apresentada n�o seja a desejada...
		ramificaArvore(																	// solicita a palavra pensada e pergunta que diferencie
			posicao, 																	// as informa��es, recebendo a pergunta e armazenado-a, tranformando
			" Ok, voc� venceu! Me diga em que estava pensando: ",						// a folha em n� e armazenando a informacao anterior em outra folha, assim
			" T� bom espertalh�o! Fa�a uma pergunta que diferencie '%s' de '%s': "		// como com a informa��o recebida
		);
	}
	
}


/*********************************************************************************************/
/* iniciaArvore                                                                              */
/* objetivo: Recebe as duas primeiras informa��es e a pegunta diferenciadora, armazenando-as */
/* 		na arvore																			 */
/*********************************************************************************************/
void iniciaArvore ( ARVORE** r )
{
	
    ARVORE* no = alocaArvore();													// aloca novo n�

	printf("\n[ Inicializando a �rvore ]\n");									// mostra mensagem

	fflush(stdin);																// limpa buffer
	printf("\n Informe algo que esteja pensando: ");							// mostra mensagem, solicitando informa��o
	fgets(no->info.informacao, ORA_LARG_INFO, stdin);							// recebe uma das informacoes
	no->info.informacao[strcspn(no->info.informacao, "\n")] = 0;				// elimina o \n
	
	*r = no;																	// aponta para a raiz
	
	ramificaArvore(
		no,																		// transforma uma informa��o em nova folha
		" Informe outra coisa que esteja pensando: ",								// recebe nova informacao, transferindo as informacoes para	
		" Informe uma pergunta que diferencie '%s' de '%s': ");						// as folhas e armazenado no n� inicial a pergunta
		
	mensagem("\n �rvore inicializada com Sucesso!\n\n");						// mostra mensagem de sucesso
	
}


/*********************************************************************************************/
/* salvaRecursivo                                                                            */
/* objetivo: Salva recursivamente a �rvore em linhas no arquivo apontado pelo ponteiro		 */
/*********************************************************************************************/
void salvaRecursivo ( FILE *pont_arq, ARVORE* r, int pos )
{
	pos ++;																		// contador de nivel
	if (r != NULL) {															// se o ponteiro para o n� recebido <> NULL 
		for(int i=0; i<pos; i++)												// grava n caracteres (correspondentes aos n�veis)
			fprintf(pont_arq, "%c", ORA_NIVEL_CHAR);
			
		fprintf(pont_arq, "%s\n", r->info.informacao);							// grava a informacao
	
    	salvaRecursivo(pont_arq, r->sube, pos);									// segue recursivamente para a esq
    	salvaRecursivo(pont_arq, r->subd, pos);									// segue recursivamente para a dir
	}
}


/*********************************************************************************************/
/* imprimeArvore                                                                             */
/* objetivo: Mostrar na tela a �rvore de decis�es criada at� o momento.                      */
/*********************************************************************************************/
void imprimeArvore ( ARVORE* r, int pos )
{
	if (pos == 0)
		printf("\n[ Visualiza��o da �rvore ]\n\n");

	if (r != NULL) {
		for(int i=0; i<pos; i++)
			printf("| ");
			
		printf("+%s\n", r->info.informacao);
	
    	imprimeArvore(r->sube, pos+1);
    	imprimeArvore(r->subd, pos+1);
	}
	if (pos == 0)
		mensagem("\n");
}

/*********************************************************************************************/
/* contaFolhas                                                                               */
/* objetivo: Verificar a quantidades de folhas que possuem na �rvore para                    */
/* saber a quantidade de palavras aprendidas (respostas).                                    */
/*********************************************************************************************/

int contaFolhas ( ARVORE* r )
{
	if (r == NULL)
		return 0;

	if ( r->sube == NULL && r->subd == NULL )                                                // sou uma folha?
		return 1;
		
	return 
		contaFolhas(r->sube) +
		contaFolhas(r->subd);
}

/*********************************************************************************************/
/* confirma                                                                                  */
/* objetivo: Confirmar as perguntas feitas no programa                                       */
/*********************************************************************************************/

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

/*********************************************************************************************/
/* mensagem                                                                                  */
/* objetivo: Exibe mensagem em tela e espera que o usu�rio press. alguma tecla               */
/*********************************************************************************************/
void mensagem( char* msg )
{
	printf(msg); 		                                							 // Exibe a mensagem
	system ("pause");                                                             	 // Pausa a tela at� pressionar alguma tecla
	system ("cls");                                                                  // Limpa a tela
	
}


/*********************************************************************************************/
/* removeRecursivo                                                                           */
/* objetivo: remove recursivamente a �rvore, liberando a mem�ria utilizada                   */
/*********************************************************************************************/
void removeRecursivo( ARVORE **p, int pos ){	
	if(pos == 0 && confirma("\nDeseja excluir a �rvore em mem�ria? ", NULL, NULL, "sn")=='n')
		return;
		
	if ((*p) != NULL) {	
    	removeRecursivo(&(*p)->sube, pos+1);
    	removeRecursivo(&(*p)->subd, pos+1);
	}
	free (*p);
	*p = NULL;
	
	if(pos == 0)
	{
		mensagem("\n�rvore exclu�da com sucesso!\n\n"); 		                                // Mensagem sucesso!
	}
		
 	return;
}

