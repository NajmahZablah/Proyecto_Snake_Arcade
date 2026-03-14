#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "PantallaMenu.h"
#include "PantallaSeleccion.h"

//  Renderer: Controlador visual principal
//  Delega el dibujo a cada clase de pantalla
class Renderer {
public:
    Renderer(int anchoVentana, int altoVentana);

    bool ventanaAbierta() const;
    void cerrar();

    void procesarEventos(Juego& juego);
    void dibujar(Juego& juego);

private:
    // Ventana principal
    sf::RenderWindow ventana;
    int anchoV, altoV;

    // Recursos compartidos entre pantallas
    sf::Font fuente;
    sf::Texture texFondoMenu;
    sf::Texture texFondoSeleccion;

    // Pantallas activas
    PantallaMenu pantallaMenu;
    PantallaSeleccion pantallaSeleccion;

    // Carga de recursos
    void cargarTexturas();
    void cargarFuente();
};
