#include "Zerpent.h"
using namespace std;

// Calcula donde quedaría la nueva cabeza sumando la dirección actual
// Inserta esa nueva cabeza frente al vector y elimina la cola del final
// Constructor
Serpiente::Serpiente(int inicioX, int inicioY, int tamanioInicial) :
    direccionX(1), direccionY(0), viva(true), creciendo(false) {

    // Inicializar el cuerpo de la serpiente de forma horizontal hacia la izquierda
    for (int i = 0; i < tamanioInicial; i++) {
        cuerpo.push_back({inicioX - i, inicioY});
    }
}

// Movimiento
void Serpiente::mover() {
    if (!viva) {
        return;
    }

    // Nueva posición de la cabeza
    Coordenada nuevaCabeza = {cuerpo[0].x + direccionX, cuerpo[0].y + direccionY};

    // Insertar nueva cabeza al frente
    cuerpo.insert(cuerpo.begin(), nuevaCabeza);

    // Si no está creciendo se elimina la cola
    if (!creciendo) {
        cuerpo.pop_back();
    } else {
        creciendo = false;
    }
}

// No permite ir en dirección opuesta
void Serpiente::cambiarDireccion(int dX, int dY) {
    // Evitar una colisión inmediata
    if (dX == -direccionX && dY == -direccionY) {
        return;
    }

    direccionX = dX;
    direccionY = dY;
}

void Serpiente::crecer() {
    creciendo = true;
}

// Getters
Coordenada* Serpiente::getCabeza() {
    return &cuerpo[0]; // uso de punteros para manejar la cabeza
}

const vector<Coordenada>& Serpiente::getCuerpo() const {
    return cuerpo;
}

int Serpiente::getLongitud() const {
    return (int)cuerpo.size();
}

bool Serpiente::estaViva() const {
    return viva;
}

// Colisiones
bool Serpiente::colisionConSiMisma() const {
    // Comparar cabeza con cada segmento del cuerpo
    for (size_t i = 1; i < cuerpo.size(); i++) {
        if (cuerpo[0] == cuerpo[i]) {
            return true;
        }
    }
    return false;
}

bool Serpiente::colisionCon(const Coordenada& posicion) const {
    return cuerpo[0] == posicion;
}

// Control sobre viva
void Serpiente::matar() {
    viva = false;
}

void Serpiente::reset(int inicioX, int inicioY) {
    cuerpo.clear();
    direccionX = 1; direccionY = 0;
    viva = true;
    creciendo = false;
    for (int i = 0; i < 3; i++) {
        cuerpo.push_back({inicioX - i, inicioY });
    }
}
