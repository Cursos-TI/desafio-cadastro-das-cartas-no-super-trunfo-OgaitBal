#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Protótipos
Territorio* criarMapa(int n);
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirTerritorios(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

int main() {
    srand(time(NULL)); // Garante aleatoriedade dos dados

    int n;
    printf("=== Simulador de Batalha WAR ===\n");
    printf("Informe quantos territorios deseja cadastrar: ");
    scanf("%d", &n);
    getchar(); // limpa buffer do scanf

    // Alocação dinâmica de memória para o vetor de territórios
    Territorio* mapa = (Territorio*) malloc(n * sizeof(Territorio));
    if (mapa == NULL) {
        printf("Erro: nao foi possivel alocar memoria.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);
    exibirTerritorios(mapa, n);

    int idxAtacante, idxDefensor;
    char continuar;

    do {
        printf("\nEscolha o territorio ATACANTE (0 a %d): ", n - 1);
        scanf("%d", &idxAtacante);
        getchar();

        printf("Escolha o territorio DEFENSOR (0 a %d): ", n - 1);
        scanf("%d", &idxDefensor);
        getchar();

        if (idxAtacante < 0 || idxAtacante >= n || idxDefensor < 0 || idxDefensor >= n) {
            printf("Índices inválidos!\n");
            continue;
        }

        if (strcmp(mapa[idxAtacante].cor, mapa[idxDefensor].cor) == 0) {
            printf("Nao e permitido atacar territorios da mesma cor!\n");
            continue;
        }

        atacar(&mapa[idxAtacante], &mapa[idxDefensor]);
        exibirTerritorios(mapa, n);

        printf("\nDeseja realizar outro ataque? (s/n): ");
        scanf(" %c", &continuar);
        getchar();

    } while (continuar == 's' || continuar == 'S');

    liberarMemoria(mapa);
    printf("\nMemoria liberada. Encerrando o programa.\n");

    return 0;
}

/* Função para cadastrar os territórios dinamicamente */
void cadastrarTerritorios(Territorio* mapa, int n) {
    printf("\n--- Cadastro de Territorios ---\n");
    for (int i = 0; i < n; i++) {
        printf("\nTerritorio #%d\n", i);
        printf("Nome: ");
        fgets(mapa[i].nome, 30, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Cor do exercito: ");
        fgets(mapa[i].cor, 10, stdin);
        mapa[i].cor[strcspn(mapa[i].cor, "\n")] = '\0';

        printf("Quantidade inicial de tropas: ");
        scanf("%d", &mapa[i].tropas);
        getchar(); // limpa buffer
    }
}

/* Exibe o estado atual dos territórios */
void exibirTerritorios(Territorio* mapa, int n) {
    printf("\n--- ESTADO ATUAL DOS TERRITORIOS ---\n");
    for (int i = 0; i < n; i++) {
        printf("[%d] %-15s | Cor: %-10s | Tropas: %d\n", i, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

/* Simula um ataque entre dois territórios */
void atacar(Territorio* atacante, Territorio* defensor) {
    printf("\n--- Ataque em andamento ---\n");
    printf("%s (%s) ataca %s (%s)\n", atacante->nome, atacante->cor, defensor->nome, defensor->cor);

    if (atacante->tropas <= 1) {
        printf("O atacante precisa ter mais de 1 tropa para atacar!\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;

    printf("Dado atacante: %d\n", dadoAtacante);
    printf("Dado defensor: %d\n", dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf(">>> Ataque bem-sucedido!\n");

        // Atualiza o defensor: muda de dono e recebe metade das tropas do atacante
        strcpy(defensor->cor, atacante->cor);
        defensor->tropas = atacante->tropas / 2;

        // O atacante perde metade das tropas (restantes)
        atacante->tropas = atacante->tropas / 2;

        printf("O territorio %s agora pertence a %s.\n", defensor->nome, defensor->cor);
    } else {
        printf(">>> Defesa bem-sucedida!\n");

        // O atacante perde uma tropa
        atacante->tropas -= 1;
        if (atacante->tropas < 1) atacante->tropas = 1;

        printf("%s perdeu uma tropa, restam %d tropas.\n", atacante->nome, atacante->tropas);
    }
}

/* Libera a memória alocada dinamicamente */
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}