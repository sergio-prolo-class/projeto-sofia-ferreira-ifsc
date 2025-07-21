#include <stdio.h>
#include <string.h>

// Definido o número máximo de receitas e o tamanho máximo de caracteres aceitos para os ingredientes e o modo de preparo.
#define MAXRECEITAS 100
#define TAMANHO 1000

// Estrutura da receita
typedef struct{
    char nome[100];
    char ingredientes[TAMANHO];
    char preparo[TAMANHO];
    int porcoes;
} Receita;

// Declaração das funções
void carregarReceitas(Receita *receitas, int *totalReceitas);
void adicionarReceita(Receita *receitas, int *totalReceitas, char *nome, char *ingredientes, char *preparo, int porcoes);
int listarReceitas(Receita *receitas, int totalReceitas);
int buscarReceita(Receita *receitas, int totalReceitas, char *nomeReceita);
void editarReceita(Receita *receitas, int indice, char *novoNome, char *ingredientes, char *preparo, int porcoes);
void removerReceita(Receita *receitas, int *totalReceitas, int indice);
int salvarReceitas(Receita *receitas, int totalReceitas);

int main(){
    Receita receitas[MAXRECEITAS];
    int opcao, totalReceitas = 0;
    char nome[100], ingredientes[TAMANHO], preparo[TAMANHO];
    int porcoes;

    carregarReceitas(receitas, &totalReceitas);

    do {
        printf("\n--- GERENCIADOR DE RECEITAS ---\n");  
        printf("1. Adicionar nova receita\n");
        printf("2. Listar receitas\n");
        printf("3. Buscar receita por nome\n");
        printf("4. Editar receita\n");
        printf("5. Remover receita\n");
        printf("6. Salvar e sair\n");
        printf("Escolha uma opcao: ");
        scanf("%d%*c", &opcao); //recebe um inteiro referente a porção e lê, sem salvar, o \n do enter dado pelo usuário

        switch (opcao){
            case 1: 
                printf("Nome da receita: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                printf("Ingredientes: ");
                fgets(ingredientes, TAMANHO, stdin);
                ingredientes[strcspn(ingredientes, "\n")] = '\0';

                printf("Modo de preparo: ");
                fgets(preparo, TAMANHO, stdin);
                preparo[strcspn(preparo, "\n")] = '\0';

                printf("Rendimento em porcoes: ");
                scanf("%d%*c", &porcoes);

                adicionarReceita(receitas, &totalReceitas, nome, ingredientes, preparo, porcoes);
                printf("Receita adicionada com sucesso!\n");
                break;
            case 2: 
                int quantidadeReceitasSalvas = 0;
                quantidadeReceitasSalvas = listarReceitas(receitas, totalReceitas);
                if(quantidadeReceitasSalvas == 0){
                    printf("\nNenhuma receita cadastrada!\n");
                } else {
                    for (int i = 0; i < quantidadeReceitasSalvas; i++){
                        printf("\nReceita %d: ", i+1);
                        printf("%s", receitas[i].nome);
                        printf("\nIngredientes: %s", receitas[i].ingredientes);
                        printf("\nModo de preparo: %s", receitas[i].preparo);
                        printf("\nRendimento em porcoes: %d\n", receitas[i].porcoes);
                    }
                }
                break;
            case 3: 
                printf("Digite o nome da receita: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                int indiceBusca;
                indiceBusca = buscarReceita(receitas, totalReceitas, nome);
                if(indiceBusca != -1){
                    Receita receitaBusca = receitas[indiceBusca];
                    printf("\n--- Receita encontrada ---\n");
                    printf("Nome: %s\nIngredientes: %s\nPreparo: %s\nRendimento em porcoes: %d\n", receitaBusca.nome, 
                            receitaBusca.ingredientes, receitaBusca.preparo, receitaBusca.porcoes);
                } else {
                    printf("\nReceita nao encontrada.\n");
                }
                break;
            case 4: 
                printf("Digite o nome da receita que deseja editar: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                int indiceEdicao = buscarReceita(receitas, totalReceitas, nome);
                if(indiceEdicao != -1) {
                    printf("Novo nome da receita: ");
                    fgets(nome, 100, stdin);
                    nome[strcspn(nome, "\n")] = '\0';

                    printf("Novos ingredientes: ");
                    fgets(ingredientes, TAMANHO, stdin);
                    ingredientes[strcspn(ingredientes, "\n")] = '\0';

                    printf("Novo modo de preparo: ");
                    fgets(preparo, TAMANHO, stdin);
                    preparo[strcspn(preparo, "\n")] = '\0';

                    printf("Novo rendimento de porcoes: ");
                    scanf("%d%*c", &porcoes);

                    editarReceita(receitas, indiceEdicao, nome, ingredientes, preparo, porcoes);
                    printf("Receita atualizada com sucesso!\n");
                } else {
                    printf("Receita não encontrada.\n");
                }
                break;
            case 5: 
                printf("Digite o nome da receita que deseja remover: ");
                fgets(nome, 100, stdin);
                nome[strcspn(nome, "\n")] = '\0';

                int indiceRemover = buscarReceita(receitas, totalReceitas, nome);
                if(indiceRemover != -1){
                    removerReceita(receitas, &totalReceitas, indiceRemover);
                    printf("Receita removida com sucesso!\n");
                } else {
                    printf("Receita não encontrada.\n");
                }
                break;
            case 6: 
                if(salvarReceitas(receitas, totalReceitas)){
                    printf("Receitas salvas.\n");
                } else {
                    printf("Erro ao salvar receitas.\n");
                }
                break;
            default: 
                printf("\nOpcao invalida!");
        }
    } while (opcao > 0 && opcao <= 6);
    return 0;
}

// Carrega todas as receitas salvas no arquivo 'receitas.txt'
void carregarReceitas(Receita *receitas, int *totalReceitas){
    FILE *arquivo = fopen("receitas.txt", "r");
    if(arquivo == NULL) return;

    while(fscanf(arquivo, " %[^\n]\n %[^\n]\n %[^\n]\n %d\n", receitas[*totalReceitas].nome, receitas[*totalReceitas].ingredientes,
            receitas[*totalReceitas].preparo, &receitas[*totalReceitas].porcoes) == 4){
        (*totalReceitas)++;
    }
    fclose(arquivo);
}

// Adiciona uma nova receita
void adicionarReceita(Receita *receitas, int *totalReceitas, char *nome, char *ingredientes, char *preparo, int porcoes){
    if(*totalReceitas >= MAXRECEITAS) return;

    strcpy(receitas[*totalReceitas].nome, nome);
    strcpy(receitas[*totalReceitas].ingredientes, ingredientes);
    strcpy(receitas[*totalReceitas].preparo, preparo);
    receitas[*totalReceitas].porcoes = porcoes;

    (*totalReceitas)++;
}

// Retorna o número de receitas cadastradas e a interação com o usuário é feita na função main
int listarReceitas(Receita *receitas, int totalReceitas){
    return totalReceitas;
}

// Retorna o indice da receita através uma pesquisa pelo nome ou -1 se não encontrar 
int buscarReceita(Receita *receitas, int totalReceitas, char *nomeReceita){
    for (int i = 0; i < totalReceitas; i++) {
        if (strcmp(receitas[i].nome, nomeReceita) == 0) {
            return i;
        }
    }
    return -1;
}

// Edita todos os dados de uma receita já existente
void editarReceita(Receita *receitas, int indice, char *novoNome, char *novosIngredientes, char *novoPreparo, int novaPorcao) {
    strcpy(receitas[indice].nome, novoNome);
    strcpy(receitas[indice].ingredientes, novosIngredientes);
    strcpy(receitas[indice].preparo, novoPreparo);
    receitas[indice].porcoes = novaPorcao;
}

// Remove uma receita adicionada através do indice dela e reorganizada o indice das outras receitas cadastradas
void removerReceita(Receita *receitas, int *totalReceitas, int indice){
    for(int i = indice; i < *totalReceitas; i++){
        receitas[i] = receitas[i + 1];
    }
    (*totalReceitas)--;
}

// Salva as receitas no arquivo 'receitas.txt'
int salvarReceitas(Receita *receitas, int totalReceitas){
    FILE *arquivo = fopen("receitas.txt", "w");
    if(arquivo == NULL) return 0;

    for(int i = 0; i < totalReceitas; i++){
        fprintf(arquivo, "%s\n%s\n%s\n%d\n", receitas[i].nome, receitas[i].ingredientes, receitas[i].preparo, receitas[i].porcoes);
    }
    fclose(arquivo);
    return 1;
}