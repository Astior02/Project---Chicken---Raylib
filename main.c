#include "raylib.h"
#include <math.h>
#include <stdio.h>
#define RAYGUI_IMPLEMENTATION
#include "deps/raygui.h"

// --- variáveis globais do player

float tempoJogo = 0.0f;
int tamanhoTelaX = 1280;
int tamanhoTelaY = 720;
int larguraFase = 8916;
float playerX = 30;
float playerY = 500;
bool direcao = true; // true direita, false esquerda
float playerVelocidade = 100.0f;
int vida = 0; // -1 morreu, 0 vivendo, 1 nasceu

// -- checkpoint/ Vitória

int playerCkeckpointX = 30, playerCkeckpointY = 500;
bool vitoria = false;
float tempoRun = 0.0f;

// -- variáveis para pulo

float velocidadePuloY = 0.0f;
bool podePular = true;
float focarPuloInicial = -400.0f;
float gravidadeAceleracao = 800.0f;

// -- variáveis de animação

int frameAtual = 0;
int contadorFrames = 0;
int estadoAnimacao = -1;

// --- variaveis de animação viva/morte
float tempoAnimacaoMorte = 0.0f;
float duracaoAnimacaoMorte = 1.0f; // Duração da animação de morte
float tempoAnimacaoNascer = 0.0f;
float duracaoAnimacaoNascer = 1.0f; // Duração da animação de nascimento
bool podeControlarPlayer = true;    // Controla se o jogador pode se mover

// -- variáveis para dash

bool dashAtivo = false;
float dashVelocidade = 200.0f;
float dashTempo;
float dashDuracao = 0.7f;
bool dashVertical = false;
bool dashVerticalUsadoNoAr = false;
int dashHorizontalUsosNoAr = 0;
int dashHorizontalMaxUsos = 2;

// -- variáveis para colisao
bool colisaoDireita = false;
bool colisaoEsquerda = false;
bool grudandoParede = false;
float velocidadeDescidaParede = 100.0f;

void reiniciarGame()
{

    tempoJogo = 0.0f;
    tamanhoTelaX = 1280;
    tamanhoTelaY = 720;
    larguraFase = 8916;
    playerX = 30;
    playerY = 500;
    direcao = true; // true direita, false esquerda
    playerVelocidade = 200.0f;
    vida = 0; // -1 morreu, 0 vivendo, 1 nasceu

    // -- checkpoint/ Vitória

    playerCkeckpointX = 30, playerCkeckpointY = 500;
    vitoria = false;

    // -- variáveis para pulo

    velocidadePuloY = 0.0f;
    podePular = true;
    focarPuloInicial = -400.0f;
    gravidadeAceleracao = 800.0f;

    // -- variáveis de animação

    frameAtual = 0;
    contadorFrames = 0;
    estadoAnimacao = -1;

    // --- variaveis de animação viva/morte
    tempoAnimacaoMorte = 0.0f;
    duracaoAnimacaoMorte = 1.0f; // Duração da animação de morte
    tempoAnimacaoNascer = 0.0f;
    duracaoAnimacaoNascer = 1.0f; // Duração da animação de nascimento
    podeControlarPlayer = true;   // Controla se o jogador pode se mover

    // -- variáveis para dash

    dashAtivo = false;
    dashVelocidade = 200.0f;
    dashTempo;
    dashDuracao = 0.4f;
    dashVertical = false;
    dashVerticalUsadoNoAr = false;

    // -- variáveis para colisao
    colisaoDireita = false;
    colisaoEsquerda = false;
    grudandoParede = false;
    velocidadeDescidaParede = 100.0f;
}

typedef struct
{
    Rectangle retangulo;

    // Texture2D sprite;
    bool solido;
    Color cor;

} ObjetosCena;

void desenhaCenario(Texture2D fundo)
{

    // DrawTexture(fundo, 0, 0, WHITE);
    // DrawTexture(solo, 0, 0, WHITE);

    // -- DESENHA FUNDO

    Rectangle origem = {0.0f, 0.0f, (float)fundo.width, (float)fundo.height};
    Rectangle destino = {0.0f, 0.0f, (float)larguraFase, (float)tamanhoTelaY};
    DrawTexturePro(fundo, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void animarMovimento(int framesTotais, int alturaFrame, int frameVelocidade)
{

    contadorFrames++;

    if (contadorFrames >= frameVelocidade)
    {

        frameAtual++;

        if (frameAtual >= framesTotais)
        {

            frameAtual = 0;
        }

        contadorFrames = 0;
    }
}

void personagemParado(Texture2D playerParado)
{

    int totalFrames = 6;
    int frameVelocidade = 10;

    int alturaFrame = playerParado.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame, frameVelocidade);

    Rectangle origem = {
        0.0f,
        (float)frameAtual * alturaFrame,
        (float)playerParado.width,
        alturaFrame};

    Rectangle destino = {
        playerX,
        playerY,
        playerParado.width,
        alturaFrame};

    if (!direcao)
    {
        origem.width *= -1;
    }

    DrawTexturePro(playerParado, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void personagemMovendo(Texture2D playerMovendo)
{

    int totalFrames = 10;
    int frameVelocidade = 6;

    int alturaFrame = playerMovendo.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame, frameVelocidade);

    Rectangle origem = {
        0.0f,
        (float)frameAtual * alturaFrame,
        (float)playerMovendo.width,
        alturaFrame};

    Rectangle destino = {
        playerX,
        playerY,
        playerMovendo.width,
        alturaFrame};

    if (!direcao)
    {
        origem.width *= -1;
    }

    DrawTexturePro(playerMovendo, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void personagemPulando(Texture2D playerPulando)
{

    int totalFrames = 5;
    int frameVelocidade = 6;

    int alturaFrame = playerPulando.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame, frameVelocidade);

    Rectangle origem = {
        0.0f,
        (float)frameAtual * alturaFrame,
        (float)playerPulando.width,
        alturaFrame};

    Rectangle destino = {
        playerX,
        playerY,
        playerPulando.width,
        alturaFrame};

    if (!direcao)
    {
        origem.width *= -1;
    }

    DrawTexturePro(playerPulando, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void personagemDash(Texture2D personagemEmDash)
{

    int totalFrames = 7;
    int frameVelocidade = 6;

    int alturaFrame = personagemEmDash.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame, frameVelocidade);

    Rectangle origem = {
        0.0f,
        (float)frameAtual * alturaFrame,
        (float)personagemEmDash.width,
        alturaFrame};

    Rectangle destino = {
        playerX,
        playerY,
        personagemEmDash.width,
        alturaFrame};

    if (!direcao)
    {
        origem.width *= -1;
    }

    DrawTexturePro(personagemEmDash, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void persoangemParede(Texture2D personagemParede)
{

    int totalFrames = 1;
    int frameVelocidade = 6;

    int alturaFrame = personagemParede.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame, frameVelocidade);

    Rectangle origem = {
        0.0f,
        (float)frameAtual * alturaFrame,
        (float)personagemParede.width,
        alturaFrame};

    Rectangle destino = {
        playerX,
        playerY,
        personagemParede.width,
        alturaFrame};

    if (!direcao)
    {
        origem.width *= -1;
    }

    DrawTexturePro(personagemParede, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void personagemMorrer(Texture2D personagemMorrerTex)
{
    int totalFrames = 7;
    int frameVelocidade = 8;

    int alturaFrame = personagemMorrerTex.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame, frameVelocidade);

    Rectangle origem = {
        0.0f,
        (float)frameAtual * alturaFrame,
        (float)personagemMorrerTex.width,
        alturaFrame};

    Rectangle destino = {
        playerX,
        playerY,
        personagemMorrerTex.width,
        alturaFrame};

    if (!direcao)
    {
        origem.width *= -1;
    }

    DrawTexturePro(personagemMorrerTex, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void personagemNascer(Texture2D personagemNascerTex)
{
    int totalFrames = 10;
    int frameVelocidade = 8;

    int alturaFrame = personagemNascerTex.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame, frameVelocidade);

    Rectangle origem = {
        0.0f,
        (float)frameAtual * alturaFrame,
        (float)personagemNascerTex.width,
        alturaFrame};

    Rectangle destino = {
        playerX,
        playerY,
        personagemNascerTex.width,
        alturaFrame};

    if (!direcao)
    {
        origem.width *= -1;
    }

    DrawTexturePro(personagemNascerTex, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void movimentoHorizontal(float tempo)
{

    if (IsKeyDown(KEY_RIGHT))
    {
        playerX += playerVelocidade * tempo;
        direcao = true;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        playerX -= playerVelocidade * tempo;
        direcao = false;
    }
}

void cameraPrincipal(Camera2D *camera)
{
    float inicioFase = tamanhoTelaX / 2.0f;
    float fimFase = larguraFase - tamanhoTelaX / 2.0f;

    float alvoX = playerX;

    if (alvoX < inicioFase)
    {
        alvoX = inicioFase;
    }
    if (alvoX > fimFase)
    {
        alvoX = fimFase;
    }

    camera->target = (Vector2){alvoX, tamanhoTelaY / 2.0f};               // segue o player
    camera->offset = (Vector2){tamanhoTelaX / 2.0f, tamanhoTelaY / 2.0f}; // Centraliza
    camera->rotation = 0.0f;
    camera->zoom = 1.0f;
}

void pulo(float tempo, Sound somPulo)
{
    if (velocidadePuloY > 0) // Está caindo
    {
        velocidadePuloY += (gravidadeAceleracao * 0.3f) * tempo; // Cai devagar
    }

    else // Está subindo ou parado
    {
        velocidadePuloY += gravidadeAceleracao * tempo; // Gravidade normal
    }

    if ((IsKeyPressed(KEY_UP) /*|| IsKeyDown(KEY_UP)*/ && podePular && !grudandoParede))
    {

        velocidadePuloY = focarPuloInicial;

        PlaySound(somPulo);
    }

    playerY += velocidadePuloY * tempo;
}

// ranking

// -- variáveis para o ranking

#define MAX_NOME 10 // tamanho máximo do nome
#define MAX_RANK 5  // TAMANHO MÁXIMO DE POSIÇÕES DO RANK

typedef struct
{
    char nome[MAX_NOME];
    float tempo;
} Ranking;

bool focoCaixaRank = true;

Ranking jogadorRank = {"", 0.0f};

void desenharRanking(Texture2D fundoRank)
{
    ClearBackground(WHITE);

    DrawTexture(fundoRank, 0, 0, WHITE);

    static bool salvou = false;
    static bool rankingCarregado = false;
    static Ranking ranking[MAX_RANK];
    static int totalRanking = 0;

    Rectangle caixaTexto = {40, tamanhoTelaY - 150, 300, 40};

    // carrega o ranking 1 vez
    if (!rankingCarregado && FileExists("ranking.txt"))
    {

        FILE *file = fopen("ranking.txt", "r");

        while (!feof(file) && totalRanking < MAX_RANK)
        {

            fscanf(file, "Nome: %s      Tempo: %f segundos\n", ranking[totalRanking].nome, &ranking[totalRanking].tempo);
            totalRanking++;
        }

        fclose(file);
        rankingCarregado = true;
    }

    // Controle da caixa de texto para digitar o nome
    if (GuiTextBox(caixaTexto, jogadorRank.nome, MAX_NOME, !salvou))
    {
        // focoCaixaRank = !focoCaixaRank;
    }

    // Salvamento do ranking
    if (IsKeyPressed(KEY_ENTER) && !salvou)
    {
        if (strlen(jogadorRank.nome) > 0)
        {

            jogadorRank.tempo = tempoRun;

            // adiciona um novo ranking

            if (totalRanking < MAX_RANK)
            {
                ranking[totalRanking] = jogadorRank;
                totalRanking++;
            }
            else
            {
                if (jogadorRank.tempo < ranking[MAX_RANK - 1].tempo)
                {
                    ranking[MAX_RANK - 1] = jogadorRank;
                }
            }

            // ordena pelo menor tempo

            for (int i = 0; i < totalRanking - 1; i++)
            {

                for (int j = i + 1; j < totalRanking; j++)
                {

                    if (ranking[j].tempo < ranking[i].tempo)

                    {
                        Ranking temp = ranking[i];
                        ranking[i] = ranking[j];
                        ranking[j] = temp;
                    }
                }
            }

            // salvar no arquivo

            FILE *file = fopen("ranking.txt", "w");

            for (int i = 0; i < totalRanking; i++)
            {
                fprintf(file, "Nome: %s     Tempo: %.1f segundos\n", ranking[i].nome, ranking[i].tempo);
            }
            fclose(file);

            salvou = true;
        }

        focoCaixaRank = false;
    }

    // Textos informativos
    DrawText("Digite seu nome:", caixaTexto.x, caixaTexto.y - 30, 20, DARKGRAY);
    DrawText("Pressione ENTER para salvar", caixaTexto.x, caixaTexto.y + 50, 20, DARKGRAY);

    if (salvou)
    {
        DrawText("Ranking salvo com sucesso!", caixaTexto.x, caixaTexto.y + 80, 20, GREEN);
    }

    // Mostrar ranking

    // DrawText("Ranking Top 5:", 300, 20, DARKBLUE);

    const char *titulo = "Ranking Top 5:";
    int fonte = 20;
    int larguraTexto = MeasureText(titulo, fonte);
    int posX = (tamanhoTelaX - larguraTexto) / 2;
    DrawText(titulo, posX, 300, fonte, DARKBLUE);

    for (int i = 0; i < totalRanking; i++)
    {
        char linha[50]; // formata o texto do ranking
        snprintf(linha, sizeof(linha), "%d. %s - %.1fs", i + 1, ranking[i].nome, ranking[i].tempo);

        int fonte = 20;
        int larguraTexto = MeasureText(linha, fonte);
        int posX = (tamanhoTelaX - larguraTexto) / 2;

        DrawText(linha, posX, 340 + i * 30, fonte, BLACK);
    }

    // - - Reiniciar Game

    const char *textoReiniciar = "Pressione Espaço para jogar novamente";
    int fonteTextoReiniciar = 24;
    int larguraTextoRestart = MeasureText(textoReiniciar, fonteTextoReiniciar);
    int posicaoXRestartTexto = (tamanhoTelaX - larguraTextoRestart) / 2;
    int posicaoYRestartTexto = tamanhoTelaY - 60;
    DrawText(textoReiniciar, posicaoXRestartTexto, posicaoYRestartTexto, fonteTextoReiniciar, WHITE);

    if (IsKeyPressed(KEY_SPACE))
    {
        reiniciarGame();
        jogadorRank = (Ranking){"", 0.0f};
        salvou = false;
        vitoria = false;
    }
}

int main(void)
{

    Camera2D cameraPlayer;

    InitAudioDevice();

    // -- CARREGAR AUDIO DO JOGO
    Sound somPulo = LoadSound("audio/somPulo.mp3");
    SetSoundVolume(somPulo, 0.1f);

    Sound somDash = LoadSound("audio/somDash.mp3");
    SetSoundVolume(somDash, 5.0f);

    Sound somVitoria = LoadSound("audio/somVitoria.mp3");
    SetSoundVolume(somVitoria, 0.5f);

    Sound somNascer = LoadSound("audio/somNascendo.mp3");
    SetSoundVolume(somNascer, 0.5f);

    Music somTemaC = LoadMusicStream("audio/somTemaC.mp3");
    SetMusicVolume(somTemaC, 0.03f);

    PlayMusicStream(somTemaC);

    Sound somMorrendo = LoadSound("audio/somMorrendo.mp3");
    SetSoundVolume(somMorrendo, 0.5f);

    InitWindow(tamanhoTelaX, tamanhoTelaY, "Project Chicken");

    SetTargetFPS(60);

    // -- CARREGAR TEXTURA DO CENÁRIO

    Texture2D background = LoadTexture("sprites/Mapa_completo.png");
    Texture2D fundoRank = LoadTexture("sprites/Cenario/fundoRank.png");
    Texture2D FundoMenu = LoadTexture("sprites/Fundo_Menu.png");

    // -- CARREGAR SPRITE DO PLAYER

    Texture2D jogadorParado = LoadTexture("sprites/Chick-Boy Free Pack/ChikBoy_idle.png");
    Texture2D jogadorMovendo = LoadTexture("sprites/Chick-Boy Free Pack/ChikBoy_run.png");
    Texture2D jogadorPulando = LoadTexture("sprites/Galinha_Pulando.png");
    Texture2D jogadorDash = LoadTexture("sprites/Galinha_Dash.png");
    Texture2D jogadorParede = LoadTexture("sprites/parede.png");
    Texture2D jogadorNascer = LoadTexture("sprites/Nascer.png");
    Texture2D jogadorMorrer = LoadTexture("sprites/morrer.png");

    // Plataformas da fase
    // -- plataformas ok
    ObjetosCena plataforma[] = {

        // foi aqui q eu mexi, MAYKON
        {(Rectangle){0, 672, 100, 96}, true, BLANK},
        {(Rectangle){0, 319, 188, 32}, true, BLANK},
        {(Rectangle){20, 287, 32, 96}, true, BLANK},
        {(Rectangle){256, 383, 32, 223}, true, BLANK},
        {(Rectangle){354, 0, 29, 540}, true, BLANK},
        {(Rectangle){354, 225, 636, 29}, true, BLANK},
        {(Rectangle){670, 415, 32, 32}, true, BLANK},
        {(Rectangle){540, 510, 64, 32}, true, BLANK},
        {(Rectangle){766, 320, 32, 32}, true, BLANK},
        {(Rectangle){894, 320, 32, 32}, true, BLANK},
        {(Rectangle){1025, 364, 96, 96}, true, BLANK},
        {(Rectangle){1366, 672, 160, 64}, true, BLANK},
        {(Rectangle){1620, 608, 96, 32}, true, BLANK},
        {(Rectangle){1846, 546, 32, 32}, true, BLANK},
        {(Rectangle){1940, 514, 32, 32}, true, BLANK},
        {(Rectangle){1878, 448, 28, 32}, true, BLANK},
        {(Rectangle){1814, 416, 32, 32}, true, BLANK},
        {(Rectangle){1718, 416, 32, 32}, true, BLANK},
        {(Rectangle){1780, 318, 32, 32}, true, BLANK},
        {(Rectangle){1748, 250, 28, 28}, true, BLANK},
        {(Rectangle){1812, 250, 28, 28}, true, BLANK},
        {(Rectangle){2036, 318, 192, 64}, true, BLANK},
        {(Rectangle){2324, 0, 32, 284}, true, BLANK},
        {(Rectangle){2324, 356, 32, 364}, true, BLANK},
        {(Rectangle){2452, 0, 32, 254}, true, BLANK},
        {(Rectangle){2452, 324, 32, 396}, true, BLANK},
        {(Rectangle){2580, 0, 32, 412}, true, BLANK},
        {(Rectangle){2580, 478, 32, 364}, true, BLANK},
        {(Rectangle){2708, 0, 32, 608}, true, BLANK},
        {(Rectangle){2708, 672, 32, 364}, true, BLANK},
        {(Rectangle){2772, 640, 96, 96}, true, BLANK},
        {(Rectangle){2958, 608, 32, 32}, true, BLANK},
        {(Rectangle){3122, 578, 96, 160}, true, BLANK},
        {(Rectangle){3246, 578, 224, 32}, true, BLANK},
        {(Rectangle){3646, 606, 96, 40}, true, BLANK},
        {(Rectangle){3864, 670, 144, 64}, true, BLANK},
        {(Rectangle){4076, 198, 32, 472}, true, BLANK},
        {(Rectangle){4080, 196, 212, 32}, true, BLANK},
        {(Rectangle){4236, 198, 32, 376}, true, BLANK},
        {(Rectangle){4380, 0, 32, 600}, true, BLANK},
        {(Rectangle){4268, 700, 148, 32}, true, BLANK},
        {(Rectangle){4556, 672, 600, 64}, true, BLANK},
        {(Rectangle){5188, 492, 64, 222}, true, BLANK},
        {(Rectangle){5188, 0, 64, 428}, true, BLANK},
        {(Rectangle){5350, 150, 64, 350}, true, BLANK},
        {(Rectangle){5462, 170, 158, 80}, true, BLANK},
        {(Rectangle){5716, 712, 164, 32}, true, BLANK},
        {(Rectangle){5944, 674, 156, 64}, true, BLANK},
        {(Rectangle){6194, 640, 32, 32}, true, BLANK},
        {(Rectangle){6310, 0, 68, 600}, true, BLANK},
        {(Rectangle){6310, 672, 148, 64}, true, BLANK},
        {(Rectangle){6496, 608, 64, 32}, true, BLANK},
        {(Rectangle){6592, 544, 32, 32}, true, BLANK},
        {(Rectangle){6720, 544, 96, 32}, true, BLANK},
        {(Rectangle){6910, 552, 32, 116}, true, BLANK},
        {(Rectangle){6974, 708, 96, 32}, true, BLANK},
        {(Rectangle){7102, 644, 96, 32}, true, BLANK},
        {(Rectangle){7102, 580, 96, 32}, true, BLANK},
        {(Rectangle){7264, 554, 22, 116}, true, BLANK},
        {(Rectangle){7328, 426, 22, 116}, true, BLANK},
        {(Rectangle){7448, 420, 92, 22}, true, BLANK},
        {(Rectangle){7680, 320, 22, 92}, true, BLANK},
        {(Rectangle){7524, 326, 92, 22}, true, BLANK},
        {(Rectangle){7486, 224, 26, 92}, true, BLANK},
        {(Rectangle){7550, 196, 188, 32}, true, BLANK},
        {(Rectangle){7834, 228, 188, 32}, true, BLANK},
        {(Rectangle){8096, 0, 32, 368}, true, BLANK},
        {(Rectangle){7958, 484, 64, 32}, true, BLANK},
        {(Rectangle){8086, 546, 64, 32}, true, BLANK},
        {(Rectangle){8214, 484, 64, 32}, true, BLANK},
        {(Rectangle){8278, 516, 32, 32}, true, BLANK},
        {(Rectangle){8374, 388, 32, 96}, true, BLANK},
        {(Rectangle){8470, 388, 32, 32}, true, BLANK},
        {(Rectangle){8596, 452, 32, 32}, true, BLANK},
        {(Rectangle){8660, 452, 32, 32}, true, BLANK},
        {(Rectangle){8692, 420, 182, 32}, true, BLANK},

        // BARREIRAS DO MUNDO
        {(Rectangle){0, 0, 20, 1000}, true, BLANK},
        {(Rectangle){0, 0, 8912, 16}, true, BLANK},
        {(Rectangle){8912, 0, 32, 732}, true, BLANK},

    };

    int numPlataformas = sizeof(plataforma) / sizeof(plataforma[0]);

    //-- plataformas de armadilhas

    ObjetosCena plataformaArmadihas[] = {

        //{(Rectangle){0, 710, 1000, 20}, true, BLANK},
        {(Rectangle){188, 319, 6, 32}, true, BLANK},
        {(Rectangle){256, 650, 384, 16}, true, BLANK},
        {(Rectangle){614, 450, 16, 224}, true, BLANK},
        {(Rectangle){710, 396, 64, 16}, true, BLANK},
        {(Rectangle){742, 360, 16, 64}, true, BLANK},
        {(Rectangle){770, 332, 128, 16}, true, BLANK},
        {(Rectangle){770, 256, 224, 16}, true, BLANK},
        {(Rectangle){1432, 128, 28, 492}, true, BLANK},
        {(Rectangle){1756, 550, 26, 26}, true, BLANK},
        {(Rectangle){1944, 506, 28, 10}, true, BLANK},
        {(Rectangle){1880, 482, 24, 28}, true, BLANK},
        {(Rectangle){1818, 410, 42, 42}, true, BLANK},
        {(Rectangle){1784, 322, 32, 32}, true, BLANK},
        {(Rectangle){1752, 260, 32, 32}, true, BLANK},
        {(Rectangle){1808, 260, 32, 32}, true, BLANK},
        {(Rectangle){1886, 230, 18, 18}, true, BLANK},
        {(Rectangle){1916, 166, 18, 18}, true, BLANK},
        {(Rectangle){1948, 198, 18, 18}, true, BLANK},
        {(Rectangle){1980, 230, 18, 18}, true, BLANK},
        {(Rectangle){2012, 262, 18, 18}, true, BLANK},
        {(Rectangle){2900, 672, 192, 96}, true, BLANK},
        {(Rectangle){3540, 555, 18, 542}, true, BLANK},
        {(Rectangle){3540, 358, 18, 146}, true, BLANK},
        {(Rectangle){4376, 0, 10, 600}, true, BLANK},
        {(Rectangle){4262, 256, 8, 32}, true, BLANK},
        {(Rectangle){4262, 352, 8, 32}, true, BLANK},
        {(Rectangle){4262, 448, 8, 32}, true, BLANK},
        {(Rectangle){4262, 544, 8, 32}, true, BLANK},
        {(Rectangle){4306, 296, 18, 18}, true, BLANK},
        {(Rectangle){4344, 426, 18, 18}, true, BLANK},
        {(Rectangle){4312, 550, 18, 18}, true, BLANK},
        {(Rectangle){4780, 640, 26, 32}, true, BLANK},
        {(Rectangle){4844, 670, 26, 10}, true, BLANK},
        {(Rectangle){4944, 646, 18, 18}, true, BLANK},
        {(Rectangle){4944, 582, 18, 18}, true, BLANK},
        {(Rectangle){5014, 600, 36, 36}, true, BLANK},
        {(Rectangle){5082, 670, 50, 10}, true, BLANK},
        {(Rectangle){5320, 248, 48, 80}, true, BLANK},
        {(Rectangle){5250, 100, 48, 80}, true, BLANK},
        {(Rectangle){5706, 142, 64, 70}, true, BLANK},
        {(Rectangle){5738, 32, 64, 70}, true, BLANK},
        {(Rectangle){5614, 274, 32, 32}, true, BLANK},
        {(Rectangle){5614, 274, 32, 32}, true, BLANK},
        {(Rectangle){5658, 322, 36, 36}, true, BLANK},
        {(Rectangle){5850, 332, 64, 64}, true, BLANK},
        {(Rectangle){5760, 428, 64, 64}, true, BLANK},
        {(Rectangle){5624, 624, 64, 64}, true, BLANK},
        {(Rectangle){6162, 682, 96, 50}, true, BLANK},
        {(Rectangle){6650, 448, 48, 48}, true, BLANK},
        {(Rectangle){6650, 598, 48, 48}, true, BLANK},
        {(Rectangle){6876, 410, 36, 100}, true, BLANK},
        {(Rectangle){6942, 506, 26, 26}, true, BLANK},
        {(Rectangle){7198, 644, 32, 32}, true, BLANK},
        {(Rectangle){7556, 416, 100, 28}, true, BLANK},
        {(Rectangle){7384, 322, 96, 28}, true, BLANK},
        {(Rectangle){7756, 170, 48, 52}, true, BLANK},
        {(Rectangle){7864, 64, 48, 48}, true, BLANK},
        {(Rectangle){7916, 200, 42, 32}, true, BLANK},
        {(Rectangle){8032, 376, 48, 48}, true, BLANK},
        {(Rectangle){8032, 528, 48, 48}, true, BLANK},
        {(Rectangle){8184, 508, 32, 32}, true, BLANK},
        {(Rectangle){8278, 420, 32, 32}, true, BLANK},
        {(Rectangle){842, 484, 32, 32}, true, BLANK},
        {(Rectangle){8342, 484, 32, 32}, true, BLANK},
        {(Rectangle){8534, 420, 32, 32}, true, BLANK},

        // FUNDO DA MORTE
        {(Rectangle){0, 732, 9000, 16}, true, BLANK},

    };

    int numPlataformasArmadilhas = sizeof(plataformaArmadihas) / sizeof(plataformaArmadihas[0]);

    // -- plataformas de checkpoint/vitória

    ObjetosCena plataformaCheckpoint[] = {

        {(Rectangle){540, 450, 20, 100}, true, BLANK},
        {(Rectangle){3343, 500, 20, 100}, true, BLANK},
        {(Rectangle){4648, 570, 20, 100}, true, BLANK},
        {(Rectangle){5496, 100, 20, 100}, true, BLANK},
        {(Rectangle){6400, 570, 20, 100}, true, BLANK},
        {(Rectangle){7620, 100, 20, 100}, true, BLANK},

    };

    int numPlataformasCheckpoint = sizeof(plataformaCheckpoint) / sizeof(plataformaCheckpoint[0]);

    ObjetosCena plataformaVitoria[] = {

        {(Rectangle){8800, 350, 20, 100}, true, BLANK},

        //  {(Rectangle){100, 0, 20,100000000000000000000 }, true, PURPLE},

    };

    int numPlataformasVitoria = sizeof(plataformaVitoria) / sizeof(plataformaVitoria[0]);

    while (!WindowShouldClose())
    {
        // -- Lógica para Audio ---

        if (!vitoria)
        {

            StopSound(somVitoria);
            UpdateMusicStream(somTemaC);
        }

        // --- Lógica do Jogo ---

        cameraPrincipal(&cameraPlayer);

        float tempoDoFrame = GetFrameTime();

        tempoJogo += tempoDoFrame;

        // --- Lógica de Morte/Renascimento ---
        if (vida == -1) // Jogador morreu
        {

            tempoAnimacaoMorte += tempoDoFrame;

            if (tempoAnimacaoMorte >= duracaoAnimacaoMorte)
            {
                vida = 1;
                PlaySound(somNascer);
                playerX = playerCkeckpointX;
                playerY = playerCkeckpointY;
                tempoAnimacaoNascer = 0.0f; // Reseta o tempo de nascimento
                velocidadePuloY = 0.0f;     // Reseta a velocidade do pulo
            }
        }
        else if (vida == 1) // Jogador está nascendo
        {

            tempoAnimacaoNascer += tempoDoFrame;
            if (tempoAnimacaoNascer >= duracaoAnimacaoNascer)
            {
                vida = 0;                   // Transiciona para "vivendo"
                podeControlarPlayer = true; // Permite o controle do jogador
            }
        }

        if (podeControlarPlayer)
        {

            if (IsKeyPressed(KEY_LEFT_SHIFT) && IsKeyDown(KEY_UP) && !dashAtivo && !grudandoParede && !dashVerticalUsadoNoAr)
            {
                dashVertical = true;
                dashAtivo = true;
                dashTempo = dashDuracao;
                dashVerticalUsadoNoAr = true; // Marca que o dash vertical foi usado

                PlaySound(somDash);
            }

            else if (IsKeyPressed(KEY_LEFT_SHIFT) && (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) && !dashAtivo && !grudandoParede && dashHorizontalUsosNoAr < dashHorizontalMaxUsos)
            {
                dashVertical = false;
                dashAtivo = true;
                dashTempo = dashDuracao;
                dashHorizontalUsosNoAr++;
                PlaySound(somDash);
            }

            if (dashAtivo)
            {

                dashTempo -= tempoDoFrame;

                if (dashVertical)
                {
                    velocidadePuloY = -1;

                    playerY -= dashVelocidade * tempoDoFrame;
                }
                else
                {
                    velocidadePuloY = 0;

                    if (direcao)
                    {
                        playerX += dashVelocidade * tempoDoFrame;
                    }
                    else
                    {
                        playerX -= dashVelocidade * tempoDoFrame;
                    }
                }

                if (dashTempo <= 0.0f)
                {
                    gravidadeAceleracao = 800.0f;
                    dashAtivo = false;
                    dashVertical = false;
                }
            }
            else
            {
                pulo(tempoDoFrame, somPulo);
                movimentoHorizontal(tempoDoFrame);
            }
        }

        // -- DETECTAR Colisão

        grudandoParede = false;

        bool emChao = false;

        Rectangle playerRect = {playerX, playerY, (float)jogadorParado.width, (float)(jogadorParado.height / 6)};

        for (int i = 0; i < numPlataformas; i++)
        {
            // Colisão geral com a plataforma
            if (CheckCollisionRecs(playerRect, plataforma[i].retangulo) && plataforma[i].solido)
            {
                // Verifica se a colisão é por cima
                if (playerRect.y + playerRect.height - velocidadePuloY * tempoDoFrame <= plataforma[i].retangulo.y && velocidadePuloY >= 0)
                {
                    playerY = plataforma[i].retangulo.y - playerRect.height;
                    velocidadePuloY = 0.0f;
                    emChao = true;
                    podePular = true;
                    dashVerticalUsadoNoAr = false; // Reseta o dash vertical ao tocar o chão
                }
                // Colisão por baixo
                else if (
                    playerRect.y < plataforma[i].retangulo.y + plataforma[i].retangulo.height &&
                    playerRect.y > plataforma[i].retangulo.y + plataforma[i].retangulo.height - 100 && // margem de tolerância (100 pixels)
                    velocidadePuloY < 0)
                {
                    playerY = plataforma[i].retangulo.y + plataforma[i].retangulo.height;
                    velocidadePuloY = -1;
                }

                // colisao direita

                else if (playerRect.x + playerRect.width > plataforma[i].retangulo.x &&
                         playerRect.x < plataforma[i].retangulo.x)
                {
                    playerX = plataforma[i].retangulo.x - playerRect.width;
                    grudandoParede = true;
                    if (dashAtivo)
                    {

                        dashAtivo = false;
                    }
                }

                // Colisão pela esquerda (player encosta na lateral direita da plataforma)
                else if (playerRect.x < plataforma[i].retangulo.x + plataforma[i].retangulo.width &&
                         playerRect.x + playerRect.width > plataforma[i].retangulo.x + plataforma[i].retangulo.width)
                {
                    playerX = plataforma[i].retangulo.x + plataforma[i].retangulo.width;
                    grudandoParede = true;
                    if (dashAtivo)
                    {

                        dashAtivo = false;
                    }
                }
            }
        }

        // Atualiza o estado do personagem
        if (!emChao && !grudandoParede)
        {
            // Se não está no chão e não está grudando na parede, aplica gravidade
            gravidadeAceleracao = 800.0f;
            podePular = false;
        }
        else if (!emChao && grudandoParede)
        {

            gravidadeAceleracao = 1800.0f;
            velocidadePuloY = -1;
            podePular = true;
            dashVerticalUsadoNoAr = false; // Reseta o dash vertical ao grudar na parede

            dashHorizontalUsosNoAr = 0;

            if (IsKeyDown(KEY_UP))
            {
                velocidadePuloY = -1;

                playerY -= 200.0f * tempoDoFrame;
            }
        }
        else
        {
            // Se está no chão, reseta a velocidade vertical e permite pular
            velocidadePuloY = 0.0f;
            podePular = true;
            dashVerticalUsadoNoAr = false; // Reseta o dash vertical ao tocar o chão

            dashHorizontalUsosNoAr = 0;
        }

        // --- colisao com armadilhas/Respaw
        for (int i = 0; i < numPlataformasArmadilhas; i++)
        {
            if (CheckCollisionRecs(playerRect, plataformaArmadihas[i].retangulo) && plataformaArmadihas[i].solido)
            {
                if (vida == 0)
                { // Só morre se estiver vivo
                    vida = -1;
                    PlaySound(somMorrendo); // Sinaliza que o jogador morreu

                    podeControlarPlayer = false; // Desativa o controle
                    tempoAnimacaoMorte = 0.0f;   // Reinicia o tempo da animação de morte

                    // --- reset de dash ---

                    dashAtivo = false;
                    dashTempo = 0.0f;
                    dashVertical = false;
                    dashVerticalUsadoNoAr = false;
                    dashHorizontalUsosNoAr = 0;
                }
            }
        }

        // -- colisao com respanw

        for (int i = 0; i < numPlataformasCheckpoint; i++)
        {
            if (CheckCollisionRecs(playerRect, plataformaCheckpoint[i].retangulo) && plataformaCheckpoint[i].solido)
            {
                playerCkeckpointX = plataformaCheckpoint[i].retangulo.x;
                playerCkeckpointY = plataformaCheckpoint[i].retangulo.y;
            }
        }

        // -- colisao vitoria

        for (int i = 0; i < numPlataformasVitoria; i++)
        {
            if (CheckCollisionRecs(playerRect, plataformaVitoria[i].retangulo) && plataformaVitoria[i].solido)
            {
                if (!vitoria)
                {
                    vitoria = true;
                    tempoRun = tempoJogo;
                    PlaySound(somVitoria);
                }
            }
        }

        // Controle de estado de animacao

        int proximaAnimacao = -1;

        if (vida == -1)
        { // Jogador está morrendo
            proximaAnimacao = 6;
        }
        else if (vida == 1)
        { // Jogador está nascendo
            proximaAnimacao = 5;
        }
        else if (dashAtivo)
        {

            proximaAnimacao = 3;
        }
        else if (!emChao && grudandoParede)
        {

            proximaAnimacao = 4;
        }

        else if (!emChao)
        {

            proximaAnimacao = 2;
        }
        else
        {

            if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
            {
                proximaAnimacao = 1;
            }
            else
            {

                proximaAnimacao = 0;
            }
        }

        if (proximaAnimacao != estadoAnimacao)
        {
            frameAtual = 0;
            contadorFrames = 0;
            estadoAnimacao = proximaAnimacao;
        }

        // --- Desenho na Tela ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        BeginMode2D(cameraPlayer);

        desenhaCenario(background);

        // Desenha plataformas

        // -- plataformas ok

        for (int i = 0; i < numPlataformas; i++)
        {
            DrawRectangleRec(plataforma[i].retangulo, plataforma[i].cor);
        }

        //-- plataformas armadilhas

        for (int i = 0; i < numPlataformasArmadilhas; i++)
        {
            DrawRectangleRec(plataformaArmadihas[i].retangulo, plataformaArmadihas[i].cor);
        }

        // -- plataforma Checkpoint
        for (int i = 0; i < numPlataformasCheckpoint; i++)
        {
            DrawRectangleRec(plataformaCheckpoint[i].retangulo, plataformaCheckpoint[i].cor);
        }

        // -- plataforma vitoria

        for (int i = 0; i < numPlataformasVitoria; i++)
        {
            DrawRectangleRec(plataformaVitoria[i].retangulo, plataformaVitoria[i].cor);
        }

        // Chama animacao do player
        if (estadoAnimacao == 6)
        {
            personagemMorrer(jogadorMorrer);
        }
        else if (estadoAnimacao == 5)
        {
            personagemNascer(jogadorNascer);
        }
        else if (estadoAnimacao == 4)
        {

            persoangemParede(jogadorParede);
        }
        else if (estadoAnimacao == 3)
        {
            personagemDash(jogadorDash);
        }
        else if (estadoAnimacao == 2)
        {
            personagemPulando(jogadorPulando);
        }
        else if (estadoAnimacao == 1)
        {
            personagemMovendo(jogadorMovendo);
        }
        else
        {
            personagemParado(jogadorParado);
        }

        DrawRectangleLines(playerX, playerY, (float)jogadorParado.width, (float)jogadorParado.height / 6, BLANK);

        EndMode2D();

        // -- UI

        const char *textoTempo = TextFormat("%.1f", tempoJogo);
        int larguraTexto = MeasureText(textoTempo, 30);
        int posicaoXTexto = (tamanhoTelaX - larguraTexto) / 2;
        DrawText(textoTempo, posicaoXTexto, 20, 30, DARKBLUE);

        /* Debug Visual para o estado do player
        const char *estadoPlayerTexto;
        Color estadoPlayerCor;

        if (vida == -1)
        {
            estadoPlayerTexto = "Estado: Morreu!";
            estadoPlayerCor = DARKPURPLE;
        }
        else if (vida == 1)
        {
            estadoPlayerTexto = "Estado: Nascendo...";
            estadoPlayerCor = SKYBLUE;
        }
        else if (emChao)
        {
            estadoPlayerTexto = "Estado: No Chão";
            estadoPlayerCor = GREEN;
        }
        else
        {
            estadoPlayerTexto = "Estado: No Ar (Pulando/Caindo)";
            estadoPlayerCor = ORANGE;
        }
        DrawText(estadoPlayerTexto, 10, 10, 20, estadoPlayerCor); // Posição (10, 10) na tela

        DEBUG: Mostrar posição do player no canto superior direito
        char posicaoTexto[50];
        snprintf(posicaoTexto, sizeof(posicaoTexto), "X: %.1f | Y: %.1f", playerX, playerY);
        int tmanhoT = MeasureText(posicaoTexto, 20);
        DrawText(posicaoTexto, tamanhoTelaX - tmanhoT - 10, 10, 20, DARKGRAY);
        */

        if (IsKeyPressed(KEY_SPACE))
        {
            tempoJogo = 0;
            podeControlarPlayer = true;
            FundoMenu = LoadTexture("sprites/Fundo_Nulo.png");
        };

        /* // DEBUG: Mostrar velocidadePuloY no canto superior direito
         char textoVelocidade[50];
         snprintf(textoVelocidade, sizeof(textoVelocidade), "velocidadePuloY: %.2f", velocidadePuloY);
         int larguraVelocidade = MeasureText(textoVelocidade, 20);
         DrawText(textoVelocidade, tamanhoTelaX - larguraVelocidade - 10, 40, 20, MAROON);
         DrawTexture(FundoMenu, 0, 0, WHITE);
         */
        if (vitoria)
        {

            desenharRanking(fundoRank);

            /*if (FileExists("ranking.txt"))
            {
                DrawText("Último ranking salvo:", 800, 580, 20, DARKGRAY);
                char *fileData = LoadFileText("ranking.txt");
                DrawText(fileData, 800, 600, 20, BLACK);
                UnloadFileText(fileData);
            }*/
        }

        EndDrawing();
    }

    // Descarrega a textura antes de fechar
    UnloadTexture(jogadorParado);
    UnloadTexture(jogadorMovendo);
    UnloadTexture(jogadorPulando);
    UnloadTexture(jogadorDash);
    UnloadTexture(jogadorParede);
    UnloadTexture(background);
    UnloadTexture(jogadorNascer);
    UnloadTexture(jogadorMorrer);

    CloseWindow();

    return 0;
}