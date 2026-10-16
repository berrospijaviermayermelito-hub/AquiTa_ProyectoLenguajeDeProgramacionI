#include "GestorObjetos.h"

#include <algorithm>
#include <iostream>

#include "Utilidades.h"

int GestorObjetos::cargar(const std::string& ruta) {
    objetos_.clear();
    int omitidas = 0;
    for (const std::string& linea : util::leerLineas(ruta)) {
        ObjetoEncontrado o;
        if (ObjetoEncontrado::desdeLinea(linea, o)) {
            objetos_.push_back(o);  // el archivo ya está ordenado: se respeta su orden
        } else {
            ++omitidas;
        }
    }
    return omitidas;
}

bool GestorObjetos::guardar(const std::string& ruta) const {
    std::vector<std::string> lineas;
    for (const ObjetoEncontrado& o : objetos_) lineas.push_back(o.aLinea());
    return util::escribirLineas(ruta, lineas);
}

void GestorObjetos::agregar(const ObjetoEncontrado& objeto) {
    // Primera posición cuyo hallazgo es más antiguo que el nuevo.
    auto pos = std::find_if(objetos_.begin(), objetos_.end(), [&](const ObjetoEncontrado& x) {
        return x.fecha() < objeto.fecha();
    });
    objetos_.insert(pos, objeto);
}

ObjetoEncontrado* GestorObjetos::buscarPorCodigo(const std::string& codigo) {
    for (ObjetoEncontrado& o : objetos_) {
        if (util::normalizar(o.codigo()) == util::normalizar(codigo)) return &o;
    }
    return nullptr;
}

std::vector<ObjetoEncontrado*> GestorObjetos::filtrar(
    const std::function<bool(const ObjetoEncontrado&)>& criterio) {
    std::vector<ObjetoEncontrado*> resultado;
    for (ObjetoEncontrado& o : objetos_) {
        if (criterio(o)) resultado.push_back(&o);
    }
    return resultado;
}

bool GestorObjetos::depurar(const std::string& codigo) {
    for (auto it = objetos_.begin(); it != objetos_.end(); ++it) {
        if (it->codigo() == codigo) {
            if (it->estado() != cat::EstadoObjeto::Desactivado) return false;  // solo desactivados
            objetos_.erase(it);
            return true;
        }
    }
    return false;
}
