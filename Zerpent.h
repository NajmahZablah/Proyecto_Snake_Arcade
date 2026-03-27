#pragma once
#include <vector>
#include <string>

// Estructuras de datos principales

// Un punto en el tablero x e y
struct Coordenada {
    int x;
    int y;

    bool operator == (const Coordenada& otra) const {
        return x == otra.x && y == otra.y;
    }
};

// El objeto que la serpiente come
struct Aceituna {
    Coordenada posicion;
    int tipo; // 0 = aceituna normal +1 segmento + 10 pts, 1 = aceituna dorada +1 segmento + 25pts (altera la velocidad... broom! broom!)
    int puntos; // pts que otorga
    bool activa;
};

// Clase Zerpent
// Guarda un vector<Coordenada> que representa todo el cuerpo y cuerpo[0] es la cabeza
class Serpiente {
public:
    // Constructor
    Serpiente(int incioX, int incioY, int tamanioIncial = 3);

    // Movimiento
    void mover();
    void cambiarDireccion(int dx, int dy);
    void crecer();

    // Getters
    Coordenada* getCabeza(); // devuelve un puntero a cuerpo[0]
    const std:: vector<Coordenada>& getCuerpo() const;
    int getLongitud() const;
    bool estaViva() const;

    // Colisiones
    bool colisionConSiMisma() const;
    bool colisionCon(const Coordenada& pos) const;

    void matar();
    void reset(int incioX, int incioY);

private:
    std::vector<Coordenada> cuerpo;
    int direccionX, direccionY;
    bool viva;
    bool creciendo;
};

// Estructura Estampilla (coleccionable por nivel)
// Nivel 1 -> Mono (Debonheir)
// Nivel 2 -> Erizo (Doremin)
// Nivel 3 -> Gato (Seinyan)
struct Estampilla {
    Coordenada posicion;
    int id; // 0 = mono, 1 = erizo, 2 = gato
    bool recolectada;
    bool visible;
    int puntosBono; // pts extras
};

// Estructura para High Scores
struct PuntajeRecord {
    std:: string nombreJugador;
    int puntaje;
    int nivel;
};
