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

typedef struct Pilhas{
    char def; // armazena a definição de uma variavel [p = Parametro / v = Variavel (vetor ou variavel inteira)]
    char tipo; // armazena o tipo de uma variavel [a = Array / i = Inteiro]
    int numero; // armazena o indice da variavel ou parametro 
    int posi; // armazena a posição na pilha q a variavel ou parametro vai ocupar [%rbp]
    int tam_vet; // armazena um possivel tamanho do vetor (se for apenas um inteiro recebe somente 1)
    int *var; // armazena o valor q vai ficar na variavel
}Pilha;


int main()
{
    char line[LINESZ];
    int cont = 0;   
    int r; // Conta a quantidade de parametros de um sscanf

    int func_c; // variavel para os indices das funções

    int par_i[3]; // Armazena os indices dos parametros
    char par_t[3]; // Armazena os tipos dos parametros [i = INTEIRO / a = VETOR(array)]

    int var_i[5]; // Armazena os indices das variaveis
    char var_t[5]; // Armazena os tipos das variaveis [i = INTEIRO / a = VETOR(array)]
    int var_cont = 0; // variavel auxiliar para indicar a posição dos indices ou dos tipos do vetor de variaveis [var_i / var_t]
    int key = 0; // Variavel auxiliar para indicar a entrada em uma definição de variaveis
    int var_const; // armazena o valor de uma constante (exemplo: atribução da quantidade de elementos do vetor)

    Pilha p1[10]; // vetor da struct pilha
    int i_pilha = 0; // armazena o indice q o vetor pilha está
    int tam_pilha = 0; // armazena o tamanho da pilha

    int i = 0;

    int atr_i0; // armazenar o indice da variavel que recebe alguma atribuição
    int atr_iop1, atr_iop2; // armazenar o indice das variaveis q vão realizar uma operação ou armazena o valor da constante
    char atr_top1, atr_top2; // armazenar os tipos das variaveis q vão realizar uma operação
    char atr_opa; // armazena o operador (* / - +) 
    int atr_pos0; // armazena a posição da pilha q a variavel vai receber na operação
    int atr_poso1, atr_poso2; // armazena a posição da pilha das variaveis q vão realizar a operação

    FILE *arq; // usado para escrever em um arquivo, facilitando a visualização dos testes
    arq = fopen("testeAssemble.txt", "w");
    //Testando a abertura do arquivo
    if (arq == NULL){
        printf("Erro ao tentar abrir o arquivo!");
        exit(1);
    }

    printf(".section.rodata\n.data\n\n.text\n\n");
    fprintf(arq, ".section.rodata\n.data\n\n.text\n\n");
    // Lê uma linha por vez
    while (fgets(line, LINESZ, stdin) != NULL) 
    {
        cont++;
        remove_newline(line);
        
        //DEFINIÇÃO DOS PARAMETROS NA PILHA
        r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &func_c, &par_t[0], &par_i[0], &par_t[1], &par_i[1], &par_t[2], &par_i[2]);
        if (r==1){
            i_pilha = 0;
            tam_pilha = 0;
        }

        if (r==3){
            i_pilha = 1;
            tam_pilha = 8;

            p1[0].def = 'p';
            p1[0].tipo = par_t[0];
            p1[0].numero = par_i[0];
            p1[0].posi = - tam_pilha;
            //printf("Definição: %c\n", p1[0].def);
            //printf("Tipo: %c\n", p1[0].tipo);
            //printf("Posição: %d\n", p1[0].posi);
        }

        if (r==5){
            i_pilha = 2;
            tam_pilha = 16;

            p1[0].def = 'p';
            p1[0].tipo = par_t[0];
            p1[0].numero = par_i[0];
            p1[0].posi = - 8;
            //printf("Definição: %c\n", p1[0].def);
            //printf("Tipo: %c\n", p1[0].tipo);
            //printf("Posição: %d\n", p1[0].posi);

            p1[1].def = 'p';
            p1[1].tipo = par_t[1];
            p1[1].numero = par_i[1];
            p1[1].posi = - tam_pilha;
            //printf("Definição: %c\n", p1[1].def);
            //printf("Tipo: %c\n", p1[1].tipo);
            //printf("Posição: %d\n", p1[1].posi);
        }

        if (r==7){
            i_pilha = 3;
            tam_pilha = 24;

            p1[0].def = 'p';
            p1[0].tipo = par_t[0];
            p1[0].numero = par_i[0];
            p1[0].posi = - 8;
            //printf("Definição: %c\n", p1[0].def);
            //printf("Tipo: %c\n", p1[0].tipo);
            //printf("Posição: %d\n", p1[0].posi);

            p1[1].def = 'p';
            p1[1].tipo = par_t[1];
            p1[1].numero = par_i[1];
            p1[1].posi = - 16;
            //printf("Definição: %c\n", p1[1].def);
            //printf("Tipo: %c\n", p1[1].tipo);
            //printf("Posição: %d\n", p1[1].posi);

            p1[2].def = 'p';
            p1[2].tipo = par_t[2];
            p1[2].numero = par_i[2];
            p1[2].posi = - tam_pilha;
            //printf("Definição: %c\n", p1[2].def);
            //printf("Tipo: %c\n", p1[2].tipo);
            //printf("Posição: %d\n", p1[2].posi);
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

        //VARIAVEIS LOCAIS
        if(strcmp(line, "def") == 0){
            key = 1;
            continue;
        }

        if(key == 1){
            
            if(strncmp(line, "var", 3) == 0){
                r = sscanf(line, "var v%c%d", &var_t[var_cont], &var_i[var_cont]);
                tam_pilha = tam_pilha + 4;

                p1[i_pilha].def = 'v';
                p1[i_pilha].tipo = var_t[var_cont];
                p1[i_pilha].numero = var_i[var_cont];
                p1[i_pilha].posi = - tam_pilha;
                p1[i_pilha].tam_vet = 1;
                //printf("Definição: %c\n", p1[i_pilha].def);
                //printf("Tipo: %c\n", p1[i_pilha].tipo);
                //printf("Tamanho da variavel/vetor: %d\n", p1[i_pilha].tam_vet);
                //printf("Posição: %d\n", p1[i_pilha].posi);

                i_pilha = i_pilha + 1;
                //printf("Pilha: %d elementos / Tamanho %d\n", i_pilha, tam_pilha);
            }

            if(strncmp(line, "vet", 3) == 0){
                r = sscanf(line, "vet v%c%d size ci%d", &var_t[var_cont], &var_i[var_cont], &var_const);
                tam_pilha = tam_pilha + 4*var_const;

                p1[i_pilha].def = 'v';
                p1[i_pilha].tipo = var_t[var_cont];
                p1[i_pilha].numero = var_i[var_cont];
                p1[i_pilha].posi = - tam_pilha;
                p1[i_pilha].tam_vet = var_const;
                //printf("Definição: %c\n", p1[i_pilha].def);
                //printf("Tipo: %c\n", p1[i_pilha].tipo);
                //printf("Tamanho da variavel/vetor: %d\n", p1[i_pilha].tam_vet);
                //printf("Posição: %d\n", p1[i_pilha].posi);

                i_pilha = i_pilha + 1;
                //printf("Pilha: %d elementos / Tamanho %d\n", i_pilha, tam_pilha);
            }

            //printf("Tam pilha: %d\n", i_pilha);

            if(strcmp(line, "enddef") == 0){
                for(i = 0; i < i_pilha; i++)
                {
                    printf("    # %c%c%d: %d\n", p1[i].def, p1[i].tipo, p1[i].numero, p1[i].posi);
                    fprintf(arq, "    # %c%c%d: %d\n", p1[i].def, p1[i].tipo, p1[i].numero, p1[i].posi);
                }

                while(tam_pilha % 16 != 0)
                    tam_pilha = tam_pilha + 4;

                if(tam_pilha > 0){
                    printf("    subq $%d, %%rsp\n", tam_pilha);
                    fprintf(arq, "    subq $%d, %%rsp\n", tam_pilha);
                }
                
                key = 0;
                var_cont = 0;

                continue;
            }

            var_cont++;

            continue;
        }        

        //CORPO DA FUNÇÃO
        //ATRIBUIÇÃO
        r = sscanf(line, "vi%d = %ci%d %c %ci%d", &atr_i0, &atr_top1, &atr_iop1, &atr_opa, &atr_top2, &atr_iop2);

        if(r == 3){
            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].numero == atr_i0 && p1[i].def == 'v'){
                    atr_pos0 = p1[i].posi;
                }

                if(atr_top1 != 'p'){
                    if(atr_top1 == 'c'){
                        atr_poso1 = 1;
                    }
                    if(p1[i].numero == atr_iop1 && p1[i].def == atr_top1){
                        atr_poso1 = p1[i].posi;
                    }
                } else
                    atr_poso1 = 1;
            }

            if(atr_top1 != 'p'){
                if(atr_poso1 != 1){
                    printf("    movl %d(%%rbp), %%ecx\n    movl %%ecx, %d(%%rbp)\n", atr_poso1, atr_pos0);
                    fprintf(arq, "    movl %d(%%rbp), %%ecx\n    movl %%ecx, %d(%%rbp)\n", atr_poso1, atr_pos0); 
                } else {
                    printf("    movl $%d, %d(%%rbp)\n", atr_iop1, atr_pos0);
                    fprintf(arq, "    movl $%d, %d(%%rbp)\n", atr_iop1, atr_pos0);
                }
            } else {
                if (atr_iop1 == 1){
                    printf("    movl %%edi, %%ecx\n    movl %%ecx, %d(%%rbp)\n", atr_pos0);
                    fprintf(arq, "    movl %%edi, %%ecx\n    movl %%ecx, %d(%%rbp)\n", atr_pos0); 
                }

                if (atr_iop1 == 2){
                    printf("    movl %%esi, %%ecx\n    movl %%ecx, %d(%%rbp)\n", atr_pos0);
                    fprintf(arq, "    movl %%esi, %%ecx\n    movl %%ecx, %d(%%rbp)\n", atr_pos0); 
                }

                if (atr_iop1 == 3){
                    printf("    movl %%edx, %%ecx\n    movl %%ecx, %d(%%rbp)\n", atr_pos0);
                    fprintf(arq, "    movl %%edx, %%ecx\n    movl %%ecx, %d(%%rbp)\n", atr_pos0); 
                }
            }

            continue;
        }

        if(r == 6){
            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].numero == atr_i0 && p1[i].def == 'v'){
                    atr_pos0 = p1[i].posi;
                }

                if(atr_top1 != 'p'){
                    if(atr_top1 == 'c'){
                        atr_poso1 = 1;
                    }
                    if(p1[i].numero == atr_iop1 && p1[i].def == atr_top1){
                        atr_poso1 = p1[i].posi;
                    }
                } else
                    atr_poso1 = 2;

                if(atr_top2 != 'p'){
                    if(atr_top2 == 'c'){
                        atr_poso2 = 1;
                    }
                    if(p1[i].numero == atr_iop2 && p1[i].def == atr_top2){
                        atr_poso2 = p1[i].posi;
                    }
                } else
                    atr_poso2 = 2;
            }

            if(atr_poso1 != 1){
                if(atr_top1 != 'p'){
                    printf("    movl %d(%%rbp), %%ecx\n", atr_poso1);
                    fprintf(arq, "    movl %d(%%rbp), %%ecx\n", atr_poso1);
                }else{
                    if (atr_iop1 == 1){
                        printf("    movl %%edi, %%ecx\n");
                        fprintf(arq, "    movl %%edi, %%ecx\n"); 
                    }

                    if (atr_iop1 == 2){
                        printf("    movl %%esi, %%ecx\n");
                        fprintf(arq, "    movl %%esi, %%ecx\n"); 
                    }

                    if (atr_iop1 == 3){
                        printf("    movl %%edc, %%ecx\n");
                        fprintf(arq, "    movl %%edx, %%ecx\n"); 
                    }
                }

                if(atr_opa == '+'){
                    if(atr_top1 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    addl %d(%%rbp), %%ecx\n", atr_poso2);
                            fprintf(arq, "    addl %d(%%rbp), %%ecx\n", atr_poso2);
                        }else{
                            printf("    addl $%d, %%ecx\n", atr_iop2);
                            fprintf(arq, "    addl $%d, %%ecx\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    addl %%edi, %%ecx\n");
                            fprintf(arq, "    addl %%edi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    addl %%esi, %%ecx\n");
                            fprintf(arq, "    addl %%esi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    addl %%edc, %%ecx\n");
                            fprintf(arq, "    addl %%edx, %%ecx\n"); 
                        }
                    }
                }

                if(atr_opa == '-'){
                    if(atr_top1 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    subl %d(%%rbp), %%ecx\n", atr_poso2);
                            fprintf(arq, "    subl %d(%%rbp), %%ecx\n", atr_poso2);
                        }else{
                            printf("    subl $%d, %%ecx\n", atr_iop2);
                            fprintf(arq, "    subl $%d, %%ecx\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    addl %%edi, %%ecx\n");
                            fprintf(arq, "    addl %%edi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    addl %%esi, %%ecx\n");
                            fprintf(arq, "    addl %%esi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    addl %%edc, %%ecx\n");
                            fprintf(arq, "    addl %%edx, %%ecx\n"); 
                        }
                    }
                }

                if(atr_opa == '*'){
                    if(atr_top1 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    imull %d(%%rbp), %%ecx\n", atr_poso2);
                            fprintf(arq, "    imull %d(%%rbp), %%ecx\n", atr_poso2);
                        }else{
                            printf("    imull $%d, %%ecx\n", atr_iop2);
                            fprintf(arq, "    imull $%d, %%ecx\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    addl %%edi, %%ecx\n");
                            fprintf(arq, "    addl %%edi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    addl %%esi, %%ecx\n");
                            fprintf(arq, "    addl %%esi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    addl %%edc, %%ecx\n");
                            fprintf(arq, "    addl %%edx, %%ecx\n"); 
                        }
                    }
                }
            } else {
                printf("    movl $%d, %%ecx\n", atr_iop1);
                fprintf(arq, "    movl $%d, %%ecx\n", atr_iop1);

                if(atr_opa == '+'){
                    if(atr_top1 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    addl %d(%%rbp), %%ecx\n", atr_poso2);
                            fprintf(arq, "    addl %d(%%rbp), %%ecx\n", atr_poso2);
                        }else{
                            printf("    addl $%d, %%ecx\n", atr_iop2);
                            fprintf(arq, "    addl $%d, %%ecx\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    addl %%edi, %%ecx\n");
                            fprintf(arq, "    addl %%edi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    addl %%esi, %%ecx\n");
                            fprintf(arq, "    addl %%esi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    addl %%edc, %%ecx\n");
                            fprintf(arq, "    addl %%edx, %%ecx\n"); 
                        }
                    }
                }

                if(atr_opa == '-'){
                    if(atr_top1 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    subl %d(%%rbp), %%ecx\n", atr_poso2);
                            fprintf(arq, "    subl %d(%%rbp), %%ecx\n", atr_poso2);
                        }else{
                            printf("    subl $%d, %%ecx\n", atr_iop2);
                            fprintf(arq, "    subl $%d, %%ecx\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    addl %%edi, %%ecx\n");
                            fprintf(arq, "    addl %%edi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    addl %%esi, %%ecx\n");
                            fprintf(arq, "    addl %%esi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    addl %%edc, %%ecx\n");
                            fprintf(arq, "    addl %%edx, %%ecx\n"); 
                        }
                    }
                }

                if(atr_opa == '*'){
                    if(atr_top1 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    imull %d(%%rbp), %%ecx\n", atr_poso2);
                            fprintf(arq, "    imull %d(%%rbp), %%ecx\n", atr_poso2);
                        }else{
                            printf("    imull $%d, %%ecx\n", atr_iop2);
                            fprintf(arq, "    imull $%d, %%ecx\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    addl %%edi, %%ecx\n");
                            fprintf(arq, "    addl %%edi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    addl %%esi, %%ecx\n");
                            fprintf(arq, "    addl %%esi, %%ecx\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    addl %%edc, %%ecx\n");
                            fprintf(arq, "    addl %%edx, %%ecx\n"); 
                        }
                    }
                }
            }
                
            printf("    movl %%ecx, %d(%%rbp)\n", atr_pos0);
            fprintf(arq, "    movl %%ecx, %d(%%rbp)\n", atr_pos0);
            continue;   
        }
    }

    fclose(arq);

    return 0;
}
