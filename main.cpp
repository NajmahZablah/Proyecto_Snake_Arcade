#include "Game.h"
#include "Renderer.h"
#include <SFML/Graphics.hpp>

int main() {
    // Tablero 40x30 celdas | Ventana 800x600 pixeles
    Juego juego(40, 30);
    Renderer renderer(800, 600);

    sf::Clock reloj;

    while (renderer.ventanaAbierta()) {

        // 1) Eventos
        renderer.procesarEventos(juego);

        // 2) Lógica del juego (solo cuando está jugando)
        if (juego.getEstado() == EstadoJuego::JUGANDO) {
            if (reloj.getElapsedTime().asSeconds() >= juego.getVelocidad()) {
                juego.actualizar();
                reloj.restart();
            }
        }

        // 3) Dibujar
        renderer.dibujar(juego);
    }

    return 0;
}
