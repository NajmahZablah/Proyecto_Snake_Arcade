#include "PantallaSeleccion.h"
using namespace std;

// Misma paleta que PantallaMenu
static const sf::Color COLOR_NARANJA (255, 175, 70);
static const sf::Color COLOR_MORADO (75, 55, 110);
static const sf::Color COLOR_MORADO_OSC (45, 30, 70);
static const sf::Color COLOR_BORDE (200, 145, 60);
static const sf::Color COLOR_GRIS_AZU (55, 55, 85);
static const sf::Color COLOR_ROJO_OSC (130, 40, 40);
static const sf::Color COLOR_VERDE_OSC (40, 110, 40);
static const sf::Color COLOR_AZUL_OSC (35, 65, 140);

// Constructor
PantallaSeleccion::PantallaSeleccion(sf::RenderWindow& ventana, sf::Font& fuente, sf::Texture& texFondo)
    : ventana(ventana), fuente(fuente), texFondo(texFondo)
{}

// Dibujar según estado
void PantallaSeleccion::dibujar(Juego& juego) {
    switch (juego.getEstado()) {
        case EstadoJuego::SELECCION: {
            dibujarSeleccion(juego);
            break;
        }
        case EstadoJuego::NIVELES: {
            dibujarNiveles();
            break;
        }
        default: {
            break;
        }
    }
}

// Procesar eventos según estado
void PantallaSeleccion::procesarEventos(sf::Event& evento, Juego& juego) {
    switch (juego.getEstado()) {
        case EstadoJuego::SELECCION: {
            procesarSeleccion(evento, juego);
            break;
        }
        case EstadoJuego::NIVELES: {
            procesarNiveles(evento, juego);
            break;
        }
        default: {
            break;
        }
    }
}

//  SUB-PANTALLA 1: Selección post-login
void PantallaSeleccion::dibujarSeleccion(Juego& juego) {
    ponerFondo();

    // Título centrado (800px de ancho)
    dibujarTextoCentrado("SNAKE ARCADE", 70, 48, COLOR_NARANJA);

    // Saludo centrado
    string saludo = "Bienvenido, " + juego.getUsuarioActual() + "!";
    dibujarTextoCentrado(saludo, 148, 22, sf::Color::White);

    // Botones con paleta correcta
    dibujarBoton("Niveles", 290, 220, 220, 55, COLOR_VERDE_OSC);
    dibujarBoton("Puntajes", 290, 295, 220, 55, COLOR_AZUL_OSC);
    dibujarBoton("Eliminar Cuenta", 290, 370, 220, 55, COLOR_ROJO_OSC);
    dibujarBoton("Cerrar Sesion", 290, 445, 220, 55, COLOR_GRIS_AZU);
}

void PantallaSeleccion::procesarSeleccion(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {

        if (botonPresionado(290, 220, 220, 55)) {
            juego.setEstado(EstadoJuego::NIVELES);
        }

        if (botonPresionado(290, 295, 220, 55)) {
            juego.setEstado(EstadoJuego::HIGH_SCORES);
        }

        if (botonPresionado(290, 370, 220, 55)) {
            juego.setEstado(EstadoJuego::CONFIRMAR_ELIMINAR);
        }

        // Cerrar Sesión -> volver al menú de login
        if (botonPresionado(290, 445, 220, 55)) {
            juego.setEstado(EstadoJuego::MENU);
        }
    }
}

//  SUB-PANTALLA 2: Selección de Nivel
void PantallaSeleccion::dibujarNiveles() {
    ponerFondo();

    dibujarTextoCentrado("SELECCIONA NIVEL", 65, 42, COLOR_NARANJA);

    dibujarBoton("Nivel 1  -  Facil", 240, 175, 320, 65, COLOR_VERDE_OSC);
    dibujarBoton("Nivel 2  -  Medio", 240, 278, 320, 65, sf::Color(160, 110, 0));
    dibujarBoton("Nivel 3  -  Dificil", 240, 381, 320, 65, COLOR_ROJO_OSC);

    // Descripciones
    dibujarTexto("Velocidad constante, sin obstaculos", 258, 243, 13, sf::Color(200, 185, 215));
    dibujarTexto("Velocidad incremental + muros letales", 258, 346, 13, sf::Color(200, 185, 215));
    dibujarTexto("Obstaculos aleatorios + aceitunas especiales", 248, 449, 13, sf::Color(200, 185, 215));

    dibujarBoton("Volver", 290, 490, 220, 45, COLOR_GRIS_AZU);
}

void PantallaSeleccion::procesarNiveles(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {

        if (botonPresionado(240, 175, 320, 65)) {
            juego.iniciar(Nivel::UNO);
        }
        if (botonPresionado(240, 278, 320, 65)) {
            juego.iniciar(Nivel::DOS);
        }
        if (botonPresionado(240, 381, 320, 65)) {
            juego.iniciar(Nivel::TRES);
        }
        if (botonPresionado(290, 490, 220, 45)) {
            juego.setEstado(EstadoJuego::SELECCION);
        }
    }

    if (evento.type == sf::Event::KeyPressed && evento.key.code == sf::Keyboard::Escape) {
        juego.setEstado(EstadoJuego::SELECCION);
    }
}

// Helpers visuales
void PantallaSeleccion::ponerFondo() {
    sprFondo.setTexture(texFondo);
    sf::Vector2u tam = texFondo.getSize();
    sprFondo.setScale(800.f / tam.x, 600.f / tam.y);
    ventana.draw(sprFondo);
}

// Texto centrado horizontalmente en la ventana de 800px
void PantallaSeleccion::dibujarTextoCentrado(const string& texto, float y, int tamano, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tamano);
    t.setFillColor(color);
    float x = (800.f - t.getLocalBounds().width) / 2.f;
    t.setPosition(x, y);
    ventana.draw(t);
}

void PantallaSeleccion::dibujarTexto(const string& texto, float x, float y, int tamano, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tamano);
    t.setFillColor(color);
    t.setPosition(x, y);
    ventana.draw(t);
}

void PantallaSeleccion::dibujarBoton(const string& texto, float x, float y, float ancho, float alto, sf::Color color) {
    sf::RectangleShape rect(sf::Vector2f(ancho, alto));
    rect.setPosition(x, y);
    rect.setFillColor(color);
    rect.setOutlineColor(COLOR_BORDE);
    rect.setOutlineThickness(2.f);
    ventana.draw(rect);

    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(20);
    t.setFillColor(COLOR_NARANJA);
    sf::FloatRect b = t.getLocalBounds();
    t.setPosition(x + (ancho - b.width) / 2.f, y + (alto  - b.height) / 2.f - 4.f);
    ventana.draw(t);
}

bool PantallaSeleccion::botonPresionado(float x, float y, float ancho, float alto) {
    sf::Vector2i mouse = sf::Mouse::getPosition(ventana);
    return mouse.x >= x && mouse.x <= x + ancho && mouse.y >= y && mouse.y <= y + alto;
}
