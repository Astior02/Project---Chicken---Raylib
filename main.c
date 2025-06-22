#include "raylib.h"
#include <math.h>
#include <stdio.h>

float tempoJogo = 0.0f;

int tamanhoTelaX = 1280;
int tamanhoTelaY = 720;
float playerX = 30;
float playerY = 500;
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
float dashVelocidade = 200.0f;
float dashTempo;
float dashDuracao = 0.4f;
bool dashVertical = false;

// -- variáveis para colisao
bool colisaoDireita = false;
bool colisaoEsquerda = false;
bool grudandoParede = false;
float velocidadeDescidaParede = 100.0f;

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

void persoangemParede(Texture2D personagemParede){

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

void pulo(float tempo)
{
    velocidadePuloY += gravidadeAceleracao * tempo;

    if ((IsKeyPressed(KEY_UP) || IsKeyDown(KEY_UP)) && podePular)
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
    Texture2D jogadorParede = LoadTexture("sprites/parede.png");



    // Plataformas da fase

    ObjetosCena plataforma[] = {

        {(Rectangle){0, 0, 20, 1000}, true, BLUE},
        {(Rectangle){0, 710, 1000, 20}, true, BLUE},

        {(Rectangle){0, 672, 100, 20}, true, BLUE},
        {(Rectangle){150, 200, 50, 1000}, true, RED},
        {(Rectangle){300, 200, 50, 1000}, true, RED},
        {(Rectangle){600, 500, 50, 1000}, true, BLUE},

        {(Rectangle){300, 672, 200, 20}, true, BLUE},
        {(Rectangle){600, 672, 150, 20}, true, BLUE},
        {(Rectangle){850, 672, 100, 20}, true, BLUE}

    };

    int numPlataformas = sizeof(plataforma) / sizeof(plataforma[0]);

    while (!WindowShouldClose())
    {
        // --- Lógica do Jogo ---

        cameraPrincipal(&cameraPlayer);

        float tempoDoFrame = GetFrameTime();

        tempoJogo += tempoDoFrame;

        if (IsKeyPressed(KEY_LEFT_SHIFT) && !dashAtivo && !grudandoParede)
        {

            if (IsKeyDown(KEY_UP))
            {
                dashVertical = true;
                dashAtivo = true;
                dashTempo = dashDuracao;
            }
            else if ((IsKeyDown(KEY_RIGHT)) || (IsKeyDown(KEY_LEFT)))
            {
                dashVertical = false;
                dashAtivo = true;
                dashTempo = dashDuracao;
            }
        }

        if (dashAtivo)
        {

            dashTempo -= tempoDoFrame;

            if (dashVertical)
            {

                playerY -= dashVelocidade * tempoDoFrame;
            }
            else
            {

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
                dashAtivo = false;
                dashVertical = false;
            }
        }
        else
        {
            pulo(GetFrameTime());
            movimentoHorizontal(GetFrameTime());
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
                }
                // Colisão por baixo
                else if (playerRect.y - velocidadePuloY * tempoDoFrame >= plataforma[i].retangulo.y + plataforma[i].retangulo.height && velocidadePuloY < 0)
                {
                    playerY = plataforma[i].retangulo.y + plataforma[i].retangulo.height;
                    velocidadePuloY = 0.0f;
                }

                // Colisão pela direita
                else if (playerRect.x + playerRect.width >= plataforma[i].retangulo.x &&
                         playerRect.x < plataforma[i].retangulo.x &&
                         playerRect.y + playerRect.height > plataforma[i].retangulo.y &&
                         playerRect.y < plataforma[i].retangulo.y + plataforma[i].retangulo.height)
                {

                    if (dashAtivo)
                    {

                        dashAtivo = false;
                    }

                    playerX = plataforma[i].retangulo.x - playerRect.width;

                    grudandoParede = true;
                }

                // colisao pela esquerda

                else if (playerRect.x < plataforma[i].retangulo.x + plataforma[i].retangulo.width &&
                         playerRect.x + playerRect.width > plataforma[i].retangulo.x + plataforma[i].retangulo.width - (velocidadePuloY * tempoDoFrame) &&
                         playerRect.y + playerRect.height > plataforma[i].retangulo.y + (velocidadePuloY * tempoDoFrame) &&
                         playerRect.y < plataforma[i].retangulo.y + plataforma[i].retangulo.height - (velocidadePuloY * tempoDoFrame))
                {

                    if (dashAtivo)
                    {

                        dashAtivo = false;
                    }

                    playerX = plataforma[i].retangulo.x + plataforma[i].retangulo.width;

                    grudandoParede = true;
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
            velocidadePuloY = 0.0f;
            podePular = true;
        }
        else
        {
            // Se está no chão, reseta a velocidade vertical e permite pular
            velocidadePuloY = 0.0f;
            podePular = true;
        }

        //  Controle de estado de animacao

        int proximaAnimacao = -1;

        if (dashAtivo)
        {

            proximaAnimacao = 3;
        }
        else if (!emChao && grudandoParede){

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

        DrawText("PROJECT CHICKEN", 400, 50, 30, RED);

        BeginMode2D(cameraPlayer);

        desenhaCenario(background, solo, armadilhas);

        for (int i = 0; i < numPlataformas; i++)
        {
            DrawRectangleRec(plataforma[i].retangulo, plataforma[i].cor);
        }

        // Chama animacao do player
        if (estadoAnimacao == 4){

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
        { // estadoAnimacao == 0 (parado) ou qualquer outro caso padrão
            personagemParado(jogadorParado);
        }

        DrawRectangleLines(playerX, playerY, (float)jogadorParado.width, (float)jogadorParado.height / 6, RED);

        EndMode2D();

        // -- UI

        const char *textoTempo = TextFormat("%.1f", tempoJogo);
        int larguraTexto = MeasureText(textoTempo, 30);
        int posicaoXTexto = (tamanhoTelaX - larguraTexto) / 2;
        DrawText(textoTempo, posicaoXTexto, 20, 30, DARKBLUE);

        // Debug Visual para o estado do player
        const char *estadoPlayerTexto;
        Color estadoPlayerCor;

        if (emChao)
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

        EndDrawing();
    }

    // Descarrega a textura antes de fechar
    UnloadTexture(jogadorParado);
    UnloadTexture(jogadorMovendo);
    UnloadTexture(jogadorPulando);
    UnloadTexture(jogadorDash);
    UnloadTexture(background);
    UnloadTexture(solo);
    UnloadTexture(armadilhas);
    CloseWindow();

    return 0;
}