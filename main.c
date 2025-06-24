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
float playerVelocidade = 200.0f;
int vida = 0; // -1 morreu, 0 vivendo, 1 nasceu

int playerCkeckpointX = 30, playerCkeckpointY = 500;

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
bool podeControlarPlayer = true; // Controla se o jogador pode se mover


// -- variáveis para dash

bool dashAtivo = false;
float dashVelocidade = 200.0f;
float dashTempo;
float dashDuracao = 0.4f;
bool dashVertical = false;
bool dashVerticalUsadoNoAr = false; 

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

void desenhaCenario(Texture2D fundo)
{

   // DrawTexture(fundo, 0, 0, WHITE);
    //DrawTexture(solo, 0, 0, WHITE);

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

    camera->target = (Vector2){fmaxf(inicioFase, playerX), tamanhoTelaY / 2.0f}; // segue o player
    camera->offset = (Vector2){tamanhoTelaX / 2.0f, tamanhoTelaY / 2.0f};          // Centraliza
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

// ranking
typedef struct
{
    char nomeRaking[32];
    int pontuacao;
} PosicaoRank;

#define MAX_ENTRADA_RANK 10
PosicaoRank ranks[MAX_ENTRADA_RANK];

int rankOcupados = 0;

const char *rank = "ranking.txt";

void CarregarRank(){

    FILE *file = fopen(rank,"r");

    rankOcupados = 0;

    while (fscanf(file, "%31s %d",ranks[rankOcupados].nomeRaking, &ranks[rankOcupados].pontuacao) == 2)
    {
        rankOcupados++;

        if (rankOcupados >= MAX_ENTRADA_RANK ){

            break;
        }
    }

    fclose(file);

}

void salvarRank(){

    FILE *file = fopen(rank, "w");

    for (int i=0; i < rankOcupados; i++){
        fprintf(file, "%s %d\n", ranks[i].nomeRaking, ranks[i].pontuacao);
    }

    fclose(file);
}

void ordenarRanking(){

    for (int i=0; i< rankOcupados - 1; i++){

        for (int j=0; j < rankOcupados - i - 1; j++){

            if (ranks[j].pontuacao < ranks[j+1].pontuacao) {

                PosicaoRank temp = ranks[j];
                ranks[j] = ranks[j+1];
                ranks[j+1] = temp;
            }
        }
    }
}



int main(void)
{

    Camera2D cameraPlayer;

    InitWindow(tamanhoTelaX, tamanhoTelaY, "Project Chicken");
    SetTargetFPS(60);

    // -- CARREGAR TEXTURA DO CENÁRIO

    Texture2D background = LoadTexture("sprites/Mapa_completo.png");
    Texture2D solo = LoadTexture("sprites/Cenario/fase_blocos_1.png");
    Texture2D armadilhas = LoadTexture("sprites/Cenario/fase_espinhos_1.png");

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

        {(Rectangle){0, 0, 20, 1000}, true, BLUE},
        //{(Rectangle){0, 710, 1000, 20}, true, BLUE},

        {(Rectangle){0, 672, 100, 20}, true, BLUE},
        {(Rectangle){150, 200, 50, 1000}, true, BLUE},
        {(Rectangle){300, 200, 50, 1000}, true, BLUE},
        {(Rectangle){600, 500, 50, 1000}, true, BLUE},

        {(Rectangle){300, 672, 200, 20}, true, BLUE},
        {(Rectangle){600, 672, 150, 20}, true, BLUE},
        {(Rectangle){850, 672, 100, 20}, true, BLUE}

    };

    int numPlataformas = sizeof(plataforma) / sizeof(plataforma[0]);

    //-- plataformas de armadilhas

    ObjetosCena plataformaArmadihas[] = {


        {(Rectangle){0, 710, 1000, 20}, true, RED},



    };

    int numPlataformasArmadilhas = sizeof(plataformaArmadihas) / sizeof(plataformaArmadihas[0]);

    while (!WindowShouldClose())
    {
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
                playerX = playerCkeckpointX;
                playerY = playerCkeckpointY;
                tempoAnimacaoNascer = 0.0f; // Reseta o tempo de nascimento
                velocidadePuloY = 0.0f; // Reseta a velocidade do pulo
            }
        }
        else if (vida == 1) // Jogador está nascendo
        {
            tempoAnimacaoNascer += tempoDoFrame;
            if (tempoAnimacaoNascer >= duracaoAnimacaoNascer)
            {
                vida = 0; // Transiciona para "vivendo"
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
            }
            
            else if (IsKeyPressed(KEY_LEFT_SHIFT) && (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_LEFT)) && !dashAtivo && !grudandoParede)
            {
                dashVertical = false;
                dashAtivo = true;
                dashTempo = dashDuracao;
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
                    gravidadeAceleracao = 800.0f;
                    dashAtivo = false;
                    dashVertical = false;
                }
            }
            else
            {
                pulo(GetFrameTime());
                movimentoHorizontal(GetFrameTime());
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
            dashVerticalUsadoNoAr = false; // Reseta o dash vertical ao grudar na parede
        }
        else
        {
            // Se está no chão, reseta a velocidade vertical e permite pular
            velocidadePuloY = 0.0f;
            podePular = true;
            dashVerticalUsadoNoAr = false; // Reseta o dash vertical ao tocar o chão
        }

        // --- colisao com armadilhas
        for (int i = 0; i < numPlataformasArmadilhas; i++){
            if (CheckCollisionRecs(playerRect, plataformaArmadihas[i].retangulo) && plataformaArmadihas[i].solido){
                if (vida == 0) { // Só morre se estiver vivo
                    vida = -1; // Sinaliza que o jogador morreu
                    podeControlarPlayer = false; // Desativa o controle
                    tempoAnimacaoMorte = 0.0f; // Reinicia o tempo da animação de morte
                }
            }
        }


        // Controle de estado de animacao

        int proximaAnimacao = -1;

        if (vida == -1) { // Jogador está morrendo
            proximaAnimacao = 6;
        } else if (vida == 1) { // Jogador está nascendo
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

        DrawText("PROJECT CHICKEN", 400, 50, 30, RED);

        BeginMode2D(cameraPlayer);

        desenhaCenario(background);

        // Desenha plataformas

        for (int i = 0; i < numPlataformas; i++)
        {
            DrawRectangleRec(plataforma[i].retangulo, plataforma[i].cor);
        }



        for (int i = 0; i < numPlataformasArmadilhas; i++)
        {
            DrawRectangleRec(plataformaArmadihas[i].retangulo, plataformaArmadihas[i].cor);
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

        if (vida == -1) {
            estadoPlayerTexto = "Estado: Morreu!";
            estadoPlayerCor = DARKPURPLE;
        } else if (vida == 1) {
            estadoPlayerTexto = "Estado: Nascendo...";
            estadoPlayerCor = SKYBLUE;
        } else if (emChao)
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
    UnloadTexture(jogadorParede);
    UnloadTexture(background);
    UnloadTexture(solo);
    UnloadTexture(armadilhas);

    UnloadTexture(jogadorNascer);
    UnloadTexture(jogadorMorrer);

    CloseWindow();

    return 0;
}