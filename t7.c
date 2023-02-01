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
    int par_qtd = 0;

    int i = 0;

    int atr_i0; // armazenar o indice da variavel que recebe alguma atribuição
    int atr_iop1, atr_iop2; // armazenar o indice das variaveis q vão realizar uma operação ou armazena o valor da constante
    char atr_top1, atr_top2; // armazenar os tipos das variaveis q vão realizar uma operação
    char atr_opa; // armazena o operador (* / - +) 
    int atr_pos0; // armazena a posição da pilha q a variavel vai receber na operação
    int atr_poso1, atr_poso2; // armazena a posição da pilha das variaveis q vão realizar a operação

    int ch_iv0, ch_ifunc, ch_ip1, ch_ip2, ch_ip3;
    char ch_dp1, ch_tp1, ch_dp2, ch_tp2, ch_dp3, ch_tp3;
    int ch_pos0, ch_pos;

    char ary_ad;
    int ary_ai;
    int ary_const;
    int ary_vi;
    int ary_pos;
    int set_i0, set_i1, set_const;
    char set_def0, set_def1;

    char if_def; // armazena a definição
    int if_pos; // armazena a posição do parametro ou variavel
    int contador_if = 0; // conta quantos if tem

    char ret_def; // armazena a definição
    int ret_pos; // armazena a posição do parametro ou variavel

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
        
        //----------------------------------------------------------------------------------------------------------------------------------

        //DEFINIÇÃO DOS PARAMETROS NA PILHA
        r = sscanf(line, "function f%d p%c%d p%c%d p%c%d", &func_c, &par_t[0], &par_i[0], &par_t[1], &par_i[1], &par_t[2], &par_i[2]);

        //Não tem parametro
        if (r==1){
            i_pilha = 0;
            tam_pilha = 0;
            par_qtd = 0;
        }

        //Tem um unico parametro
        if (r==3){
            i_pilha = 1;
            tam_pilha = 8;
            par_qtd = 1;

            p1[0].def = 'p';
            p1[0].tipo = par_t[0];
            p1[0].numero = par_i[0];
            p1[0].posi = - tam_pilha;
        }

        //Tem dois parametros
        if (r==5){
            i_pilha = 2;
            tam_pilha = 16;
            par_qtd = 2;

            p1[0].def = 'p';
            p1[0].tipo = par_t[0];
            p1[0].numero = par_i[0];
            p1[0].posi = - 8;

            p1[1].def = 'p';
            p1[1].tipo = par_t[1];
            p1[1].numero = par_i[1];
            p1[1].posi = - tam_pilha;
        }

        //Tem tres parametros
        if (r==7){
            i_pilha = 3;
            tam_pilha = 24;
            par_qtd = 3;

            p1[0].def = 'p';
            p1[0].tipo = par_t[0];
            p1[0].numero = par_i[0];
            p1[0].posi = - 8;

            p1[1].def = 'p';
            p1[1].tipo = par_t[1];
            p1[1].numero = par_i[1];
            p1[1].posi = - 16;

            p1[2].def = 'p';
            p1[2].tipo = par_t[2];
            p1[2].numero = par_i[2];
            p1[2].posi = - tam_pilha;
        }

        //CABEÇALHO DAS FUNÇÕES
        if(strncmp(line, "function", 8) == 0){
            printf(".globl f%d\nf%d:\n    pushq %%rbp\n    movq %%rsp, %%rbp\n", func_c, func_c);   
            fprintf(arq, ".globl f%d\nf%d:\n    pushq %%rbp\n    movq %%rsp, %%rbp\n", func_c, func_c);

            continue; // Sua utilização dentro de um laço faz com que a execução da instrução atual (ou bloco de instruções atual) seja interrompida e passa para a próxima iteração do laço.
        }

        //----------------------------------------------------------------------------------------------------------------------------------

        //FINAL DAS FUNÇÕES
        if(strcmp(line, "end") == 0){
            printf("    leave\n    ret\n");
            fprintf(arq, "    leave\n    ret\n");

            continue;
        }

        //----------------------------------------------------------------------------------------------------------------------------------

        //VARIAVEIS LOCAIS
        //caso for def, utilizamos uma variavel auxiliar para ler os parametros até q seja encontrado o enddef
        if(strcmp(line, "def") == 0){
            key = 1;
            continue;
        }

        //leitura dos parametros
        if(key == 1){
            //caso seja variavel inteira normal
            if(strncmp(line, "var", 3) == 0){
                r = sscanf(line, "var v%c%d", &var_t[var_cont], &var_i[var_cont]);
                tam_pilha = tam_pilha + 4;

                p1[i_pilha].def = 'v';
                p1[i_pilha].tipo = var_t[var_cont];
                p1[i_pilha].numero = var_i[var_cont];
                p1[i_pilha].posi = - tam_pilha;
                p1[i_pilha].tam_vet = 1;

                i_pilha = i_pilha + 1;
            }

            //caso seja um vetor
            if(strncmp(line, "vet", 3) == 0){
                r = sscanf(line, "vet v%c%d size ci%d", &var_t[var_cont], &var_i[var_cont], &var_const);
                tam_pilha = tam_pilha + 4*var_const;

                p1[i_pilha].def = 'v';
                p1[i_pilha].tipo = var_t[var_cont];
                p1[i_pilha].numero = var_i[var_cont];
                p1[i_pilha].posi = - tam_pilha;
                p1[i_pilha].tam_vet = var_const;

                i_pilha = i_pilha + 1;
            }

            //caso seja enddef printamos a pilha e fazemos o subq
            if(strcmp(line, "enddef") == 0){
                //passamos pelo vetor pilha printando sua possição
                for(i = 0; i < i_pilha; i++)
                {
                    //se for p (parametro), printamos o registrador q ele está alocado
                    if(p1[i].def != 'p'){
                        printf("    # %c%c%d: %d\n", p1[i].def, p1[i].tipo, p1[i].numero, p1[i].posi);
                        fprintf(arq, "    # %c%c%d: %d\n", p1[i].def, p1[i].tipo, p1[i].numero, p1[i].posi);
                    } else {
                        if(p1[i].numero == 1){
                            printf("    # rdi: %d\n",  p1[i].posi);
                            fprintf(arq, "    # rdi: %d\n", p1[i].posi);
                        }

                        if(p1[i].numero == 2){
                            printf("    # rsi: %d\n",  p1[i].posi);
                            fprintf(arq, "    # rsi: %d\n", p1[i].posi);
                        }

                        if(p1[i].numero == 3){
                            printf("    # rdx: %d\n",  p1[i].posi);
                            fprintf(arq, "    # rdx: %d\n", p1[i].posi);
                        }
                    }
                }

                //ajuste no subq para ser multiplo de 16
                while(tam_pilha % 16 != 0)
                    tam_pilha = tam_pilha + 4;

                //subq com o tamanho da pilha
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

        //----------------------------------------------------------------------------------------------------------------------------------

        //CORPO DA FUNÇÃO
        //ATRIBUIÇÃO
        r = sscanf(line, "vi%d = %ci%d %c %ci%d", &atr_i0, &atr_top1, &atr_iop1, &atr_opa, &atr_top2, &atr_iop2);

        //atribuição simples (o lado esquerdo recebe oq ta no lado direito)
        if(r == 3){
            //Procura a variavel do lado esquerdo de uma atribuição na pilha
            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].tipo == 'i'){

                    if(p1[i].numero == atr_i0 && p1[i].def == 'v'){
                        atr_pos0 = p1[i].posi;
                        break;
                    }
                }
            }

            //Procura a primeira variavel do lado direito da atribuição na pilha
            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].tipo == 'i'){

                    if(atr_top1 != 'p'){
                        if(atr_top1 == 'c'){
                            atr_poso1 = 1;
                            break;
                        }
                        if(p1[i].numero == atr_iop1 && p1[i].def == atr_top1){
                            atr_poso1 = p1[i].posi;
                            break;
                        }
                    } else {
                        atr_poso1 = 2;
                        break;
                    }
                }
            }

            //se for um parametro utilizamos os registradores, caso contrario vamos atribuir o registrador r8d com a posição da variavel na pilha
            if(atr_top1 != 'p'){
                //se for uma constante atribuimos o valor direto ao registrador, caso contrario pegamos a posição na pilha
                if(atr_poso1 != 1){
                    printf("    movl %d(%%rbp), %%r8d\n    movl %%r8d, %d(%%rbp)\n", atr_poso1, atr_pos0);
                    fprintf(arq, "    movl %d(%%rbp), %%r8d\n    movl %%r8d, %d(%%rbp)\n", atr_poso1, atr_pos0); 
                } else {
                    printf("    movl $%d, %d(%%rbp)\n", atr_iop1, atr_pos0);
                    fprintf(arq, "    movl $%d, %d(%%rbp)\n", atr_iop1, atr_pos0);
                }
            } else {
                if (atr_iop1 == 1){
                    printf("    movl %%edi, %%r8d\n    movl %%r8d, %d(%%rbp)\n", atr_pos0);
                    fprintf(arq, "    movl %%edi, %%r8d\n    movl %%r8d, %d(%%rbp)\n", atr_pos0); 
                }

                if (atr_iop1 == 2){
                    printf("    movl %%esi, %%r8d\n    movl %%r8d, %d(%%rbp)\n", atr_pos0);
                    fprintf(arq, "    movl %%esi, %%r8d\n    movl %%r8d, %d(%%rbp)\n", atr_pos0); 
                }

                if (atr_iop1 == 3){
                    printf("    movl %%edx, %%r8d\n    movl %%r8d, %d(%%rbp)\n", atr_pos0);
                    fprintf(arq, "    movl %%edx, %%r8d\n    movl %%r8d, %d(%%rbp)\n", atr_pos0); 
                }
            }

            continue;
        }

        //Atribuição q envolve um operador (+, /, *, -)
        if(r == 6){
            //Procura a variavel do lado esquerdo de uma atribuição na pilha
            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].tipo == 'i'){

                    if(p1[i].numero == atr_i0 && p1[i].def == 'v'){
                        atr_pos0 = p1[i].posi;
                        break;
                    }
                }
            }

            //Procura a primeira variavel do lado direito da atribuição na pilha
            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].tipo == 'i'){

                    if(atr_top1 != 'p'){

                        if(atr_top1 == 'c'){
                            atr_poso1 = 1;
                            break;
                        }
                        if(p1[i].numero == atr_iop1 && p1[i].def == atr_top1){
                            atr_poso1 = p1[i].posi;
                            break;
                        }
                    } else {
                        atr_poso1 = 2;
                        break;
                    }
                }
            }

            //Procura a segunda variavel do lado direito da atribuição na pilha
            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].tipo == 'i'){

                    if(atr_top2 != 'p'){

                        if(atr_top2 == 'c'){
                            atr_poso2 = 1;
                            break;
                        }
                        if(p1[i].numero == atr_iop2 && p1[i].def == atr_top2){
                            atr_poso2 = p1[i].posi;
                            break;
                        }
                    } else {
                        atr_poso2 = 2;
                        break;
                    }
                }
            }

            //se o primeiro operador da direita for uma variavel, entramos nesse if. Caso ele seja uma constante, utilizamos  o else. //Dava para reduzir e muito o codigo aqui mas sem tempo irmão kkkkkk
            if(atr_poso1 != 1){
                //se for um parametro utilizamos os registradores
                if(atr_top1 != 'p'){
                    printf("    movl %d(%%rbp), %%r8d\n", atr_poso1);
                    fprintf(arq, "    movl %d(%%rbp), %%r8d\n", atr_poso1);
                }else{
                    if (atr_iop1 == 1){
                        printf("    movl %%edi, %%r8d\n");
                        fprintf(arq, "    movl %%edi, %%r8d\n"); 
                    }

                    if (atr_iop1 == 2){
                        printf("    movl %%esi, %%r8d\n");
                        fprintf(arq, "    movl %%esi, %%r8d\n"); 
                    }

                    if (atr_iop1 == 3){
                        printf("    movl %%edc, %%r8d\n");
                        fprintf(arq, "    movl %%edx, %%r8d\n"); 
                    }
                }

                //adição
                if(atr_opa == '+'){
                    if(atr_top2 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    addl %d(%%rbp), %%r8d\n", atr_poso2);
                            fprintf(arq, "    addl %d(%%rbp), %%r8d\n", atr_poso2);
                        }else{
                            printf("    addl $%d, %%r8d\n", atr_iop2);
                            fprintf(arq, "    addl $%d, %%r8d\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    addl %%edi, %%r8d\n");
                            fprintf(arq, "    addl %%edi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    addl %%esi, %%r8d\n");
                            fprintf(arq, "    addl %%esi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    addl %%edc, %%r8d\n");
                            fprintf(arq, "    addl %%edx, %%r8d\n"); 
                        }
                    }
                }

                //subtração
                if(atr_opa == '-'){
                    if(atr_top2 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    subl %d(%%rbp), %%r8d\n", atr_poso2);
                            fprintf(arq, "    subl %d(%%rbp), %%r8d\n", atr_poso2);
                        }else{
                            printf("    subl $%d, %%r8d\n", atr_iop2);
                            fprintf(arq, "    subl $%d, %%r8d\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    subl %%edi, %%r8d\n");
                            fprintf(arq, "    subl %%edi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    subl %%esi, %%r8d\n");
                            fprintf(arq, "    subl %%esi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    subl %%edc, %%r8d\n");
                            fprintf(arq, "    subl %%edx, %%r8d\n"); 
                        }
                    }
                }

                //multiplicação
                if(atr_opa == '*'){
                    if(atr_top2 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    imull %d(%%rbp), %%r8d\n", atr_poso2);
                            fprintf(arq, "    imull %d(%%rbp), %%r8d\n", atr_poso2);
                        }else{
                            printf("    imull $%d, %%r8d\n", atr_iop2);
                            fprintf(arq, "    imull $%d, %%r8d\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    imull %%edi, %%r8d\n");
                            fprintf(arq, "    imull %%edi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    imull %%esi, %%r8d\n");
                            fprintf(arq, "    imull %%esi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    imull %%edc, %%r8d\n");
                            fprintf(arq, "    imull %%edx, %%r8d\n"); 
                        }
                    }
                }

                //divisão
                if(atr_opa == '/'){
                    if(atr_top2 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    movl %d(%%rbp), %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n", atr_poso2);
                            fprintf(arq, "    movl %d(%%rbp), %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n", atr_poso2);
                        }else{
                            printf("    movl $%d, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n", atr_iop2);
                            fprintf(arq, "    movl $%d, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    movl %%edi, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                            fprintf(arq, "    movl %%edi, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                        }

                        if (atr_iop2 == 2){
                            printf("    movl %%esi, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                            fprintf(arq, "    movl %%esi, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                        }

                        if (atr_iop2 == 3){
                            printf("    movl %%edx, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                            fprintf(arq, "    movl %%edx, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                        }
                    }
                }
            } else {
                printf("    movl $%d, %%r8d\n", atr_iop1);
                fprintf(arq, "    movl $%d, %%r8d\n", atr_iop1);

                if(atr_opa == '+'){
                    if(atr_top2 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    addl %d(%%rbp), %%r8d\n", atr_poso2);
                            fprintf(arq, "    addl %d(%%rbp), %%r8d\n", atr_poso2);
                        }else{
                            printf("    addl $%d, %%r8d\n", atr_iop2);
                            fprintf(arq, "    addl $%d, %%r8d\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    addl %%edi, %%r8d\n");
                            fprintf(arq, "    addl %%edi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    addl %%esi, %%r8d\n");
                            fprintf(arq, "    addl %%esi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    addl %%edc, %%r8d\n");
                            fprintf(arq, "    addl %%edx, %%r8d\n"); 
                        }
                    }
                }

                if(atr_opa == '-'){
                    if(atr_top2 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    subl %d(%%rbp), %%r8d\n", atr_poso2);
                            fprintf(arq, "    subl %d(%%rbp), %%r8d\n", atr_poso2);
                        }else{
                            printf("    subl $%d, %%r8d\n", atr_iop2);
                            fprintf(arq, "    subl $%d, %%r8d\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    subl %%edi, %%r8d\n");
                            fprintf(arq, "    addl %%edi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    subl %%esi, %%r8d\n");
                            fprintf(arq, "    addl %%esi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    subl %%edc, %%r8d\n");
                            fprintf(arq, "    subl %%edx, %%r8d\n"); 
                        }
                    }
                }

                if(atr_opa == '*'){
                    if(atr_top2 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    imull %d(%%rbp), %%r8d\n", atr_poso2);
                            fprintf(arq, "    imull %d(%%rbp), %%r8d\n", atr_poso2);
                        }else{
                            printf("    imull $%d, %%r8d\n", atr_iop2);
                            fprintf(arq, "    imull $%d, %%r8d\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    imull %%edi, %%r8d\n");
                            fprintf(arq, "    imull %%edi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 2){
                            printf("    imull %%esi, %%r8d\n");
                            fprintf(arq, "    imull %%esi, %%r8d\n"); 
                        }

                        if (atr_iop2 == 3){
                            printf("    imull %%edc, %%r8d\n");
                            fprintf(arq, "    imull %%edx, %%r8d\n"); 
                        }
                    }
                }

                if(atr_opa == '/'){
                    if(atr_top2 != 'p'){
                        if(atr_poso2 != 1){
                            printf("    movl %d(%%rbp), %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n", atr_poso2);
                            fprintf(arq, "    movl %d(%%rbp), %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n", atr_poso2);
                        }else{
                            printf("    movl $%d, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n", atr_iop2);
                            fprintf(arq, "    movl $%d, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n", atr_iop2);
                        }
                    } else {
                        if (atr_iop2 == 1){
                            printf("    movl %%edi, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                            fprintf(arq, "    movl %%edi, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                        }

                        if (atr_iop2 == 2){
                            printf("    movl %%esi, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                            fprintf(arq, "    movl %%esi, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                        }

                        if (atr_iop2 == 3){
                            printf("    movl %%edx, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                            fprintf(arq, "    movl %%edx, %%ecx\n    cltd\n    idivl %%ecx\n    movl %%eax, %%r8d\n");
                        }
                    }
                }
            }
                
            printf("    movl %%r8d, %d(%%rbp)\n", atr_pos0);
            fprintf(arq, "    movl %%r8d, %d(%%rbp)\n", atr_pos0);
            continue;   
        }

        //----------------------------------------------------------------------------------------------------------------------------------

        //CHAMADA DE FUNÇÃO
        r = sscanf(line, "vi%d = call f%d %c%c%d %c%c%d %c%c%d", &ch_iv0, &ch_ifunc, &ch_dp1, &ch_tp1, &ch_ip1, &ch_dp2, &ch_tp2, &ch_ip2, &ch_dp3, &ch_tp3, &ch_ip3);
        if(r == 5){
            if(par_qtd == 1){
                printf("    movq %%rdi, -8(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n");
            }

            if(par_qtd == 2){
                printf("    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n");
            }

            if(par_qtd == 3){
                printf("    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n    movq %%rdx, -24(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n    movq %%rdx, -24(%%rbp)\n");
            }

            if(ch_dp1 != 'c'){
                if(ch_dp1 != 'p'){
                    for (i = 0; i < i_pilha; i++)
                    {
                        if(p1[i].def == ch_dp1 && p1[i].tipo == ch_tp1 && p1[i].numero == ch_ip1){
                            ch_pos = p1[i].posi;
                            break;
                        }
                    }
                    if(ch_tp1 == 'a'){
                        printf("    leaq %d(%%rbp), %%rdi\n", ch_pos);
                        fprintf(arq, "    leaq %d(%%rbp), %%rdi\n", ch_pos);
                    } else {
                        printf("    movq %d(%%rbp), %%rdi\n", ch_pos);
                        fprintf(arq, "    movq %d(%%rbp), %%rdi\n", ch_pos);
                    }
                } else {
                    if (ch_ip1 == 2){
                        printf("    movq %%rsi, %%rdi\n");
                        fprintf(arq, "    movq %%rsi, %%rdi\n");
                    }

                    if (ch_ip1 == 3){
                        printf("    movq %%rdx, %%rdi\n");
                        fprintf(arq, "    movq %%rdx, %%rdi\n");
                    }
                }
            } else {   
                printf("    movl $%d, %%edi\n ", ch_ip1);
                fprintf(arq, "    movl $%d, %%edi\n", ch_ip1);
            }

            printf("    movl $0, %%eax\n    call f%d\n", ch_ifunc);
            fprintf(arq, "    movl $0, %%eax\n    call f%d\n", ch_ifunc);

            if(par_qtd == 1){
                printf("    movq -8(%%rbp), %%rdi\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n");
            }

            if(par_qtd == 2){
                printf("    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n");
            }

            if(par_qtd == 3){
                printf("    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n    movq -24(%%rbp), %%rdx\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n    movq -24(%%rbp), %%rdx\n");
            }

            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].def == 'v' && p1[i].tipo == 'i' && p1[i].numero == ch_iv0){
                    ch_pos0 = p1[i].posi;
                    break;
                }
            }

            printf("    movl %%eax, %d(%%rbp)\n", ch_pos0);
            fprintf(arq, "    movl %%eax, %d(%%rbp)\n", ch_pos0);

            continue;
        }
	    
        if(r == 8){
            if(par_qtd == 1){
                printf("    movq %%rdi, -8(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n");
            }

            if(par_qtd == 2){
                printf("    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n");
            }

            if(par_qtd == 3){
                printf("    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n    movq %%rdx, -24(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n    movq %%rdx, -24(%%rbp)\n");
            }



            if(ch_dp2 != 'c'){
                if(ch_dp2 != 'p'){
                    for (i = 0; i < i_pilha; i++)
                    {
                        if(p1[i].def == ch_dp2 && p1[i].tipo == ch_tp2 && p1[i].numero == ch_ip2){
                            ch_pos = p1[i].posi;
                            break;
                        }
                    }
                    if(ch_tp2 == 'a'){
                        printf("    leaq %d(%%rbp), %%rsi\n", ch_pos);
                        fprintf(arq, "    leaq %d(%%rbp), %%rsi\n", ch_pos);
                    } else {
                        printf("    movq %d(%%rbp), %%rsi\n", ch_pos);
                        fprintf(arq, "    movq %d(%%rbp), %%rsi\n", ch_pos);
                    }
                } else {
                    if (ch_ip2 == 1){
                        printf("    movq %%rdi, %%rsi\n");
                        fprintf(arq, "    movq %%rdi, %%rsi\n");
                    }

                    if (ch_ip2 == 3){
                        printf("    movq %%rdx, %%rsi\n");
                        fprintf(arq, "    movq %%rdx, %%rsi\n");
                    }
                }
            } else {   
                printf("    movl $%d, %%esi\n ", ch_ip2);
                fprintf(arq, "    movl $%d, %%esi\n", ch_ip2);
            }



            if(ch_dp1 != 'c'){
                if(ch_dp1 != 'p'){
                    for (i = 0; i < i_pilha; i++)
                    {
                        if(p1[i].def == ch_dp1 && p1[i].tipo == ch_tp1 && p1[i].numero == ch_ip1){
                            ch_pos = p1[i].posi;
                            break;
                        }
                    }
                    if(ch_tp1 == 'a'){
                        printf("    leaq %d(%%rbp), %%rdi\n", ch_pos);
                        fprintf(arq, "    leaq %d(%%rbp), %%rdi\n", ch_pos);
                    } else {
                        printf("    movq %d(%%rbp), %%rdi\n", ch_pos);
                        fprintf(arq, "    movq %d(%%rbp), %%rdi\n", ch_pos);
                    }
                } else {
                    if (ch_ip1 == 2){
                        printf("    movq %%rsi, %%rdi\n");
                        fprintf(arq, "    movq %%rsi, %%rdi\n");
                    }

                    if (ch_ip1 == 3){
                        printf("    movq %%rdx, %%rdi\n");
                        fprintf(arq, "    movq %%rdx, %%rdi\n");
                    }
                }
            } else {   
                printf("    movl $%d, %%edi\n ", ch_ip1);
                fprintf(arq, "    movl $%d, %%edi\n", ch_ip1);
            }



            printf("    movl $0, %%eax\n    call f%d\n", ch_ifunc);
            fprintf(arq, "    movl $0, %%eax\n    call f%d\n", ch_ifunc);

            if(par_qtd == 1){
                printf("    movq -8(%%rbp), %%rdi\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n");
            }

            if(par_qtd == 2){
                printf("    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n");
            }

            if(par_qtd == 3){
                printf("    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n    movq -24(%%rbp), %%rdx\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n    movq -24(%%rbp), %%rdx\n");
            }

            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].def == 'v' && p1[i].tipo == 'i' && p1[i].numero == ch_iv0){
                    ch_pos0 = p1[i].posi;
                    break;
                }
            }

            printf("    movl %%eax, %d(%%rbp)\n", ch_pos0);
            fprintf(arq, "    movl %%eax, %d(%%rbp)\n", ch_pos0);

            continue;
        }
	    
        if(r == 11){
            if(par_qtd == 1){
                printf("    movq %%rdi, -8(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n");
            }

            if(par_qtd == 2){
                printf("    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n");
            }

            if(par_qtd == 3){
                printf("    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n    movq %%rdx, -24(%%rbp)\n");
                fprintf(arq, "    movq %%rdi, -8(%%rbp)\n    movq %%rsi, -16(%%rbp)\n    movq %%rdx, -24(%%rbp)\n");
            }



            if(ch_dp1 != 'c'){
                for (i = 0; i < i_pilha; i++)
                {
                    if(p1[i].def == ch_dp1 && p1[i].tipo == ch_tp1 && p1[i].numero == ch_ip1){
                        ch_pos = p1[i].posi;
                        break;
                    }
                }
                if(ch_tp1 == 'a'){
                    printf("    leaq %d(%%rbp), %%rdi\n", ch_pos);
                    fprintf(arq, "    leaq %d(%%rbp), %%rdi\n", ch_pos);
                } else {
                    printf("    movq %d(%%rbp), %%rdi\n", ch_pos);
                    fprintf(arq, "    movq %d(%%rbp), %%rdi\n", ch_pos);
                }
            } else {   
                printf("    movl $%d, %%edi\n ", ch_ip1);
                fprintf(arq, "    movl $%d, %%edi\n", ch_ip1);
            }



            if(ch_dp2 != 'c'){
                for (i = 0; i < i_pilha; i++)
                {
                    if(p1[i].def == ch_dp2 && p1[i].tipo == ch_tp2 && p1[i].numero == ch_ip2){
                        ch_pos = p1[i].posi;
                        break;
                    }
                }
                if(ch_tp2 == 'a'){
                    printf("    leaq %d(%%rbp), %%rsi\n", ch_pos);
                    fprintf(arq, "    leaq %d(%%rbp), %%rsi\n", ch_pos);
                } else {
                    printf("    movq %d(%%rbp), %%rsi\n", ch_pos);
                    fprintf(arq, "    movq %d(%%rbp), %%rsi\n", ch_pos);
                }
            } else {   
                printf("    movl $%d, %%esi\n ", ch_ip2);
                fprintf(arq, "    movl $%d, %%esi\n", ch_ip2);
            }



            if(ch_dp3 != 'c'){
                for (i = 0; i < i_pilha; i++)
                {
                    if(p1[i].def == ch_dp3 && p1[i].tipo == ch_tp3 && p1[i].numero == ch_ip3){
                        ch_pos = p1[i].posi;
                        break;
                    }
                }
                if(ch_tp3 == 'a'){
                    printf("    leaq %d(%%rbp), %%rdx\n", ch_pos);
                    fprintf(arq, "    leaq %d(%%rbp), %%rdx\n", ch_pos);
                } else {
                    printf("    movq %d(%%rbp), %%rdx\n", ch_pos);
                    fprintf(arq, "    movq %d(%%rbp), %%rdx\n", ch_pos);
                }
            } else {   
                printf("    movl $%d, %%edx\n ", ch_ip3);
                fprintf(arq, "    movl $%d, %%edx\n", ch_ip3);
            }



            printf("    movl $0, %%eax\n    call f%d\n", ch_ifunc);
            fprintf(arq, "    movl $0, %%eax\n    call f%d\n", ch_ifunc);

            if(par_qtd == 1){
                printf("    movq -8(%%rbp), %%rdi\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n");
            }

            if(par_qtd == 2){
                printf("    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n");
            }

            if(par_qtd == 3){
                printf("    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n    movq -24(%%rbp), %%rdx\n");
                fprintf(arq, "    movq -8(%%rbp), %%rdi\n    movq -16(%%rbp), %%rsi\n    movq -24(%%rbp), %%rdx\n");
            }

            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].def == 'v' && p1[i].tipo == 'i' && p1[i].numero == ch_iv0){
                    ch_pos0 = p1[i].posi;
                    break;
                }
            }

            printf("    movl %%eax, %d(%%rbp)\n", ch_pos0);
            fprintf(arq, "    movl %%eax, %d(%%rbp)\n", ch_pos0);

            continue;
        }

        //------------------------------------------------------------------------------------------------------------------------
        //ACESSO AO ARRAY
        //get
        r = sscanf(line, "get %ca%d index ci%d to vi%d", &ary_ad, &ary_ai, &ary_const, &ary_vi);
        if(r == 4){
            if(ary_ad != 'p'){
                for (i = 0; i < i_pilha; i++)
                {
                    if(p1[i].def == 'v' && p1[i].tipo == 'a' && p1[i].numero == ary_ai){
                        ary_pos = p1[i].posi;
                        break;
                    }
                }

                printf("    leaq %d(%%rbp), %%r9\n", ary_pos);
                fprintf(arq, "    leaq %d(%%rbp), %%r9\n", ary_pos);
            } else {
                if(ary_ai == 1){
                    printf("    movq %%rdi, %%r9\n");
                    fprintf(arq, "    movq %%rdi, %%r9\n");
                }

                if(ary_ai == 2){
                    printf("    movq %%rsi, %%r9\n");
                    fprintf(arq, "    movq %%rsi, %%r9\n");
                }

                if(ary_ai == 3){
                    printf("    movq %%rdx, %%r9\n");
                    fprintf(arq, "    movq %%rdx, %%r9\n");    
                }
            }

            printf("    movabs $%d, %%r10\n    imulq $4, %%r10\n    addq %%r9d, %%r10\n    movl(%%r10), %%r11d\n", ary_const);
            fprintf(arq, "    movabs $%d, %%r10\n    imulq $4, %%r10\n    addq %%r9d, %%r10\n    movl(%%r10), %%r11d\n", ary_const);
            
            for (i = 0; i < i_pilha; i++)
            {
                if(p1[i].def == 'v' && p1[i].tipo == 'i' && p1[i].numero == ary_vi){
                    ary_pos = p1[i].posi;
                    break;
                }
            }

            printf("    movl %%r11d, %d(%%rbp)\n", ary_pos);
            fprintf(arq, "    movl %%r11d, %d(%%rbp)\n", ary_pos);

            continue;
        }


        //set
        r = sscanf(line, "set %ca%d index ci%d with %ci%d", &set_def0, &set_i0, &set_const, &set_def1, &set_i1);

        if (r == 5) {
			//Operaçao com vetor//
			if(set_def0 == 'v') {
                set_i0--;
				printf("    leaq %d(%%rbp), %%r8\n", p1[set_i0].posi);
                fprintf(arq, "    leaq %d(%%rbp), %%r8\n", p1[set_i0].posi);
				if(set_def1 == 'c'){
					printf("    movq $%d, (%%r8)\n", set_i1);
                    fprintf(arq, "    movq $%d, (%%r8)\n", set_i1);
				}else {
					printf("    movq %d(%%rbp), %%r10\n", p1[set_i0].posi);
                    fprintf(arq, "    movq %d(%%rbp), %%r10\n", p1[set_i0].posi);
					printf("    movq %%r10, (%%r8)\n");
                    fprintf(arq, "   movq %%r10, (%%r8)\n");
				}
                continue;
			}
			//Operação com parametro//
			else if(set_def0 == 'p') {
				set_i0--;
				printf("    movq %d(%%rbp), %%r8\n", p1[set_i0].posi);
                fprintf(arq, "    movq %d(%%rbp), %%r8\n", p1[set_i0].posi);
				printf("    movabs $%d, %%r9\n", set_const);
                fprintf(arq, "    movabs $%d, %%r9\n", set_const);
				printf("    imulq $4, %%r9\n");
                fprintf(arq, "    imulq $4, %%r9\n");
				printf("    addq %%r9, %%r8\n");
                fprintf(arq, "    addq %%r9, %%r8\n");
				if(set_def1 == 'c') {
					printf("    movl $%d, (%%r8)\n", set_i1);
                    fprintf(arq, "    movl $%d, (%%r8)\n", set_i1);
				}else {
					printf("    movq %d(%%rbp), %%r10d\n", p1[set_i0].posi);
                    fprintf(arq, "    movq %d(%%rbp), %%r10d\n", p1[set_i0].posi);
					printf("    movq %%r10d, (%%r8)\n");
                    fprintf(arq, "    movq %%r10d, (%%r8)\n");
				}
                continue;
			}
			printf("\n");
		}

        //CONDICIONAL

        //if com constante
        r = sscanf(line, "if ci%d", &var_const);
        if(r == 1){
            printf("    cmpl $0, $%d\n", var_const);
            fprintf(arq, "    cmpl $0, $%d\n", var_const);	
            printf("    je end_if%d\n", contador_if);
            fprintf(arq, "    je end_if%d\n", contador_if);
            continue;
	    }

        //if com variavel ou parametro
        r = sscanf(line, "if %ci%d", &if_def, &if_pos);
        if (r == 2) {	
        	if_pos--;
		    printf("    cmpl $0, %d(%%rbp)\n", p1[if_pos].posi);
            fprintf(arq, "    cmpl $0, %d(%%rbp)\n", p1[if_pos].posi);
		    printf("    je end_if%d\n", contador_if);
            fprintf(arq, "    je end_if%d\n", contador_if);
		    continue;
	    }   

        //endif
        if (strncmp(line, "endif", 5) == 0) {
            printf("end_if%d:\n", contador_if);
            fprintf(arq, "end_if%d:\n", contador_if);
            contador_if++;
            continue;
        }

        //RETORNO
        //retorno de constante
        r = sscanf(line, "return ci%d", &var_const);
        if(r == 1){
		    printf("    movl $%d, %%eax\n", var_const);
            fprintf(arq, "    movl $%d, %%eax\n", var_const);
		    continue;
	    }
        
        //retorno parametro e variavel
        r = sscanf(line, "return %ci%d", &ret_def, &ret_pos);
        if(r == 2){
            if(ret_def == 'v'){
                ret_pos--;
                printf("    movl %d(%%rbp), %%eax\n", p1[ret_pos].posi);
                fprintf(arq, "    movl %d(%%rbp), %%eax\n", p1[ret_pos].posi);
            } else{
                if(ret_pos == 1){
                    printf("    movl %%edi, %%eax\n");
                    fprintf(arq, "    movl %%edi, %%eax\n");
                }
                else if(ret_pos == 2){
                    printf("    movl %%esi, %%eax\n");
                    fprintf(arq, "    movl %%esi, %%eax\n");
                }
                else if(ret_pos == 3){
                    printf("    movl %%edx, %%eax\n");
                    fprintf(arq, "  movl %%edx, %%eax\n");
                }
            }
        }
    }

    fclose(arq);

    return 0;
}