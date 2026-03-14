#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <string>

/* PantallaSeleccion
 * Maneja 2 pantallas post-login:
 *  Selección (Niveles / Puntajes / Salir)
 *  Niveles   (Nivel 1 / 2 / 3)
*/

class PantallaSeleccion {
public:
    PantallaSeleccion(sf::RenderWindow& ventana, sf::Font& fuente, sf::Texture& texFondo);

    void dibujar(Juego& juego);
    void procesarEventos(sf::Event& evento, Juego& juego);

private:
    sf::RenderWindow& ventana;
    sf::Font& fuente;
    sf::Texture& texFondo;
    sf::Sprite sprFondo;

    // Sub-pantallas
    void dibujarSeleccion(Juego& juego);
    void dibujarNiveles();

    void procesarSeleccion(sf::Event& evento, Juego& juego);
    void procesarNiveles(sf::Event& evento, Juego& juego);

    // Helpers visuales
    void dibujarTexto(const std::string& texto, float x, float y, int tamano, sf::Color color);
    void dibujarTextoCentrado(const std::string& texto, float y, int tamano, sf::Color color);
    void dibujarBoton(const std::string& texto, float x, float y, float ancho, float alto, sf::Color color);
    bool botonPresionado(float x, float y, float ancho, float alto);
    void ponerFondo();
};
