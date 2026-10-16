#include "ObjetoEncontrado.h"

#include <iostream>

#include "Utilidades.h"

ObjetoEncontrado::ObjetoEncontrado(const std::string& codigo, const std::string& nombre,
                                   const std::string& descripcion, const std::string& categoria,
                                   const Fecha& fecha, const std::string& lugar,
                                   const std::string& lugarGuardado)
    : Objeto(codigo, nombre, descripcion, categoria, fecha, lugar), lugarGuardado_(lugarGuardado) {}

void ObjetoEncontrado::mostrar() const {
    Objeto::mostrar();
    std::cout << "  Guardado en: " << lugarGuardado_ << "\n"
              << "  Estado:      " << cat::aTexto(estado_) << "\n";
}

// TODO (FEAT10): confirmar con el equipo qué datos muestra la ficha pública.
// Por ahora muestra lo que ayuda a reconocer el objeto y dónde recogerlo.
void ObjetoEncontrado::mostrarFicha() const {
    Objeto::mostrar();
    std::cout << "  Guardado en: " << lugarGuardado_ << "\n";
}

std::string ObjetoEncontrado::resumen() const {
    return Objeto::resumen() + " | " + cat::aTexto(estado_);
}

// Orden de campos en objetos.txt (8 campos):
// codigo|nombre|descripcion|categoria|fecha|lugar|lugarGuardado|estado
std::string ObjetoEncontrado::aLinea() const {
    std::vector<std::string> c = camposBase();
    c.push_back(lugarGuardado_);
    c.push_back(cat::aTexto(estado_));
    return util::unir(c);
}

bool ObjetoEncontrado::desdeLinea(const std::string& linea, ObjetoEncontrado& resultado) {
    std::vector<std::string> c = util::separar(linea);
    if (c.size() != 8) return false;
    ObjetoEncontrado tmp;
    if (!tmp.cargarBase(c)) return false;
    cat::EstadoObjeto e;
    if (!cat::desdeTexto(c[7], e)) return false;
    tmp.lugarGuardado_ = c[6];
    tmp.estado_ = e;
    resultado = tmp;
    return true;
}
