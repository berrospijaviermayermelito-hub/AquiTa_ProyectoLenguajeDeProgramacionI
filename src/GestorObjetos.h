// GestorObjetos.h - CRUD de los objetos encontrados sobre un std::vector.
// Los otros dos gestores (reportes y solicitudes) siguen el mismo patrón.
#pragma once
#include <functional>
#include <string>
#include <vector>

#include "ObjetoEncontrado.h"

class GestorObjetos {
public:
    // ---- Persistencia (SUPL01 y SUPL02) ----
    // Devuelve cuántas líneas inválidas se omitieron (0 si todo salió bien).
    int cargar(const std::string& ruta);
    bool guardar(const std::string& ruta) const;

    // ---- CRUD ----
    // Create: inserta manteniendo la lista ordenada por fecha, del hallazgo más reciente
    // al más antiguo (usa vector::insert).
    void agregar(const ObjetoEncontrado& objeto);

    // Read: devuelve un puntero al objeto o nullptr si no existe.
    // Los punteros dejan de ser válidos después de agregar() o depurar().
    ObjetoEncontrado* buscarPorCodigo(const std::string& codigo);
    std::vector<ObjetoEncontrado*> filtrar(
        const std::function<bool(const ObjetoEncontrado&)>& criterio);
    const std::vector<ObjetoEncontrado>& todos() const { return objetos_; }

    // Update: se hace modificando el objeto que devuelve buscarPorCodigo() con sus
    // setters y luego llamando a guardar().

    // Delete: el borrado normal es lógico (estado «desactivado», FEAT08). Esta función
    // elimina definitivamente un objeto ya desactivado (vector::erase).
    bool depurar(const std::string& codigo);

    size_t cantidad() const { return objetos_.size(); }

private:
    std::vector<ObjetoEncontrado> objetos_;
};
