#ifndef VARIAVEIS_H
#define VARIAVEIS_H

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
#endif
