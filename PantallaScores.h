#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <string>

/* PantallaScores
 * Muestra el top 10 de puntajes filtrado por nivel.
 * Tiene 3 tabs: Nivel 1 / Nivel 2 / Nivel 3
 * Los datos vienen de Game::cargarPuntajes() (ya ordenados por bubble sort).
 */

class PantallaScores {
public:
    PantallaScores(sf::RenderWindow& ventana, sf::Font& fuente, sf::Texture& texFondo);

    void dibujar(Juego& juego);
    void procesarEventos(sf::Event& evento, Juego& juego);

private:
    sf::RenderWindow& ventana;
    sf::Font&         fuente;
    sf::Texture&      texFondo;
    sf::Sprite        sprFondo;

    int tabActiva; // 1, 2 o 3

    // Sub-dibujo
    void dibujarTabs();
    void dibujarTabla(const std::vector<PuntajeRecord>& lista);

    // Helpers visuales
    void dibujarTexto(const std::string& texto, float x, float y,
                      int tamano, sf::Color color);
    void dibujarTextoCentrado(const std::string& texto, float y,
                              int tamano, sf::Color color);
    void dibujarBoton(const std::string& texto, float x, float y,
                      float ancho, float alto, sf::Color color);
    bool botonPresionado(float x, float y, float ancho, float alto);

    // Paleta
    static const sf::Color COLOR_NARANJA;
    static const sf::Color COLOR_MORADO;
    static const sf::Color COLOR_MORADO_OSC;
    static const sf::Color COLOR_BORDE;
    static const sf::Color COLOR_GRIS_AZU;
    static const sf::Color COLOR_DORADO;
    static const sf::Color COLOR_VERDE_OSC;
    static const sf::Color COLOR_AMARILLO_OSC;
    static const sf::Color COLOR_ROJO_OSC;
};
