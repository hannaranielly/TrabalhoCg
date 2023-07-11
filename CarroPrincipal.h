#ifndef CARRO_PRINCIPAL_H
#define CARRO_PRINCIPAL_H

#include <iostream>
#include "carro.h"

using namespace std;

class CarroPrincipal {
private:
    float PosX;
    float PosY;
    float PosZ;
    float Velocidade;

public:
    CarroPrincipal(float PosX, float PosY, float PosZ, float velocidade);
    void DefineVelo(float velocidade);
    const float& getPosX() const;
    const float& getPosY() const;
    const float& getPosZ() const;
    const float& getVelocidade() const;
    void setPosX(const float& novoPosX);
    void setPosY(const float& novoPosY);
    void setPosZ(const float& novoPosZ);
    void CriarCarro(float RotacaoPneu);
};

#endif
