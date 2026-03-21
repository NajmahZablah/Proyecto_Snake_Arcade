#pragma once
#include <SFML/Graphics.hpp>
#include "Game.h"
#include "PantallaMenu.h"
#include "PantallaSeleccion.h"
#include "PantallaJuego.h"
#include "PantallaScores.h"

class Renderer {
public:
    Renderer(int anchoVentana, int altoVentana);

    bool ventanaAbierta() const;
    void cerrar();

    void procesarEventos(Juego& juego);
    void actualizar(float dt, Juego& juego);
    void dibujar(Juego& juego);

private:
    sf::RenderWindow ventana;
    int anchoV, altoV;

    sf::Font fuente;

    sf::Texture texFondoMenu;
    sf::Texture texFondoSeleccion;
    sf::Texture texFondoJuego;
    sf::Texture texAceituna;
    sf::Texture texAceitunaDorada;
    sf::Texture texCaja;
    sf::Texture texEstMono;
    sf::Texture texEstErizo;
    sf::Texture texEstGato;

    PantallaMenu pantallaMenu;
    PantallaSeleccion pantallaSeleccion;
    PantallaJuego pantallaJuego;
    PantallaScores pantallaScores;

    void cargarFuente();
    void cargarTexturas();
    void cargarIcono(); // carga aceituna.png como ícono de la ventana
};
