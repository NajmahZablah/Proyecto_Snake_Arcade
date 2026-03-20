#include "PantallaScores.h"
using namespace std;

// ── Paleta ────────────────────────────────────────────────────────────────────
const sf::Color PantallaScores::COLOR_NARANJA     (255, 175,  70);
const sf::Color PantallaScores::COLOR_MORADO      ( 75,  55, 110);
const sf::Color PantallaScores::COLOR_MORADO_OSC  ( 45,  30,  70);
const sf::Color PantallaScores::COLOR_BORDE       (200, 145,  60);
const sf::Color PantallaScores::COLOR_GRIS_AZU    ( 55,  55,  85);
const sf::Color PantallaScores::COLOR_DORADO      (255, 210,  50);
const sf::Color PantallaScores::COLOR_VERDE_OSC   ( 40, 110,  40);
const sf::Color PantallaScores::COLOR_AMARILLO_OSC(140, 100,   0);
const sf::Color PantallaScores::COLOR_ROJO_OSC    (130,  40,  40);

// ── Constructor ───────────────────────────────────────────────────────────────
PantallaScores::PantallaScores(sf::RenderWindow& ventana, sf::Font& fuente, sf::Texture& texFondo)
    : ventana(ventana), fuente(fuente), texFondo(texFondo),
    tabActiva(1)
{}

// ── Dibujar ───────────────────────────────────────────────────────────────────
void PantallaScores::dibujar(Juego& juego) {
    // Fondo
    sprFondo.setTexture(texFondo);
    sf::Vector2u tam = texFondo.getSize();
    sprFondo.setScale(800.f / tam.x, 600.f / tam.y);
    ventana.draw(sprFondo);

    // Título
    dibujarTextoCentrado("HIGH SCORES", 28.f, 38, COLOR_NARANJA);

    // Tabs de nivel
    dibujarTabs();

    // Cargar todos los puntajes y filtrar por tab activa
    auto todos = juego.cargarPuntajes(); // ya vienen ordenados por bubble sort
    vector<PuntajeRecord> filtrados;
    for (const auto& r : todos) {
        if (r.nivel == tabActiva) {
            filtrados.push_back(r);
        }
    }

    dibujarTabla(filtrados);

    // Botón volver
    dibujarBoton("Volver", 290, 530, 220, 44, COLOR_GRIS_AZU);
}

// ── Procesar Eventos ──────────────────────────────────────────────────────────
void PantallaScores::procesarEventos(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {

        // Tabs
        if (botonPresionado( 90, 88, 180, 38)) tabActiva = 1;
        if (botonPresionado(310, 88, 180, 38)) tabActiva = 2;
        if (botonPresionado(530, 88, 180, 38)) tabActiva = 3;

        // Volver
        if (botonPresionado(290, 530, 220, 44)) {
            juego.setEstado(EstadoJuego::SELECCION);
        }
    }

    if (evento.type == sf::Event::KeyPressed &&
        evento.key.code == sf::Keyboard::Escape) {
        juego.setEstado(EstadoJuego::SELECCION);
    }
}

// ── Tabs ──────────────────────────────────────────────────────────────────────
void PantallaScores::dibujarTabs() {
    struct Tab { string label; int nivel; float x; sf::Color color; };
    Tab tabs[3] = {
        { "Nivel 1 - Facil",   1,  90.f, COLOR_VERDE_OSC    },
        { "Nivel 2 - Medio",   2, 310.f, COLOR_AMARILLO_OSC },
        { "Nivel 3 - Dificil", 3, 530.f, COLOR_ROJO_OSC     }
    };

    for (const auto& tab : tabs) {
        bool activa = (tab.nivel == tabActiva);

        // Fondo del tab: más brillante si está activo
        sf::Color fondo = activa
                              ? sf::Color(tab.color.r + 30, tab.color.g + 30, tab.color.b + 30)
                              : tab.color;

        sf::RectangleShape rect(sf::Vector2f(180.f, 38.f));
        rect.setPosition(tab.x, 88.f);
        rect.setFillColor(fondo);
        rect.setOutlineColor(activa ? COLOR_DORADO : COLOR_BORDE);
        rect.setOutlineThickness(activa ? 2.5f : 1.5f);
        ventana.draw(rect);

        // Texto del tab
        sf::Text t;
        t.setFont(fuente);
        t.setString(tab.label);
        t.setCharacterSize(16);
        t.setFillColor(activa ? COLOR_DORADO : sf::Color(210, 190, 210));
        sf::FloatRect b = t.getLocalBounds();
        t.setPosition(tab.x + (180.f - b.width) / 2.f, 88.f + (38.f - b.height) / 2.f - 3.f);
        ventana.draw(t);

        // Subrayado en tab activa
        if (activa) {
            sf::RectangleShape linea(sf::Vector2f(180.f, 3.f));
            linea.setPosition(tab.x, 124.f);
            linea.setFillColor(COLOR_DORADO);
            ventana.draw(linea);
        }
    }
}

// ── Tabla de puntajes ─────────────────────────────────────────────────────────
void PantallaScores::dibujarTabla(const vector<PuntajeRecord>& lista) {
    // Encabezados de columna
    float yHeader = 140.f;
    sf::RectangleShape header(sf::Vector2f(640.f, 28.f));
    header.setPosition(80.f, yHeader);
    header.setFillColor(COLOR_MORADO);
    header.setOutlineColor(COLOR_BORDE);
    header.setOutlineThickness(1.f);
    ventana.draw(header);

    dibujarTexto("#",        98.f,  yHeader + 5.f, 15, COLOR_NARANJA);
    dibujarTexto("Jugador", 148.f,  yHeader + 5.f, 15, COLOR_NARANJA);
    dibujarTexto("Puntaje", 450.f,  yHeader + 5.f, 15, COLOR_NARANJA);
    dibujarTexto("Nivel",   620.f,  yHeader + 5.f, 15, COLOR_NARANJA);

    if (lista.empty()) {
        dibujarTextoCentrado("Sin registros todavia", 310.f, 20, sf::Color(160, 140, 180));
        return;
    }

    // Filas: máximo 10
    int limite = min((int)lista.size(), 10);
    for (int i = 0; i < limite; i++) {
        float y = 172.f + i * 34.f;

        // Fondo de fila alternado
        sf::RectangleShape fila(sf::Vector2f(640.f, 32.f));
        fila.setPosition(80.f, y);
        fila.setFillColor(i % 2 == 0
                              ? sf::Color(35, 25, 55, 200)
                              : sf::Color(50, 35, 75, 200));
        fila.setOutlineColor(sf::Color(80, 60, 100, 120));
        fila.setOutlineThickness(1.f);
        ventana.draw(fila);

        // Color especial para top 3
        sf::Color colorFila = sf::Color(210, 195, 225);
        if (i == 0) colorFila = COLOR_DORADO;
        else if (i == 1) colorFila = sf::Color(200, 200, 200); // plata
        else if (i == 2) colorFila = sf::Color(200, 140,  80); // bronce

        // Medalla para top 3
        string pos;
        if      (i == 0) pos = "1.";
        else if (i == 1) pos = "2.";
        else if (i == 2) pos = "3.";
        else             pos = to_string(i + 1) + ".";

        dibujarTexto(pos,                              98.f,  y + 7.f, 16, colorFila);
        dibujarTexto(lista[i].nombreJugador,          148.f,  y + 7.f, 16, colorFila);
        dibujarTexto(to_string(lista[i].puntaje),     450.f,  y + 7.f, 16, colorFila);

        // Nivel como texto legible
        string nivelStr;
        switch (lista[i].nivel) {
        case 1: nivelStr = "Facil";   break;
        case 2: nivelStr = "Medio";   break;
        case 3: nivelStr = "Dificil"; break;
        default: nivelStr = "?";
        }
        dibujarTexto(nivelStr, 620.f, y + 7.f, 16, colorFila);
    }
}

// ── Helpers visuales ──────────────────────────────────────────────────────────
void PantallaScores::dibujarTexto(const string& texto, float x, float y, int tamano, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tamano);
    t.setFillColor(color);
    t.setPosition(x, y);
    ventana.draw(t);
}

void PantallaScores::dibujarTextoCentrado(const string& texto, float y, int tamano, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tamano);
    t.setFillColor(color);
    float x = (800.f - t.getLocalBounds().width) / 2.f;
    t.setPosition(x, y);
    ventana.draw(t);
}

void PantallaScores::dibujarBoton(const string& texto, float x, float y, float ancho, float alto, sf::Color color) {
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
    t.setPosition(x + (ancho - b.width) / 2.f, y + (alto - b.height) / 2.f - 4.f);
    ventana.draw(t);
}

bool PantallaScores::botonPresionado(float x, float y, float ancho, float alto) {
    sf::Vector2i mouse = sf::Mouse::getPosition(ventana);
    return mouse.x >= x && mouse.x <= x + ancho &&
           mouse.y >= y && mouse.y <= y + alto;
}
