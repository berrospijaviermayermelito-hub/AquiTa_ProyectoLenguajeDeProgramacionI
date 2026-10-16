#include "Contadores.h"

#include <iomanip>
#include <sstream>
#include <vector>

#include "Utilidades.h"

// Formato del archivo: una sola línea "reporte|objeto|solicitud" (por ejemplo 3|5|2).
bool Contadores::cargar(const std::string& ruta) {
    std::vector<std::string> lineas = util::leerLineas(ruta);
    if (lineas.empty()) return true;  // primera ejecución
    std::vector<std::string> c = util::separar(lineas[0]);
    if (c.size() != 3) return false;
    try {
        reporte_ = std::stoi(c[0]);
        objeto_ = std::stoi(c[1]);
        solicitud_ = std::stoi(c[2]);
    } catch (...) {
        return false;
    }
    return true;
}

bool Contadores::guardar(const std::string& ruta) const {
    return util::escribirLineas(ruta, {util::unir({std::to_string(reporte_), std::to_string(objeto_),
                                                   std::to_string(solicitud_)})});
}

std::string Contadores::formatear(const std::string& prefijo, int numero) {
    std::ostringstream os;
    os << prefijo << "-" << std::setfill('0') << std::setw(3) << numero;
    return os.str();
}

std::string Contadores::nuevoCodigoReporte() { return formatear("REP", ++reporte_); }
std::string Contadores::nuevoCodigoObjeto() { return formatear("OBJ", ++objeto_); }
std::string Contadores::nuevoCodigoSolicitud() { return formatear("SOL", ++solicitud_); }
