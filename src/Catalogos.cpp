#include "Catalogos.h"

namespace cat {

const std::vector<std::string> CATEGORIAS = {
    "electrónicos", "documentos", "ropa", "útiles", "accesorios", "llaves", "otros"};

// PROVISIONAL: el equipo debe entregar la lista real de lugares del campus.
// Al cambiarla, los archivos ya guardados siguen funcionando (se guarda el texto).
const std::vector<std::string> LUGARES = {
    "Biblioteca", "Cafetería", "Aulas", "Laboratorios", "Pasillos",
    "Patio", "Auditorio", "Estacionamiento", "Otro"};

// ---- Nombres de los estados (el índice coincide con el valor del enum) ----
static const char* NOMBRES_REPORTE[] = {"pendiente", "recuperado", "cancelado"};
static const char* NOMBRES_OBJETO[] = {"disponible", "en proceso", "devuelto", "desactivado"};
static const char* NOMBRES_SOLICITUD[] = {"pendiente", "aprobada", "rechazada", "cancelada",
                                          "entregada"};

std::string aTexto(EstadoReporte e) { return NOMBRES_REPORTE[static_cast<int>(e)]; }
std::string aTexto(EstadoObjeto e) { return NOMBRES_OBJETO[static_cast<int>(e)]; }
std::string aTexto(EstadoSolicitud e) { return NOMBRES_SOLICITUD[static_cast<int>(e)]; }

bool desdeTexto(const std::string& t, EstadoReporte& e) {
    for (int i = 0; i < 3; ++i) {
        if (t == NOMBRES_REPORTE[i]) {
            e = static_cast<EstadoReporte>(i);
            return true;
        }
    }
    return false;
}

bool desdeTexto(const std::string& t, EstadoObjeto& e) {
    for (int i = 0; i < 4; ++i) {
        if (t == NOMBRES_OBJETO[i]) {
            e = static_cast<EstadoObjeto>(i);
            return true;
        }
    }
    return false;
}

bool desdeTexto(const std::string& t, EstadoSolicitud& e) {
    for (int i = 0; i < 5; ++i) {
        if (t == NOMBRES_SOLICITUD[i]) {
            e = static_cast<EstadoSolicitud>(i);
            return true;
        }
    }
    return false;
}

}  // namespace cat
