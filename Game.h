#pragma once
#include "Zerpent.h"
#include <vector>
#include <string>

// Estados del Juego

enum class EstadoJuego {
    MENU,
    JUGANDO,
    PAUSANDO,
    GAME_OVER,
    HIGH_SCORES
};

enum class Nivel {
    UNO = 1, // velocidad constante y sin obstáculos
    DOS = 2, // Velocidad incremental + muros
    TRES = 3 // Obstáculos aleatorios + aceitunas especiales
};

// Clase principal del juego
class Juego {
public:
    Juego(int anchoTablero, int altoTablero);

    // Control principal
    void iniciar(Nivel nivelSeleccionado);
    void actualizar();
    void procesarEntrada(int dx, int dy);
    void pausar();
    void reiniciar();

    // Getters para el renderizador
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
    void ordenarPuntajes(std::vector<PuntajeRecord>& lsita) const; // Ordenamiento burbuja

private:
    // Dimensiones
    int ancho, alto;

    // Entidades
    Serpiente serpiente;
    Aceituna aceituna;
    Estampilla estampilla;
    std::vector<Coordenada> obstaculos;

    // Estado
    EstadoJuego estado;
    Nivel nivelActual;
    int puntaje;
    float velocidadBase;
    float velocidadActual;

    // Métodos internos
    void generarAceituna();
    void generarEstampilla();
    void generarObstaculos();
    void verificarColisiones();
    void ajustarVelocidad();
    bool posicionLibre(const Coordenada& posicion) const;

    // Archivo de puntajes
    const std::string ARCHIVO_PUNTAJES = "highscores.txt";
};
