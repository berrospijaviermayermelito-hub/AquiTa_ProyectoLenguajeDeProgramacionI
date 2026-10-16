// ObjetoEncontrado.h - Objeto hallado en la universidad y custodiado por el Responsable (UC04 a UC06).
#pragma once
#include <string>

#include "Catalogos.h"
#include "Objeto.h"

class ObjetoEncontrado : public Objeto {
public:
    ObjetoEncontrado() = default;
    ObjetoEncontrado(const std::string& codigo, const std::string& nombre,
                     const std::string& descripcion, const std::string& categoria,
                     const Fecha& fecha, const std::string& lugar,
                     const std::string& lugarGuardado);

    const std::string& lugarGuardado() const { return lugarGuardado_; }  // dónde queda custodiado
    cat::EstadoObjeto estado() const { return estado_; }

    void setLugarGuardado(const std::string& v) { lugarGuardado_ = v; }
    void setEstado(cat::EstadoObjeto e) { estado_ = e; }

    void mostrar() const override;     // ficha completa (Responsable)
    void mostrarFicha() const;         // ficha pública para el estudiante (FEAT10)
    std::string resumen() const override;
    std::string aLinea() const override;

    static bool desdeLinea(const std::string& linea, ObjetoEncontrado& resultado);

private:
    std::string lugarGuardado_;
    cat::EstadoObjeto estado_ = cat::EstadoObjeto::Disponible;
};
