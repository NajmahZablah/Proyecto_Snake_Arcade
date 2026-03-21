#pragma once
#include "Zerpent.h"
#include <vector>
#include <string>

using namespace std;

enum class EstadoJuego {
    MENU,
    CREAR_CUENTA,
    SELECCION,
    NIVELES,
    CONFIRMAR_ELIMINAR,
    JUGANDO,
    PAUSANDO,
    GAME_OVER,
    HIGH_SCORES
};

enum class Nivel {
    UNO = 1, // Nivel Fácil
    DOS = 2, // Nivel Medio
    TRES = 3 // Nivel Experto
};

struct Usuario {
    string nombre;
    string contrasena;
};

class Juego {
public:
    Juego(int anchoTablero, int altoTablero);

    // Control principal
    void iniciar(Nivel nivelSeleccionado);
    void actualizar();
    void procesarEntrada(int dx, int dy);
    void pausar();
    void reiniciar();
    void setEstado(EstadoJuego nuevoEstado);

    // Sistema de usuarios
    bool crearCuenta(const string& nombre, const string& contrasena);
    bool iniciarSesion(const string& nombre, const string& contrasena);
    bool eliminarCuenta(const string& contrasena);
    string getUsuarioActual() const;

    // Getters
    EstadoJuego getEstado() const;
    Nivel getNievel() const;
    const Serpiente& getSerpiente() const;
    const Aceituna& getAceituna() const;
    const Estampilla& getEstampilla() const;
    const vector<Coordenada>& getObstaculos() const;
    int getPuntaje() const;
    float getVelocidad() const;

    // High Scores
    void guardarPuntaje(const string& nombre);
    vector<PuntajeRecord> cargarPuntajes() const;
    void ordenarPuntajes(vector<PuntajeRecord>& lista) const;

private:
    int ancho, alto;

    Serpiente serpiente;
    Aceituna aceituna;
    Estampilla estampilla;
    vector<Coordenada> obstaculos;

    EstadoJuego estado;
    Nivel nivelActual;
    int puntaje;
    float velocidadBase;
    float velocidadActual;

    string usuarioActual;

    void generarAceituna();
    void generarEstampilla();
    void generarObstaculos();
    void verificarColisiones();
    void ajustarVelocidad();
    bool posicionLibre(const Coordenada& posicion) const;

    const string ARCHIVO_PUNTAJES = "highscores.txt";
    const string ARCHIVO_USUARIOS = "usuarios.txt";

    vector<Usuario> cargarUsuarios() const;
    void guardarUsuarios(const vector<Usuario>& lista) const;
};
