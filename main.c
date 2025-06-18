#include "raylib.h"
#include <math.h>


float tempoJogo = 0.0f;

int tamanhoTelaX = 1280;
int tamanhoTelaY = 720;
float playerX = 30;
float playerY = 640;
bool direcao = true; // true direita, false esquerda
float playerVelocidade = 200.0f;

// -- variáveis para pulo

float velocidadePuloY = 0.0f;
bool podePular = true;
float focarPuloInicial = -400.0f;
float gravidadeAceleracao = 800.0f;

// -- variáveis de animação

int frameAtual = 0;
int contadorFrames = 0;
int estadoAnimacao = -1;

// -- variáveis para dash

bool dashAtivo = false;
float dashVelocidade = 300.0f;
float dashTempo;
float dashDuracao = 0.7f;

typedef struct
{
    Rectangle retangulo;

    // Texture2D sprite;
    bool solido;
    Color cor;

} ObjetosCena;

void desenhaCenario(Texture2D fundo, Texture2D solo, Texture2D armadilha)
{

    DrawTexture(fundo, 0, 0, WHITE);
    DrawTexture(solo, 0, 0, WHITE);

    // -- DESENHA FUNDO

    Rectangle origem = {0.0f, 0.0f, (float)fundo.width, (float)fundo.height};
    Rectangle destino = {0.0f, 0.0f, tamanhoTelaX, tamanhoTelaY};
    DrawTexturePro(fundo, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);

    // -- DESENHA SOLO

    Rectangle origemSolo = {0.0f, 0.0f, (float)solo.width, (float)solo.height};
    Rectangle destinoSolo = {0.0f, 0.0f, tamanhoTelaX, tamanhoTelaY};
    DrawTexturePro(solo, origemSolo, destinoSolo, (Vector2){0, 0}, 0.0f, WHITE);

    // -- DESENHA ARMADILHAS

    Rectangle origemArmadilha = {0.0f, 0.0f, (float)armadilha.width, (float)armadilha.height};
    Rectangle destinoArmadilha = {0.0f, 0.0f, tamanhoTelaX, tamanhoTelaY};
    DrawTexturePro(armadilha, origemArmadilha, destinoArmadilha, (Vector2){0, 0}, 0.0f, WHITE);
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

bool VerificarColisao(Rectangle objeto1, Rectangle objeto2)
{

    return (objeto1.x < objeto2.x + objeto2.width &&
            objeto1.x + objeto1.width > objeto2.x &&
            objeto1.y < objeto2.y + objeto2.height &&
            objeto1.y + objeto1.height > objeto2.y);
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

    camera->target = (Vector2){fmaxf(inicioFase, playerX), tamanhoTelaY / 2.0f}; // segue o player
    camera->offset = (Vector2){tamanhoTelaX / 2.0f, tamanhoTelaY / 2.0f};        // Centraliza
    camera->rotation = 0.0f;
    camera->zoom = 1.0f;
}

float gravidadeMundo()
{
    float aceleracao = GetFrameTime();

    return aceleracao;
}

void pulo(float tempo)
{
    velocidadePuloY += gravidadeAceleracao * tempo;

    if (IsKeyPressed(KEY_UP) && podePular)
    {

        velocidadePuloY = focarPuloInicial;
    }

    playerY += velocidadePuloY * tempo;
}

int main(void)
{

    Camera2D cameraPlayer;

    InitWindow(tamanhoTelaX, tamanhoTelaY, "Project Chicken");
    SetTargetFPS(60);

    // -- CARREGAR TEXTURA DO CENÁRIO

    Texture2D background = LoadTexture("sprites/Cenario/cenario_1.png");
    Texture2D solo = LoadTexture("sprites/Cenario/fase_blocos_1.png");
    Texture2D armadilhas = LoadTexture("sprites/Cenario/fase_espinhos_1.png");

    // -- CARREGAR SPRITE DO PLAYER

    Texture2D jogadorParado = LoadTexture("sprites/Chick-Boy Free Pack/ChikBoy_idle.png");
    Texture2D jogadorMovendo = LoadTexture("sprites/Chick-Boy Free Pack/ChikBoy_run.png");
    Texture2D jogadorPulando = LoadTexture("sprites/Galinha_Pulando.png");
    Texture2D jogadorDash = LoadTexture("sprites/Galinha_Dash.png");

    while (!WindowShouldClose())
    {
        // --- Lógica do Jogo ---

        cameraPrincipal(&cameraPlayer);

        float tempoDoFrame = GetFrameTime();

        tempoJogo += tempoDoFrame;

        if (IsKeyPressed(KEY_LEFT_SHIFT) && !dashAtivo)
        {
            dashAtivo = true;

            dashTempo = dashDuracao;
        }

        if (dashAtivo)
        {

            dashTempo -= tempoDoFrame;

            if (direcao)
            {
                playerX += dashVelocidade * tempoDoFrame;
            }
            else
            {
                playerX -= dashVelocidade * tempoDoFrame;
            }

            if (dashTempo <= 0.0f)
            {
                dashAtivo = false;
            }
        }
        else
        {
            pulo(gravidadeMundo());
            movimentoHorizontal(gravidadeMundo());
        }

        // --- Desenho na Tela ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawText("PROJECT CHICKEN", 400, 50, 30, RED);

        BeginMode2D(cameraPlayer);

        desenhaCenario(background, solo, armadilhas);

        ObjetosCena plataforma[] = {

            {(Rectangle){0, 672, 100, 20}, true},
            {(Rectangle){300, 200, 50, 100}, true}};

        DrawRectangleRec(plataforma[0].retangulo, BLUE);

        float chaoTemporarioY = 650; //

        if (playerY >= chaoTemporarioY)
        {
            playerY = chaoTemporarioY;
            velocidadePuloY = 0.0f;
            podePular = true;
            gravidadeAceleracao = 0.0f;
        }
        else
        {
            gravidadeAceleracao = 800.0f;
            podePular = false;
        }

        // -- logica controle de estados de animacao

        int proximaAnimacao = -1;

        if (dashAtivo)
        {

            proximaAnimacao = 3;
            personagemDash(jogadorDash);
        }

        else if (playerY < chaoTemporarioY)
        {

            proximaAnimacao = 2;
            personagemPulando(jogadorPulando);
        }

        else if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
        {

            proximaAnimacao = 1;
            personagemMovendo(jogadorMovendo);
        }

        else
        {
            proximaAnimacao = 0;
            personagemParado(jogadorParado);
        }

        if (proximaAnimacao != estadoAnimacao)
        {

            frameAtual = 0;
            contadorFrames = 0;
            estadoAnimacao = proximaAnimacao;
        }

        EndMode2D();

        // -- UI


        const char* textoTempo = TextFormat ("%.1f", tempoJogo);
        int larguraTexto = MeasureText(textoTempo,30);
        int posicaoXTexto = (tamanhoTelaX - larguraTexto)/2;
        DrawText(textoTempo, posicaoXTexto, 20, 30, DARKBLUE);

        EndDrawing();
    }

    // Descarrega a textura antes de fechar
    UnloadTexture(jogadorParado);
    UnloadTexture(jogadorMovendo);
    UnloadTexture(background);
    UnloadTexture(solo);
    UnloadTexture(armadilhas);
    CloseWindow();

    return 0;
}