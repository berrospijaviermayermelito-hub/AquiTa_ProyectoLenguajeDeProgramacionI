#include "ObjetoPerdido.h"

#include <iostream>

#include "Utilidades.h"

ObjetoPerdido::ObjetoPerdido(const std::string& codigo, const std::string& nombre,
                             const std::string& descripcion, const std::string& categoria,
                             const Fecha& fecha, const std::string& lugar,
                             const std::string& contactoNombres, const std::string& contactoCodigo,
                             const std::string& contactoTelefono,
                             const std::string& caracteristicaReservada)
    : Objeto(codigo, nombre, descripcion, categoria, fecha, lugar),
      contactoNombres_(contactoNombres),
      contactoCodigo_(contactoCodigo),
      contactoTelefono_(contactoTelefono),
      caracteristicaReservada_(caracteristicaReservada) {}

// Ficha COMPLETA (la ve el dueño y el Responsable). Para el público no se usa.
void ObjetoPerdido::mostrar() const {
    Objeto::mostrar();  // datos comunes
    std::cout << "  Contacto:    " << contactoNombres_ << " (código " << contactoCodigo_
              << ", tel. " << contactoTelefono_ << ")\n"
              << "  Reservada:   " << caracteristicaReservada_ << "\n"
              << "  Estado:      " << cat::aTexto(estado_) << "\n";
}

std::string ObjetoPerdido::resumen() const {
    return Objeto::resumen() + " | " + cat::aTexto(estado_);
}

// Orden de campos en reportes.txt (11 campos):
// codigo|nombre|descripcion|categoria|fecha|lugar|contactoNombres|contactoCodigo|
// contactoTelefono|caracteristicaReservada|estado
std::string ObjetoPerdido::aLinea() const {
    std::vector<std::string> c = camposBase();
    c.push_back(contactoNombres_);
    c.push_back(contactoCodigo_);
    c.push_back(contactoTelefono_);
    c.push_back(caracteristicaReservada_);
    c.push_back(cat::aTexto(estado_));
    return util::unir(c);
}

bool ObjetoPerdido::desdeLinea(const std::string& linea, ObjetoPerdido& resultado) {
    std::vector<std::string> c = util::separar(linea);
    if (c.size() != 11) return false;
    ObjetoPerdido tmp;
    if (!tmp.cargarBase(c)) return false;
    cat::EstadoReporte e;
    if (!cat::desdeTexto(c[10], e)) return false;
    tmp.contactoNombres_ = c[6];
    tmp.contactoCodigo_ = c[7];
    tmp.contactoTelefono_ = c[8];
    tmp.caracteristicaReservada_ = c[9];
    tmp.estado_ = e;
    resultado = tmp;
    return true;
}
