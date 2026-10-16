#include "Objeto.h"

#include <iostream>

Objeto::Objeto(const std::string& codigo, const std::string& nombre,
               const std::string& descripcion, const std::string& categoria, const Fecha& fecha,
               const std::string& lugar)
    : codigo_(codigo),
      nombre_(nombre),
      descripcion_(descripcion),
      categoria_(categoria),
      fecha_(fecha),
      lugar_(lugar) {}

void Objeto::mostrar() const {
    std::cout << "  Código:      " << codigo_ << "\n"
              << "  Nombre:      " << nombre_ << "\n"
              << "  Descripción: " << descripcion_ << "\n"
              << "  Categoría:   " << categoria_ << "\n"
              << "  Fecha:       " << fecha_.aTexto() << "\n"
              << "  Lugar:       " << lugar_ << "\n";
}

std::string Objeto::resumen() const {
    return codigo_ + " | " + nombre_ + " | " + categoria_ + " | " + lugar_ + " | " +
           fecha_.aTexto();
}

std::vector<std::string> Objeto::camposBase() const {
    return {codigo_, nombre_, descripcion_, categoria_, fecha_.aTexto(), lugar_};
}

bool Objeto::cargarBase(const std::vector<std::string>& c) {
    if (c.size() < 6) return false;
    Fecha f;
    if (!Fecha::parsear(c[4], f)) return false;
    codigo_ = c[0];
    nombre_ = c[1];
    descripcion_ = c[2];
    categoria_ = c[3];
    fecha_ = f;
    lugar_ = c[5];
    return true;
}
