#include "GestorSolicitudes.h"

#include "Utilidades.h"

int GestorSolicitudes::cargar(const std::string& ruta) {
    solicitudes_.clear();
    int omitidas = 0;
    for (const std::string& linea : util::leerLineas(ruta)) {
        Solicitud s;
        if (Solicitud::desdeLinea(linea, s)) {
            solicitudes_.push_back(s);
        } else {
            ++omitidas;
        }
    }
    return omitidas;
}

bool GestorSolicitudes::guardar(const std::string& ruta) const {
    std::vector<std::string> lineas;
    for (const Solicitud& s : solicitudes_) lineas.push_back(s.aLinea());
    return util::escribirLineas(ruta, lineas);
}

void GestorSolicitudes::agregar(const Solicitud& solicitud) {
    solicitudes_.push_back(solicitud);
}

Solicitud* GestorSolicitudes::buscarPorCodigo(const std::string& codigo) {
    for (Solicitud& s : solicitudes_) {
        if (util::normalizar(s.codigo()) == util::normalizar(codigo)) return &s;
    }
    return nullptr;
}

std::vector<Solicitud*> GestorSolicitudes::filtrar(
    const std::function<bool(const Solicitud&)>& criterio) {
    std::vector<Solicitud*> resultado;
    for (Solicitud& s : solicitudes_) {
        if (criterio(s)) resultado.push_back(&s);
    }
    return resultado;
}
