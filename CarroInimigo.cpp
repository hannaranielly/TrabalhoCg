#include <iostream>
#include "Carro.cpp"



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
    CarroInimigo(float PosX, float PosY, float PosZ) : PosX(PosX), PosY(PosY), PosZ(PosZ), velocidadeFixa(0.0) {}

    void DefineVeloFixa(float velocidade) {
        if (velocidade > velocidadeMax) {
            velocidadeFixa = velocidadeMax;
        } else {
            velocidadeFixa = velocidade;
        }
    }

    void DefineVeloRelativa(float velocidade, float MaxVelo) {
        velocidadeRelativa = (velocidadeFixa - velocidade) / MaxVelo;
    }

    void MoveCar() {
        PosY += velocidadeRelativa;
    }

    const float& getPosX() const {
        return PosX;
    }

    const float& getPosY() const {
        return PosY;
    }

    const float& getPosZ() const {
        return PosZ;
    }

    const float& getVeloFixa() const {
        return velocidadeFixa;
    }

    void setVeloMax(const float& velocidade) {
        velocidadeMax = velocidade;
    }

    void setPosX(const float& novoPosX) {
        PosX = novoPosX;
    }

    void CriarCarro(float RotacaoPneu) {
        Carro Car(RotacaoPneu);
        Car.desCarro(0, 0, 1, PosX, PosY, PosZ);
        Car.desPneu(-0.15, 0.352, 0.115);
        Car.desPneu(-0.15, 0.122, 0.115);
    }
};
