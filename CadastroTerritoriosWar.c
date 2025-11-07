#include <stdio.h>
#include <string.h>

// Estrutura que representa um território

struct Territorio {
    char nome[30];
    char cor[10];
    int tropas;
};


int main() {
    struct Territorio territorios[5]; // Vetor para armazenar 5 territórios
    int i;

    printf("*Cadastro de Territórios*\n\n");

    //Entrada de dados dos territórios

for(i = 0; i <5; i++) {
    printf("Território %d\n", i + 1);

    printf("Nome do Território: ");
    scanf(" %29[^\n]", territorios[i].nome); // lê até 29 caracteres ou até o Enter

    printf("Cor do Exército: ");
    scanf(" %9s", territorios[i].cor); // lê até 9 caracteres (sem espaços)

    printf("Quantidade de Tropas: ");
    scanf("%d", &territorios[i].tropas);

    printf("\n");
}

//Exibição dos dados Cadastrados

printf("*Territórios Cadastrados*\n\n");
for( i = 0; i < 5; i++) {
    printf("Território %d:\n", i + 1);
    printf("Nome: %s\n", territorios[i].nome);
    printf("Cor do exército: %s\n", territorios[i].cor);
    printf("Tropas: %d\n", territorios[i].tropas);
    printf("------------------------------\n");
}

return 0;
}
