#include "Utilidades.h"

#include <cctype>
#include <cstdlib>
#include <fstream>
#include <iostream>

namespace util {

// ---------------------------------------------------------------- Texto

std::string recortar(const std::string& s) {
    size_t ini = 0, fin = s.size();
    while (ini < fin && std::isspace(static_cast<unsigned char>(s[ini]))) ++ini;
    while (fin > ini && std::isspace(static_cast<unsigned char>(s[fin - 1]))) --fin;
    return s.substr(ini, fin - ini);
}

// Pasa a minúsculas y cambia las vocales con tilde y la ñ (UTF-8) por su letra base.
// Así "Cafetería" y "cafeteria" se consideran iguales al buscar.
std::string normalizar(const std::string& s) {
    std::string r;
    r.reserve(s.size());
    for (size_t i = 0; i < s.size(); ++i) {
        unsigned char c = static_cast<unsigned char>(s[i]);
        if (c == 0xC3 && i + 1 < s.size()) {
            unsigned char d = static_cast<unsigned char>(s[i + 1]);
            char base = 0;
            switch (d) {
                case 0xA1: case 0x81: base = 'a'; break;
                case 0xA9: case 0x89: base = 'e'; break;
                case 0xAD: case 0x8D: base = 'i'; break;
                case 0xB3: case 0x93: base = 'o'; break;
                case 0xBA: case 0x9A: case 0xBC: case 0x9C: base = 'u'; break;
                case 0xB1: case 0x91: base = 'n'; break;
                default: break;
            }
            if (base != 0) {
                r.push_back(base);
                ++i;
                continue;
            }
        }
        r.push_back(static_cast<char>(std::tolower(c)));
    }
    return r;
}

bool contiene(const std::string& texto, const std::string& patron) {
    std::string p = normalizar(patron);
    if (p.empty()) return true;
    return normalizar(texto).find(p) != std::string::npos;
}

// Conserva los campos vacíos: "a||b" -> {"a", "", "b"}.
std::vector<std::string> separar(const std::string& linea, char delim) {
    std::vector<std::string> campos;
    std::string actual;
    for (char c : linea) {
        if (c == delim) {
            campos.push_back(actual);
            actual.clear();
        } else {
            actual.push_back(c);
        }
    }
    campos.push_back(actual);
    return campos;
}

std::string unir(const std::vector<std::string>& campos, char delim) {
    std::string r;
    for (size_t i = 0; i < campos.size(); ++i) {
        if (i > 0) r.push_back(delim);
        r += campos[i];
    }
    return r;
}

// ------------------------------------------------------ Entrada por consola

std::string leerLinea(const std::string& mensaje) {
    std::cout << mensaje;
    std::string s;
    if (!std::getline(std::cin, s)) {
        // Fin de la entrada (Ctrl+D / Ctrl+Z o entrada simulada que se acabó).
        std::cout << "\nEntrada finalizada. Hasta luego.\n";
        std::exit(0);
    }
    return recortar(s);
}

std::string leerObligatorio(const std::string& mensaje) {
    while (true) {
        std::string s = leerLinea(mensaje);
        if (s.empty()) {
            std::cout << "  El campo no puede estar vacío.\n";
        } else if (s.find(DELIM) != std::string::npos) {
            std::cout << "  No se permite el carácter '" << DELIM << "'.\n";
        } else {
            return s;
        }
    }
}

int leerEntero(const std::string& mensaje, int minimo, int maximo) {
    while (true) {
        std::string t = leerLinea(mensaje);
        try {
            size_t pos = 0;
            int v = std::stoi(t, &pos);
            if (pos == t.size() && v >= minimo && v <= maximo) return v;
        } catch (...) {
            // no era un número: se vuelve a pedir
        }
        std::cout << "  Opción inválida. Ingresa un número entre " << minimo << " y "
                  << maximo << ".\n";
    }
}

bool confirmar(const std::string& mensaje) {
    while (true) {
        std::string t = normalizar(leerLinea(mensaje + " (s/n): "));
        if (t == "s" || t == "si") return true;
        if (t == "n" || t == "no") return false;
        std::cout << "  Responde 's' o 'n'.\n";
    }
}

std::string elegirDeLista(const std::string& titulo, const std::vector<std::string>& opciones) {
    std::cout << titulo << "\n";
    for (size_t i = 0; i < opciones.size(); ++i) {
        std::cout << "  " << (i + 1) << ". " << opciones[i] << "\n";
    }
    int n = leerEntero("Elige una opción: ", 1, static_cast<int>(opciones.size()));
    return opciones[static_cast<size_t>(n - 1)];
}

void pausa() {
    leerLinea("\nPresiona Enter para continuar...");
}

// ------------------------------------------------------------- Archivos

std::vector<std::string> leerLineas(const std::string& ruta) {
    std::vector<std::string> lineas;
    std::ifstream in(ruta);
    if (!in) return lineas;  // aún no existe: es la primera ejecución
    std::string linea;
    while (std::getline(in, linea)) {
        linea = recortar(linea);
        if (!linea.empty()) lineas.push_back(linea);
    }
    return lineas;
}

bool escribirLineas(const std::string& ruta, const std::vector<std::string>& lineas) {
    std::ofstream out(ruta);
    if (!out) return false;
    for (const std::string& l : lineas) out << l << "\n";
    return static_cast<bool>(out);
}

}  // namespace util
