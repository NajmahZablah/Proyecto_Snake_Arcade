#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <string>

class PantallaMenu {
public:
    PantallaMenu(sf::RenderWindow& ventana, sf::Font& fuente,
                 sf::Texture& texFondo, sf::Texture& texFondoSel);

    void dibujar(Juego& juego);
    void procesarEventos(sf::Event& evento, Juego& juego);

private:
    sf::RenderWindow& ventana;
    sf::Font& fuente;
    sf::Texture& texFondo;
    sf::Texture& texFondoSel;
    sf::Sprite sprFondo;

    std::string inputNombre;
    std::string inputContrasena;
    std::string inputConfirmar;
    bool campoNombreActivo;
    int  campoActivoReg;   // 0=nombre, 1=pass, 2=confirmar
    std::string mensajeError;

    void dibujarLogin();
    void dibujarRegistro();
    void dibujarConfirmarEliminar();

    void procesarLogin(sf::Event& evento, Juego& juego);
    void procesarRegistro(sf::Event& evento, Juego& juego);
    void procesarConfirmarEliminar(sf::Event& evento, Juego& juego);

    // Texto plano
    void dibujarTexto(const std::string& texto, float x, float y, int tamano, sf::Color color);
    // Mensaje con fondo negro (para errores sobre fondos de imagen)
    void dibujarMensaje(const std::string& texto, float x, float y, int tamano, sf::Color color);
    // Hint con fondo semitransparente y color amarillo
    void dibujarHint(const std::string& texto, float x, float y);
    void dibujarBoton(const std::string& texto, float x, float y, float ancho, float alto, sf::Color color);
    void dibujarCampoTexto(const std::string& etiqueta, const std::string& valor,
                           float x, float y, bool activo, bool esClave = false);
    bool botonPresionado(float x, float y, float ancho, float alto);
    void ponerFondo(sf::Texture& tex);
    void limpiarCampos();

    static const sf::Color COLOR_NARANJA;
    static const sf::Color COLOR_MORADO;
    static const sf::Color COLOR_MORADO_OSC;
    static const sf::Color COLOR_BORDE;
    static const sf::Color COLOR_GRIS_AZU;
    static const sf::Color COLOR_ROJO_OSC;
    static const sf::Color COLOR_HINT;
};
