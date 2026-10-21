#include "Coincidencias.h"
#include <sstream>
#include <unordered_set>
#include <algorithm>
#include "Utilidades.h"

// Función auxiliar para extraer palabras normalizadas de un texto en un conjunto
static std::unordered_set<std::string> extraerPalabras(const std::string& texto) {
    std::unordered_set<std::string> palabras;
    std::istringstream stream(texto);
    std::string palabra;
    while (stream >> palabra) {
        std::string norm = util::normalizar(palabra);
        if (!norm.empty()) {
            palabras.insert(norm);
        }
    }
    return palabras;
}

double calcularPuntaje(const ObjetoPerdido& reporte, const ObjetoEncontrado& objeto) {
    // 1. Factor de Nombre (peso 50)
    // Proporción de palabras en común respecto al total de palabras únicas de ambos o intersección/unión (Jaccard)
    std::unordered_set<std::string> palabrasRep = extraerPalabras(reporte.nombre());
    std::unordered_set<std::string> palabrasObj = extraerPalabras(objeto.nombre());

    if (palabrasRep.empty() || palabrasObj.empty()) {
        return 0.0;
    }

    int comunes = 0;
    for (const std::string& p : palabrasRep) {
        if (palabrasObj.count(p) > 0) {
            comunes++;
        }
    }
    // Proporción de coincidencia basada en el tamaño del reporte o unión
    // Usaremos la proporción en relación con las palabras del reporte buscado:
    double factorNombre = static_cast<double>(comunes) / static_cast<double>(palabrasRep.size());

    // 2. Factor de Categoría (peso 30) - 1 si coincide, 0 si no
    double factorCategoria = (util::normalizar(reporte.categoria()) == util::normalizar(objeto.categoria())) ? 1.0 : 0.0;

    // 3. Factor de Lugar (peso 20) - 1 si coincide la zona, 0 si no
    double factorLugar = (util::normalizar(reporte.lugar()) == util::normalizar(objeto.lugar())) ? 1.0 : 0.0;

    // Fórmula oficial de la matriz: 50 * nombre + 30 * categoría + 20 * lugar
    double puntaje = (50.0 * factorNombre) + (30.0 * factorCategoria) + (20.0 * factorLugar);
    return puntaje;
}