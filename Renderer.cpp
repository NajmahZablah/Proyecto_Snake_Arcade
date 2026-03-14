#include "Renderer.h"
#include <iostream>
using namespace std;

//  Constructor
Renderer::Renderer(int anchoVentana, int altoVentana)
    : ventana(sf::VideoMode(anchoVentana, altoVentana), "Snake Arcade", sf::Style::Titlebar | sf::Style::Close),
    anchoV(anchoVentana),
    altoV(altoVentana),
    pantallaMenu(ventana, fuente, texFondoMenu, texFondoSeleccion),
    pantallaSeleccion(ventana, fuente, texFondoSeleccion)
{
    ventana.setFramerateLimit(60);
    cargarFuente();
    cargarTexturas();
}

//  Control de ventana
bool Renderer::ventanaAbierta() const {
    return ventana.isOpen();
}
void Renderer::cerrar() {
    ventana.close();
}

//  Carga de recursos
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
}

// Procesar eventos: delega según estado
void Renderer::procesarEventos(Juego& juego) {
    sf::Event evento;
    while (ventana.pollEvent(evento)) {

        // Cerrar ventana siempre
        if (evento.type == sf::Event::Closed) {
            ventana.close();
        }

        EstadoJuego est = juego.getEstado();

        // Delegar a la pantalla correspondiente
        if (est == EstadoJuego::MENU || est == EstadoJuego::CREAR_CUENTA || est == EstadoJuego::CONFIRMAR_ELIMINAR) {
            pantallaMenu.procesarEventos(evento, juego);
        }
        else if (est == EstadoJuego::SELECCION || est == EstadoJuego::NIVELES) {
            pantallaSeleccion.procesarEventos(evento, juego);
        }
    }
}

//  Dibujar: delega según estado
void Renderer::dibujar(Juego& juego) {
    ventana.clear(sf::Color::Black);

    EstadoJuego est = juego.getEstado();

    if (est == EstadoJuego::MENU || est == EstadoJuego::CREAR_CUENTA || est == EstadoJuego::CONFIRMAR_ELIMINAR) {
        pantallaMenu.dibujar(juego);
    }
    else if (est == EstadoJuego::SELECCION || est == EstadoJuego::NIVELES) {
        pantallaSeleccion.dibujar(juego);
    }

    ventana.display();
}
