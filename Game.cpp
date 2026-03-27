#include "Game.h"
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
using namespace std;

Juego::Juego(int anchoTablero, int altoTablero):
    ancho(anchoTablero), alto(altoTablero),
    serpiente(anchoTablero/2, altoTablero/2),
    estado(EstadoJuego::MENU),
    nivelActual(Nivel::UNO),
    puntaje(0),
    velocidadBase(0.15f),
    velocidadActual(0.25f),
    usuarioActual("")
{
    srand((unsigned)time(nullptr));
    aceituna = {{0, 0}, 0, 10, false};
    estampilla = {{0, 0}, 0, false, false, 50};
}

void Juego::iniciar(Nivel nivelSeleccionado) {
    nivelActual = nivelSeleccionado;
    puntaje = 0;
    estado = EstadoJuego::JUGANDO;
    serpiente.reset(ancho/2, alto/2);
    obstaculos.clear();

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

void Juego::setEstado(EstadoJuego nuevoEstado) {
    estado = nuevoEstado;
}

// Usuarios
bool Juego::crearCuenta(const string& nombre, const string& contrasena) {
    auto lista = cargarUsuarios();
    for (const auto& u : lista) {
        if (u.nombre == nombre) {
            return false;
        }
    }
    lista.push_back({ nombre, contrasena });
    guardarUsuarios(lista);
    usuarioActual = nombre;
    estado = EstadoJuego::SELECCION;
    return true;
}

bool Juego::iniciarSesion(const string& nombre, const string& contrasena) {
    auto lista = cargarUsuarios();
    for (const auto& u : lista) {
        if (u.nombre == nombre && u.contrasena == contrasena) {
            usuarioActual = nombre;
            estado = EstadoJuego::SELECCION;
            return true;
        }
    }
    return false;
}

bool Juego::eliminarCuenta(const string& contrasena) {
    auto lista = cargarUsuarios();
    for (auto it = lista.begin(); it != lista.end(); ++it) {
        if (it->nombre == usuarioActual && it->contrasena == contrasena) {
            lista.erase(it);
            guardarUsuarios(lista);
            usuarioActual = "";
            estado = EstadoJuego::MENU;
            return true;
        }
    }
    return false;
}

string Juego::getUsuarioActual() const {
    return usuarioActual;
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

// Colisiones
void Juego::verificarColisiones() {
    Coordenada* cabeza = serpiente.getCabeza();

    // Wrap en Nivel 1
    // Atraviesa los bordes y aparece al lado opuesto
    if (nivelActual == Nivel::UNO) {
        if (cabeza->x < 0) {
            cabeza->x = ancho - 1;
        }
        if (cabeza->x >= ancho) {
            cabeza->x = 0;
        }
        if (cabeza->y < 0) {
            cabeza->y = alto - 1;
        }
        if (cabeza->y >= alto) {
            cabeza->y = 0;
        }
    }

    // Paredes en Nivel 2 y 3
    // Pared letal, si la cabeza está fuera de limites es un GAME OVER
    if (nivelActual != Nivel::UNO) {
        if (cabeza->x < 0 || cabeza->x >= ancho || cabeza->y < 0 || cabeza->y >= alto) {
            serpiente.matar();
            estado = EstadoJuego::GAME_OVER;
            return;
        }
    }

    // Sí misma
    // Compara cuerpo[0] con cada segmento
    if (serpiente.colisionConSiMisma()) {
        serpiente.matar();
        estado = EstadoJuego::GAME_OVER;
        return;
    }

    // Obstáculos
    for (const auto& obs : obstaculos) {
        if (*cabeza == obs) {
            serpiente.matar();
            estado = EstadoJuego::GAME_OVER;
            return;
        }
    }

    // Aceituna — colisión exacta de 1 celda (tamaño visual 36px, razonable)
    if (aceituna.activa && *cabeza == aceituna.posicion) {
        serpiente.crecer();
        puntaje += aceituna.puntos;
        if (aceituna.tipo == 1) {
            velocidadActual = velocidadBase * 0.7f;
        }
        aceituna.activa = false;
        generarAceituna();
    }

    /* Estampilla — colisión expandida por radio de 1 celda
     * El sprite es 54px (2.7 celdas), así que activamos la recolección
     * si la cabeza toca la celda central O cualquier celda adyacente (8 direcciones)
    */
    if (estampilla.visible && !estampilla.recolectada) {
        int dx = abs(cabeza->x - estampilla.posicion.x);
        int dy = abs(cabeza->y - estampilla.posicion.y);
        if (dx <= 1 && dy <= 1) { // radio 1 -> cubre las 9 celdas alrededor
            puntaje += estampilla.puntosBono;
            estampilla.recolectada = true;
            estampilla.visible = false;
        }
    }
}

// Generación
void Juego::generarEstampilla() {
    const int MARGEN = 2;
    int id = (int)nivelActual - 1;
    Coordenada pos;
    do {
        pos.x = MARGEN + rand() % (ancho - MARGEN * 2);
        pos.y = MARGEN + rand() % (alto  - MARGEN * 2);
    } while (!posicionLibre(pos));
    estampilla = {pos, id, false, true, 50};
}

void Juego::generarAceituna() {
    Coordenada pos;
    do {
        pos.x = rand() % ancho;
        pos.y = rand() % alto;
    } while (!posicionLibre(pos));
    int tipo = (nivelActual == Nivel::TRES && (rand() % 10) < 3) ? 1 : 0;
    aceituna = {pos, tipo, (tipo == 1 ? 25 : 10), true};
}

void Juego::generarObstaculos() {
    int cantidad = 8 + (rand() % 5);
    for (int i = 0; i < cantidad; i++) {
        Coordenada obs;
        do {
            obs.x = rand() % ancho;
            obs.y = rand() % alto;
        } while (!posicionLibre(obs));
        obstaculos.push_back(obs);
    }
}

void Juego::ajustarVelocidad() {
    float factor = 1.0f - (puntaje / 50) * 0.05f;
    if (factor < 0.4f) factor = 0.4f;
    velocidadActual = velocidadBase * factor;
}

bool Juego::posicionLibre(const Coordenada& pos) const {
    for (const auto& seg : serpiente.getCuerpo()) {
        if (seg == pos) {
            return false;
        }
    }
    for (const auto& obs : obstaculos) {
        if (obs == pos) {
            return false;
        }
    }
    if (aceituna.activa && aceituna.posicion == pos) {
        return false;
    }
    if (estampilla.visible && !estampilla.recolectada && estampilla.posicion == pos) {
        return false;
    }
    return true;
}

// High Scores
void Juego::guardarPuntaje(const string& nombre) {
    auto lista = cargarPuntajes();
    int nivelInt = (int)nivelActual;
    bool encontrado = false;
    for (auto& r : lista) {
        if (r.nombreJugador == nombre && r.nivel == nivelInt) {
            if (puntaje > r.puntaje) {
                r.puntaje = puntaje;
            }
            encontrado = true;
            break;
        }
    }
    if (!encontrado) lista.push_back({nombre, puntaje, nivelInt});

    ordenarPuntajes(lista);
    ofstream archivo(ARCHIVO_PUNTAJES);
    int limite = min((int)lista.size(), 10);
    for (int i = 0; i < limite; i++) {
        archivo << lista[i].nombreJugador << "|"
                << lista[i].puntaje << "|"
                << lista[i].nivel << "\n";
    }
}

vector<PuntajeRecord> Juego::cargarPuntajes() const {
    vector<PuntajeRecord> lista;
    ifstream archivo(ARCHIVO_PUNTAJES);
    string linea;
    while (getline(archivo, linea)) {
        size_t p1 = linea.find('|');
        size_t p2 = linea.find('|', p1 + 1);

        if (p1 == string::npos || p2 == string::npos) {
            continue;
        }

        PuntajeRecord r;
        r.nombreJugador = linea.substr(0, p1);
        r.puntaje = stoi(linea.substr(p1 + 1, p2 - p1 - 1));
        r.nivel = stoi(linea.substr(p2 + 1));
        lista.push_back(r);
    }
    return lista;
}

// Ordenamiento búrbuja
void Juego::ordenarPuntajes(vector<PuntajeRecord>& lista) const {
    int n = (int)lista.size();
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (lista[j].puntaje < lista[j + 1].puntaje) {
                PuntajeRecord tmp = lista[j];
                lista[j] = lista[j + 1];
                lista[j + 1] = tmp;
            }
        }
    }
}

vector<Usuario> Juego::cargarUsuarios() const {
    vector<Usuario> lista;
    ifstream archivo(ARCHIVO_USUARIOS);
    string linea;
    while (getline(archivo, linea)) {
        size_t pipe = linea.find('|');

        if (pipe == string::npos) {
            continue;
        }

        lista.push_back({ linea.substr(0, pipe), linea.substr(pipe + 1) });
    }
    return lista;
}

void Juego::guardarUsuarios(const vector<Usuario>& lista) const {
    ofstream archivo(ARCHIVO_USUARIOS);
    for (const auto& u : lista) {
        archivo << u.nombre << "|" << u.contrasena << "\n";
    }
}
