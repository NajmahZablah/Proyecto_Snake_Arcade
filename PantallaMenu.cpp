#include "PantallaMenu.h"
using namespace std;

const sf::Color PantallaMenu::COLOR_NARANJA   (255, 175,  70);
const sf::Color PantallaMenu::COLOR_MORADO    ( 75,  55, 110);
const sf::Color PantallaMenu::COLOR_MORADO_OSC( 45,  30,  70);
const sf::Color PantallaMenu::COLOR_BORDE     (200, 145,  60);
const sf::Color PantallaMenu::COLOR_GRIS_AZU  ( 55,  55,  85);
const sf::Color PantallaMenu::COLOR_ROJO_OSC  (130,  40,  40);
const sf::Color PantallaMenu::COLOR_HINT      (210, 190, 140);

PantallaMenu::PantallaMenu(sf::RenderWindow& ventana, sf::Font& fuente,
                           sf::Texture& texFondo, sf::Texture& texFondoSel)
    : ventana(ventana), fuente(fuente),
    texFondo(texFondo), texFondoSel(texFondoSel),
    inputNombre(""), inputContrasena(""),
    campoNombreActivo(true), mensajeError(""),
    inputConfirmar(""), campoActivoReg(0)
{}

void PantallaMenu::dibujar(Juego& juego) {
    switch (juego.getEstado()) {
    case EstadoJuego::MENU:               dibujarLogin();             break;
    case EstadoJuego::CREAR_CUENTA:       dibujarRegistro();          break;
    case EstadoJuego::CONFIRMAR_ELIMINAR: dibujarConfirmarEliminar(); break;
    default: break;
    }
}

void PantallaMenu::procesarEventos(sf::Event& evento, Juego& juego) {
    switch (juego.getEstado()) {
    case EstadoJuego::MENU:               procesarLogin(evento, juego);             break;
    case EstadoJuego::CREAR_CUENTA:       procesarRegistro(evento, juego);          break;
    case EstadoJuego::CONFIRMAR_ELIMINAR: procesarConfirmarEliminar(evento, juego); break;
    default: break;
    }
}

// ── Login ─────────────────────────────────────────────────────────────────────
void PantallaMenu::dibujarLogin() {
    ponerFondo(texFondo);

    sf::Text tTitulo;
    tTitulo.setFont(fuente);
    tTitulo.setString("SNAKE ARCADE");
    tTitulo.setCharacterSize(36);
    tTitulo.setFillColor(COLOR_NARANJA);
    float xTitulo = 450.f + (350.f - tTitulo.getLocalBounds().width) / 2.f;
    tTitulo.setPosition(xTitulo, 60);
    ventana.draw(tTitulo);

    sf::Text tSub;
    tSub.setFont(fuente);
    tSub.setString("Iniciar Sesion");
    tSub.setCharacterSize(20);
    tSub.setFillColor(sf::Color::White);
    float xSub = 450.f + (350.f - tSub.getLocalBounds().width) / 2.f;
    tSub.setPosition(xSub, 112);
    ventana.draw(tSub);

    dibujarCampoTexto("Usuario:",    inputNombre,    450, 190, campoNombreActivo);
    dibujarCampoTexto("Contrasena:", inputContrasena, 450, 268, !campoNombreActivo, true);

    dibujarBoton("Ingresar",     450, 338, 148, 46, COLOR_MORADO);
    dibujarBoton("Crear Cuenta", 608, 338, 152, 46, COLOR_MORADO_OSC);

    // FIX #1: mensaje de error con fondo negro para que se vea sobre cualquier fondo
    if (!mensajeError.empty())
        dibujarMensaje(mensajeError, 450, 398, 15, sf::Color(255, 110, 110));

    dibujarHint("Tab = cambiar campo  |  Enter = ingresar", 452, 519);
}

void PantallaMenu::procesarLogin(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Tab)
            campoNombreActivo = !campoNombreActivo;
        if (evento.key.code == sf::Keyboard::BackSpace) {
            if (campoNombreActivo && !inputNombre.empty())           inputNombre.pop_back();
            else if (!campoNombreActivo && !inputContrasena.empty()) inputContrasena.pop_back();
        }
        if (evento.key.code == sf::Keyboard::Return) {
            mensajeError = "";
            if (!juego.iniciarSesion(inputNombre, inputContrasena))
                mensajeError = "Usuario o contrasena incorrectos";
            else limpiarCampos();
        }
    }
    if (evento.type == sf::Event::TextEntered) {
        char c = static_cast<char>(evento.text.unicode);
        if (c >= 32 && c < 127) {
            if (campoNombreActivo && inputNombre.size() < 12)           inputNombre += c;
            else if (!campoNombreActivo && inputContrasena.size() < 12) inputContrasena += c;
        }
    }
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {
        if (botonPresionado(450, 190, 320, 40)) campoNombreActivo = true;
        if (botonPresionado(450, 268, 320, 40)) campoNombreActivo = false;
        if (botonPresionado(450, 338, 148, 46)) {
            mensajeError = "";
            if (!juego.iniciarSesion(inputNombre, inputContrasena))
                mensajeError = "Usuario o contrasena incorrectos";
            else limpiarCampos();
        }
        if (botonPresionado(608, 338, 152, 46)) {
            mensajeError = "";
            limpiarCampos();
            juego.setEstado(EstadoJuego::CREAR_CUENTA);
        }
    }
}

// ── Registro ──────────────────────────────────────────────────────────────────
void PantallaMenu::dibujarRegistro() {
    ponerFondo(texFondoSel);

    sf::Text tTitulo;
    tTitulo.setFont(fuente);
    tTitulo.setString("CREAR CUENTA");
    tTitulo.setCharacterSize(44);
    tTitulo.setFillColor(COLOR_NARANJA);
    float x = (800.f - tTitulo.getLocalBounds().width) / 2.f;
    tTitulo.setPosition(x, 55);
    ventana.draw(tTitulo);

    dibujarCampoTexto("Nombre de usuario:",   inputNombre,    240, 165, campoActivoReg == 0);
    dibujarCampoTexto("Contrasena:",          inputContrasena, 240, 255, campoActivoReg == 1, true);
    dibujarCampoTexto("Confirmar contrasena:", inputConfirmar, 240, 345, campoActivoReg == 2, true);

    dibujarBoton("Registrarse", 290, 420, 220, 48, COLOR_MORADO);
    dibujarBoton("Volver",      290, 483, 220, 42, COLOR_GRIS_AZU);

    // FIX #1: mensaje con fondo visible
    if (!mensajeError.empty())
        dibujarMensaje(mensajeError, 240, 537, 15, sf::Color(255, 110, 110));

    dibujarHint("Tab = cambiar campo", 272, 562);
}

void PantallaMenu::procesarRegistro(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Tab)
            campoActivoReg = (campoActivoReg + 1) % 3;
        if (evento.key.code == sf::Keyboard::BackSpace) {
            if (campoActivoReg == 0 && !inputNombre.empty())     inputNombre.pop_back();
            if (campoActivoReg == 1 && !inputContrasena.empty()) inputContrasena.pop_back();
            if (campoActivoReg == 2 && !inputConfirmar.empty())  inputConfirmar.pop_back();
        }
    }
    if (evento.type == sf::Event::TextEntered) {
        char c = static_cast<char>(evento.text.unicode);
        if (c >= 32 && c < 127) {
            if (campoActivoReg == 0 && inputNombre.size() < 12)     inputNombre += c;
            if (campoActivoReg == 1 && inputContrasena.size() < 12) inputContrasena += c;
            if (campoActivoReg == 2 && inputConfirmar.size() < 12)  inputConfirmar += c;
        }
    }
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {
        if (botonPresionado(240, 165, 320, 40)) campoActivoReg = 0;
        if (botonPresionado(240, 255, 320, 40)) campoActivoReg = 1;
        if (botonPresionado(240, 345, 320, 40)) campoActivoReg = 2;
        if (botonPresionado(290, 420, 220, 48)) {
            mensajeError = "";
            if (inputNombre.empty() || inputContrasena.empty() || inputConfirmar.empty())
                mensajeError = "Completa todos los campos";
            else if (inputContrasena != inputConfirmar)
                mensajeError = "Las contrasenas no coinciden";
            else if (!juego.crearCuenta(inputNombre, inputContrasena))
                mensajeError = "El usuario ya existe";
            else {
                limpiarCampos();
                juego.setEstado(EstadoJuego::MENU);
            }
        }
        if (botonPresionado(290, 483, 220, 42)) {
            mensajeError = "";
            limpiarCampos();
            juego.setEstado(EstadoJuego::MENU);
        }
    }
}

// ── Confirmar Eliminar ────────────────────────────────────────────────────────
void PantallaMenu::dibujarConfirmarEliminar() {
    ponerFondo(texFondoSel);

    // FIX #3: todos los textos alineados al mismo x = 240
    const float X = 240.f;

    dibujarTexto("ELIMINAR CUENTA",           X,  75, 38, sf::Color(255, 90, 90));
    dibujarTexto("Esta accion es permanente.", X, 140, 18, sf::Color(210, 190, 210));
    dibujarTexto("La cuenta y sus datos se eliminaran.", X, 166, 16, sf::Color(185, 165, 185));
    dibujarTexto("Esta accion no se puede deshacer.",    X, 188, 16, sf::Color(185, 165, 185));

    dibujarCampoTexto("Confirma tu contrasena:", inputContrasena, X, 232, true, true);

    dibujarBoton("Confirmar", 200, 310, 160, 48, COLOR_ROJO_OSC);
    dibujarBoton("Cancelar",  440, 310, 160, 48, COLOR_GRIS_AZU);

    // FIX #1: mensaje con fondo
    if (!mensajeError.empty())
        dibujarMensaje(mensajeError, X, 372, 15, sf::Color(255, 110, 110));
}

void PantallaMenu::procesarConfirmarEliminar(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::BackSpace && !inputContrasena.empty())
            inputContrasena.pop_back();
        if (evento.key.code == sf::Keyboard::Escape) {
            limpiarCampos();
            juego.setEstado(EstadoJuego::SELECCION);
        }
    }
    if (evento.type == sf::Event::TextEntered) {
        char c = static_cast<char>(evento.text.unicode);
        if (c >= 32 && c < 127 && inputContrasena.size() < 12)
            inputContrasena += c;
    }
    if (evento.type == sf::Event::MouseButtonPressed &&
        evento.mouseButton.button == sf::Mouse::Left) {
        if (botonPresionado(200, 310, 160, 48)) {
            mensajeError = "";
            if (!juego.eliminarCuenta(inputContrasena))
                mensajeError = "Contrasena incorrecta";
            else limpiarCampos();
        }
        if (botonPresionado(440, 310, 160, 48)) {
            limpiarCampos();
            juego.setEstado(EstadoJuego::SELECCION);
        }
    }
}

// ── Helpers ───────────────────────────────────────────────────────────────────
void PantallaMenu::ponerFondo(sf::Texture& tex) {
    sprFondo.setTexture(tex);
    sf::Vector2u tam = tex.getSize();
    sprFondo.setScale(800.f / tam.x, 600.f / tam.y);
    ventana.draw(sprFondo);
}

void PantallaMenu::limpiarCampos() {
    inputNombre = "";
    inputContrasena = "";
    inputConfirmar = "";
    campoNombreActivo = true;
    campoActivoReg = 0;
    mensajeError = "";
}

void PantallaMenu::dibujarTexto(const string& texto, float x, float y, int tamano, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tamano);
    t.setFillColor(color);
    t.setPosition(x, y);
    ventana.draw(t);
}

// FIX #1: mensaje con rectángulo negro detrás para visibilidad sobre cualquier fondo
void PantallaMenu::dibujarMensaje(const string& texto, float x, float y, int tamano, sf::Color color) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(tamano);
    t.setFillColor(color);

    // Fondo negro semitransparente del tamaño exacto del texto
    sf::FloatRect bounds = t.getLocalBounds();
    sf::RectangleShape bg(sf::Vector2f(bounds.width + 12.f, bounds.height + 8.f));
    bg.setPosition(x - 2.f, y - 2.f);
    bg.setFillColor(sf::Color(0, 0, 0, 170));
    bg.setOutlineColor(sf::Color(200, 80, 80, 150));
    bg.setOutlineThickness(1.f);
    ventana.draw(bg);

    t.setPosition(x + 4.f, y + 2.f);
    ventana.draw(t);
}

// Hint con fondo semitransparente
void PantallaMenu::dibujarHint(const string& texto, float x, float y) {
    sf::Text t;
    t.setFont(fuente);
    t.setString(texto);
    t.setCharacterSize(14);

    sf::FloatRect b = t.getLocalBounds();
    sf::RectangleShape bg(sf::Vector2f(b.width + 12.f, 22.f));
    bg.setPosition(x - 2.f, y - 2.f);
    bg.setFillColor(sf::Color(0, 0, 0, 130));
    ventana.draw(bg);

    t.setFillColor(COLOR_HINT);
    t.setPosition(x + 4.f, y);
    ventana.draw(t);
}

void PantallaMenu::dibujarBoton(const string& texto, float x, float y, float ancho, float alto, sf::Color color) {
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

void PantallaMenu::dibujarCampoTexto(const string& etiqueta, const string& valor,
                                     float x, float y, bool activo, bool esClave) {
    if (!etiqueta.empty())
        dibujarTexto(etiqueta, x, y - 22, 17, sf::Color(210, 190, 210));

    sf::RectangleShape caja(sf::Vector2f(320.f, 40.f));
    caja.setPosition(x, y);
    caja.setFillColor(sf::Color(20, 15, 35));
    caja.setOutlineColor(activo ? COLOR_BORDE : sf::Color(90, 75, 110));
    caja.setOutlineThickness(2.f);
    ventana.draw(caja);

    string mostrar = esClave ? string(valor.size(), '*') : valor;
    dibujarTexto(mostrar + (activo ? "_" : ""), x + 8, y + 8, 20, sf::Color::White);
}

bool PantallaMenu::botonPresionado(float x, float y, float ancho, float alto) {
    sf::Vector2i mouse = sf::Mouse::getPosition(ventana);
    return mouse.x >= x && mouse.x <= x + ancho && mouse.y >= y && mouse.y <= y + alto;
}
