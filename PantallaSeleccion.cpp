#include "PantallaSeleccion.h"
using namespace std;

const sf::Color PantallaSeleccion::COLOR_NARANJA   (255, 175,  70);
const sf::Color PantallaSeleccion::COLOR_MORADO    ( 75,  55, 110);
const sf::Color PantallaSeleccion::COLOR_MORADO_OSC( 45,  30,  70);
const sf::Color PantallaSeleccion::COLOR_BORDE     (200, 145,  60);
const sf::Color PantallaSeleccion::COLOR_GRIS_AZU  ( 55,  55,  85);
const sf::Color PantallaSeleccion::COLOR_ROJO_OSC  (130,  40,  40);
const sf::Color PantallaSeleccion::COLOR_VERDE_OSC ( 40, 110,  40);
const sf::Color PantallaSeleccion::COLOR_AZUL_OSC  ( 35,  65, 140);
const sf::Color PantallaSeleccion::COLOR_DORADO    (255, 210,  50);

PantallaSeleccion::PantallaSeleccion(sf::RenderWindow& ventana, sf::Font& fuente,
                                     sf::Texture& texFondo,
                                     sf::Texture& texEstMono,
                                     sf::Texture& texEstErizo,
                                     sf::Texture& texEstGato)
    : ventana(ventana), fuente(fuente), texFondo(texFondo),
    texEstMono(texEstMono), texEstErizo(texEstErizo), texEstGato(texEstGato),
    mostrarEstampillas(false)
{}

void PantallaSeleccion::dibujar(Juego& juego) {
    switch (juego.getEstado()) {
    case EstadoJuego::SELECCION: dibujarSeleccion(juego); break;
    case EstadoJuego::NIVELES:   dibujarNiveles();         break;
    default: break;
    }
    if (mostrarEstampillas)
        dibujarOverlayEstampillas();
}

void PantallaSeleccion::procesarEventos(sf::Event& evento, Juego& juego) {
    if (mostrarEstampillas) {
        if (evento.type == sf::Event::KeyPressed &&
            evento.key.code == sf::Keyboard::Escape) {
            mostrarEstampillas = false;
            return;
        }
        if (evento.type == sf::Event::MouseButtonPressed &&
            evento.mouseButton.button == sf::Mouse::Left) {
            if (botonPresionado(300, 460, 200, 44)) {
                mostrarEstampillas = false;
                return;
            }
        }
        return;
    }

    switch (juego.getEstado()) {
    case EstadoJuego::SELECCION: procesarSeleccion(evento, juego); break;
    case EstadoJuego::NIVELES:   procesarNiveles(evento, juego);   break;
    default: break;
    }
}

// ── Selección ─────────────────────────────────────────────────────────────────
void PantallaSeleccion::dibujarSeleccion(Juego& juego) {
    ponerFondo();
    dibujarTextoCentrado("SNAKE ARCADE", 70, 48, COLOR_NARANJA);
    string saludo = "Bienvenido, " + juego.getUsuarioActual() + "!";
    dibujarTextoCentrado(saludo, 148, 22, sf::Color::White);
    dibujarBoton("Niveles",         290, 220, 220, 55, COLOR_VERDE_OSC);
    dibujarBoton("Puntajes",        290, 295, 220, 55, COLOR_AZUL_OSC);
    dibujarBoton("Eliminar Cuenta", 290, 370, 220, 55, COLOR_ROJO_OSC);
    dibujarBoton("Cerrar Sesion",   290, 445, 220, 55, COLOR_GRIS_AZU);
}

void PantallaSeleccion::procesarSeleccion(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {
        if (botonPresionado(290, 220, 220, 55)) juego.setEstado(EstadoJuego::NIVELES);
        if (botonPresionado(290, 295, 220, 55)) juego.setEstado(EstadoJuego::HIGH_SCORES);
        if (botonPresionado(290, 370, 220, 55)) juego.setEstado(EstadoJuego::CONFIRMAR_ELIMINAR);
        if (botonPresionado(290, 445, 220, 55)) juego.setEstado(EstadoJuego::MENU);
    }
}

// ── Niveles ───────────────────────────────────────────────────────────────────
void PantallaSeleccion::dibujarNiveles() {
    ponerFondo();
    dibujarTextoCentrado("SELECCIONA NIVEL", 55, 42, COLOR_NARANJA);
    dibujarBoton("Nivel 1  -  Facil",   240, 155, 320, 65, COLOR_VERDE_OSC);
    dibujarBoton("Nivel 2  -  Medio",   240, 253, 320, 65, sf::Color(140, 100, 0));
    dibujarBoton("Nivel 3  -  Dificil", 240, 351, 320, 65, COLOR_ROJO_OSC);
    dibujarTexto("Velocidad constante, sin obstaculos",          258, 223, 13, sf::Color(200, 185, 215));
    dibujarTexto("Velocidad incremental + muros letales",        258, 321, 13, sf::Color(200, 185, 215));
    dibujarTexto("Obstaculos aleatorios + aceitunas especiales", 248, 419, 13, sf::Color(200, 185, 215));
    dibujarBoton("Ver Estampillas", 270, 445, 260, 42, sf::Color(80, 45, 130));
    dibujarBoton("Volver",          290, 500, 220, 42, COLOR_GRIS_AZU);
}

void PantallaSeleccion::procesarNiveles(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {
        if (botonPresionado(240, 155, 320, 65)) juego.iniciar(Nivel::UNO);
        if (botonPresionado(240, 253, 320, 65)) juego.iniciar(Nivel::DOS);
        if (botonPresionado(240, 351, 320, 65)) juego.iniciar(Nivel::TRES);
        if (botonPresionado(270, 445, 260, 42)) mostrarEstampillas = true;
        if (botonPresionado(290, 500, 220, 42)) juego.setEstado(EstadoJuego::SELECCION);
    }
    if (evento.type == sf::Event::KeyPressed &&
        evento.key.code == sf::Keyboard::Escape)
        juego.setEstado(EstadoJuego::SELECCION);
}

// ── Overlay: Ver Estampillas ──────────────────────────────────────────────────
void PantallaSeleccion::dibujarOverlayEstampillas() {
    sf::RectangleShape ov(sf::Vector2f(800.f, 600.f));
    ov.setFillColor(sf::Color(0, 0, 0, 200));
    ventana.draw(ov);

    // Panel: x=120, y=75, w=560, h=400
    const float PX = 120.f, PY = 75.f, PW = 560.f, PH = 400.f;
    sf::RectangleShape panel(sf::Vector2f(PW, PH));
    panel.setPosition(PX, PY);
    panel.setFillColor(sf::Color(45, 25, 75, 240));
    panel.setOutlineColor(COLOR_BORDE);
    panel.setOutlineThickness(2.f);
    ventana.draw(panel);

    dibujarTextoCentrado("ESTAMPILLAS",                              PY + 16.f, 28, COLOR_DORADO);
    dibujarTextoCentrado("Coleccionalas recolectandolas en cada nivel!", PY + 52.f, 13, sf::Color(200, 185, 220));

    // Tres zonas iguales dentro del panel
    // FIX #2: sprites centrados tanto horizontal como verticalmente en su zona
    struct Info { sf::Texture* tex; string nombreP; string nombreA; string nivel; };
    Info infos[3] = {
        { &texEstMono,  "Debonheir", "Mono",  "Nivel 1" },
        { &texEstErizo, "Doremin",   "Erizo", "Nivel 2" },
        { &texEstGato,  "Seinyan",   "Gato",  "Nivel 3" }
    };

    float zonaW   = PW / 3.f;          // ~186px por zona
    float sprTam  = 90.f;
    // Zona de sprite: entre y=85 y y=260 dentro del panel → centro en y=172
    float sprZonaCY = PY + 85.f + (180.f / 2.f); // centro vertical de la zona de sprite

    for (int i = 0; i < 3; i++) {
        float zonaCX = PX + zonaW * i + zonaW / 2.f; // centro X de la zona

        // FIX #2: centrar sprite tanto en X como en Y dentro de su zona
        sf::Sprite spr(*infos[i].tex);
        sf::Vector2u sz = infos[i].tex->getSize();
        float escala = sprTam / (float)max(sz.x, sz.y);
        float sw = sz.x * escala;
        float sh = sz.y * escala;
        spr.setScale(escala, escala);
        spr.setPosition(zonaCX - sw / 2.f, sprZonaCY - sh / 2.f);
        ventana.draw(spr);

        // Nombre del personaje
        sf::Text tP;
        tP.setFont(fuente);
        tP.setString(infos[i].nombreP);
        tP.setCharacterSize(16);
        tP.setFillColor(COLOR_DORADO);
        tP.setPosition(zonaCX - tP.getLocalBounds().width / 2.f, PY + 278.f);
        ventana.draw(tP);

        // Nombre del animal
        sf::Text tA;
        tA.setFont(fuente);
        tA.setString("(" + infos[i].nombreA + ")");
        tA.setCharacterSize(13);
        tA.setFillColor(COLOR_NARANJA);
        tA.setPosition(zonaCX - tA.getLocalBounds().width / 2.f, PY + 300.f);
        ventana.draw(tA);

        // Nivel
        sf::Text tN;
        tN.setFont(fuente);
        tN.setString(infos[i].nivel);
        tN.setCharacterSize(13);
        tN.setFillColor(sf::Color(180, 165, 205));
        tN.setPosition(zonaCX - tN.getLocalBounds().width / 2.f, PY + 320.f);
        ventana.draw(tN);
    }

    // Botón Cerrar dentro del panel
    dibujarBoton("Cerrar", 300, 460, 200, 44, COLOR_GRIS_AZU);

    // Hint con fondo
    sf::RectangleShape hintBg(sf::Vector2f(160.f, 20.f));
    hintBg.setPosition(320.f, 511.f);
    hintBg.setFillColor(sf::Color(0, 0, 0, 130));
    ventana.draw(hintBg);
    dibujarTextoCentrado("Esc = cerrar", 512.f, 13, sf::Color(210, 190, 140));
}

// ── Helpers visuales ──────────────────────────────────────────────────────────
void PantallaSeleccion::ponerFondo() {
    sprFondo.setTexture(texFondo);
    sf::Vector2u tam = texFondo.getSize();
    sprFondo.setScale(800.f / tam.x, 600.f / tam.y);
    ventana.draw(sprFondo);
}

void PantallaSeleccion::dibujarTextoCentrado(const string& texto, float y, int tamano, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tamano);
    t.setFillColor(color);
    t.setPosition((800.f - t.getLocalBounds().width) / 2.f, y);
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
    t.setPosition(x + (ancho - b.width) / 2.f, y + (alto - b.height) / 2.f - 4.f);
    ventana.draw(t);
}

bool PantallaSeleccion::botonPresionado(float x, float y, float ancho, float alto) {
    sf::Vector2i mouse = sf::Mouse::getPosition(ventana);
    return mouse.x >= x && mouse.x <= x + ancho && mouse.y >= y && mouse.y <= y + alto;
}
