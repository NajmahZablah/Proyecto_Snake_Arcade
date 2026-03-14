#include "PantallaMenu.h"
using namespace std;

// Paleta de colores
static const sf::Color COLOR_NARANJA (255, 175, 70); // lámpara
static const sf::Color COLOR_MORADO (75, 55, 110); // fondo oscuro
static const sf::Color COLOR_MORADO_OSC (45, 30, 70); // fondo más oscuro
static const sf::Color COLOR_BORDE (200, 145, 60); // borde dorado
static const sf::Color COLOR_GRIS_AZU (55, 55, 85); // botón volver
static const sf::Color COLOR_ROJO_OSC (130, 40, 40); // botón eliminar

// Constructor
PantallaMenu::PantallaMenu(sf::RenderWindow& ventana, sf::Font& fuente, sf::Texture& texFondo, sf::Texture& texFondoSel)
    : ventana(ventana),
    fuente(fuente),
    texFondo(texFondo),
    texFondoSel(texFondoSel),
    inputNombre(""),
    inputContrasena(""),
    campoNombreActivo(true),
    mensajeError("")
{}

// Dibujar según estado
void PantallaMenu::dibujar(Juego& juego) {
    switch (juego.getEstado()) {
        case EstadoJuego::MENU: {
            dibujarLogin();
            break;
        }
        case EstadoJuego::CREAR_CUENTA: {
            dibujarRegistro();
            break;
        }
        case EstadoJuego::CONFIRMAR_ELIMINAR: {
            dibujarConfirmarEliminar();
            break;
        }
        default: {
            break;
        }
    }
}

// Procesar eventos según estado
void PantallaMenu::procesarEventos(sf::Event& evento, Juego& juego) {
    switch (juego.getEstado()) {
        case EstadoJuego::MENU: {
            procesarLogin(evento, juego);
            break;
        }
        case EstadoJuego::CREAR_CUENTA: {
            procesarRegistro(evento, juego);
            break;
        }
        case EstadoJuego::CONFIRMAR_ELIMINAR: {
            procesarConfirmarEliminar(evento, juego);
            break;
        }
        default: {
            break;
        }
    }
}

// SUB-PANTALLA 1: Login
void PantallaMenu::dibujarLogin() {
    ponerFondo(texFondo);

    // Título centrado dentro de la zona derecha (x = 450 a x = 800 Y ancho = 350)
    sf::Text tTitulo;
    tTitulo.setFont(fuente);
    tTitulo.setString("SNAKE ARCADE");
    tTitulo.setCharacterSize(36);
    float xTitulo = 450.f + (350.f - tTitulo.getLocalBounds().width) / 2.f;
    tTitulo.setFillColor(COLOR_NARANJA);
    tTitulo.setPosition(xTitulo, 60);
    ventana.draw(tTitulo);

    // Subtítulo centrado igual
    sf::Text tSub;
    tSub.setFont(fuente);
    tSub.setString("Iniciar Sesion");
    tSub.setCharacterSize(20);
    float xSub = 450.f + (350.f - tSub.getLocalBounds().width) / 2.f;
    tSub.setFillColor(sf::Color::White);
    tSub.setPosition(xSub, 112);
    ventana.draw(tSub);

    dibujarCampoTexto("Usuario:", inputNombre, 450, 190, campoNombreActivo);
    dibujarCampoTexto("Contrasena:", inputContrasena, 450, 268, !campoNombreActivo, true);

    dibujarBoton("Ingresar", 450, 338, 148, 46, COLOR_MORADO);
    dibujarBoton("Crear Cuenta", 608, 338, 152, 46, COLOR_MORADO_OSC);

    if (!mensajeError.empty()) {
        dibujarTexto(mensajeError, 450, 400, 15, sf::Color(255, 100, 100));
    }

    dibujarTexto("Tab = cambiar campo | Enter = ingresar", 452, 520, 13, sf::Color(160, 140, 180));
}

void PantallaMenu::procesarLogin(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Tab) {
            campoNombreActivo = !campoNombreActivo;
        }

        if (evento.key.code == sf::Keyboard::BackSpace) {
            if (campoNombreActivo && !inputNombre.empty()) {
                inputNombre.pop_back();
            } else if (!campoNombreActivo && !inputContrasena.empty()) {
                inputContrasena.pop_back();
            }
        }

        if (evento.key.code == sf::Keyboard::Return) {
            mensajeError = "";
            if (!juego.iniciarSesion(inputNombre, inputContrasena)) {
                mensajeError = "Usuario o contrasena incorrectos";
            } else {
                limpiarCampos();
            }
        }
    }

    if (evento.type == sf::Event::TextEntered) {
        char c = static_cast<char>(evento.text.unicode);
        if (c >= 32 && c < 127) {
            if (campoNombreActivo && inputNombre.size() < 12) {
                inputNombre += c;
            }
            else if (!campoNombreActivo && inputContrasena.size() < 12) {
                inputContrasena += c;
            }
        }
    }

    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {

        if (botonPresionado(450, 190, 320, 40)) {
            campoNombreActivo = true;
        }
        if (botonPresionado(450, 268, 320, 40)) {
            campoNombreActivo = false;
        }

        if (botonPresionado(450, 338, 148, 46)) {
            mensajeError = "";
            if (!juego.iniciarSesion(inputNombre, inputContrasena)) {
                mensajeError = "Usuario o contrasena incorrectos";
            } else {
                limpiarCampos();
            }
        }

        if (botonPresionado(608, 338, 152, 46)) {
            mensajeError = "";
            limpiarCampos();
            juego.setEstado(EstadoJuego::CREAR_CUENTA);
        }
    }
}

// SUB-PANTALLA 2: Registro
void PantallaMenu::dibujarRegistro() {
    ponerFondo(texFondoSel);

    dibujarTexto("CREAR CUENTA", 255, 70, 44, COLOR_NARANJA);

    dibujarCampoTexto("Nombre de usuario:", inputNombre, 240, 195, campoNombreActivo);
    dibujarCampoTexto("Contrasena:", inputContrasena, 240, 285, !campoNombreActivo, true);

    dibujarBoton("Registrarse", 290, 365, 220, 48, COLOR_MORADO);
    dibujarBoton("Volver", 290, 428, 220, 42, COLOR_GRIS_AZU);

    if (!mensajeError.empty()) {
        dibujarTexto(mensajeError, 255, 488, 16, sf::Color(255, 100, 100));
    }

    dibujarTexto("Tab = cambiar campo", 305, 540, 13, sf::Color(160, 140, 180));
}

void PantallaMenu::procesarRegistro(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::Tab) {
            campoNombreActivo = !campoNombreActivo;
        }

        if (evento.key.code == sf::Keyboard::BackSpace) {
            if (campoNombreActivo && !inputNombre.empty()) {
                inputNombre.pop_back();
            } else if (!campoNombreActivo && !inputContrasena.empty()) {
                inputContrasena.pop_back();
            }
        }
    }

    if (evento.type == sf::Event::TextEntered) {
        char c = static_cast<char>(evento.text.unicode);
        if (c >= 32 && c < 127) {
            if (campoNombreActivo && inputNombre.size() < 12) {
                inputNombre += c;
            } else if (!campoNombreActivo && inputContrasena.size() < 12) {
                inputContrasena += c;
            }
        }
    }

    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {

        if (botonPresionado(240, 195, 320, 40)) {
            campoNombreActivo = true;
        }
        if (botonPresionado(240, 285, 320, 40)) {
            campoNombreActivo = false;
        }

        // Registrarse -> crear cuenta y regresar al login
        if (botonPresionado(290, 365, 220, 48)) {
            mensajeError = "";
            if (inputNombre.empty() || inputContrasena.empty()) {
                mensajeError = "Completa todos los campos";
            } else if (!juego.crearCuenta(inputNombre, inputContrasena)) {
                mensajeError = "El usuario ya existe";
            } else {
                // Cuenta creada -> regresar al login
                limpiarCampos();
                juego.setEstado(EstadoJuego::MENU);
            }
        }

        // Volver -> login sin crear cuenta
        if (botonPresionado(290, 428, 220, 42)) {
            mensajeError = "";
            limpiarCampos();
            juego.setEstado(EstadoJuego::MENU);
        }
    }
}

//  SUB-PANTALLA 3: Confirmar Eliminar
void PantallaMenu::dibujarConfirmarEliminar() {
    ponerFondo(texFondoSel);

    dibujarTexto("ELIMINAR CUENTA", 240, 80,  40, sf::Color(255, 90, 90));
    dibujarTexto("Esta accion es permanente.", 230, 150, 20, sf::Color(210, 190, 210));
    dibujarTexto("Confirma tu contrasena:", 240, 195, 18, sf::Color(200, 180, 200));

    dibujarCampoTexto("", inputContrasena, 240, 228, true, true);

    dibujarBoton("Confirmar", 200, 310, 160, 48, COLOR_ROJO_OSC);
    dibujarBoton("Cancelar", 440, 310, 160, 48, COLOR_GRIS_AZU);

    if (!mensajeError.empty()) {
        dibujarTexto(mensajeError, 265, 375, 16, sf::Color(255, 100, 100));
    }
}

void PantallaMenu::procesarConfirmarEliminar(sf::Event& evento, Juego& juego) {
    if (evento.type == sf::Event::KeyPressed) {
        if (evento.key.code == sf::Keyboard::BackSpace && !inputContrasena.empty()) {
            inputContrasena.pop_back();
        }
        if (evento.key.code == sf::Keyboard::Escape) {
            limpiarCampos();
            juego.setEstado(EstadoJuego::SELECCION);
        }
    }

    if (evento.type == sf::Event::TextEntered) {
        char c = static_cast<char>(evento.text.unicode);
        if (c >= 32 && c < 127 && inputContrasena.size() < 12) {
            inputContrasena += c;
        }
    }

    if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {

        if (botonPresionado(200, 310, 160, 48)) {
            mensajeError = "";
            if (!juego.eliminarCuenta(inputContrasena)) {
                mensajeError = "Contrasena incorrecta";
            } else {
                limpiarCampos();
            }
        }

        if (botonPresionado(440, 310, 160, 48)) {
            limpiarCampos();
            juego.setEstado(EstadoJuego::SELECCION);
        }
    }
}

//  Helpers visuales
void PantallaMenu::ponerFondo(sf::Texture& tex) {
    sprFondo.setTexture(tex);
    sf::Vector2u tam = tex.getSize();
    sprFondo.setScale(800.f / tam.x, 600.f / tam.y);
    ventana.draw(sprFondo);
}

void PantallaMenu::limpiarCampos() {
    inputNombre = "";
    inputContrasena = "";
    campoNombreActivo = true;
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
    t.setPosition(x + (ancho - b.width) / 2.f, y + (alto  - b.height) / 2.f - 4.f);
    ventana.draw(t);
}

void PantallaMenu::dibujarCampoTexto(const string& etiqueta, const string& valor, float x, float y, bool activo, bool esClave) {
    if (!etiqueta.empty()) {
        dibujarTexto(etiqueta, x, y - 22, 17, sf::Color(210, 190, 210));
    }

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
