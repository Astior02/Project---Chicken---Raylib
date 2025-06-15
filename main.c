#include "raylib.h"

const int tamanhoTelaX = 1080;
const int tamanhoTelaY = 720;
float playerX = 20;
float playerY = 20;
bool direcao = true; // true direita, false esquerda

int frameAtual = 0;
int contadorFrames = 0;
int frameVelocidade = 8;

void animarMovimento(int framesTotais, int alturaFrame)
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

    int alturaFrame = playerParado.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame);

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

    DrawTexturePro(playerParado, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void personagemMovendo(Texture2D playerMovendo)
{

    int totalFrames = 10;

    int alturaFrame = playerMovendo.height / totalFrames;

    animarMovimento(totalFrames, alturaFrame);

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

        if(!direcao){
            origem.width *= -1;
        }

    DrawTexturePro(playerMovendo, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

void movimento()
{
    float playerVelocidade = 1;

    if (IsKeyDown(KEY_RIGHT))
    {
        playerX += playerVelocidade;
        direcao = true;
    }

    if (IsKeyDown(KEY_LEFT))
    {
        playerX -= playerVelocidade;
        direcao = false;
    }
}

int main(void)
{
    InitWindow(tamanhoTelaX, tamanhoTelaY, "Project Chicken");
    SetTargetFPS(60);

    Texture2D jogadorParado = LoadTexture("sprites/Chick-Boy Free Pack/ChikBoy_idle.png");
    Texture2D jogadorMovendo = LoadTexture("sprites/Chick-Boy Free Pack/ChikBoy_run.png");

    

    while (!WindowShouldClose())
    {
        // --- Lógica do Jogo ---

        // --- Desenho na Tela ---
        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
        {

            movimento();

            
            personagemMovendo(jogadorMovendo);
        }
        else
        {

            personagemParado(jogadorParado);
        }

        EndDrawing();
    }

    // Descarrega a textura antes de fechar
    UnloadTexture(jogadorParado);
    UnloadTexture(jogadorMovendo);
    CloseWindow();

    return 0;
}