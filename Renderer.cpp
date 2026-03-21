#include "Renderer.h"
#include <iostream>
using namespace std;

Renderer::Renderer(int anchoVentana, int altoVentana)
    : ventana(sf::VideoMode(anchoVentana, altoVentana), "Snake Arcade", sf::Style::Titlebar | sf::Style::Close),
    anchoV(anchoVentana), altoV(altoVentana),
    pantallaMenu(ventana, fuente, texFondoMenu, texFondoSeleccion),
    pantallaSeleccion(ventana, fuente, texFondoSeleccion,
                    texEstMono, texEstErizo, texEstGato),
    pantallaJuego(ventana, fuente, texFondoJuego,
                texAceituna, texAceitunaDorada, texCaja,
                texEstMono, texEstErizo, texEstGato),
    pantallaScores(ventana, fuente, texFondoSeleccion)
{
    ventana.setFramerateLimit(60);
    cargarFuente();
    cargarTexturas();
    cargarIcono();
}

bool Renderer::ventanaAbierta() const {
    return ventana.isOpen();
}

void Renderer::cerrar() {
    ventana.close();
}

void Renderer::cargarFuente() {
    if (!fuente.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "Error cargando fuente\n";
    }
}

void Renderer::cargarTexturas() {
    if (!texFondoMenu.loadFromFile("assets/fondo_menu.png")) {
        cerr << "Error: fondo_menu.png\n";
    }
    if (!texFondoSeleccion.loadFromFile("assets/fondo_seleccion.png")) {
        cerr << "Error: fondo_seleccion.png\n";
    }
    if (!texFondoJuego.loadFromFile("assets/fondo_juego.png")) {
        cerr << "Error: fondo_juego.png\n";
    }
    if (!texAceituna.loadFromFile("assets/aceituna.png")) {
        cerr << "Error: aceituna.png\n";
    }
    if (!texAceitunaDorada.loadFromFile("assets/aceituna_dorada.png")) {
        cerr << "Error: aceituna_dorada.png\n";
    }
    if (!texCaja.loadFromFile("assets/caja.png")) {
        cerr << "Error: caja.png\n";
    }
    if (!texEstMono.loadFromFile("assets/estampilla_mono.png")) {
        cerr << "Error: estampilla_mono.png\n";
    }
    if (!texEstErizo.loadFromFile("assets/estampilla_erizo.png")) {
        cerr << "Error: estampilla_erizo.png\n";
    }
    if (!texEstGato.loadFromFile("assets/estampilla_gato.png")) {
        cerr << "Error: estampilla_gato.png\n";
    }
}

void Renderer::cargarIcono() {
    // Usar aceituna.png como ícono de la ventana
    sf::Image icono;
    if (icono.loadFromFile("assets/aceituna.png")) {
        ventana.setIcon(icono.getSize().x, icono.getSize().y, icono.getPixelsPtr());
    } else {
        cerr << "Error cargando icono\n";
    }
}

// Procesar eventos
void Renderer::procesarEventos(Juego& juego) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {
        if (evento.type == sf::Event::Closed) {
            ventana.close();
        }

        EstadoJuego est = juego.getEstado();
        if (est == EstadoJuego::MENU || est == EstadoJuego::CREAR_CUENTA || est == EstadoJuego::CONFIRMAR_ELIMINAR) {
            pantallaMenu.procesarEventos(evento, juego);
        } else if (est == EstadoJuego::SELECCION || est == EstadoJuego::NIVELES) {
            pantallaSeleccion.procesarEventos(evento, juego);
        } else if (est == EstadoJuego::JUGANDO || est == EstadoJuego::PAUSANDO || est == EstadoJuego::GAME_OVER) {
            pantallaJuego.procesarEventos(evento, juego);
        } else if (est == EstadoJuego::HIGH_SCORES) {
            pantallaScores.procesarEventos(evento, juego);
        }
    }
}

void Renderer::actualizar(float dt, Juego& juego) {
    EstadoJuego est = juego.getEstado();
    if (est == EstadoJuego::JUGANDO || est == EstadoJuego::PAUSANDO || est == EstadoJuego::GAME_OVER) {
        pantallaJuego.actualizar(dt);
    }
}

// Dibujar
void Renderer::dibujar(Juego& juego) {
    ventana.clear(sf::Color::Black);

    EstadoJuego est = juego.getEstado();
    if (est == EstadoJuego::MENU || est == EstadoJuego::CREAR_CUENTA || est == EstadoJuego::CONFIRMAR_ELIMINAR) {
        pantallaMenu.dibujar(juego);
    }
    else if (est == EstadoJuego::SELECCION || est == EstadoJuego::NIVELES) {
        pantallaSeleccion.dibujar(juego);
    } else if (est == EstadoJuego::JUGANDO || est == EstadoJuego::PAUSANDO || est == EstadoJuego::GAME_OVER) {
        pantallaJuego.dibujar(juego);
    } else if (est == EstadoJuego::HIGH_SCORES) {
        pantallaScores.dibujar(juego);
    }

    ventana.display();
}
