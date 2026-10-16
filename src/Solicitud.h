// Solicitud.h - Solicitud de recuperación: relaciona un reporte con un objeto encontrado (UC08 a UC12).
#pragma once
#include <string>

#include "Catalogos.h"
#include "Fecha.h"

class Solicitud {
public:
    Solicitud() = default;
    Solicitud(const std::string& codigo, const std::string& codigoReporte,
              const std::string& codigoObjeto, const Fecha& fechaSolicitud);

    // --- getters ---
    const std::string& codigo() const { return codigo_; }
    const std::string& codigoReporte() const { return codigoReporte_; }
    const std::string& codigoObjeto() const { return codigoObjeto_; }
    const Fecha& fechaSolicitud() const { return fechaSolicitud_; }
    cat::EstadoSolicitud estado() const { return estado_; }
    const std::string& observacion() const { return observacion_; }
    const Fecha& fechaDevolucion() const { return fechaDevolucion_; }
    const std::string& receptorNombres() const { return receptorNombres_; }
    const std::string& receptorCodigo() const { return receptorCodigo_; }

    // --- setters ---
    void setEstado(cat::EstadoSolicitud e) { estado_ = e; }
    void setObservacion(const std::string& v) { observacion_ = v; }       // FEAT15
    void setEntrega(const Fecha& fecha, const std::string& nombres,        // FEAT16
                    const std::string& codigoUniversitario) {
        fechaDevolucion_ = fecha;
        receptorNombres_ = nombres;
        receptorCodigo_ = codigoUniversitario;
    }

    void mostrar() const;
    std::string resumen() const;
    std::string aLinea() const;
    static bool desdeLinea(const std::string& linea, Solicitud& resultado);

private:
    std::string codigo_;
    std::string codigoReporte_;
    std::string codigoObjeto_;
    Fecha fechaSolicitud_;
    cat::EstadoSolicitud estado_ = cat::EstadoSolicitud::Pendiente;
    std::string observacion_;
    Fecha fechaDevolucion_;       // vacía hasta que se entrega el objeto
    std::string receptorNombres_; // quién recibió el objeto
    std::string receptorCodigo_;
};
