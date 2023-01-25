#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define LINESZ 256

// Remove o '\n' do fim da linha
void remove_newline(char *ptr)
{
    while (*ptr) 
    {
        if (*ptr == '\n')
            *ptr = 0;
        else
            ptr++;
    }
}

//###############################################################
//---------------INFORMAÇÕES IMPORTANTES-------------------------
/*  
    01- USAR 4 espaços para tabulação
*/
//###############################################################

int main()
{
    char line[LINESZ];
    int cont = 0;   
    int r; // Conta a quantidade de parametros de um sscanf

    int func_c; // variavel para os indices das funções

    int par_i[3]; // Armazena os indices dos parametros
    char par_t[3]; // Armazena os tipos dos parametros [i = INTEIRO / a = VETOR(array)]

    FILE *arq;
    arq = fopen("testeAssemble.txt", "w");
    //Testando a abertura do arquivo
    if (arq == NULL){
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }
    //fprintf(arq, "Linha(%d): %s\n", count, line);

    // Lê uma linha por vez
    while (fgets(line, LINESZ, stdin) != NULL) 
    {
        cont++;
        remove_newline(line);
        
        r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &func_c, &par_t[0], &par_i[0], &par_t[1], &par_i[1], &par_t[2], &par_i[2]);
        if (r==1){
            printf("<<< NENHUM parametro >>>\n");
        }

        if (r==3){
            printf("<<< 1 parametro >>>\n");
        }

        if (r==5){
            printf("<<< 2 parametros >>>\n");
        }

        if (r==7){
            printf("<<< 3 parametros >>>\n");
        }

        //CABEÇALHO DAS FUNÇÕES
        if(strncmp(line, "function", 8) == 0){
            printf(".globl f%d\nf%d:\n    pushq %%rbp\n    movq %%rsp, %%rbp\n", func_c, func_c);   
            fprintf(arq, ".globl f%d\nf%d:\n    pushq %%rbp\n    movq %%rsp, %%rbp\n", func_c, func_c);

            continue; // Sua utilização dentro de um laço faz com que a execução da instrução atual (ou bloco de instruções atual) seja interrompida e passa para a próxima iteração do laço.
        }

        //FINAL DAS FUNÇÕES
        if(strcmp(line, "end") == 0){
            printf("    leave\n    ret\n");
            fprintf(arq, "    leave\n    ret\n");

            continue;
        }
    }

    fclose(arq);

    return 0;
}
