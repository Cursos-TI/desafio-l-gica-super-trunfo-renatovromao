// TrunfoComparacao.c — Desafio de Lógica: Comparação de Cartas (Países)
// =====================================================================
// Programa em C que simula comparações de cartas do Super Trunfo.
// Está dividido em três níveis: Novato, Aventureiro e Mestre.
// Cada nível aumenta a complexidade da lógica usada para decidir o vencedor.
//
// Como compilar:   gcc -std=c11 -Wall -Wextra -O2 TrunfoComparacao.c -o TrunfoComparacao
// Como executar:   ./TrunfoComparacao   (ou TrunfoComparacao.exe no Windows)
// =====================================================================

#include <stdio.h>
#include <string.h>
#include <ctype.h>

// ---------------------------------------------------------------------
// Estrutura de uma carta
// Representa uma cidade com atributos básicos e derivados.
// ---------------------------------------------------------------------
typedef struct {
    char estado;              // Estado (A..H)
    int  cidade;              // Número da cidade (1..4)
    char codigo[4];           // Código da carta (ex.: A01)
    char nome[64];            // Nome da cidade
    unsigned long populacao;  // Habitantes
    float area;               // Área em km²
    float pib;                // PIB em R$
    int   pontos;             // Número de pontos turísticos
    float densidade;          // População / área
    float pib_per_capita;     // PIB / população
} Carta;

// ---------------------------------------------------------------------
// Funções auxiliares de entrada e cálculo
// ---------------------------------------------------------------------

// Limpa caracteres extras no buffer do teclado após um scanf
static void limparRestoDaLinha(void) {
    int c; while ((c = getchar()) != '\n' && c != EOF) {}
}

// Leitura de string (nome da cidade)
static void lerString(char *buf, size_t tam, const char *prompt) {
    printf("%s", prompt);
    if (fgets(buf, (int)tam, stdin) == NULL) { buf[0] = '\0'; return; }
    size_t n = strlen(buf);
    if (n && buf[n-1] == '\n') buf[n-1] = '\0';
}

// Leitura de números
static unsigned long lerULong(const char *prompt) {
    unsigned long v; int ok;
    do { printf("%s", prompt); ok = scanf("%lu", &v); limparRestoDaLinha(); } while (ok != 1);
    return v;
}

static int lerInt(const char *prompt) {
    int v, ok; do { printf("%s", prompt); ok = scanf("%d", &v); limparRestoDaLinha(); } while (ok != 1);
    return v;
}

static float lerFloat(const char *prompt) {
    float v; int ok; do { printf("%s", prompt); ok = scanf("%f", &v); limparRestoDaLinha(); } while (ok != 1);
    return v;
}

// Leitura de estado (uma letra, convertida para maiúscula)
static char lerEstadoChar(const char *prompt) {
    char ch = 0; int ok;
    do { printf("%s", prompt); ok = scanf(" %c", &ch); limparRestoDaLinha(); } while (ok != 1);
    return (char)toupper((unsigned char)ch);
}

// Monta o código da carta (ex.: A03)
static void montarCodigo(Carta *c) {
    snprintf(c->codigo, sizeof c->codigo, "%c%02d", c->estado, c->cidade);
}

// Calcula atributos derivados (densidade e PIB per capita)
static void calcularDerivados(Carta *c) {
    c->densidade = (c->area != 0.0f) ? (float)((double)c->populacao / (double)c->area) : 0.0f;
    c->pib_per_capita = (c->populacao != 0UL) ? (float)((double)c->pib / (double)c->populacao) : 0.0f;
}

// Cadastro de uma carta via terminal
static void cadastrarCarta(Carta *c, const char *rotulo) {
    printf("\n--- %s ---\n", rotulo);
    c->estado = lerEstadoChar("Estado (A-H, 1 letra): ");
    c->cidade = lerInt("Cidade (1-4): ");
    lerString(c->nome, sizeof c->nome, "Nome da cidade: ");
    c->populacao = lerULong("Populacao (unsigned long): ");
    c->area = lerFloat("Area (float, km2): ");
    c->pib = lerFloat("PIB (float, R$): ");
    c->pontos = lerInt("Pontos turisticos (int): ");
    montarCodigo(c);
    calcularDerivados(c);
}

// Exibe os dados da carta formatados
static void imprimirCarta(const Carta *c) {
    printf("[%s] %s\n", c->codigo, c->nome);
    printf("Estado: %c | Cidade: %d\n", c->estado, c->cidade);
    printf("Populacao: %lu\n", c->populacao);
    printf("Area: %.2f\n", c->area);
    printf("PIB: %.2f\n", c->pib);
    printf("Pontos turisticos: %d\n", c->pontos);
    printf("Densidade: %.4f\n", c->densidade);
    printf("PIB per capita: %.4f\n", c->pib_per_capita);
}

// ---------------------------------------------------------------------
// Funções de comparação entre atributos
// Retornam 1 (carta1 vence), 0 (carta2 vence) ou -1 (empate)
// ---------------------------------------------------------------------
static int cmpMaiorFloat(float a, float b) { return (a > b) ? 1 : (a < b) ? 0 : -1; }
static int cmpMaiorUL(unsigned long a, unsigned long b) { return (a > b) ? 1 : (a < b) ? 0 : -1; }
static int cmpMenorFloat(float a, float b) { return (a < b) ? 1 : (a > b) ? 0 : -1; }

// ---------------------------------------------------------------------
// Nível Novato — compara um atributo fixo usando if/else
// ---------------------------------------------------------------------
static void rodarNovato(void) {
    Carta c1 = {0}, c2 = {0};
    cadastrarCarta(&c1, "Carta 1");
    cadastrarCarta(&c2, "Carta 2");

    printf("\n=== CARTA 1 ===\n"); imprimirCarta(&c1);
    printf("\n=== CARTA 2 ===\n"); imprimirCarta(&c2);

    // Comparação fixa: PIB (maior vence)
    printf("\n=== COMPARACAO (PIB — maior vence) ===\n");
    if (c1.pib > c2.pib) {
        printf("Vencedora: Carta 1 (%s)\n", c1.codigo);
    } else if (c1.pib < c2.pib) {
        printf("Vencedora: Carta 2 (%s)\n", c2.codigo);
    } else {
        printf("Empate no atributo PIB.\n");
    }
}

// ---------------------------------------------------------------------
// Nível Aventureiro — menu interativo com switch
// Usuário escolhe o atributo a ser comparado.
// ---------------------------------------------------------------------
static void rodarAventureiro(void) {
    Carta c1 = {0}, c2 = {0};
    cadastrarCarta(&c1, "Carta 1");
    cadastrarCarta(&c2, "Carta 2");

    printf("\n=== CARTA 1 ===\n"); imprimirCarta(&c1);
    printf("\n=== CARTA 2 ===\n"); imprimirCarta(&c2);

    printf("\n=== MENU DE COMPARACAO ===\n");
    printf("1) Populacao (maior vence)\n");
    printf("2) Area (maior vence)\n");
    printf("3) PIB (maior vence)\n");
    printf("4) Pontos turisticos (maior vence)\n");
    printf("5) Densidade populacional (menor vence)\n");
    printf("6) PIB per capita (maior vence)\n");
    int op = lerInt("Escolha: ");

    int resultado = 0;
    switch (op) {
        case 1: resultado = cmpMaiorUL(c1.populacao, c2.populacao); break;
        case 2: resultado = cmpMaiorFloat(c1.area, c2.area); break;
        case 3: resultado = cmpMaiorFloat(c1.pib, c2.pib); break;
        case 4: resultado = cmpMaiorUL((unsigned long)c1.pontos, (unsigned long)c2.pontos); break;
        case 5: resultado = cmpMenorFloat(c1.densidade, c2.densidade); break;
        case 6: resultado = cmpMaiorFloat(c1.pib_per_capita, c2.pib_per_capita); break;
        default: printf("Opcao invalida.\n"); return;
    }

    printf("\n=== RESULTADO ===\n");
    if (resultado == 1)      printf("Vencedora: Carta 1 (%s)\n", c1.codigo);
    else if (resultado == 0) printf("Vencedora: Carta 2 (%s)\n", c2.codigo);
    else                     printf("Empate no atributo escolhido.\n");
}

// ---------------------------------------------------------------------
// Nível Mestre — comparação de dois atributos escolhidos
// Cada atributo dá um ponto à carta vencedora. Usa operadores ternários.
// ---------------------------------------------------------------------
typedef enum { ATR_POP=1, ATR_AREA, ATR_PIB, ATR_PONTOS, ATR_DENSIDADE, ATR_PPC } Atributo;

static void imprimirMenuAtributos(void) {
    printf("1) Populacao (maior)\n");
    printf("2) Area (maior)\n");
    printf("3) PIB (maior)\n");
    printf("4) Pontos turisticos (maior)\n");
    printf("5) Densidade populacional (menor)\n");
    printf("6) PIB per capita (maior)\n");
}

static int compararPorAtributo(const Carta *c1, const Carta *c2, Atributo atr) {
    return (atr == ATR_POP)       ? cmpMaiorUL(c1->populacao, c2->populacao)
         : (atr == ATR_AREA)      ? cmpMaiorFloat(c1->area, c2->area)
         : (atr == ATR_PIB)       ? cmpMaiorFloat(c1->pib, c2->pib)
         : (atr == ATR_PONTOS)    ? cmpMaiorUL((unsigned long)c1->pontos, (unsigned long)c2->pontos)
         : (atr == ATR_DENSIDADE) ? cmpMenorFloat(c1->densidade, c2->densidade)
         : (atr == ATR_PPC)       ? cmpMaiorFloat(c1->pib_per_capita, c2->pib_per_capita)
         : -1;
}

static const char* nomeAtributo(Atributo a) {
    return (a==ATR_POP)?"Populacao":(a==ATR_AREA)?"Area":(a==ATR_PIB)?"PIB":
           (a==ATR_PONTOS)?"Pontos turisticos":(a==ATR_DENSIDADE)?"Densidade":
           (a==ATR_PPC)?"PIB per capita":"?";
}

static void rodarMestre(void) {
    Carta c1 = {0}, c2 = {0};
    cadastrarCarta(&c1, "Carta 1");
    cadastrarCarta(&c2, "Carta 2");

    printf("\n=== CARTA 1 ===\n"); imprimirCarta(&c1);
    printf("\n=== CARTA 2 ===\n"); imprimirCarta(&c2);

    // Usuário escolhe dois atributos
    printf("\n=== MENU (escolha DOIS atributos) ===\n");
    imprimirMenuAtributos();
    int a1 = lerInt("Primeiro atributo (1-6): ");
    imprimirMenuAtributos();
    int a2 = lerInt("Segundo atributo (1-6): ");

    int r1 = compararPorAtributo(&c1, &c2, (Atributo)a1);
    int r2 = compararPorAtributo(&c1, &c2, (Atributo)a2);

    int p1 = (r1==1 ? 1 : 0) + (r2==1 ? 1 : 0);
    int p2 = (r1==0 ? 1 : 0) + (r2==0 ? 1 : 0);

    printf("\n=== RESULTADOS PARCIAIS ===\n");
    printf("%s: ", nomeAtributo((Atributo)a1));
    if (r1 == 1)      printf("Carta 1\n");
    else if (r1 == 0) printf("Carta 2\n");
    else              printf("Empate\n");

    printf("%s: ", nomeAtributo((Atributo)a2));
    if (r2 == 1)      printf("Carta 1\n");
    else if (r2 == 0) printf("Carta 2\n");
    else              printf("Empate\n");

    printf("\n=== PLACAR ===\nCarta 1: %d  |  Carta 2: %d\n", p1, p2);

    // Decisão final
    printf("\n=== VENCEDOR FINAL ===\n");
    if (p1 > p2)      printf("Carta 1 (%s)\n", c1.codigo);
    else if (p2 > p1) printf("Carta 2 (%s)\n", c2.codigo);
    else              printf("Empate geral\n");
}

// ---------------------------------------------------------------------
// Função principal: mostra o menu de níveis e chama a lógica escolhida
// ---------------------------------------------------------------------
int main(void) {
    printf("===== Super Trunfo — Países (Comparação de Cartas) =====\n");
    printf("1) Novato (if/if-else, atributo fixo)\n");
    printf("2) Aventureiro (switch, 1 atributo à escolha)\n");
    printf("3) Mestre (2 atributos, ternário)\n");
    int nivel = lerInt("Escolha: ");

    if (nivel == 1)      rodarNovato();
    else if (nivel == 2) rodarAventureiro();
    else                 rodarMestre();
    return 0;
}