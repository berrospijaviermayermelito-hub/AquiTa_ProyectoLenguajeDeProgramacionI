#include "Fecha.h"

#include <ctime>
#include <iomanip>
#include <iostream>
#include <sstream>

#include "Utilidades.h"

Fecha::Fecha(int dia, int mes, int anio) : dia_(dia), mes_(mes), anio_(anio) {}

std::string Fecha::aTexto() const {
    if (estaVacia()) return "-";
    std::ostringstream os;
    os << std::setfill('0') << std::setw(2) << dia_ << "/" << std::setw(2) << mes_ << "/"
       << std::setw(4) << anio_;
    return os.str();
}

// Días desde 01/01/1970 (algoritmo de calendario civil). Permite restar fechas.
long Fecha::aDias() const {
    int y = anio_;
    const int m = mes_;
    const int d = dia_;
    y -= (m <= 2) ? 1 : 0;
    const long era = (y >= 0 ? y : y - 399) / 400;
    const long yoe = y - era * 400;
    const long doy = (153 * (m + (m > 2 ? -3 : 9)) + 2) / 5 + d - 1;
    const long doe = yoe * 365 + yoe / 4 - yoe / 100 + doy;
    return era * 146097 + doe - 719468;
}

bool Fecha::esValida(int dia, int mes, int anio) {
    if (anio < 1900 || anio > 2100 || mes < 1 || mes > 12 || dia < 1) return false;
    static const int diasMes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int maxDia = diasMes[mes - 1];
    bool bisiesto = (anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0);
    if (mes == 2 && bisiesto) maxDia = 29;
    return dia <= maxDia;
}

bool Fecha::parsear(const std::string& texto, Fecha& resultado) {
    if (texto == "-") {
        resultado = Fecha();
        return true;
    }
    int d = 0, m = 0, a = 0;
    char s1 = 0, s2 = 0;
    std::istringstream is(texto);
    if (!(is >> d >> s1 >> m >> s2 >> a) || s1 != '/' || s2 != '/') return false;
    char resto;
    if (is >> resto) return false;  // sobra texto
    if (!esValida(d, m, a)) return false;
    resultado = Fecha(d, m, a);
    return true;
}

Fecha Fecha::hoy() {
    std::time_t t = std::time(nullptr);
    std::tm* lt = std::localtime(&t);
    return Fecha(lt->tm_mday, lt->tm_mon + 1, lt->tm_year + 1900);
}

long Fecha::diasEntre(const Fecha& desde, const Fecha& hasta) {
    return hasta.aDias() - desde.aDias();
}

Fecha Fecha::leer(const std::string& mensaje, bool permitirFutura) {
    while (true) {
        std::string t = util::leerLinea(mensaje + " (DD/MM/AAAA): ");
        Fecha f;
        if (t == "-" || !parsear(t, f)) {
            std::cout << "  Fecha inválida. Usa el formato DD/MM/AAAA, por ejemplo 25/10/2026.\n";
            continue;
        }
        if (!permitirFutura && f > hoy()) {
            std::cout << "  La fecha no puede ser posterior a hoy (" << hoy().aTexto() << ").\n";
            continue;
        }
        return f;
    }
}
