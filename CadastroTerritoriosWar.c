#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define QTD_TERRITORIOS 5
#define QTD_JOGADORES 5
#define TAM_NOME 30
#define TAM_COR 10
#define TAM_MISSAO 100

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    int tropas;
} Territorio;

typedef struct {
    char nome[TAM_NOME];
    char cor[TAM_COR];
    char *missao; // alocada dinamicamente
} Jogador;

/* Protótipos */
void cadastrarTerritorios(Territorio *mapa, int qtd);
void cadastrarJogadores(Jogador *jogadores, char *missoes[], int totalMissoes);
void exibirMapa(Territorio *mapa, int qtd);
void atacar(Territorio *atacante, Territorio *defensor);
void atribuirMissao(char *destino, char *missoes[], int totalMissoes);
int verificarMissao(char *missao, Territorio *mapa, int tamanho, const char *corDoJogador);
void liberarMemoria(Territorio *mapa, Jogador *jogadores, int totalJogadores);

/* MAIN */
int main() {
    srand((unsigned int)time(NULL));

    int n = QTD_TERRITORIOS;
    int numJogadores = QTD_JOGADORES;

    printf("=== WAR ESTRUTURADO - 5 TERRITORIOS E 5 JOGADORES ===\n\n");

    /* Aloca dinamicamente mapa e jogadores */
    Territorio *mapa = (Territorio *) malloc(n * sizeof(Territorio));
    if (mapa == NULL) { printf("Erro: malloc mapa\n"); return 1; }

    Jogador *jogadores = (Jogador *) malloc(numJogadores * sizeof(Jogador));
    if (jogadores == NULL) { printf("Erro: malloc jogadores\n"); free(mapa); return 1; }

    /* Missões pré-definidas */
    char *missoes[] = {
        "Conquistar 2 territorios seguidos.",
        "Eliminar todas as tropas da cor vermelha.",
        "Conquistar ao menos 2 territorios da cor azul.",
        "Ter pelo menos 5 tropas em qualquer territorio.",
        "Controlar todos os territorios do mapa."
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    /* Cadastro de territórios (nome + tropas). As cores serão atribuídas aos jogadores
       e cada jogador receberá 1 território inicial (território i -> jogador i). */
    cadastrarTerritorios(mapa, n);

    /* Cadastro de jogadores (nome, cor) e atribuição de missões */
    cadastrarJogadores(jogadores, missoes, totalMissoes);

    /* Distribui posse inicial: cada jogador controla o território de mesmo índice (0..4) */
    for (int i = 0; i < numJogadores && i < n; ++i) {
        strncpy(mapa[i].cor, jogadores[i].cor, TAM_COR);
        mapa[i].cor[TAM_COR-1] = '\0';
    }

    printf("\n--- MISSÕES ATRIBUÍDAS ---\n");
    for (int i = 0; i < numJogadores; ++i) {
        printf("Jogador %d (%s) - Missao: %s\n", i+1, jogadores[i].nome, jogadores[i].missao);
    }

    /* Loop principal de turnos */
    int vencedor = -1;
    char continuar = 's';
    int idxAtacante, idxDefensor;

    while ((continuar == 's' || continuar == 'S') && vencedor == -1) {
        for (int j = 0; j < numJogadores; ++j) {
            printf("\n=== TURNO DO JOGADOR %d - %s ===\n", j+1, jogadores[j].nome);
            exibirMapa(mapa, n);

            /* O jogador deve escolher um território que ele controla como atacante */
            printf("\nEscolha o indice do TERRITORIO ATACANTE (0 a %d) — voce deve ser dono: ", n-1);
            if (scanf("%d", &idxAtacante) != 1) { printf("Entrada invalida.\n"); while(getchar()!='\n'); continue; }
            getchar();

            if (idxAtacante < 0 || idxAtacante >= n) {
                printf("Indice atacante invalido.\n");
                continue;
            }
            if (strcmp(mapa[idxAtacante].cor, jogadores[j].cor) != 0) {
                printf("Voce nao eh dono desse territorio (cor: %s). Escolha um seu.\n", mapa[idxAtacante].cor);
                continue;
            }

            printf("Escolha o indice do TERRITORIO DEFENSOR (0 a %d): ", n-1);
            if (scanf("%d", &idxDefensor) != 1) { printf("Entrada invalida.\n"); while(getchar()!='\n'); continue; }
            getchar();

            if (idxDefensor < 0 || idxDefensor >= n) {
                printf("Indice defensor invalido.\n");
                continue;
            }
            if (strcmp(mapa[idxDefensor].cor, jogadores[j].cor) == 0) {
                printf("Nao e permitido atacar territorios da mesma cor (seu proprio territorio).\n");
                continue;
            }

            /* Realiza o ataque (passando ponteiros) */
            atacar(&mapa[idxAtacante], &mapa[idxDefensor]);

            /* Se houve conquista, a cor do defensor ja foi atualizada dentro de atacar().
               Agora verificamos se o jogador cumpriu sua missão */
            if (verificarMissao(jogadores[j].missao, mapa, n, jogadores[j].cor)) {
                vencedor = j;
                break;
            }
        }

        if (vencedor != -1) break;

        printf("\nDeseja continuar o jogo? (s/n): ");
        if (scanf(" %c", &continuar) != 1) break;
        getchar();
    }

    if (vencedor != -1) {
        printf("\n=== FIM DE JOGO ===\n");
        printf("Jogador %d (%s) cumpriu sua missao e venceu!\n", vencedor+1, jogadores[vencedor].nome);
        printf("Missao: %s\n", jogadores[vencedor].missao);
    } else {
        printf("\nJogo encerrado sem vencedor.\n");
    }

    liberarMemoria(mapa, jogadores, numJogadores);
    return 0;
}

/* IMPLEMENTAÇÕES */

/* Cadastro de territórios: apenas nome e tropas (cores atribuídas depois) */
void cadastrarTerritorios(Territorio *mapa, int qtd) {
    printf("\n--- CADASTRO DE %d TERRITORIOS ---\n", qtd);
    for (int i = 0; i < qtd; ++i) {
        printf("\nTerritorio #%d\n", i);
        printf("Nome: ");
        fgets(mapa[i].nome, TAM_NOME, stdin);
        mapa[i].nome[strcspn(mapa[i].nome, "\n")] = '\0';

        printf("Quantidade inicial de tropas: ");
        while (scanf("%d", &mapa[i].tropas) != 1) {
            printf("Entrada invalida. Digite um numero inteiro: ");
            while(getchar()!='\n');
        }
        getchar(); /* limpa newline */
        /* cor temporaria vazia até ser atribuida ao jogador */
        mapa[i].cor[0] = '\0';
    }
}

/* Cadastro de jogadores: nome, cor, missao (alocada dinamicamente) */
void cadastrarJogadores(Jogador *jogadores, char *missoes[], int totalMissoes) {
    printf("\n--- CADASTRO DE %d JOGADORES ---\n", QTD_JOGADORES);
    for (int i = 0; i < QTD_JOGADORES; ++i) {
        printf("\nJogador %d\n", i+1);
        printf("Nome: ");
        fgets(jogadores[i].nome, TAM_NOME, stdin);
        jogadores[i].nome[strcspn(jogadores[i].nome, "\n")] = '\0';

        printf("Cor do exército (ex: vermelho, azul): ");
        fgets(jogadores[i].cor, TAM_COR, stdin);
        jogadores[i].cor[strcspn(jogadores[i].cor, "\n")] = '\0';

        jogadores[i].missao = (char *) malloc(TAM_MISSAO * sizeof(char));
        if (jogadores[i].missao == NULL) {
            printf("Erro malloc missao.\n");
            /* Em caso real, tratar erro e liberar o que ja foi alocado */
            exit(1);
        }
        atribuirMissao(jogadores[i].missao, missoes, totalMissoes);
    }
}

/* Exibir mapa */
void exibirMapa(Territorio *mapa, int qtd) {
    printf("\n--- MAPA ATUAL ---\n");
    for (int i = 0; i < qtd; ++i) {
        printf("[%d] %-20s | Cor: %-10s | Tropas: %d\n", i, mapa[i].nome,
               (mapa[i].cor[0] ? mapa[i].cor : "nenhum"), mapa[i].tropas);
    }
}

/* Função de ataque entre dois territórios (usa ponteiros) */
void atacar(Territorio *atacante, Territorio *defensor) {
    printf("\n%s (%s) esta atacando %s (%s)...\n",
           atacante->nome, atacante->cor, defensor->nome, defensor->cor[0] ? defensor->cor : "nenhum");

    if (atacante->tropas <= 1) {
        printf("O atacante precisa ter mais de 1 tropa para atacar!\n");
        return;
    }

    int dadoAtacante = rand() % 6 + 1;
    int dadoDefensor = rand() % 6 + 1;
    printf("Dado atacante: %d | Dado defensor: %d\n", dadoAtacante, dadoDefensor);

    if (dadoAtacante > dadoDefensor) {
        printf(">>> Ataque bem-sucedido! %s conquistou %s!\n", atacante->nome, defensor->nome);
        /* defensor muda de dono e recebe metade das tropas do atacante */
        strncpy(defensor->cor, atacante->cor, TAM_COR);
        defensor->cor[TAM_COR-1] = '\0';
        defensor->tropas = atacante->tropas / 2;
        atacante->tropas = atacante->tropas - defensor->tropas; /* restantes */
        if (atacante->tropas < 1) atacante->tropas = 1;
    } else {
        printf(">>> Defesa bem-sucedida! %s perde uma tropa.\n", atacante->nome);
        atacante->tropas -= 1;
        if (atacante->tropas < 1) atacante->tropas = 1;
    }
}

/* Sorteia e copia uma missão para destino */
void atribuirMissao(char *destino, char *missoes[], int totalMissoes) {
    int indice = rand() % totalMissoes;
    strncpy(destino, missoes[indice], TAM_MISSAO);
    destino[TAM_MISSAO-1] = '\0';
}

/* Verifica se a missão foi cumprida.
   Recebe também a cor do jogador para facilitar checagens por cor. */
int verificarMissao(char *missao, Territorio *mapa, int tamanho, const char *corDoJogador) {
    /* Lógica simples para os exemplos de missões presentes */
    if (strstr(missao, "2 territorios")) {
        int cont = 0;
        for (int i = 0; i < tamanho; ++i) {
            if (strcmp(mapa[i].cor, corDoJogador) == 0) cont++;
        }
        return cont >= 2;
    }
    if (strstr(missao, "vermelha")) {
        /* verificar se não existe mais "vermelha" no mapa */
        int existe = 0;
        for (int i = 0; i < tamanho; ++i) {
            if (strcmp(mapa[i].cor, "vermelha") == 0) { existe = 1; break; }
        }
        return existe ? 0 : 1;
    }
    if (strstr(missao, "2 territorios seguidos")) {
        /* procura por 2 territorios consecutivos da cor do jogador */
        int seq = 0;
        for (int i = 0; i < tamanho; ++i) {
            if (strcmp(mapa[i].cor, corDoJogador) == 0) {
                seq++;
                if (seq >= 2) return 1;
            } else seq = 0;
        }
        return 0;
    }
    if (strstr(missao, "5 tropas")) {
        for (int i = 0; i < tamanho; ++i)
            if (strcmp(mapa[i].cor, corDoJogador) == 0 && mapa[i].tropas >= 5) return 1;
        return 0;
    }
    if (strstr(missao, "Controlar todos os territorios") || strstr(missao, "todos os territorios")) {
        for (int i = 0; i < tamanho; ++i)
            if (strcmp(mapa[i].cor, corDoJogador) != 0) return 0;
        return 1;
    }
    /* padrão: missão não implementada -> retorna 0 */
    return 0;
}

/* Libera toda a memória (mapa e missões dos jogadores) */
void liberarMemoria(Territorio *mapa, Jogador *jogadores, int totalJogadores) {
    free(mapa);
    for (int i = 0; i < totalJogadores; ++i) {
        free(jogadores[i].missao);
    }
    free(jogadores);
}