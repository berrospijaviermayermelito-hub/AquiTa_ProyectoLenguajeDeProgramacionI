// GestorSolicitudes.h - CRUD de las solicitudes de recuperación sobre un std::list.
// Se usa list (y no vector) porque las solicitudes se cancelan y cambian de estado
// con frecuencia, y list::erase no desplaza el resto de elementos.
#pragma once
#include <functional>
#include <list>
#include <string>
#include <vector>

#include "Solicitud.h"

class GestorSolicitudes {
public:
    int cargar(const std::string& ruta);
    bool guardar(const std::string& ruta) const;

    void agregar(const Solicitud& solicitud);  // list::push_back
    Solicitud* buscarPorCodigo(const std::string& codigo);
    std::vector<Solicitud*> filtrar(const std::function<bool(const Solicitud&)>& criterio);
    const std::list<Solicitud>& todas() const { return solicitudes_; }

    // TODO (UC10, opcional): eliminar() definitivo con list::erase, si el equipo lo decide.
    // Por la matriz, cancelar solo cambia el estado a «cancelada».

    size_t cantidad() const { return solicitudes_.size(); }

private:
    std::list<Solicitud> solicitudes_;
};
