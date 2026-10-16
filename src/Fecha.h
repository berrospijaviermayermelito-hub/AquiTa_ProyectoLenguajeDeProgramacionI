// Fecha.h - Fecha DD/MM/AAAA con validación y comparación.
// Se usa en FEAT02/06 (fechas), FEAT19/20 (rangos) y FEAT21 (días guardados).
#pragma once
#include <string>

class Fecha {
public:
    Fecha() = default;                      // fecha vacía (0/0/0): "sin fecha"
    Fecha(int dia, int mes, int anio);

    bool estaVacia() const { return anio_ == 0; }
    int dia() const { return dia_; }
    int mes() const { return mes_; }
    int anio() const { return anio_; }

    std::string aTexto() const;             // "12/10/2026", o "-" si está vacía
    long aDias() const;                     // días desde 01/01/1970 (para restar fechas)

    static bool esValida(int dia, int mes, int anio);
    static bool parsear(const std::string& texto, Fecha& resultado);  // "-" = vacía
    static Fecha hoy();
    static long diasEntre(const Fecha& desde, const Fecha& hasta);    // hasta - desde

    // Pide una fecha por consola hasta que sea válida. Si permitirFutura es false,
    // rechaza fechas posteriores a hoy (un objeto no se pierde en el futuro).
    static Fecha leer(const std::string& mensaje, bool permitirFutura = false);

    bool operator==(const Fecha& o) const { return aDias() == o.aDias(); }
    bool operator!=(const Fecha& o) const { return !(*this == o); }
    bool operator<(const Fecha& o) const { return aDias() < o.aDias(); }
    bool operator<=(const Fecha& o) const { return aDias() <= o.aDias(); }
    bool operator>(const Fecha& o) const { return aDias() > o.aDias(); }
    bool operator>=(const Fecha& o) const { return aDias() >= o.aDias(); }

private:
    int dia_ = 0;
    int mes_ = 0;
    int anio_ = 0;
};
