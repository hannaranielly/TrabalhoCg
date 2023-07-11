#ifndef CARRO_INIMIGO_H
#define CARRO_INIMIGO_H

#include <iostream>
#include "carro.h"

using namespace std;

class CarroInimigo {
private:
    float PosX;
    float PosY;
    float PosZ;
    float velocidadeFixa;
    float velocidadeRelativa;
    float velocidadeMax;

public:
    CarroInimigo(float PosX, float PosY, float PosZ);
    void DefineVeloFixa(float velocidade);
    void DefineVeloRelativa(float velocidade, float MaxVelo);
    void MoveCar();
    const float& getPosX() const;
    const float& getPosY() const;
    const float& getPosZ() const;
    const float& getVeloFixa() const;
    void setVeloMax(const float& velocidade);
    void setPosX(const float& novoPosX);
    void CriarCarro(float RotacaoPneu);
};

#endif
