#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <string>

class PantallaJuego {
public:
    PantallaJuego(sf::RenderWindow& ventana, sf::Font& fuente, sf::Texture& texFondo,
                  sf::Texture& texAceituna, sf::Texture& texAceitunaDorada, sf::Texture& texCaja,
                  sf::Texture& texEstMono, sf::Texture& texEstErizo, sf::Texture& texEstGato);

    void dibujar(Juego& juego);
    void procesarEventos(sf::Event& evento, Juego& juego);
    void actualizar(float dt);

private:
    sf::RenderWindow& ventana;
    sf::Font& fuente;
    sf::Texture& texFondo;
    sf::Texture& texAceituna;
    sf::Texture& texAceitunaDorada;
    sf::Texture& texCaja;
    sf::Texture& texEstMono;
    sf::Texture& texEstErizo;
    sf::Texture& texEstGato;
    sf::Sprite sprFondo;

    bool  puntajeGuardado;

    // Overlay estampilla
    bool mostrarOverlayEstampilla;
    float timerOverlayEstampilla;
    int idEstampillaRecolectada;
    bool estampilaYaMostrada;  // evita reactivar en el mismo nivel
    static constexpr float DURACION_OVERLAY = 2.8f;

    // Layout
    static const int HUD_ALTO = 40;
    static const int CELDA = 20;
    static const int TABLERO_X = 100;
    static const int TABLERO_Y = 40;
    static const int SPRITE_TAM = 32;

    void dibujarHUD(const Juego& juego);
    void dibujarTablero(const Juego& juego);
    void dibujarOverlayPausa();
    void dibujarOverlayGameOver(Juego& juego);
    void dibujarOverlayEstampilla();
    void resetearEstadoOverlay();

    void dibujarSpriteEnCelda(sf::Texture& tex, int cx, int cy, int tam);
    void dibujarGlow(float cx, float cy, float radio, sf::Color color, int capas = 5);
    sf::Texture& texEstPorId(int id);

    void dibujarTexto(const std::string& texto, float x, float y, int tam, sf::Color color);
    void dibujarTextoCentrado(const std::string& texto, float y, int tam, sf::Color color);
    void dibujarBoton(const std::string& texto, float x, float y, float ancho, float alto, sf::Color color);
    bool botonPresionado(float x, float y, float ancho, float alto);

    static const sf::Color COLOR_NARANJA;
    static const sf::Color COLOR_MORADO;
    static const sf::Color COLOR_MORADO_OSC;
    static const sf::Color COLOR_BORDE;
    static const sf::Color COLOR_GRIS_AZU;
    static const sf::Color COLOR_VERDE;
    static const sf::Color COLOR_DORADO;
    static const sf::Color COLOR_HINT;
};
