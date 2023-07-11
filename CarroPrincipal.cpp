#include <iostream>
#include "Carro.cpp"



using namespace std;

class CarroPrincipal {
private:
    float PosX;
    float PosY;
    float PosZ;
    float Velocidade;

public:
    CarroPrincipal(float PosX, float PosY, float PosZ, float velocidade)
        : PosX(PosX), PosY(PosY), PosZ(PosZ), Velocidade(velocidade) {}

    void DefineVelo(float velocidade) {
        Velocidade = velocidade;
        if (Velocidade > 200.0) {
            Velocidade = 200.0;
        }
        if (Velocidade <= 0.0) {
            Velocidade = 0.0;
        }
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

    const float& getVelocidade() const {
        return Velocidade;
    }

    void setPosX(const float& novoPosX) {
        PosX = novoPosX;
    }

    void setPosY(const float& novoPosY) {
        PosY = novoPosY;
    }

    void setPosZ(const float& novoPosZ) {
        PosZ = novoPosZ;
    }

    void CriarCarro(float RotacaoPneu) {
        Carro Car(RotacaoPneu);
        Car.desCarro(1, 0, 0, PosX, PosY, PosZ);
        Car.desPneu(-0.15, 0.352, 0.115);
        Car.desPneu(-0.15, 0.122, 0.115);
    }
};