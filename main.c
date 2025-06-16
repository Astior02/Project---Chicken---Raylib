#include "raylib.h"

int tamanhoTelaX = 1080;
int tamanhoTelaY = 720;
float playerX = 20;
float playerY = 20;
bool direcao = true; // true direita, false esquerda
float playerVelocidade = 2.5;

int frameAtual = 0;
int contadorFrames = 0;
int frameVelocidade = 6;

typedef struct
{
    Rectangle retangulo;

    // Texture2D sprite;
    bool solido;
    Color cor;

} ObjetosCena;

void desenhaCenario()
{

    ObjetosCena paredes[] = {

        {(Rectangle){100, 20, 50, 50}, true},
        {(Rectangle){300, 200, 50, 100}, true}

    };

    DrawRectangleRec(paredes[0].retangulo, BLUE);
}

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

    if (!direcao)
    {
        origem.width *= -1;
    }

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

    if (!direcao)
    {
        origem.width *= -1;
    }

    DrawTexturePro(playerMovendo, origem, destino, (Vector2){0, 0}, 0.0f, WHITE);
}

bool VerificarColisao(Rectangle objeto1, Rectangle objeto2)
{

    return (objeto1.x < objeto2.x + objeto2.width &&
            objeto1.x + objeto1.width > objeto2.x &&
            objeto1.y < objeto2.y + objeto2.height &&
            objeto1.y + objeto1.height > objeto2.y);
}

void movimento()
{
    

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

void cameraPrincipal(Camera2D camera)
{

    camera.target = (Vector2){playerX, playerY};                         // segue o player
    camera.offset = (Vector2){tamanhoTelaX / 2.0f, tamanhoTelaY / 2.0f}; // Centraliza
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

int main(void)
{

    Camera2D cameraPlayer;

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

        DrawText("PROJECT CHICKEN", 400, 50, 30, RED);

        if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_RIGHT))
        {

            movimento();
            cameraPrincipal(cameraPlayer);
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