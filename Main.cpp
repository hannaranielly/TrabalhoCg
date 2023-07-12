#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <vector>
#include <cstdlib>
#include "Pista.cpp"
#include "Largada.cpp"
#include "Semaforo.cpp"
#include "CarroInimigo.cpp"
#include "CarroPrincipal.cpp"
#include "StaticObjetos.cpp"
#include "stb_image.cpp"
#include "Meteoros.cpp"
#include "Variaveis.h"

#define FPS 30 // Limite de FPS no jogo
#define MaxPista 25 // Quantidade máxima de faixas centrais
#define VeloMeteoro 1.0 // Definição da velocidade que o meteoro irá percorrer o eixo z

int VelocidadeMaxima = 160;
int Pressed_Key[5] = {0,0,0,0,0};//Controle de movimentacao do teclado
float VetorDir[5];//Vetor de direcao dos carros inimigos

float PosXGlobalCamera = 30.0;//Posição inicial da camera (LookAt)
float PosYGlobalCamera = 9.0;//Posição inicial da camera (LookAt)
float PosZGlobalCamera = 12.0;//Posição inicial da camera (LookAt)
float PosXApontaCamera = 30.0;//Posição inicial para onde a camera aponta (LookAt)
float PosYApontaCamera = 40.0;//Posição inicial para onde a camera aponta (LookAt)
float PosZApontaCamera = 12.0;//Posição inicial para onde a camera aponta (LookAt)

int QtdCarrosInimigos = 2; //Controla a qtd de carros inimigos

int Temporizador = 0; //Serve para os estados do jogo
float Chegada = 0; //Verifica se tem um ganhador
float Colisao = 0; //Estado de colisão
float TempoColisao = 0; //Tempo de estado de colisão

float RotacaoColisao = 0; //Rotacao do carro quando colide
int RotacaoPneu = 0; //Serve para girar o pneu quando o carro anda
float RotacaoSemaforo = 0.0; //Serve girar o semáforo da contagem de inicio 1-2-3-Go

float TextoX = 10;
float TextoY = 10; 

GLuint textID[11]; //Vetor dos sprites
GLuint textID_velocidade[201]; //Vetor dos sprites do velocimetro


//Inicializações de tudo que vai ficar pra "sempre" no mapa
CarroPrincipal Jogador1 = CarroPrincipal(32,11.5,10.11,0.0); //Criação do carro principal
StaticObjetos ObjetosEstaticos = StaticObjetos(); //Criação dos objetos estáticos
Semaforo semaforoPartida = Semaforo(30,15,13); //Criando o bloco para usar as texturas do semaforo 1-2-3
vector <Pista> VecPista; //Vetor das faixas centrais
vector <CarroInimigo> VecCarroInimigos; //Vetor dos 5 carros inimigos
vector <Largada> VecFaixasLargada; //Vetor da faixa de largada e chegada
vector <Meteoro> VecMeteoro; //Vetor dos meteoros


using namespace std;

//Carregar as Texturas
void textura(GLuint tex_id, string filePath){
    unsigned char* imgData;
    int largura, altura, canais;
    stbi_set_flip_vertically_on_load(true);
    imgData = stbi_load(filePath.c_str(), &largura, &altura, &canais, 4);
    
    if (imgData){
        glBindTexture(GL_TEXTURE_2D,tex_id);//Juntando a região com o ID
        glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA8,largura,altura,0,GL_RGBA,GL_UNSIGNED_BYTE,imgData);
        //Casos de maior ou menor
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPLACE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPLACE);
        
        stbi_image_free(imgData);

    }else{
        cout << "Erro: Nao foi possivel carregar a textura! " << filePath.c_str() << endl;
    }   
}

void criarPista() {
    float distanciaY = 60.0;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    
    int numFaixas = std::ceil(distanciaY / 4.0);

    for (int i = 0; i < numFaixas; i++) {
        Pista faixa(530.0, distanciaY, 10.0);
        VecPista.push_back(faixa);
        distanciaY -= 4.0;
    }

    Pista faixa(0.0, 0.0, 0.0);
    faixa.CriarPista();


    glFlush();
    glutSwapBuffers();
}

//Instanciando os inimigos em faixa aleatoria da pista, e de forma dinâmica
void criarCarroInimigo() {
    float distanciaY = 12.5;
    float posInicialX = 27.0;
    float linha = 5.0;
    float velMinima = 180.0;
    float velMaxima = 190.0;

    for (int i = 1; i <= QtdCarrosInimigos; i++) {
        float posX = posInicialX + (i % 2 == 0 ? linha : 0.0);
        float posY = distanciaY + (i / 3.0);

        CarroInimigo car(posX, posY, 10.1);

        // Determinar a velocidade aleatória do carro inimigo
        float randVel = velMinima + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX / (velMaxima - velMinima)));
        car.setVeloMax(randVel);

        VecCarroInimigos.push_back(car);

        // Velocidade com que os carros inimigos se movem continuamente para a esquerda ou direita
        float dir = (i % 2 == 0) ? -0.1 : 0.1;
        VetorDir[i - 1] = dir;
    }
}

//Criando a frequência da queda dos meteoros e seu espaçamento
void criarMeteoro() {
    for (int i = 0; i <= 100; i++) {
        int x = i * 20;
        int z = 0;
        if (i % 2 == 0) {
            z = 30;
        } else {
            z = 20;
        }
        Meteoro meteoro = Meteoro(x, 50, z, textID[9]);
        VecMeteoro.push_back(meteoro);
    }
}

void criarLargada() {
    Largada Largada(25, 16.5, 10.3);
    VecFaixasLargada.push_back(Largada);
}

void initializeGL() {
    glClearColor(0.53, 0.84, 0.90, 1.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);

    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_COMPILE);
    glGenTextures(11, textID);//Gerando na memoria a textura com seu id
    
    textura(textID[0],"sprites/Start.png");
    textura(textID[1],"sprites/SinalVermelho.png");
    textura(textID[2],"sprites/SinalAmarelo.png");
    textura(textID[3],"sprites/SinalVerde.png");
    textura(textID[4],"sprites/Slogan.png");
    textura(textID[5],"sprites/GameOver.jpg");
    textura(textID[6],"sprites/YouWin.png");
    textura(textID[7],"sprites/Deserto.jpg");
    textura(textID[8],"sprites/Largada.png");
    textura(textID[9],"sprites/Meteoro.png");


    glGenTextures(201, textID_velocidade);//Gerando na memoria a textura da velocidade com seu id
    for (int i = 0; i <= 200; i++){
        string path = "sprites/sprites_velocimetro/";
        string convertion = to_string(i);
        string format = ".png";
        path = path + convertion + format;
        textura(textID_velocidade[i],path);
    }

    criarPista();
    criarCarroInimigo();
    criarLargada();
    criarMeteoro();
}

void CriarPista(){
    for (int i = 0; i < MaxPista; i++){
        VecPista[i].CriarPista();
    }
}

void CriarCarrosInimigos(){
    for (int i = 0; i < QtdCarrosInimigos; i++){
        glPushMatrix();
        float x = VecCarroInimigos[i].getPosX();
        float y = VecCarroInimigos[i].getPosY();
        float z = VecCarroInimigos[i].getPosZ();
        glTranslatef(x,y,z);
        VecCarroInimigos[i].CriarCarro(RotacaoPneu);
        glPopMatrix();
    }
}

//Funcao de desenho do mundo completo, para chamar o desenho de todos os objetos, estaticos e dinâmicos
void CriarMundo() {
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
    // Limpa o buffer de profundidade e o buffer de cor

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-4, 4, -1, 1, 0.9, 50);
    // Define a matriz de projeção para criar uma perspectiva 3D usando glFrustum

    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, 1800, 900);
    glLoadIdentity();
    gluLookAt(PosXGlobalCamera, PosYGlobalCamera, PosZGlobalCamera, PosXApontaCamera, PosYApontaCamera, PosZApontaCamera, 0, 0, 1);
    // Define a matriz de visualização do modelo, especificando a posição e a direção da câmera usando gluLookAt

    ObjetosEstaticos.EstaticObjects(TextoX, TextoY, textID[7]);
    // Desenha objetos estáticos chamando a função EstaticObjects da classe ObjetosEstaticos

    glPushMatrix();
    glTranslatef(Jogador1.getPosX() + 0.5, Jogador1.getPosY() + 0.25, Jogador1.getPosZ());
    // Aplica uma translação para a posição do jogador

    if (Colisao == 1) {
        glRotatef(RotacaoColisao, 0, 0, 1);
        // Rotaciona o jogador se houver uma colisão
    }

    glTranslatef(-0.5, -0.25, 0);
    Jogador1.CriarCarro(RotacaoPneu);
    // Desenha o carro do jogador chamando a função CriarCarro da classe Jogador1

    glPopMatrix();

    CriarPista();
    // Desenha a pista

    CriarCarrosInimigos();
    // Desenha os carros inimigos

    for (int i = 0; i < VecMeteoro.size(); i++) {
        VecMeteoro[i].DesenharMeteoro();
        // Desenha os meteoros presentes no vetor VecMeteoro
    }

    for (int i = 0; i < VecFaixasLargada.size(); i++) {
        VecFaixasLargada[i].DesenharFaixa(textID[8]);
        // Desenha as faixas de largada presentes no vetor VecFaixasLargada
    }

    if (Temporizador == 0) {
        glColor3f(1, 1, 1);
        glBindTexture(GL_TEXTURE_2D, textID[0]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(28, 9.9, 11.3);
        glTexCoord2f(1.0, 0.0); glVertex3f(32, 9.9, 11.3);
        glTexCoord2f(1.0, 1.0); glVertex3f(32, 10, 12.7);
        glTexCoord2f(0.0, 1.0); glVertex3f(28, 10, 12.7);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        // Desenha uma textura especial quando o temporizador é igual a 0
    } else if (Temporizador > 1 && Temporizador < 45) {
        semaforoPartida.CriarSemaforoPartida(RotacaoSemaforo, textID[1]);
        // Desenha o semáforo de partida em uma determinada posição e rotação, quando o temporizador está entre 1 e 45
    } else if (Temporizador >= 45 && Temporizador < 90) {
        semaforoPartida.CriarSemaforoPartida(RotacaoSemaforo, textID[2]);
        // Desenha o semáforo de partida em uma determinada posição e rotação, quando o temporizador está entre 45 e 90
    } else if (Temporizador >= 90 && Temporizador < 135) {
        semaforoPartida.CriarSemaforoPartida(RotacaoSemaforo, textID[3]);
        // Desenha o semáforo de partida em uma determinada posição e rotação, quando o temporizador está entre 90 e 135
    } else if (Temporizador >= 135 && Temporizador <= 180) {
        semaforoPartida.CriarSemaforoPartida(RotacaoSemaforo, textID[4]);
        // Desenha o semáforo de partida em uma determinada posição e rotação, quando o temporizador está entre 135 e 180
    } else if (Temporizador == -1) {
        glColor3f(1, 1, 1);
        glBindTexture(GL_TEXTURE_2D, textID[5]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(27, 10, 11);
        glTexCoord2f(1.0, 0.0); glVertex3f(33, 10, 11);
        glTexCoord2f(1.0, 1.0); glVertex3f(33, 10.11, 12.92);
        glTexCoord2f(0.0, 1.0); glVertex3f(27, 10.11, 12.92);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        // Desenha uma textura especial quando o temporizador é igual a -1
    } else if (Temporizador == -2) {
        glColor3f(1, 1, 1);
        glBindTexture(GL_TEXTURE_2D, textID[6]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(27, 10, 11);
        glTexCoord2f(1.0, 0.0); glVertex3f(33, 10, 11);
        glTexCoord2f(1.0, 1.0); glVertex3f(33, 10.11, 12.92);
        glTexCoord2f(0.0, 1.0); glVertex3f(27, 10.11, 12.92);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        // Desenha uma textura especial quando o temporizador é igual a -2
    } else if (Temporizador > 100) {
        glColor3f(1, 1, 1);
        glBindTexture(GL_TEXTURE_2D, textID_velocidade[int(Jogador1.getVelocidade())]);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(PosXGlobalCamera - 0.5, 10, 12.45);
        glTexCoord2f(1.0, 0.0); glVertex3f(PosXGlobalCamera + 0.8, 10, 12.45);
        glTexCoord2f(1.0, 1.0); glVertex3f(PosXGlobalCamera + 0.8, 10.03, 13.01);
        glTexCoord2f(0.0, 1.0); glVertex3f(PosXGlobalCamera - 0.5, 10.03, 13.01);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        // Desenha uma textura especial com base na velocidade do jogador quando o temporizador é maior que 100
    }

    glutSwapBuffers();
    // Troca os buffers para exibir o desenho na tela
}

void moverMeteoro() {
    for (int i = 0; i < VecMeteoro.size(); i ++){
        float PosZ = VecMeteoro[i].getPosZ();
        PosZ -= VeloMeteoro;
        VecMeteoro[i].setPosZ(PosZ);
    }
}

void incrementarTemporizador() {
    if (Temporizador != 0 && Temporizador > 0) {
        Temporizador++;
    }
}

void colisao(){

    // Verifica se não houve colisão anteriormente
        if (Colisao == 0) {
            // Obtém a posição do jogador
            float PosX = Jogador1.getPosX();
            float PosY = Jogador1.getPosY();

            // Verifica colisão com cada carro inimigo
            for (int i = 0; i < QtdCarrosInimigos; i++) {
                // Obtém as coordenadas do retângulo de colisão do carro inimigo
                float XsupDir = VecCarroInimigos[i].getPosX() + 1;
                float YsupDir = VecCarroInimigos[i].getPosY() + 0.5;
                float XinfEsq = VecCarroInimigos[i].getPosX();
                float YinfEsq = VecCarroInimigos[i].getPosY();

                // Verifica se houve colisão
                if (PosX + 1 >= XinfEsq && PosX <= XsupDir &&
                    PosY + 0.5 >= YinfEsq && PosY <= YsupDir) {
                    Colisao = 1;
                    break; // Interrompe o loop, pois já houve uma colisão
                } else if (PosX + 1 >= XinfEsq && PosX + 1 <= XsupDir &&
                        PosY >= YinfEsq && PosY <= YsupDir) {
                    Colisao = 1;
                    break; // Interrompe o loop, pois já houve uma colisão
                } else if (PosX >= XinfEsq && PosX <= XsupDir &&
                        PosY + 0.5 >= YinfEsq && PosY <= YsupDir) {
                    Colisao = 1;
                    break; // Interrompe o loop, pois já houve uma colisão
                } else if (PosX >= XinfEsq && PosX <= XsupDir &&
                        PosY >= YinfEsq && PosY <= YsupDir) {
                    Colisao = 1;
                    break; // Interrompe o loop, pois já houve uma colisão
                }
            }
        }

        // Se houve colisão
        if (Colisao == 1) {
            if (RotacaoColisao == 0) {
                // Reduz a velocidade do jogador
                Jogador1.DefineVelo(Jogador1.getVelocidade() - 100);
                RotacaoColisao = 5;

                if (Jogador1.getVelocidade() <= 0) {
                    Jogador1.DefineVelo(0);
                }
            } else {
                // Reduz a velocidade do jogador gradualmente
                Jogador1.DefineVelo(Jogador1.getVelocidade() - 1);

                if (Jogador1.getVelocidade() <= 0) {
                    Jogador1.DefineVelo(0);
                }

                // Move a câmera lateralmente
                if (int(TempoColisao) % 2 == 0) {
                    PosXGlobalCamera += 0.05;
                } else {
                    PosXGlobalCamera -= 0.05;
                }
            }

            // Inverte a direção da rotação da câmera
            RotacaoColisao *= -1;

            // Incrementa o tempo de colisão
            TempoColisao += 3;

            // Verifica se o tempo de colisão atingiu o limite
            if (TempoColisao >= 90) {
                RotacaoColisao = 0;
                Colisao = 0;
                TempoColisao = 0;
            }
        }

        // Verifica novamente se houve colisão (após o tratamento da colisão anterior)
        if (Colisao == 0) {
            // Obtém a posição do jogador
            float PosX = Jogador1.getPosX();
            float PosY = Jogador1.getPosY();

            // Verifica colisão com cada carro inimigo
            for (int i = 0; i < QtdCarrosInimigos; i++) {
                // Obtém as coordenadas do retângulo de colisão do carro inimigo
                float XsupDir = VecCarroInimigos[i].getPosX() + 1;
                float YsupDir = VecCarroInimigos[i].getPosY() + 0.5;
                float XinfEsq = VecCarroInimigos[i].getPosX();
                float YinfEsq = VecCarroInimigos[i].getPosY();

                // Verifica se houve colisão
                if ((PosX + 1 >= XinfEsq && PosX + 1 <= XsupDir &&
                    PosY + 0.5 >= YinfEsq && PosY + 0.5 <= YsupDir) ||
                    (PosX + 1 >= XinfEsq && PosX + 1 <= XsupDir &&
                    PosY >= YinfEsq && PosY <= YsupDir) ||
                    (PosX >= XinfEsq && PosX <= XsupDir &&
                    PosY + 0.5 >= YinfEsq && PosY + 0.5 <= YsupDir) ||
                    (PosX >= XinfEsq && PosX <= XsupDir &&
                    PosY >= YinfEsq && PosY <= YsupDir)) {
                    Colisao = 1;
                    break; // Interrompe o loop, pois já houve uma colisão
                }
            }

            // Se houve colisão
            if (Colisao == 1) {
                if (RotacaoColisao == 0) {
                    // Reduz a velocidade do jogador
                    Jogador1.DefineVelo(Jogador1.getVelocidade() - 100);
                    RotacaoColisao = 5;

                    if (Jogador1.getVelocidade() <= 0) {
                        Jogador1.DefineVelo(0);
                    }
                } else {
                    // Reduz a velocidade do jogador gradualmente
                    Jogador1.DefineVelo(Jogador1.getVelocidade() - 1);

                    if (Jogador1.getVelocidade() <= 0) {
                        Jogador1.DefineVelo(0);
                    }

                    // Move a câmera lateralmente
                    if (int(TempoColisao) % 2 == 0) {
                        PosXGlobalCamera += 0.05;
                    } else {
                        PosXGlobalCamera -= 0.05;
                    }
                }

                // Inverte a direção da rotação da câmera
                RotacaoColisao *= -1;

                // Incrementa o tempo de colisão
                TempoColisao += 3;

                // Verifica se o tempo de colisão atingiu o limite
                if (TempoColisao >= 90) {
                    RotacaoColisao = 0;
                    Colisao = 0;
                    TempoColisao = 0;
                }
            }
        }

}

void teclado(){
    // Verificação das teclas pressionadas
        if (Pressed_Key[0] == 1) {
            // W - Acelera
            Jogador1.DefineVelo(Jogador1.getVelocidade() + 1.3);
        }
        if (Pressed_Key[1] == 1) {
            // A - Vai para esquerda e desacelera
            Jogador1.setPosX(Jogador1.getPosX() - 0.07);
            Jogador1.DefineVelo(Jogador1.getVelocidade() - 0.7);
        }
        if (Pressed_Key[2] == 1) {
            // S - Desacelera
            Jogador1.DefineVelo(Jogador1.getVelocidade() - 2.0);
        }
        if (Pressed_Key[3] == 1) {
            // D - Vai para a direita e desacelera
            Jogador1.setPosX(Jogador1.getPosX() + 0.07);
            Jogador1.DefineVelo(Jogador1.getVelocidade() - 0.7);
        }
        if (Pressed_Key[4] == 1) {
            // R - Olha para trás com a câmera
            if (PosYApontaCamera > 0) {
                PosYApontaCamera = PosYApontaCamera * (-1.0);
            }
        }
        if (Pressed_Key[4] == 0) {
            // R - Ao deixar de pressionar volta a olhar para o local original
            if (PosYApontaCamera < 0) {
                PosYApontaCamera = PosYApontaCamera * (-1.0);
            }
        }
}

void moverPista(){

        // Determinar as posições finais das Faixas Centrais à medida que o meu carro se "movimenta"
        for (int i = 0; i < MaxPista; i++) {
            VecPista[i].DefineVelo(Jogador1.getVelocidade(), VelocidadeMaxima);
            VecPista[i].MoverPista();
        }

        // Move a faixa de largada com velocidade relativa ao meu carro
        for (int i = 0; i < VecFaixasLargada.size(); i++) {
            VecFaixasLargada[i].DefineVelo(Jogador1.getVelocidade(), VelocidadeMaxima);
            VecFaixasLargada[i].MoverFaixaChegada();
            Chegada += VecFaixasLargada[i].getPosY() / FPS; // Verifica a distância percorrida pela faixa
        }
}

void CarrinhoPista(){
    // Movendo os carros inimigos com suas velocidades fixas em relação à velocidade relativa ao meu carro
        for (int i = 0; i < QtdCarrosInimigos; i++) {
            int veloFixa = VecCarroInimigos[i].getVeloFixa();
            float PosX = VecCarroInimigos[i].getPosX();

            // Faz uma verificação que se rand_aux cair entre 60 e 65 o carro inimigo muda de direção no deslocamento lateral
            float rand_aux = rand() % 1000;
            if (rand_aux >= 60 && rand_aux <= 65) {
                VetorDir[i] = VetorDir[i] * (-1);
            }

            veloFixa += 1.5; // Os carros inimigos vão acelerando 1.5 a cada FPS, até seu limite de velocidade
            PosX += VetorDir[i]; // Fazendo o deslocamento lateral do carro inimigo

            // Verificando se o carro inimigo não sai da pista
            if (PosX >= 33.8) {
                VetorDir[i] = VetorDir[i] * (-1);
                PosX = 33.75;
            } else if (PosX <= 25.2) {
                VetorDir[i] = VetorDir[i] * (-1);
                PosX = 25.25;
            }

            VecCarroInimigos[i].setPosX(PosX);
            VecCarroInimigos[i].DefineVeloFixa(veloFixa); // Vai ser somada até chegar no limite dela, feito na criação do carro
            VecCarroInimigos[i].DefineVeloRelativa(Jogador1.getVelocidade(), VelocidadeMaxima); // Velocidade do inimigo em relação ao meu carro, fazendo-o se afastar ou se aproximar
            VecCarroInimigos[i].MoveCar();
        }
}

//Função que vai realmente fazer a movimentação das coordenadas de cada objeto para a posição desejada a cada FPS
void ocioso(int v){

    incrementarTemporizador();
    moverMeteoro();

    // Verifica se o tempo de chegada é maior que 300 e o temporizador não é -1 ou -2
    if (Chegada > 300 && Temporizador != -1 && Temporizador != -2) {

        // Verifica se algum dos carros inimigos ultrapassou a primeira faixa de largada
        for (int i = 0; i < VecCarroInimigos.size(); i++) {
            if (VecFaixasLargada[0].getPosY() <= VecCarroInimigos[i].getPosY()) {
                Temporizador = -1;
            }
        }

        // Verifica se o jogador ultrapassou a primeira faixa de largada
        if (VecFaixasLargada[0].getPosY() <= Jogador1.getPosY()) {
            Temporizador = -2;
        }

        // Verifica se o temporizador é igual a -1 ou -2 para determinar o resultado do jogo
        if (Temporizador == -1 || Temporizador == -2) {
            PosXGlobalCamera = 30.0;
            PosXApontaCamera = 30.0;
            glutTimerFunc(1000.0 / FPS, ocioso, 0);
            glutPostRedisplay();
        }
    }

    if (Temporizador == 0) {
    // Caso o Temporizador seja igual a 0, chama a função "ocioso" novamente após um intervalo de tempo (1000.0 / FPS) e atualiza a tela
    glutTimerFunc(1000.0 / FPS, ocioso, 0);
    glutPostRedisplay();
    } else if (Temporizador > 1 && Temporizador <= 45) {
        // Caso o Temporizador esteja entre 1 e 45 (inclusive)
        if (Temporizador > 30) {
            // Se o Temporizador for maior que 30, realiza uma ação específica, no caso, diminui o valor da variável "RotacaoSemaforo" em 6
            RotacaoSemaforo -= 6;
        }
        // Chama a função "ocioso" novamente após um intervalo de tempo (1000.0 / FPS) e atualiza a tela
        glutTimerFunc(1000.0 / FPS, ocioso, 0);
        glutPostRedisplay();
    } else if (Temporizador > 45 && Temporizador <= 90) {
        // Caso o Temporizador esteja entre 45 e 90 (inclusive)
        if (Temporizador > 75) {
            // Se o Temporizador for maior que 75, realiza uma ação específica, no caso, diminui o valor da variável "RotacaoSemaforo" em 6
            RotacaoSemaforo -= 6;
        }
        // Chama a função "ocioso" novamente após um intervalo de tempo (1000.0 / FPS) e atualiza a tela
        glutTimerFunc(1000.0 / FPS, ocioso, 0);
        glutPostRedisplay();
    } else if (Temporizador > 90 && Temporizador <= 135) {
        // Caso o Temporizador esteja entre 90 e 135 (inclusive)
        if (Temporizador > 120) {
            // Se o Temporizador for maior que 120, realiza uma ação específica, no caso, diminui o valor da variável "RotacaoSemaforo" em 6
            RotacaoSemaforo -= 6;
        }
        // Chama a função "ocioso" novamente após um intervalo de tempo (1000.0 / FPS) e atualiza a tela
        glutTimerFunc(1000.0 / FPS, ocioso, 0);
        glutPostRedisplay();
    } else if (Temporizador > 135 && Temporizador <= 180) {
        // Caso o Temporizador esteja entre 135 e 180 (inclusive)
        // Chama a função "ocioso" novamente após um intervalo de tempo (1000.0 / FPS) e atualiza a tela
        glutTimerFunc(1000.0 / FPS, ocioso, 0);
        glutPostRedisplay();
    } else if (Temporizador > 180 && Temporizador <= 210) {
        // Caso o Temporizador esteja entre 180 e 210 (inclusive)
        // Realiza uma transição da câmera para a posição inicial
        PosXGlobalCamera += (Jogador1.getPosX() - PosXGlobalCamera) / 10;
        PosXApontaCamera += (Jogador1.getPosX() - PosXApontaCamera) / 10;
        // Chama a função "ocioso" novamente após um intervalo de tempo (1000.0 / FPS) e atualiza a tela
        glutTimerFunc(1000.0 / FPS, ocioso, 0);
        glutPostRedisplay();
    }

    else if (Temporizador != -1 && Temporizador != -2) {
        // Atualiza a textura no chão
        TextoX += 0.2;
        TextoY += 0.2;
        if (TextoX >= 15) {
            TextoX = 10;
        }
        if (TextoY >= 15) {
            TextoY = 10;
        }

        // Reduz a velocidade do carro quando não está acelerando
        if (Jogador1.getVelocidade() > 0) {
            Jogador1.DefineVelo(Jogador1.getVelocidade() - 0.3);
        }

        // Atualiza a posição da câmera de acordo com a posição do carro
        PosXGlobalCamera = Jogador1.getPosX() + 0.5;
        PosXApontaCamera = Jogador1.getPosX() + 0.5;

        teclado();  // Verifica as entradas do teclado
        moverPista();  // Move a pista
        CarrinhoPista();  // Move o carrinho na pista

        // Limita a posição do carro para evitar que saia da pista, tenha velocidade negativa ou ultrapasse a velocidade máxima
        if (Jogador1.getPosX() > 33.6) {
            Jogador1.setPosX(33.6);
        } else if (Jogador1.getPosX() < 25.4) {
            Jogador1.setPosX(25.4);
        } else if (Jogador1.getVelocidade() <= 0) {
            Jogador1.DefineVelo(0.0);
        } else if (Jogador1.getVelocidade() > VelocidadeMaxima && VelocidadeMaxima == 240) {
            Jogador1.DefineVelo(VelocidadeMaxima);
        }

        // Faz a rotação dos pneus caso o carro esteja em movimento
        if (RotacaoPneu < 360 && Jogador1.getVelocidade() > 0.0) {
            RotacaoPneu += 2;
        }
        if (RotacaoPneu >= 360) {
            RotacaoPneu = 0;
        }

        // Trata a colisão com carros inimigos
        colisao();

        // Reseta os valores de colisão após o tempo limite ser atingido
        if (TempoColisao >= 90) {
            RotacaoColisao = 0;
            Colisao = 0;
            TempoColisao = 0;
        }

        // Define a posição da câmera
        glm::vec3 PosCamera(PosXGlobalCamera, PosYGlobalCamera, PosZGlobalCamera);

        // Configura o temporizador para chamar a função "ocioso" novamente
        glutTimerFunc(1000.0 / FPS, ocioso, 0);
        glutPostRedisplay();

    }

}

//Função para tratamento de teclado
void Key(unsigned char key, int x, int y){
    if (key == 'w' || key == 'W'){
        Pressed_Key[0] = 1;
    }
    if (key == 'a' || key == 'A'){
        Pressed_Key[1] = 1;
    }
    if (key == 's' || key == 'S'){
        Pressed_Key[2] = 1;
    }
    if (key == 'd' || key == 'D'){
        Pressed_Key[3] = 1;
    }
    if (key == 'r' || key == 'R'){
        Pressed_Key[4] = 1;
    }
    if (key == 'x' || key == 'X'){
        Temporizador = 1;
    }
    
}

void KeyUp(unsigned char key, int x, int y){
    if (key == 'w' || key == 'W'){
        Pressed_Key[0] = 0;
    }
    if (key == 'a' || key == 'A'){
        Pressed_Key[1] = 0;
    }
    if (key == 's' || key == 'S'){
        Pressed_Key[2] = 0;
    }
    if (key == 'd' || key == 'D'){
        Pressed_Key[3] = 0;
    }
    if (key == 'r' || key == 'R'){
        Pressed_Key[4] = 0;
    }
}

//Main
int main(int argc, char **argv){
    glutInit(&argc,argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(2000,1000);
    glutCreateWindow("ROCK N ROLL RACING DA SHOPPE");
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    initializeGL();

    glutDisplayFunc(CriarMundo);
    glutKeyboardFunc(Key);
    glutKeyboardUpFunc(KeyUp);
    glutTimerFunc(1000.0/FPS, ocioso, 0);

    glutMainLoop();
    return 0;
}
