#pragma once
#include "Zerpent.h"
#include <vector>
#include <string>

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
    UNO = 1,
    DOS = 2,
    TRES = 3
};

struct Usuario {
    std::string nombre;
    std::string contrasena;
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
    bool crearCuenta(const std::string& nombre, const std::string& contrasena);
    bool iniciarSesion(const std::string& nombre, const std::string& contrasena);
    bool eliminarCuenta(const std::string& contrasena);
    std::string getUsuarioActual() const;

    // Getters
    EstadoJuego getEstado() const;
    Nivel getNievel() const;
    const Serpiente& getSerpiente() const;
    const Aceituna& getAceituna() const;
    const Estampilla& getEstampilla() const;
    const std::vector<Coordenada>& getObstaculos() const;
    int getPuntaje() const;
    float getVelocidad() const;

    // High Scores
    void guardarPuntaje(const std::string& nombre);
    std::vector<PuntajeRecord> cargarPuntajes() const;
    void ordenarPuntajes(std::vector<PuntajeRecord>& lista) const;

private:
    int ancho, alto;

    Serpiente serpiente;
    Aceituna aceituna;
    Estampilla estampilla;
    std::vector<Coordenada> obstaculos;

    EstadoJuego estado;
    Nivel nivelActual;
    int puntaje;
    float velocidadBase;
    float velocidadActual;

    std::string usuarioActual;

    void generarAceituna();
    void generarEstampilla();
    void generarObstaculos();
    void verificarColisiones();
    void ajustarVelocidad();
    bool posicionLibre(const Coordenada& posicion) const;

    const std::string ARCHIVO_PUNTAJES = "highscores.txt";
    const std::string ARCHIVO_USUARIOS = "usuarios.txt";

    std::vector<Usuario> cargarUsuarios() const;
    void guardarUsuarios(const std::vector<Usuario>& lista) const;
};
