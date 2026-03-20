#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <string>

/* PantallaSeleccion
 * SUB-PANTALLAS:
 *   SELECCION — Niveles / Puntajes / Eliminar / Cerrar Sesión
 *   NIVELES   — Nivel 1/2/3 + botón "Ver Estampillas"
 * OVERLAY:
 *   Ver Estampillas — muestra los 3 animales con su nivel
 */

class PantallaSeleccion {
public:
    PantallaSeleccion(sf::RenderWindow& ventana, sf::Font& fuente,
                      sf::Texture& texFondo,
                      sf::Texture& texEstMono,
                      sf::Texture& texEstErizo,
                      sf::Texture& texEstGato);

    void dibujar(Juego& juego);
    void procesarEventos(sf::Event& evento, Juego& juego);

private:
    sf::RenderWindow& ventana;
    sf::Font&         fuente;
    sf::Texture&      texFondo;
    sf::Texture&      texEstMono;
    sf::Texture&      texEstErizo;
    sf::Texture&      texEstGato;
    sf::Sprite        sprFondo;

    bool mostrarEstampillas; // overlay de "Ver Estampillas"

    void dibujarSeleccion(Juego& juego);
    void dibujarNiveles();
    void dibujarOverlayEstampillas();

    void procesarSeleccion(sf::Event& evento, Juego& juego);
    void procesarNiveles(sf::Event& evento, Juego& juego);

    void dibujarTexto(const std::string& texto, float x, float y, int tamano, sf::Color color);
    void dibujarTextoCentrado(const std::string& texto, float y, int tamano, sf::Color color);
    void dibujarBoton(const std::string& texto, float x, float y, float ancho, float alto, sf::Color color);
    void dibujarSpriteEscalado(sf::Texture& tex, float x, float y, float tam);
    bool botonPresionado(float x, float y, float ancho, float alto);
    void ponerFondo();

    static const sf::Color COLOR_NARANJA;
    static const sf::Color COLOR_MORADO;
    static const sf::Color COLOR_MORADO_OSC;
    static const sf::Color COLOR_BORDE;
    static const sf::Color COLOR_GRIS_AZU;
    static const sf::Color COLOR_ROJO_OSC;
    static const sf::Color COLOR_VERDE_OSC;
    static const sf::Color COLOR_AZUL_OSC;
    static const sf::Color COLOR_DORADO;
};
