#include <iostream>
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <math.h>

using namespace std;

class Meteoro {
private:
    float PosX;
    float PosY;
    float PosZ;
    GLuint textID;

public:
    Meteoro(float PosX, float PosY, float PosZ, GLuint text_ID) : PosX(PosX), PosY(PosY), PosZ(PosZ), textID(text_ID) {}

    const float& getPosX() const {
        return PosX;
    }

    const float& getPosY() const {
        return PosY;
    }

    const float& getPosZ() const {
        return PosZ;
    }

    void setPosX(const float& newX) {
        PosX = newX;
    }

    void setPosY(const float& newY) {
        PosY = newY;
    }

    void setPosZ(const float& newZ) {
        if (newZ <= -90) {
            PosZ = newZ + 180;
        } else {
            PosZ = newZ;
        }
    }

    void TextMeteoro(GLfloat R, GLfloat G, GLfloat B) {
        glColor3f(R, G, B);
        glBindTexture(GL_TEXTURE_2D, textID);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0, -1.0);
        glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 0, -1.0);
        glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 0, 1.0);
        glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 0, 1.0);
        glEnd();
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void DesenharMeteoro() {
        glPushMatrix();
        glTranslatef(PosX, PosY, PosZ);
        glTranslatef(-PosZ, 0, -PosX);
        glScalef(4, 4, 2);
        TextMeteoro(1, 1, 1);
        glPopMatrix();
    }
};
