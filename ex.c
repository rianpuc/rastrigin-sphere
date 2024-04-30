#include <stdio.h> // Header necessario pra tudo
#include <stdlib.h> // Header para o malloc/realloc etc.
#include <math.h> // Header para operacoes matematicas
#define PI 3.14159265358979323846 // Definindo o valor de PI

int last_pos = 0; // Declaracao e inicializacao da variavel global para rastrear a posicao do array
double rmsd = 0; // Declaracao e inicializacao da variavel global para pegarmos a raiz quadrada do desvio quadratico medio

int main(); // Declaracao do prototipo da funcao main

void precision(float a, float b){ // Funcao precision para comecar a calcular o rmsd
    rmsd += pow((a - b), 2);  // Pegando a diferenca do valor esperado com o valor que o algoritmo conseguiu e elevando ao quadrado
}

float calc_rastrigin(float *arr, int n){ // Funcao para calcular o rastrigin
    float soma = 0; // Declarando e inicializando a variavel pro somatorio
    printf("Dados passados: "); // Mostrando no terminal os dados que estao sendo lidos do arquivo
    for(int i=0; i<n; i++){ // ..
        printf("%f ", arr[i]); // ..
        soma += pow(arr[i],2) - 10*cos(2*PI*(arr[i])); // Primeira parte do calculo de rastrigin
    }
    return 10*n+soma; // Retornando de fato o resultado de rastrigin
}

float calc_sphere(float *arr, int n){ // Funcao para calcular a sphere
    float soma = 0; // Declarando e inicializando a variavel pro somatorio
    printf("Dados passados: "); // Mostrando no terminal os dados que estao sendo lidos do arquivo
    for(int i=0; i<n; i++){ // ..
        printf("%f ", arr[i]); // ..
        soma += pow(arr[i], 2); // Calculo da sphere
    }
    return soma; // Retornando o resultado
}

int rastrigin(){ // Funcao para dar inicio ao rastrigin
    FILE *arquivo; // Declaracao do arquivo 
    float *numeros, *esperados; // Declaracao dos ponteiros para guardarmos os numeros lidos e os resultados esperados
    float numero, numero_esperado; // Declaracao das variaveis do numero lido e o resultado esperado
    int pos; // Declaracao da variavel pos para rastrear onde esta no array
    size_t tamanho_rastrigin = 0; size_t esperados_rastrigin = 0; // Declaracao e inicializacao dos tamanhos dos arrays de numeros lidos e resultados esperados
    size_t capacidade_n = 10; // Capacidade inicial do array dos numeros lidos
    size_t capacidade_e = 10; // Capacidade inicial do array dos resultados esperados
    arquivo = fopen("dados_rastrigin.txt", "r"); // Abrindo o arquivo de dados no modo leitura
    if (arquivo == NULL) { // Caso nao consiga abrir o arquivo o if ira dar uma mensagem de erro e fechar o programa
        printf("Erro ao abrir o arquivo.\n");
        return 1;
    }

    // Alocando a memoria inicial pros arrays
    numeros = (float *)malloc(capacidade_n * sizeof(float));
    esperados = (float *)malloc(capacidade_e * sizeof(float));

    if (numeros == NULL || esperados == NULL) { // Caso nao consiga alocar memoria pros arrays o if retornara um erro e fechara o programa
        printf("Erro ao alocar memória.\n"); // ..
        fclose(arquivo); // Fechando o arquivo que estava aberto
        free(esperados); // Liberando o espaco da memoria
        free(numeros); // Liberando o espaco da memoria
        return 1;
    }
    

    // Aqui comeca a graca
    while (fscanf(arquivo, "%f", &numero) == 1) { // Loop while para ler todos os FLOATS dentro do arquivo, vai ler ate achar um numero que nao seja float
        // If para verificar se precisa de mais memoria para alocar as variaveis
        if (tamanho_rastrigin >= capacidade_n) {
            capacidade_n *= 2; // Dobrando a capacidade atual
            float *temp = (float *)realloc(numeros, capacidade_n * sizeof(float)); // Inicializando e declarando a variavel temp para a expansao da memoria
            if (temp == NULL) { // Caso nao consiga alocar memoria pro array o if retornara um erro e fechara o programa
                printf("Erro ao realocar memória.\n"); // ..
                free(numeros); // Liberando o espaco da memoria
                fclose(arquivo); // Fechando o arquivo aberto
                return 1;
            }
            numeros = temp; // Caso tudo tenha dado certo o array sera expandido
        }
        if(numero>5.12){ // Breakpoint que eu pensei para organizar os numeros esperados e comecar os calculos
            if (esperados_rastrigin >= capacidade_e) { // Verificando se precisa de mais memoria para alocar as variaveis esperadas
                capacidade_e *= 2; // Dobrando a capacidade atual
                float *temp = (float *)realloc(esperados, capacidade_e * sizeof(float)); // Inicializando e declarando a variavel temp para a expansao da memoria
                if (temp == NULL) { // Caso nao consiga alocar memoria pro array o if retornara um erro e fechara o programa
                    printf("Erro ao realocar memória.\n"); // ..
                    free(esperados); // Liberando o espaco da memoria
                    fclose(arquivo); // Fechando o arquivo aberto
                    return 1;
                }
                esperados = temp; // Caso tudo tenha dado certo o array sera expandido
            }
            pos = tamanho_rastrigin; // Salvando a posicao em que estamos no array
            float soma = calc_rastrigin(numeros+last_pos, tamanho_rastrigin-last_pos); // Chamando a funcao para calcular o rastrigin, e guardando-a em "soma"
            printf("\nValor esperado %f | Valor encontrado %f\n", numero, soma); // Mostrando ao usuario o dado esperado (que pegamos agora) e comparando com o que encontramos no calculo
            precision(numero, soma); // Enviando o numero esperado junto com o nosso calculo para dar inicio ao calculo da precisao
            last_pos = tamanho_rastrigin; // Salvando a posicao em que estamos para utilizar mais tarde
            esperados[esperados_rastrigin] = numero; // Colocando o resultado esperado (lido no arquivo) no array de resultados esperados
            esperados_rastrigin++; // Achamos um resultado esperado entao incrementamos 1 a contagem
        } else { // Caso nao seja maior que 5,12
            numeros[tamanho_rastrigin] = numero; // Vai salvar o numero no array dos numeros lidos do arquivo, ou seja, nao eh um resultado esperado 
            tamanho_rastrigin++; // Incrementando 1 a contagem de numeros lidos
        }
    }

    printf("Precisao: %g | %.15f\n", sqrt((rmsd/esperados_rastrigin)), sqrt((rmsd/esperados_rastrigin))); // Imprimindo o calculo final da precisao RMSD, em notacao cientifica e decimal com 15 casas.
    fclose(arquivo); // Fechando o arquivo
    free(numeros); // Liberando a memoria
    free(esperados); // Liberando a memoria
    main(); // Voltando pra funcao main
}

int sphere(){ // Funcao para dar inicio a sphere
    FILE *arquivo; // Declaracao do arquivo 
    float *numeros, *esperados; // Declaracao dos ponteiros para guardarmos os numeros lidos e os resultados esperados
    float numero, numero_esperado; // Declaracao das variaveis do numero lido e o resultado esperado
    int pos; // Declaracao da variavel pos para rastrear onde esta no array
    size_t tamanho_sphere = 0; size_t esperados_sphere = 0; // Declaracao e inicializacao dos tamanhos dos arrays de numeros lidos e resultados esperados
    size_t capacidade_n = 10; // Capacidade inicial do array dos numeros lidos
    size_t capacidade_e = 10; // Capacidade inicial do array dos resultados esperados
    arquivo = fopen("dados_sphere.txt", "r"); // Abrindo o arquivo de dados no modo leitura
    if (arquivo == NULL) { // Caso nao consiga abrir o arquivo o if ira dar uma mensagem de erro e fechar o programa
        printf("Erro ao abrir o arquivo.\n");
        return 1; 
    }

    // Alocando a memoria inicial pros arrays
    numeros = (float *)malloc(capacidade_n * sizeof(float));
    esperados = (float *)malloc(capacidade_e * sizeof(float));

    if (numeros == NULL || esperados == NULL) { // Caso nao consiga alocar memoria pros arrays o if retornara um erro e fechara o programa
        printf("Erro ao alocar memória.\n"); // ..
        fclose(arquivo); // Fechando o arquivo que estava aberto
        free(esperados); // Liberando o espaco da memoria
        free(numeros); // Liberando o espaco da memoria
        return 1;
    }
    

    // Here we go again
    while (fscanf(arquivo, "%f", &numero) == 1) { // Loop while para ler todos os FLOATS dentro do arquivo, vai ler ate achar um numero que nao seja float
        // If para verificar se precisa de mais memoria para alocar as variaveis
        if (tamanho_sphere >= capacidade_n) {
            capacidade_n *= 2; // Dobrando a capacidade atual
            float *temp = (float *)realloc(numeros, capacidade_n * sizeof(float)); // Inicializando e declarando a variavel temp para a expansao da memoria
            if (temp == NULL) { // Caso nao consiga alocar memoria pro array o if retornara um erro e fechara o programa
                printf("Erro ao realocar memória.\n"); // ..
                free(numeros); // Liberando o espaco da memoria
                fclose(arquivo); // Fechando o arquivo aberto
                return 1;
            }
            numeros = temp; // Caso tudo tenha dado certo o array sera expandido
        }
        if(numero>100){ // Mesma logica do breakpoint do rastrigin
            if (esperados_sphere >= capacidade_e) { // Verificando se precisa de mais memoria para alocar as variaveis esperadas
                capacidade_e *= 2; // Dobrando a capacidade atual
                float *temp = (float *)realloc(esperados, capacidade_e * sizeof(float)); // Inicializando e declarando a variavel temp para a expansao da memoria
                if (temp == NULL) { // Caso nao consiga alocar memoria pro array o if retornara um erro e fechara o programa
                    printf("Erro ao realocar memória.\n"); // .. 
                    free(esperados); // Liberando o espaco da memoria
                    fclose(arquivo); // Fechando o arquivo aberto
                    return 1;
                }
                esperados = temp; // Caso tudo tenha dado certo o array sera expandido
            }
            pos = tamanho_sphere; // Salvando a posicao em que estamos no array
            float soma = calc_sphere(numeros+last_pos, tamanho_sphere-last_pos); // Chamando a funcao para calcular a sphere, e guardando-a em "soma"
            printf("\nValor esperado %f | Valor encontrado %f\n", numero, soma); // Mostrando ao usuario o dado esperado (que pegamos agora) e comparando com o que encontramos no calculo
            precision(numero, soma); // Enviando o numero esperado junto com o nosso calculo para dar inicio ao calculo da precisao
            last_pos = tamanho_sphere; // Salvando a posicao em que estamos para utilizar mais tarde
            esperados[esperados_sphere] = numero; // Colocando o resultado esperado (lido no arquivo) no array de resultados esperados
            esperados_sphere++; // Achamos um resultado esperado entao incrementamos 1 a contagem
        } else { // Caso nao seja maior que 100
            numeros[tamanho_sphere] = numero; // Vai salvar o numero no array dos numeros lidos do arquivo, ou seja, nao eh um resultado esperado
            tamanho_sphere++; // Incrementando 1 a contagem de numeros lidos
        }
    }
    printf("Precisao: %g | %.15f\n", sqrt((rmsd/esperados_sphere)), sqrt((rmsd/esperados_sphere))); // Imprimindo o calculo final da precisao RMSD, em notacao cientifica e decimal com 15 casas.
    fclose(arquivo); // Fechando o arquivo
    free(numeros); // Liberando a memoria
    free(esperados); // Liberando a memoria
    main(); // Voltando pra funcao main
}

int main() {
    rmsd = 0; // Setando o rmsd para 0 toda vez que voltar no main
    last_pos = 0; // Setando a posicao para 0 toda vez que voltar no main
    int sel = 0; // Declarando e inicializando a variavel para a selecao do menu
    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n"); // O menu 
    printf("\n[1] - Rastringin\n"); // ..
    printf("[2] - Sphere\n"); // ..
    printf("[3] - Sair\n"); // ..
    printf("\n-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-\n"); // ..    
    printf("\nSelecione alguma das opcoes: "); // ..
    scanf("%d", &sel); // Aguardando o usuario selecionar alguma das opcoes disponiveis
    switch (sel) { // Aplicacao do switch para mandar o usuario onde ele escolheu
        case 1: // Se escolheu 1 (digitou 1) ele vai para o rastrigin
            rastrigin(); // Chamando a funcao rastrigin
            break; // Interrompendo a execucao do switch
        case 2: // Se escolheu 2 (digitou 2) ele vai para a sphere
            sphere(); // Chamando a funcao sphere
            break; // Interrompendo a execucao do switch
        case 3: // Se escolheu 3 (digitou 3) vai desligar
            return 0; // Retornando 0 pois ele escolheu sair
    }
}