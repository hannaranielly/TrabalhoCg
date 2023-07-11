
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

#define FPS 30 //Limite de FPS no jogo
#define MaxPista 25 //Quantidade máxima de faixas centrais
#define VeloMeteoro  1.0; //Definição da velocidade que o meteoro irá percorrer o eixo z

using namespace std;

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
void CriarMundo(){
    glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glFrustum(-4,4,-1,1,0.9,50);

    glMatrixMode(GL_MODELVIEW);
    glViewport(0,0,1800,900);
    glLoadIdentity();
    gluLookAt(PosXGlobalCamera, PosYGlobalCamera,PosZGlobalCamera, //posição da câmera
              PosXApontaCamera, PosYApontaCamera,PosZApontaCamera,//Posição inicial da camera, //para onde a câmera olha
              0, 0, 1); //para onde o topo da câmera aponta

    ObjetosEstaticos.EstaticObjects(TextoX,TextoY,textID[7]);
    
    //Desenhar meu carro
    glPushMatrix();
    glTranslatef(Jogador1.getPosX()+0.5,Jogador1.getPosY()+0.25,Jogador1.getPosZ());
    //Mostrar a colisao
    if (Colisao == 1){
        glRotatef(RotacaoColisao,0,0,1);
    }
    glTranslatef(-0.5,-0.25,0);
    Jogador1.CriarCarro(RotacaoPneu);
    glPopMatrix();

    //Desenhar as 20 faixas centrais
    CriarPista();
    //Desenhar os inimigos
    CriarCarrosInimigos();

    //Desenhar os meteoros
    for (int i = 0; i < VecMeteoro.size(); i++){
        VecMeteoro[i].DesenharMeteoro();
    }

    //Desenhar a faixa de largada e consequente chegada
    for (int i = 0; i < VecFaixasLargada.size(); i++){
        VecFaixasLargada[i].DesenharFaixa(textID[8]);
    }

    //Verificando os "estado do jogo"(tela de menu, contagem antes de iniciar a corrida, inicio da corrida, e chegada)
    if (Temporizador == 0){//Tela de "Press Start"
        
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D, textID[0]);
        glBegin(GL_QUADS);//(0,0)
            glTexCoord2f(0.0,0.0); glVertex3f(28,9.9,11.3);
            glTexCoord2f(1.0,0.0); glVertex3f(32,9.9,11.3);
            glTexCoord2f(1.0,1.0); glVertex3f(32,10,12.7);
            glTexCoord2f(0.0,1.0); glVertex3f(28,10,12.7);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);

    }else if (Temporizador > 1 && Temporizador < 45){//Contador "1"
        semaforoPartida.CriarSemaforoPartida(RotacaoSemaforo, textID[1]);
    }else if (Temporizador >= 45 && Temporizador < 90){//Contador "2"
        semaforoPartida.CriarSemaforoPartida(RotacaoSemaforo, textID[2]);
    }else if (Temporizador >= 90 && Temporizador < 135){//Contador "3"
        semaforoPartida.CriarSemaforoPartida(RotacaoSemaforo, textID[3]);
    }else if (Temporizador >= 135 && Temporizador <= 180){//"GO!"
        semaforoPartida.CriarSemaforoPartida(RotacaoSemaforo, textID[4]);
    }else if (Temporizador == -1){//"Perdeu"
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D, textID[5]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(27,10,11);
            glTexCoord2f(1.0,0.0); glVertex3f(33,10,11);
            glTexCoord2f(1.0,1.0); glVertex3f(33,10.11,12.92);
            glTexCoord2f(0.0,1.0); glVertex3f(27,10.11,12.92);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador == -2){//"Ganhou"
        glColor3f(1,1,1);
        glBindTexture(GL_TEXTURE_2D, textID[6]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0,0.0); glVertex3f(27,10,11);
            glTexCoord2f(1.0,0.0); glVertex3f(33,10,11);
            glTexCoord2f(1.0,1.0); glVertex3f(33,10.11,12.92);
            glTexCoord2f(0.0,1.0); glVertex3f(27,10.11,12.92);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }else if (Temporizador > 100){

        glColor3f(1, 1, 1);
        glBindTexture(GL_TEXTURE_2D, textID_velocidade [int(Jogador1.getVelocidade())]);
        glBegin(GL_QUADS);
            glTexCoord2f(0.0, 0.0); glVertex3f(PosXGlobalCamera - 0.5, 10, 12.45);
            glTexCoord2f(1.0, 0.0); glVertex3f(PosXGlobalCamera + 0.8, 10, 12.45);
            glTexCoord2f(1.0, 1.0); glVertex3f(PosXGlobalCamera + 0.8, 10.03, 13.01);
            glTexCoord2f(0.0, 1.0); glVertex3f(PosXGlobalCamera - 0.5, 10.03, 13.01);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
        
    }
 
    glutSwapBuffers();
}