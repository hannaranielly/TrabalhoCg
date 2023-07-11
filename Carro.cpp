#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>

#ifndef Carro_H //Header guards para nao ocorrer redefinição e evitar erro na compilação.
#define Carro_H

using namespace std;

//Classe que desenha o carro principal e os carros inimigos.
class Carro{
    private:
        float RotacaoPneu;

    public:


         //É definido um valor inicial para a rotação quando se cria um carro.
        Carro(float RotacaoPneu){
            this->RotacaoPneu = RotacaoPneu;
        }
        //Desenha uma circunferencia (utiliza triângulos).
        //Essa função desenha os pneus do carro.
        void desCircunferencia(GLfloat x, GLfloat y, GLfloat z, GLfloat radius, GLfloat faces, GLfloat altura){
            
            int i;
            int QtdTriangulos = faces;
            GLfloat twicePi = 2.0f * M_PI;

            // Desenhar o polígono base
            glBegin(GL_POLYGON);
                glColor3f(0,0,0);
                glVertex3f(x, y, z);
                for (i = 0; i <= QtdTriangulos; i++) {
                    float angle = i * twicePi / QtdTriangulos;
                    glVertex3f(
                        x + (radius * cos(angle)),
                        y + (radius * sin(angle)),
                        z
                    );
                }
            glEnd();

            // Desenhar o polígono superior
            glBegin(GL_POLYGON);
                glColor3f(0,0,0);
                glVertex3f(x, y, z + altura);
                for (i = 0; i <= QtdTriangulos; i++) {
                    float angle = i * twicePi / QtdTriangulos;
                    glVertex3f(
                        x + (radius * cos(angle)),
                        y + (radius * sin(angle)),
                        z + altura
                    );
                }
            glEnd();

            // Desenhar a parte lateral
            glBegin(GL_QUAD_STRIP);
                glColor3f(0,0,0);
                for (i = 0; i <= QtdTriangulos; i++) {
                    float angle = i * twicePi / QtdTriangulos;
                    glVertex3f(
                        x + (radius * cos(angle)),
                        y + (radius * sin(angle)),
                        z
                    );
                    glVertex3f(
                        x + (radius * cos(angle)),
                        y + (radius * sin(angle)),
                        z + altura
                    );
                }
            glEnd();

        }

         // Desenha os pneus e rotaciona de acordo com os movimentos.
        void desPneu(GLfloat X, GLfloat Y, GLfloat Z){
            
            glPushMatrix();
            glTranslatef(X,Y,Z);
            glRotatef(this->RotacaoPneu,1,0,0);
            glRotatef(90,0,1,0);
            desCircunferencia(0,0,0,0.12,8,1.3);
            glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
            desCircunferencia(0,0,0,0.12,8,1.3);
            glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
            glPopMatrix();
        }

        //Desenha o carro.


        void desCarro(int R, int G, int B, float PosX, float PosY, float PosZ) {
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
            glm::vec3 cor = glm::vec3(R, G, B);
            glColor3f(cor.r, cor.g, cor.b);

            // Desenhando as faces do carro
            // Cima
            glBegin(GL_QUADS);
                glVertex3f(0.0,0.0,0.4);
                glVertex3f(1,0,0.4);
                glVertex3f(1,0.5,0.4);
                glVertex3f(0,0.5,0.4);
            glEnd();

            // Atras
            glBegin(GL_QUADS);
                glVertex3f(0.0,0.0,0.0);   
                glVertex3f(1.0,0.0,0.0);
                glVertex3f(1.0,0.0,0.4);
                glVertex3f(0.0,0.0,0.4);
            glEnd();

            // Baixo
            glBegin(GL_QUADS);
                glVertex3f(0,0,0);
                glVertex3f(1,0,0); 
                glVertex3f(1,0.5,0);
                glVertex3f(0,0.5,0);
            glEnd();

            // Frente
            glBegin(GL_QUADS);
                glVertex3f(0,0.5,0);
                glVertex3f(1,0.5,0);
                glVertex3f(1,0.5,0.4);
                glVertex3f(0,0.5,0.4);
            glEnd();

            // Esquerda
            glBegin(GL_QUADS);
                glVertex3f(0,0,0);
                glVertex3f(0,0,0.4); 
                glVertex3f(0,0.5,0.4); 
                glVertex3f(0,0.5,0);
            glEnd();

            // Direita
            glBegin(GL_QUADS);   
                glVertex3f(1,0,0); 
                glVertex3f(1,0,0.4);
                glVertex3f(1,0.5,0.4);
                glVertex3f(1,0.5,0);
            glEnd();


            // Adicionando Janela esquerda
            glBegin(GL_QUADS);
            glColor3f(0.8, 0.8, 0.8); // Cor da janela
                glVertex3f(0, 0.1, 0.2);
                glVertex3f(0, 0.1, 0.3);
                glVertex3f(0, 0.4, 0.3);
                glVertex3f(0, 0.4, 0.2);
            glEnd();

            // Adicionando Janela direita
            glBegin(GL_QUADS);
            glColor3f(0.8, 0.8, 0.8); // Cor da janela
                glVertex3f(1, 0.1, 0.2);
                glVertex3f(1, 0.1, 0.3);
                glVertex3f(1, 0.4, 0.3);
                glVertex3f(1, 0.4, 0.2);
            glEnd();
        }   
};

#endif