#include "Solicitud.h"

#include <iostream>
#include <vector>

#include "Utilidades.h"

Solicitud::Solicitud(const std::string& codigo, const std::string& codigoReporte,
                     const std::string& codigoObjeto, const Fecha& fechaSolicitud)
    : codigo_(codigo),
      codigoReporte_(codigoReporte),
      codigoObjeto_(codigoObjeto),
      fechaSolicitud_(fechaSolicitud) {}

void Solicitud::mostrar() const {
    std::cout << "  Código:        " << codigo_ << "\n"
              << "  Reporte:       " << codigoReporte_ << "\n"
              << "  Objeto:        " << codigoObjeto_ << "\n"
              << "  Fecha:         " << fechaSolicitud_.aTexto() << "\n"
              << "  Estado:        " << cat::aTexto(estado_) << "\n"
              << "  Observación:   " << (observacion_.empty() ? "-" : observacion_) << "\n"
              << "  Devolución:    " << fechaDevolucion_.aTexto() << "\n"
              << "  Recibió:       " << (receptorNombres_.empty() ? "-" : receptorNombres_)
              << " (código " << (receptorCodigo_.empty() ? "-" : receptorCodigo_) << ")\n";
}

std::string Solicitud::resumen() const {
    return codigo_ + " | reporte " + codigoReporte_ + " | objeto " + codigoObjeto_ + " | " +
           fechaSolicitud_.aTexto() + " | " + cat::aTexto(estado_);
}

// Orden de campos en solicitudes.txt (9 campos):
// codigo|codigoReporte|codigoObjeto|fechaSolicitud|estado|observacion|
// fechaDevolucion|receptorNombres|receptorCodigo
std::string Solicitud::aLinea() const {
    return util::unir({codigo_, codigoReporte_, codigoObjeto_, fechaSolicitud_.aTexto(),
                       cat::aTexto(estado_), observacion_, fechaDevolucion_.aTexto(),
                       receptorNombres_, receptorCodigo_});
}

bool Solicitud::desdeLinea(const std::string& linea, Solicitud& resultado) {
    std::vector<std::string> c = util::separar(linea);
    if (c.size() != 9) return false;
    Fecha fSol, fDev;
    if (!Fecha::parsear(c[3], fSol) || fSol.estaVacia()) return false;
    if (!Fecha::parsear(c[6], fDev)) return false;
    cat::EstadoSolicitud e;
    if (!cat::desdeTexto(c[4], e)) return false;
    Solicitud tmp(c[0], c[1], c[2], fSol);
    tmp.estado_ = e;
    tmp.observacion_ = c[5];
    tmp.fechaDevolucion_ = fDev;
    tmp.receptorNombres_ = c[7];
    tmp.receptorCodigo_ = c[8];
    resultado = tmp;
    return true;
}
