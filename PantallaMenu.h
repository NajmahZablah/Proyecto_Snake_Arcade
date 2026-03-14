#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include <string>

/* PantallaMenu
 * Maneja 3 pantallas relacionadas al login:
 *  Login (Iniciar Sesión / Crear Cuenta)
 *  Registro (Crear Cuenta)
 *  Confirmar Eliminar Cuenta
*/

class PantallaMenu {
public:
    PantallaMenu(sf::RenderWindow& ventana, sf::Font& fuente, sf::Texture& texFondo, sf::Texture& texFondoSel);

    void dibujar(Juego& juego);
    void procesarEventos(sf::Event& evento, Juego& juego);

private:
    sf::RenderWindow& ventana;
    sf::Font& fuente;
    sf::Texture& texFondo; // fondo_menu.png
    sf::Texture& texFondoSel; // fondo_seleccion.png
    sf::Sprite sprFondo;

    // Campos de formulario
    std::string inputNombre;
    std::string inputContrasena;
    bool campoNombreActivo;
    std::string mensajeError;

    // Sub-pantallas
    void dibujarLogin();
    void dibujarRegistro();
    void dibujarConfirmarEliminar();

    void procesarLogin(sf::Event& evento, Juego& juego);
    void procesarRegistro(sf::Event& evento, Juego& juego);
    void procesarConfirmarEliminar(sf::Event& evento, Juego& juego);

    // Helpers visuales
    void dibujarTexto(const std::string& texto, float x, float y, int tamano, sf::Color color);
    void dibujarBoton(const std::string& texto, float x, float y, float ancho, float alto, sf::Color color);
    void dibujarCampoTexto(const std::string& etiqueta, const std::string& valor, float x, float y, bool activo, bool esClave = false);
    bool botonPresionado(float x, float y, float ancho, float alto);
    void ponerFondo(sf::Texture& tex);
    void limpiarCampos();
};
