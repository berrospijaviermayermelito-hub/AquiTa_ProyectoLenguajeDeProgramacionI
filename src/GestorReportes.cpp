#include "GestorReportes.h"

#include "Utilidades.h"

int GestorReportes::cargar(const std::string& ruta) {
    reportes_.clear();
    int omitidas = 0;
    for (const std::string& linea : util::leerLineas(ruta)) {
        ObjetoPerdido r;
        if (ObjetoPerdido::desdeLinea(linea, r)) {
            reportes_.push_back(r);
        } else {
            ++omitidas;
        }
    }
    return omitidas;
}

bool GestorReportes::guardar(const std::string& ruta) const {
    std::vector<std::string> lineas;
    for (const ObjetoPerdido& r : reportes_) lineas.push_back(r.aLinea());
    return util::escribirLineas(ruta, lineas);
}

void GestorReportes::agregar(const ObjetoPerdido& reporte) {
    reportes_.push_back(reporte);
}

ObjetoPerdido* GestorReportes::buscarPorCodigo(const std::string& codigo) {
    for (ObjetoPerdido& r : reportes_) {
        if (util::normalizar(r.codigo()) == util::normalizar(codigo)) return &r;
    }
    return nullptr;
}

std::vector<ObjetoPerdido*> GestorReportes::filtrar(
    const std::function<bool(const ObjetoPerdido&)>& criterio) {
    std::vector<ObjetoPerdido*> resultado;
    for (ObjetoPerdido& r : reportes_) {
        if (criterio(r)) resultado.push_back(&r);
    }
    return resultado;
}
