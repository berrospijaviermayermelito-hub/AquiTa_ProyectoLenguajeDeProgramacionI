// GestorReportes.h - CRUD de los reportes de objetos perdidos sobre un std::vector.
// Mismo patrón que GestorObjetos.
#pragma once
#include <functional>
#include <string>
#include <vector>

#include "ObjetoPerdido.h"

class GestorReportes {
public:
    int cargar(const std::string& ruta);
    bool guardar(const std::string& ruta) const;

    void agregar(const ObjetoPerdido& reporte);  // vector::push_back
    ObjetoPerdido* buscarPorCodigo(const std::string& codigo);
    std::vector<ObjetoPerdido*> filtrar(const std::function<bool(const ObjetoPerdido&)>& criterio);
    const std::vector<ObjetoPerdido>& todos() const { return reportes_; }

    // TODO (opcional): depurar() para eliminar definitivamente reportes cancelados,
    // igual que GestorObjetos::depurar().

    size_t cantidad() const { return reportes_.size(); }

private:
    std::vector<ObjetoPerdido> reportes_;
};
