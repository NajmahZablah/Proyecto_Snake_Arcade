#include "Game.h"
#include "Renderer.h"
#include <SFML/Graphics.hpp>

int main() {
    // Tablero 30x28 celdas | Ventana 800x600 px
    Juego juego(30, 28);
    Renderer renderer(800, 600);

    sf::Clock reloj;
    sf::Clock relojDelta; // para dt de animaciones

    while (renderer.ventanaAbierta()) {
        float dt = relojDelta.restart().asSeconds();

        // 1) Eventos
        renderer.procesarEventos(juego);

        // 2) Lógica del juego
        if (juego.getEstado() == EstadoJuego::JUGANDO) {
            if (reloj.getElapsedTime().asSeconds() >= juego.getVelocidad()) {
                juego.actualizar();
                reloj.restart();
            }
        }

        // 3) Actualizar timers visuales (overlay estampilla, etc.)
        renderer.actualizar(dt, juego);

        // 4) Dibujar
        renderer.dibujar(juego);
    }

    return 0;
}
