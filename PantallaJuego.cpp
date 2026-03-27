#include "PantallaJuego.h"
using namespace std;

const sf::Color PantallaJuego::COLOR_NARANJA (255, 175, 70);
const sf::Color PantallaJuego::COLOR_MORADO (75, 55, 110);
const sf::Color PantallaJuego::COLOR_MORADO_OSC(45, 30, 70);
const sf::Color PantallaJuego::COLOR_BORDE (200, 145, 60);
const sf::Color PantallaJuego::COLOR_GRIS_AZU (55, 55, 85);
const sf::Color PantallaJuego::COLOR_VERDE (60, 160, 60);
const sf::Color PantallaJuego::COLOR_DORADO (255, 210, 50);
const sf::Color PantallaJuego::COLOR_HINT (210, 190, 140);

PantallaJuego::PantallaJuego(sf::RenderWindow& ventana, sf::Font& fuente, sf::Texture& texFondo,
                             sf::Texture& texAceituna, sf::Texture& texAceitunaDorada,
                             sf::Texture& texCaja,
                             sf::Texture& texEstMono, sf::Texture& texEstErizo, sf::Texture& texEstGato)
    : ventana(ventana), fuente(fuente), texFondo(texFondo),
    texAceituna(texAceituna), texAceitunaDorada(texAceitunaDorada),
    texCaja(texCaja),
    texEstMono(texEstMono), texEstErizo(texEstErizo), texEstGato(texEstGato),
    puntajeGuardado(false),
    mostrarOverlayEstampilla(false),
    timerOverlayEstampilla(0.f),
    idEstampillaRecolectada(0),
    estampilaYaMostrada(false)
{}

void PantallaJuego::actualizar(float dt) {
    if (mostrarOverlayEstampilla) {
        timerOverlayEstampilla -= dt;
        if (timerOverlayEstampilla <= 0.f) {
            mostrarOverlayEstampilla = false;
        }
    }
}

//  DIBUJAR
void PantallaJuego::dibujar(Juego& juego) {
    const Estampilla& est = juego.getEstampilla();
    if (est.recolectada && !estampilaYaMostrada) {
        mostrarOverlayEstampilla = true;
        timerOverlayEstampilla = DURACION_OVERLAY;
        idEstampillaRecolectada = est.id;
        estampilaYaMostrada = true;
    }

    sprFondo.setTexture(texFondo);
    sf::Vector2u tam = texFondo.getSize();
    sprFondo.setScale(800.f / tam.x, 600.f / tam.y);
    ventana.draw(sprFondo);

    dibujarTablero(juego);
    dibujarHUD(juego);

    EstadoJuego estado = juego.getEstado();
    if (estado == EstadoJuego::PAUSANDO) {
        dibujarOverlayPausa();
    } else if (estado == EstadoJuego::GAME_OVER) {
        dibujarOverlayGameOver(juego);
    }

    if (mostrarOverlayEstampilla && estado == EstadoJuego::JUGANDO) {
        dibujarOverlayEstampilla();
    }
}

//  PROCESAR EVENTOS
void PantallaJuego::procesarEventos(sf::Event& evento, Juego& juego) {
    EstadoJuego estado = juego.getEstado();

    if (estado == EstadoJuego::JUGANDO) {
        if (evento.type == sf::Event::KeyPressed) {
            switch (evento.key.code) {
            case sf::Keyboard::Up:
            case sf::Keyboard::W:    juego.procesarEntrada( 0,-1); break;
            case sf::Keyboard::Down:
            case sf::Keyboard::S:    juego.procesarEntrada( 0, 1); break;
            case sf::Keyboard::Left:
            case sf::Keyboard::A:    juego.procesarEntrada(-1, 0); break;
            case sf::Keyboard::Right:
            case sf::Keyboard::D:    juego.procesarEntrada( 1, 0); break;
            case sf::Keyboard::P:    juego.pausar(); break;
            case sf::Keyboard::Escape:
                resetearEstadoOverlay();
                juego.setEstado(EstadoJuego::NIVELES);
                break;
            case sf::Keyboard::R:
                resetearEstadoOverlay();
                juego.reiniciar();
                break;
            default: break;
            }
        }
    }
    else if (estado == EstadoJuego::PAUSANDO) {
        if (evento.type == sf::Event::KeyPressed) {
            if (evento.key.code == sf::Keyboard::P) juego.pausar();
            if (evento.key.code == sf::Keyboard::Escape) {
                resetearEstadoOverlay();
                juego.setEstado(EstadoJuego::NIVELES);
            }
        }
        if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
            if (botonPresionado(300, 280, 200, 46)) juego.pausar();
            if (botonPresionado(300, 340, 200, 46)) {
                resetearEstadoOverlay();
                juego.setEstado(EstadoJuego::NIVELES);
            }
        }
    }
    else if (estado == EstadoJuego::GAME_OVER) {
        if (evento.type == sf::Event::KeyPressed) {
            if (evento.key.code == sf::Keyboard::R) {
                resetearEstadoOverlay();
                juego.reiniciar();
            }
            if (evento.key.code == sf::Keyboard::Escape) {
                resetearEstadoOverlay();
                juego.setEstado(EstadoJuego::NIVELES);
            }
        }
        if (evento.type == sf::Event::MouseButtonPressed &&
            evento.mouseButton.button == sf::Mouse::Left) {
            if (botonPresionado(255, 318, 130, 42)) {
                resetearEstadoOverlay();
                juego.reiniciar();
            }
            if (botonPresionado(415, 318, 130, 42)) {
                resetearEstadoOverlay();
                juego.setEstado(EstadoJuego::NIVELES);
            }
        }
    }
}

void PantallaJuego::resetearEstadoOverlay() {
    puntajeGuardado = false;
    mostrarOverlayEstampilla = false;
    timerOverlayEstampilla = 0.f;
    estampilaYaMostrada = false;
}

//  HUD
void PantallaJuego::dibujarHUD(const Juego& juego) {
    sf::RectangleShape barra(sf::Vector2f(800.f, (float)HUD_ALTO));
    barra.setPosition(0, 0);
    barra.setFillColor(COLOR_MORADO_OSC);
    barra.setOutlineColor(COLOR_BORDE);
    barra.setOutlineThickness(1.f);
    ventana.draw(barra);

    dibujarTexto("Puntaje: " + to_string(juego.getPuntaje()), 14, 8, 20, COLOR_NARANJA);

    string nivelStr;
    switch (juego.getNievel()) {
        case Nivel::UNO:  {
            nivelStr = "Nivel 1 - Facil";
            break;
        }
        case Nivel::DOS:  {
            nivelStr = "Nivel 2 - Medio";
            break;
        }
        case Nivel::TRES: {
            nivelStr = "Nivel 3 - Dificil";
            break;
        }
    }
    dibujarTextoCentrado(nivelStr, 10, 18, sf::Color::White);

    string longStr = "Largo: " + to_string(juego.getSerpiente().getLongitud());
    sf::Text tL;
    tL.setFont(fuente);
    tL.setString(longStr);
    tL.setCharacterSize(18);
    tL.setFillColor(sf::Color(200, 185, 215));
    tL.setPosition(800.f - tL.getLocalBounds().width - 14.f, 10.f);
    ventana.draw(tL);

    const Estampilla& est = juego.getEstampilla();
    sf::Texture& texEst = texEstPorId(est.id);
    sf::Sprite spr(texEst);
    sf::Vector2u sz = texEst.getSize();
    float escala = 28.f / (float)max(sz.x, sz.y);
    spr.setScale(escala, escala);
    spr.setPosition(tL.getPosition().x - 38.f, 5.f);
    spr.setColor(est.recolectada ? sf::Color::White : sf::Color(180, 180, 180, 130));
    ventana.draw(spr);
}

//  TABLERO
void PantallaJuego::dibujarTablero(const Juego& juego) {
    // Marco
    sf::RectangleShape marco(sf::Vector2f(600.f, 560.f));
    marco.setPosition((float)TABLERO_X, (float)TABLERO_Y);
    marco.setFillColor(sf::Color(20, 14, 36, 100));
    marco.setOutlineColor(COLOR_BORDE);
    marco.setOutlineThickness(2.f);
    ventana.draw(marco);

    // Grilla
    for (int c = 0; c <= 30; c++) {
        sf::RectangleShape l(sf::Vector2f(1.f, 560.f));
        l.setPosition(TABLERO_X + c * CELDA, (float)TABLERO_Y);
        l.setFillColor(sf::Color(40, 28, 60, 40));
        ventana.draw(l);
    }
    for (int f = 0; f <= 28; f++) {
        sf::RectangleShape l(sf::Vector2f(600.f, 1.f));
        l.setPosition((float)TABLERO_X, TABLERO_Y + f * CELDA);
        l.setFillColor(sf::Color(40, 28, 60, 40));
        ventana.draw(l);
    }

    // Obstáculos: PNG a 44px
    for (const auto& obs : juego.getObstaculos()) {
        dibujarSpriteEnCelda(texCaja, obs.x, obs.y, 44);
    }

    // Aceitunas: PNG a 44px + glow
    const Aceituna& ac = juego.getAceituna();
    if (ac.activa) {
        float cx = TABLERO_X + ac.posicion.x * CELDA + CELDA / 2.f;
        float cy = TABLERO_Y + ac.posicion.y * CELDA + CELDA / 2.f;
        sf::Color glowAc = (ac.tipo == 1)
                               ? sf::Color(255, 200,  50, 90)
                               : sf::Color( 80, 220,  80, 90);
        dibujarGlow(cx, cy, 22.f, glowAc, 6);
        sf::Texture& texAc = (ac.tipo == 1) ? texAceitunaDorada : texAceituna;
        dibujarSpriteEnCelda(texAc, ac.posicion.x, ac.posicion.y, 44);
    }

    // Estampilla: PNG 54px + glow blanco
    const Estampilla& est = juego.getEstampilla();
    if (est.visible && !est.recolectada) {
        float cx = TABLERO_X + est.posicion.x * CELDA + CELDA / 2.f;
        float cy = TABLERO_Y + est.posicion.y * CELDA + CELDA / 2.f;
        dibujarGlow(cx, cy, 30.f, sf::Color(255, 255, 255, 70), 7);
        dibujarSpriteEnCelda(texEstPorId(est.id), est.posicion.x, est.posicion.y, 54);
    }

    // Serpiente
    const vector<Coordenada>& cuerpo = juego.getSerpiente().getCuerpo();
    for (size_t i = 0; i < cuerpo.size(); i++) {
        int px = cuerpo[i].x;
        int py = cuerpo[i].y;
        if (juego.getNievel() == Nivel::UNO) {
            px = ((px % 30) + 30) % 30;
            py = ((py % 28) + 28) % 28;
        }

        sf::RectangleShape seg(sf::Vector2f(CELDA - 2.f, CELDA - 2.f));
        seg.setPosition(TABLERO_X + px * CELDA + 1.f, TABLERO_Y + py * CELDA + 1.f);

        if (i == 0) {
            // CABEZA
            seg.setFillColor(sf::Color(130, 220, 100));
            seg.setOutlineColor(sf::Color::White);
            seg.setOutlineThickness(1.5f);
            ventana.draw(seg);

            float bx = TABLERO_X + px * CELDA + 1.f;
            float by = TABLERO_Y + py * CELDA + 1.f;
            float cs = (float)(CELDA - 2);

            // Dirección real desde cuerpo[0] - cuerpo[1], normalizada
            int ddx = 1, ddy = 0; // default: derecha
            if (cuerpo.size() >= 2) {
                int rawDx = cuerpo[0].x - cuerpo[1].x;
                int rawDy = cuerpo[0].y - cuerpo[1].y;
                // Normalizar por si hay wrap (nivel 1 puede dar salto de 29)
                if (rawDx >  1) rawDx = -1;
                if (rawDx < -1) rawDx =  1;
                if (rawDy >  1) rawDy = -1;
                if (rawDy < -1) rawDy =  1;
                ddx = rawDx;
                ddy = rawDy;
            }

            // Posición de los dos ojos según dirección
            float ox1, oy1, ox2, oy2;
            float er = 2.4f;
            if (ddx == 1) {           // → derecha
                ox1 = bx + cs*0.73f;  oy1 = by + cs*0.22f;
                ox2 = bx + cs*0.73f;  oy2 = by + cs*0.70f;
            } else if (ddx == -1) {   // ← izquierda
                ox1 = bx + cs*0.27f;  oy1 = by + cs*0.22f;
                ox2 = bx + cs*0.27f;  oy2 = by + cs*0.70f;
            } else if (ddy == -1) {   // ↑ arriba
                ox1 = bx + cs*0.22f;  oy1 = by + cs*0.27f;
                ox2 = bx + cs*0.70f;  oy2 = by + cs*0.27f;
            } else {                  // ↓ abajo
                ox1 = bx + cs*0.22f;  oy1 = by + cs*0.73f;
                ox2 = bx + cs*0.70f;  oy2 = by + cs*0.73f;
            }

            // Blanco del ojo
            sf::CircleShape ojoBl(er + 1.2f);
            ojoBl.setFillColor(sf::Color::White);
            ojoBl.setOrigin(er + 1.2f, er + 1.2f);
            ojoBl.setPosition(ox1, oy1); ventana.draw(ojoBl);
            ojoBl.setPosition(ox2, oy2); ventana.draw(ojoBl);

            // Pupila
            sf::CircleShape pupila(er * 0.58f);
            pupila.setFillColor(sf::Color(20, 20, 20));
            pupila.setOrigin(er * 0.58f, er * 0.58f);
            pupila.setPosition(ox1, oy1); ventana.draw(pupila);
            pupila.setPosition(ox2, oy2); ventana.draw(pupila);

            // Brillo del ojo
            sf::CircleShape brillo(er * 0.28f);
            brillo.setFillColor(sf::Color(255, 255, 255, 200));
            brillo.setOrigin(er * 0.28f, er * 0.28f);
            brillo.setPosition(ox1 + er*0.25f, oy1 - er*0.25f); ventana.draw(brillo);
            brillo.setPosition(ox2 + er*0.25f, oy2 - er*0.25f); ventana.draw(brillo);

            // SOMBRERO NEGRO
            float hAlaW  = cs * 0.90f;  // ancho del ala
            float hAlaH  = cs * 0.14f;  // alto del ala
            float hCuerW = cs * 0.56f;  // ancho del cuerpo
            float hCuerH = cs * 0.44f;  // alto del cuerpo
            float hAlaX  = bx + (cs - hAlaW)  / 2.f;
            float hCuerX = bx + (cs - hCuerW) / 2.f;
            float hBaseY = by - 1.f;          // base del sombrero sobre la celda
            float hCuerY = hBaseY - hCuerH;   // cuerpo encima del ala
            float hAlaY  = hBaseY - hAlaH * 0.6f;

            // Sombra suave
            sf::RectangleShape sombra(sf::Vector2f(hAlaW + 2.f, hAlaH * 0.5f));
            sombra.setPosition(hAlaX - 1.f, hBaseY + 1.f);
            sombra.setFillColor(sf::Color(0, 0, 0, 60));
            ventana.draw(sombra);

            // Ala
            sf::RectangleShape ala(sf::Vector2f(hAlaW, hAlaH));
            ala.setPosition(hAlaX, hAlaY);
            ala.setFillColor(sf::Color(18, 18, 18));
            ala.setOutlineColor(sf::Color(55, 55, 55));
            ala.setOutlineThickness(1.f);
            ventana.draw(ala);

            // Cuerpo del sombrero
            sf::RectangleShape cuerpoS(sf::Vector2f(hCuerW, hCuerH));
            cuerpoS.setPosition(hCuerX, hCuerY);
            cuerpoS.setFillColor(sf::Color(22, 22, 22));
            cuerpoS.setOutlineColor(sf::Color(55, 55, 55));
            cuerpoS.setOutlineThickness(1.f);
            ventana.draw(cuerpoS);

            // Cinta MORADA (COLOR_MORADO = 75, 55, 110)
            float cintaH = hCuerH * 0.19f;
            sf::RectangleShape cinta(sf::Vector2f(hCuerW, cintaH));
            cinta.setPosition(hCuerX, hCuerY + hCuerH * 0.70f);
            cinta.setFillColor(COLOR_MORADO);
            ventana.draw(cinta);

        } else {
            // CUERPO normal
            int brillo = max(60, 180 - (int)i * 4);
            seg.setFillColor(sf::Color(40, brillo, 40));
            seg.setOutlineColor(sf::Color(60, 140, 60));
            seg.setOutlineThickness(1.f);
            ventana.draw(seg);
        }
    }
}

//  OVERLAY PAUSA
void PantallaJuego::dibujarOverlayPausa() {
    sf::RectangleShape ov(sf::Vector2f(800.f, 600.f));
    ov.setFillColor(sf::Color(0, 0, 0, 160));
    ventana.draw(ov);

    const float PX = 260.f, PY = 200.f, PW = 280.f, PH = 250.f;
    sf::RectangleShape panel(sf::Vector2f(PW, PH));
    panel.setPosition(PX, PY);
    panel.setFillColor(COLOR_MORADO_OSC);
    panel.setOutlineColor(COLOR_BORDE);
    panel.setOutlineThickness(2.f);
    ventana.draw(panel);

    dibujarTextoCentrado("PAUSA", PY + 18.f, 34, COLOR_NARANJA);
    dibujarBoton("Reanudar", 300, PY + 80.f,  200, 46, COLOR_MORADO);
    dibujarBoton("Salir",    300, PY + 140.f, 200, 46, COLOR_GRIS_AZU);

    sf::RectangleShape hintBg(sf::Vector2f(230.f, 22.f));
    hintBg.setPosition(285.f, PY + 202.f);
    hintBg.setFillColor(sf::Color(0, 0, 0, 150));
    ventana.draw(hintBg);
    dibujarTextoCentrado("P = reanudar  |  Esc = salir", PY + 204.f, 14, COLOR_HINT);
}

//  OVERLAY GAME OVER
void PantallaJuego::dibujarOverlayGameOver(Juego& juego) {
    if (!puntajeGuardado && !juego.getUsuarioActual().empty()) {
        juego.guardarPuntaje(juego.getUsuarioActual());
        puntajeGuardado = true;
    }

    sf::RectangleShape ov(sf::Vector2f(800.f, 600.f));
    ov.setFillColor(sf::Color(0, 0, 0, 175));
    ventana.draw(ov);

    const float PX = 235.f, PY = 190.f, PW = 330.f, PH = 230.f;
    sf::RectangleShape panel(sf::Vector2f(PW, PH));
    panel.setPosition(PX, PY);
    panel.setFillColor(COLOR_MORADO_OSC);
    panel.setOutlineColor(COLOR_BORDE);
    panel.setOutlineThickness(2.f);
    ventana.draw(panel);

    dibujarTextoCentrado("GAME OVER", PY + 15.f, 34, sf::Color(255, 90, 90));
    dibujarTextoCentrado("Puntaje: " + to_string(juego.getPuntaje()), PY + 60.f, 22, sf::Color::White);
    dibujarTextoCentrado("Guardado como: " + juego.getUsuarioActual(), PY + 92.f, 14, sf::Color(180, 165, 200));

    dibujarBoton("Reiniciar", 255, PY + 128.f, 130, 42, COLOR_VERDE);
    dibujarBoton("Volver",    415, PY + 128.f, 130, 42, COLOR_GRIS_AZU);

    sf::RectangleShape hintBg(sf::Vector2f(290.f, 22.f));
    hintBg.setPosition(PX + 20.f, PY + 196.f);
    hintBg.setFillColor(sf::Color(0, 0, 0, 150));
    ventana.draw(hintBg);
    dibujarTextoCentrado("R = reiniciar  |  Esc = volver", PY + 198.f, 14, COLOR_HINT);
}

//  OVERLAY ESTAMPILLA
void PantallaJuego::dibujarOverlayEstampilla() {
    float progreso = timerOverlayEstampilla / DURACION_OVERLAY;
    float alpha = 1.f;
    if (progreso > 0.85f)      alpha = (1.f - progreso) / 0.15f;
    else if (progreso < 0.20f) alpha = progreso / 0.20f;

    sf::Uint8 a      = (sf::Uint8)(alpha * 230);
    sf::Uint8 aTexto = (sf::Uint8)(alpha * 255);

    sf::RectangleShape panel(sf::Vector2f(300.f, 250.f));
    panel.setPosition(250.f, 170.f);
    panel.setFillColor(sf::Color(45, 20, 70, a));
    panel.setOutlineColor(sf::Color(160, 80, 220, aTexto));
    panel.setOutlineThickness(3.f);
    ventana.draw(panel);

    sf::Text tFeliz;
    tFeliz.setFont(fuente);
    tFeliz.setString("Felicidades!");
    tFeliz.setCharacterSize(22);
    tFeliz.setFillColor(sf::Color(COLOR_DORADO.r, COLOR_DORADO.g, COLOR_DORADO.b, aTexto));
    tFeliz.setPosition(400.f - tFeliz.getLocalBounds().width / 2.f, 178.f);
    ventana.draw(tFeliz);

    sf::Texture& tex = texEstPorId(idEstampillaRecolectada);
    sf::Sprite spr(tex);
    sf::Vector2u sz = tex.getSize();
    float escala = 86.f / (float)max(sz.x, sz.y);
    float sprW = sz.x * escala;
    float sprH = sz.y * escala;
    spr.setScale(escala, escala);
    spr.setPosition(400.f - sprW / 2.f, 210.f);
    spr.setColor(sf::Color(255, 255, 255, aTexto));
    dibujarGlow(400.f, 210.f + sprH / 2.f, 50.f, sf::Color(255, 255, 255, (sf::Uint8)(alpha * 45)), 6);
    ventana.draw(spr);

    string nombreP, nombreA;
    switch (idEstampillaRecolectada) {
        case 0: {
            nombreP = "Debonheir";
            nombreA = "el Mono";
            break;
        }
        case 1: {
            nombreP = "Doremin";
            nombreA = "el Erizo";
            break;
        }
        case 2: {
            nombreP = "Seinyan";
            nombreA = "el Gato";
            break;
        }
    }

    sf::Text tDesc;
    tDesc.setFont(fuente);
    tDesc.setString("Has coleccionado a " + nombreP + " (" + nombreA + ")!");
    tDesc.setCharacterSize(13);
    tDesc.setFillColor(sf::Color(220, 200, 240, aTexto));
    tDesc.setPosition(400.f - tDesc.getLocalBounds().width / 2.f, 310.f);
    ventana.draw(tDesc);

    sf::Text tPts;
    tPts.setFont(fuente);
    tPts.setString("+50 puntos!");
    tPts.setCharacterSize(17);
    tPts.setFillColor(sf::Color(COLOR_NARANJA.r, COLOR_NARANJA.g, COLOR_NARANJA.b, aTexto));
    tPts.setPosition(400.f - tPts.getLocalBounds().width / 2.f, 332.f);
    ventana.draw(tPts);
}

//  HELPERS
void PantallaJuego::dibujarGlow(float cx, float cy, float radio, sf::Color color, int capas) {
    for (int i = capas; i >= 1; i--) {
        float r = radio * ((float)i / capas);
        sf::Uint8 a = (sf::Uint8)(color.a * (1.f - (float)i / (capas + 1)));
        sf::CircleShape c(r);
        c.setOrigin(r, r);
        c.setPosition(cx, cy);
        c.setFillColor(sf::Color(color.r, color.g, color.b, a));
        ventana.draw(c);
    }
}

void PantallaJuego::dibujarSpriteEnCelda(sf::Texture& tex, int cx, int cy, int tam) {
    sf::Sprite spr(tex);
    sf::Vector2u sz = tex.getSize();
    float escala = (float)tam / (float)max(sz.x, sz.y);
    spr.setScale(escala, escala);
    float offset = (CELDA - tam) / 2.f;
    spr.setPosition(TABLERO_X + cx * CELDA + offset, TABLERO_Y + cy * CELDA + offset);
    ventana.draw(spr);
}

sf::Texture& PantallaJuego::texEstPorId(int id) {
    switch (id) {
        case 1:  {
            return texEstErizo;
        }
        case 2:  {
            return texEstGato;
        }
        default: {
            return texEstMono;
        }
    }
}

void PantallaJuego::dibujarTexto(const string& texto, float x, float y, int tam, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tam);
    t.setFillColor(color);
    t.setPosition(x, y);
    ventana.draw(t);
}

void PantallaJuego::dibujarTextoCentrado(const string& texto, float y, int tam, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tam);
    t.setFillColor(color);
    t.setPosition((800.f - t.getLocalBounds().width) / 2.f, y);
    ventana.draw(t);
}

void PantallaJuego::dibujarBoton(const string& texto, float x, float y, float ancho, float alto, sf::Color color) {
    sf::RectangleShape rect(sf::Vector2f(ancho, alto));
    rect.setPosition(x, y);
    rect.setFillColor(color);
    rect.setOutlineColor(COLOR_BORDE);
    rect.setOutlineThickness(2.f);
    ventana.draw(rect);

    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(18);
    t.setFillColor(COLOR_NARANJA);
    sf::FloatRect b = t.getLocalBounds();
    t.setPosition(x + (ancho - b.width) / 2.f, y + (alto - b.height) / 2.f - 3.f);
    ventana.draw(t);
}

bool PantallaJuego::botonPresionado(float x, float y, float ancho, float alto) {
    sf::Vector2i m = sf::Mouse::getPosition(ventana);
    return m.x >= x && m.x <= x + ancho && m.y >= y && m.y <= y + alto;
}
