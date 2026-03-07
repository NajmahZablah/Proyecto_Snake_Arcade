#include "Game.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

// Constructor
Juego::Juego(int anchoTablero, int altoTablero):
    ancho(anchoTablero), alto(altoTablero),
    serpiente(anchoTablero/2, altoTablero/2),
    estado(EstadoJuego::MENU),
    nivelActual(Nivel::UNO),
    puntaje(0),
    velocidadBase(0.15f),
    velocidadActual(0.25f)

{
    srand((unsigned)time(nullptr));
    aceituna = { {0, 0}, 0, 10, false };
    estampilla = { {0, 0}, 0, false, false, 50};
}

// Control principal
void Juego::iniciar(Nivel nivelSeleccionado) {
    nivelActual = nivelSeleccionado;
    puntaje = 0;
    estado = EstadoJuego::JUGANDO;

    serpiente.reset(ancho/2, alto/2);
    obstaculos.clear();

    // Velocidad base según el nivel
    switch (nivelActual) {
        case Nivel::UNO: {
            velocidadBase = 0.15f;
            break;
        }
        case Nivel::DOS: {
            velocidadBase = 0.12f;
            break;
        }
        case Nivel::TRES: {
            velocidadBase = 0.10f;
            break;
        }
    }

    velocidadActual = velocidadBase;

    // Nivel #3: generar obstáculos
    if (nivelActual == Nivel::TRES) {
        generarObstaculos();
    }

    generarAceituna();
    generarEstampilla();
}

void Juego::actualizar() {
    if (estado != EstadoJuego::JUGANDO) {
        return;
    }

    serpiente.mover();
    verificarColisiones();

    // Nivel #2 y #3: incrementar velocidad con puntaje
    if (nivelActual != Nivel::UNO) {
        ajustarVelocidad();
    }
}

void Juego::procesarEntrada(int dx, int dy) {
    if (estado == EstadoJuego::JUGANDO) {
        serpiente.cambiarDireccion(dx, dy);
    }
}

void Juego::pausar() {
    if (estado == EstadoJuego::JUGANDO) {
        estado = EstadoJuego::PAUSANDO;
    } else if (estado == EstadoJuego::PAUSANDO) {
        estado = EstadoJuego::JUGANDO;
    }
}

void Juego::reiniciar() {
    iniciar(nivelActual);
}

// Getters
EstadoJuego Juego::getEstado() const {
    return estado;
}

Nivel Juego::getNievel() const {
    return nivelActual;
}

const Serpiente& Juego::getSerpiente() const {
    return serpiente;
}

const Aceituna& Juego::getAceituna() const {
    return aceituna;
}

const Estampilla& Juego::getEstampilla() const {
    return estampilla;
}

const vector<Coordenada>& Juego::getObstaculos() const {
    return obstaculos;
}

int Juego::getPuntaje() const {
    return puntaje;
}

float Juego::getVelocidad() const {
    return velocidadActual;
}

// Lógica
void Juego::verificarColisiones() {
    Coordenada* cabeza = serpiente.getCabeza();

    // 1) Colisión con paredes (Nivel #2 y #3)
    if (nivelActual != Nivel::UNO) {
        if (cabeza->x < 0 || cabeza->x >= ancho || cabeza->y < 0 || cabeza->y >= alto) {
            serpiente.matar();
            estado = EstadoJuego::GAME_OVER;
            return;
        }
    } else {
        // Nivel #1: atravesar paredes
    }

    // 2) Colisión consigo misma lol
    if (serpiente.colisionConSiMisma()) {
        serpiente.matar();
        estado = EstadoJuego::GAME_OVER;
        return;
    }

    // 3) Colisión con obstáculos
    for (const auto& obs : obstaculos) { // for-each
        if (*cabeza == obs) {
            serpiente.matar();
            estado = EstadoJuego::GAME_OVER;
            return;
        }
    }

    // 4) Colisión con aceituna
    if (aceituna.activa && *cabeza == aceituna.posicion) {
        serpiente.crecer();
        puntaje += aceituna.puntos;
        if (aceituna.tipo == 1) {
            velocidadActual = velocidadBase * 0.7f;
        }
        aceituna.activa = false;
        generarAceituna();
    }
}

void Juego::generarEstampilla() {
    // id según nivel: 0 = mono, 1 = erizo, 2 = gato
    int id = (int)nivelActual -1;

    Coordenada posicion;
    do {
        posicion.x = rand() % ancho;
        posicion.y = rand() % alto;
    } while (!posicionLibre(posicion));

    estampilla = {posicion, id, false, true, 50};
}

void Juego::generarAceituna() {
    Coordenada nuevaPosicion;
    do {
        nuevaPosicion.x = rand() % ancho;
        nuevaPosicion.y = rand() % alto;
    } while (!posicionLibre(nuevaPosicion));

    // Nivel 3: 30% de probabilidad de aceituna dorada
    int tipo = (nivelActual == Nivel::TRES && (rand() % 10) < 3) ? 1 : 0;
    aceituna = { nuevaPosicion, tipo, (tipo == 1 ? 25 : 10), true };
}

void Juego::generarObstaculos() {
    int cantidad = 8 + (rand() % 5);
    for (int i = 0; i < cantidad; i++) {
        Coordenada obstaculo;
        do {
            obstaculo.x = rand() % ancho;
            obstaculo.y = rand() % alto;
        } while (!posicionLibre(obstaculo));
        obstaculos.push_back(obstaculo);
    }
}

void Juego::ajustarVelocidad() {
    // Cada 50 puntos se aumenta 5% la velocidad
    float factor = 1.0f - (puntaje/50) * 0.05f;
    if (factor < 0.4f) {
        factor = 0.4f; // límite máximo de velocidad
    }
    velocidadActual = velocidadBase * factor;
}

bool Juego::posicionLibre(const Coordenada& posicion) const {
    // Verificar que no esté dentro del cuerpo de la serpiente
    for (const auto& seg : serpiente.getCuerpo()) {
        if (seg == posicion) {
            return false;
        }
    }

    // Verificar que no esté dentro de los obstáculos
    for (const auto& obstaculo : obstaculos) {
        if (obstaculo == posicion) {
            return false;
        }
    }
    return true;
}

// Ordenar mediante el método burbuja los High Scores
void Juego::guardarPuntaje(const string& nombre) {
    auto lista = cargarPuntajes();
    lista.push_back( { nombre, puntaje, (int)nivelActual });
    ordenarPuntajes(lista);

    // Guardar el top #10
    ofstream archivo(ARCHIVO_PUNTAJES);
    int limite = min((int)lista.size(), 10);
    for (int i = 0; i < limite; i++) {
        archivo << lista[i].nombreJugador << "," << lista[i].puntaje << "," << lista[i].nivel << "\n";
    }
}

vector<PuntajeRecord> Juego::cargarPuntajes() const {
    vector<PuntajeRecord> lista;
    ifstream archivo(ARCHIVO_PUNTAJES);
    string linea;
    while (getline(archivo, linea)) {
        // Formato: "Nombre, puntaje, nivel"
        size_t columna1 = linea.find(',');
        size_t columna2 = linea.find(',', columna1 + 1);

        if (columna1 == string::npos || columna2 == string::npos) {
            continue;
        }

        PuntajeRecord record;
        record.nombreJugador = linea.substr(0, columna1);
        record.puntaje = stoi(linea.substr(columna1 + 1, columna2 - columna1 - 1));
        record.nivel = stoi(linea.substr(columna2 + 1));
        lista.push_back(record);
    }
    return lista;
}

void Juego::ordenarPuntajes(vector<PuntajeRecord>& lista) const {
    // Ordenamiento burbuja de mayor a menor puntaje
    int n = (int)lista.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (lista[j].puntaje < lista[j + 1].puntaje) {
                PuntajeRecord temp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = temp;
            }
        }
    }
}
