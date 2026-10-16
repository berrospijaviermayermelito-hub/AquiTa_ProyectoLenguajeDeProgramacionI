// ObjetoPerdido.h - Reporte que hace un estudiante por un objeto que perdió (UC01 a UC03).
#pragma once
#include <string>

#include "Catalogos.h"
#include "Objeto.h"

class ObjetoPerdido : public Objeto {
public:
    ObjetoPerdido() = default;
    ObjetoPerdido(const std::string& codigo, const std::string& nombre,
                  const std::string& descripcion, const std::string& categoria,
                  const Fecha& fecha, const std::string& lugar,
                  const std::string& contactoNombres, const std::string& contactoCodigo,
                  const std::string& contactoTelefono, const std::string& caracteristicaReservada);

    // --- getters / setters propios ---
    const std::string& contactoNombres() const { return contactoNombres_; }
    const std::string& contactoCodigo() const { return contactoCodigo_; }       // código universitario
    const std::string& contactoTelefono() const { return contactoTelefono_; }
    // Detalle que solo conoce el dueño. NUNCA se muestra en una ficha pública (FEAT02, FEAT10).
    const std::string& caracteristicaReservada() const { return caracteristicaReservada_; }
    cat::EstadoReporte estado() const { return estado_; }

    void setContactoNombres(const std::string& v) { contactoNombres_ = v; }
    void setContactoTelefono(const std::string& v) { contactoTelefono_ = v; }
    void setCaracteristicaReservada(const std::string& v) { caracteristicaReservada_ = v; }
    void setEstado(cat::EstadoReporte e) { estado_ = e; }

    // --- polimorfismo ---
    void mostrar() const override;
    std::string resumen() const override;
    std::string aLinea() const override;

    // Crea un reporte a partir de una línea del archivo. Devuelve false si la línea es inválida.
    static bool desdeLinea(const std::string& linea, ObjetoPerdido& resultado);

private:
    std::string contactoNombres_;
    std::string contactoCodigo_;
    std::string contactoTelefono_;
    std::string caracteristicaReservada_;
    cat::EstadoReporte estado_ = cat::EstadoReporte::Pendiente;
};
